#include "byzantine/records.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace byzantine {
namespace {
constexpr std::size_t kRecordLimit = 0x15e;

void read_string(const std::vector<std::uint8_t>& data, std::size_t& at,
                 std::string& destination) {
    if (at >= data.size()) {
        throw std::runtime_error("truncated engine record table");
    }
    const auto begin = data.begin() + static_cast<std::ptrdiff_t>(at);
    const auto end = std::find(begin, data.end(), 0);
    if (end == data.end()) {
        throw std::runtime_error("unterminated engine record string");
    }
    destination.assign(begin, end);
    at = static_cast<std::size_t>(end - data.begin()) + 1;
    if (destination.empty()) {
        throw std::runtime_error("empty engine record name");
    }
}
}  // namespace

std::vector<NamedRecord> parse_named_records(const std::vector<std::uint8_t>& data) {
    std::vector<NamedRecord> records;
    std::size_t at = 0;
    while (at < data.size()) {
        if (records.size() >= kRecordLimit) {
            throw std::runtime_error("engine record table exceeds 0x15e entries");
        }
        NamedRecord record;
        read_string(data, at, record.name);
        read_string(data, at, record.source);
        if (at >= data.size()) {
            throw std::runtime_error("truncated engine record type byte");
        }
        record.type = data[at++];
        records.push_back(std::move(record));
    }
    return records;
}

std::vector<std::uint16_t> parse_scene_switch_table(
    const std::vector<std::uint8_t>& data) {
    // 0x438173: __fread_lk(&count, 2); then count u16 values. DAT_00482460 holds
    // 50 dwords initialised to 0xffff by 0x42c8a0, so a larger count would
    // overrun the table.
    if (data.size() < 2) {
        throw std::runtime_error("truncated scene switch table count");
    }
    const auto count = static_cast<std::size_t>(data[0] | (data[1] << 8));
    if (count > 50) {
        throw std::runtime_error("scene switch table exceeds the 50-entry array");
    }
    if (data.size() != 2 + count * 2) {
        throw std::runtime_error("scene switch table size does not match its count");
    }
    std::vector<std::uint16_t> targets;
    targets.reserve(count);
    for (std::size_t index = 0; index < count; ++index) {
        const auto at = 2 + index * 2;
        targets.push_back(static_cast<std::uint16_t>(data[at] | (data[at + 1] << 8)));
    }
    return targets;
}

}  // namespace byzantine
