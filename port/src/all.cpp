#include "byzantine/all.hpp"

#include <cctype>
#include <stdexcept>
#include <utility>

namespace byzantine {
namespace {

std::uint16_t read_u16(const std::vector<std::uint8_t>& file, std::size_t at) {
    if (at > file.size() || file.size() - at < 2) {
        throw std::runtime_error("truncated ALL header");
    }
    return static_cast<std::uint16_t>(file[at] | (file[at + 1] << 8));
}

std::uint32_t read_u32(const std::vector<std::uint8_t>& file, std::size_t at) {
    return static_cast<std::uint32_t>(read_u16(file, at)) |
           (static_cast<std::uint32_t>(read_u16(file, at + 2)) << 16);
}

}  // namespace

AllArchive parse_all(const std::vector<std::uint8_t>& file) {
    if (file.size() < 2) {
        throw std::runtime_error("truncated ALL archive");
    }
    const std::uint16_t count = read_u16(file, 0);
    std::size_t at = 2;
    AllArchive archive;
    archive.entries.reserve(count);
    std::uint32_t expected_start = 0;
    for (std::size_t i = 0; i < count; ++i) {
        std::string name;
        while (at < file.size() && file[at] != 0) {
            const unsigned char ch = file[at++];
            if (!std::isalnum(ch) && ch != '_') {
                throw std::runtime_error("unsafe ALL entry name");
            }
            name.push_back(static_cast<char>(ch));
            if (name.size() > 255) {
                throw std::runtime_error("ALL entry name is too long");
            }
        }
        if (at >= file.size() || name.empty()) {
            throw std::runtime_error("truncated ALL entry name");
        }
        ++at;
        const std::uint32_t start = read_u32(file, at);
        const std::uint32_t end = read_u32(file, at + 4);
        at += 8;
        if (start != expected_start || end < start) {
            throw std::runtime_error("ALL entry offsets are inconsistent");
        }
        archive.entries.push_back({std::move(name), start, end});
        expected_start = end;
    }
    archive.data_offset = at;
    if (expected_start != file.size() - at) {
        throw std::runtime_error("ALL index does not cover the data section");
    }
    return archive;
}

}  // namespace byzantine
