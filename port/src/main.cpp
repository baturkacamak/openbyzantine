#include "byzantine/rle.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("cannot open input: " + path.string());
    }
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void write_tga(const std::filesystem::path& path, const byzantine::RleFrame& frame) {
    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw std::runtime_error("cannot create output: " + path.string());
    }
    std::uint8_t header[18]{};
    header[2] = 2;  // Uncompressed true-color image.
    header[12] = static_cast<std::uint8_t>(frame.width);
    header[13] = static_cast<std::uint8_t>(frame.width >> 8);
    header[14] = static_cast<std::uint8_t>(frame.height);
    header[15] = static_cast<std::uint8_t>(frame.height >> 8);
    header[16] = 32;
    header[17] = 0x28;  // Eight alpha bits, top-left origin.
    output.write(reinterpret_cast<const char*>(header), sizeof(header));
    for (std::size_t i = 0; i < frame.rgba.size(); i += 4) {
        const char bgra[4] = {
            static_cast<char>(frame.rgba[i + 2]), static_cast<char>(frame.rgba[i + 1]),
            static_cast<char>(frame.rgba[i]), static_cast<char>(frame.rgba[i + 3])};
        output.write(bgra, sizeof(bgra));
    }
    if (!output) {
        throw std::runtime_error("failed while writing output: " + path.string());
    }
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: byzantine-rle <input.rle> <output-directory>\n";
        return 2;
    }
    try {
        const std::filesystem::path input = argv[1];
        const std::filesystem::path output_dir = argv[2];
        const auto frames = byzantine::decode_rle(read_file(input));
        std::filesystem::create_directories(output_dir);
        for (std::size_t i = 0; i < frames.size(); ++i) {
            std::ostringstream name;
            name << "frame_" << std::setw(3) << std::setfill('0') << i << ".tga";
            const auto path = output_dir / name.str();
            write_tga(path, frames[i]);
            std::cout << path << ": " << frames[i].width << "x" << frames[i].height << "\n";
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
