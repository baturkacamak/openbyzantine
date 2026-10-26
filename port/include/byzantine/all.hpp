#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace byzantine {

struct AllEntry {
    std::string name;
    std::uint32_t start = 0;
    std::uint32_t end = 0;
};

struct AllArchive {
    std::size_t data_offset = 0;
    std::vector<AllEntry> entries;
};

// The .ALL index stores null-terminated names and pairs of data-relative
// start/end offsets. Throws std::runtime_error on malformed input.
AllArchive parse_all(const std::vector<std::uint8_t>& file);

}  // namespace byzantine
