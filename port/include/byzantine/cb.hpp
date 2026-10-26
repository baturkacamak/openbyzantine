#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace byzantine {

struct BackgroundEntry {
    std::string name;
    std::string source;
};

// Parses one extracted CB.ALL scene record. Entries are stored in scene ID order.
std::vector<BackgroundEntry> parse_cb(const std::vector<std::uint8_t>& file);

}  // namespace byzantine
