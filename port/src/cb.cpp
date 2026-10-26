#include "byzantine/cb.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace byzantine {
namespace {

std::string read_string(const std::vector<std::uint8_t>& file, std::size_t& at) {
    const auto start = at;
    while (at < file.size() && file[at] != 0) {
        ++at;
    }
    if (at == file.size() || at == start) {
        throw std::runtime_error("invalid CB string");
    }
    const std::string value(file.begin() + start, file.begin() + at);
    ++at;
    return value;
}

}  // namespace

std::vector<BackgroundEntry> parse_cb(const std::vector<std::uint8_t>& file) {
    std::size_t at = 0;
    std::vector<BackgroundEntry> entries;
    while (at < file.size()) {
        BackgroundEntry entry;
        entry.name = read_string(file, at);
        entry.source = read_string(file, at);
        if (at >= file.size() || file[at] != 0xff) {
            throw std::runtime_error("invalid CB entry terminator");
        }
        ++at;
        entries.push_back(std::move(entry));
    }
    return entries;
}

}  // namespace byzantine
