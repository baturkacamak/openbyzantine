#include "byzantine/all.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: byzantine-all <input.all> [output-directory]\n";
        return 2;
    }
    try {
        std::ifstream input(argv[1], std::ios::binary);
        if (!input) {
            throw std::runtime_error("cannot open ALL archive");
        }
        const std::vector<std::uint8_t> file{std::istreambuf_iterator<char>(input),
                                             std::istreambuf_iterator<char>()};
        const auto archive = byzantine::parse_all(file);
        const bool extract = argc == 3;
        if (extract) {
            std::filesystem::create_directories(argv[2]);
        }
        for (const auto& entry : archive.entries) {
            std::cout << entry.name << " " << entry.start << " " << entry.end << "\n";
            if (extract) {
                const auto path = std::filesystem::path(argv[2]) / entry.name;
                std::ofstream output(path, std::ios::binary);
                if (!output) {
                    throw std::runtime_error("cannot create extracted entry");
                }
                const auto* begin = file.data() + archive.data_offset + entry.start;
                output.write(reinterpret_cast<const char*>(begin), entry.end - entry.start);
                if (!output) {
                    throw std::runtime_error("cannot write extracted entry");
                }
            }
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
