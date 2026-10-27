#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace byzantine {

// One per-id entry of the engine tables loaded by 0x411dfc: the name at +0, the
// source string at +4 (for example "EA.CCG") and the type byte at +8.
struct NamedRecord {
    std::string name;
    std::string source;
    std::uint8_t type = 0;
};

// 0x411dfc: repeated `<name>\0<source>\0<type byte>` records, used by the
// per-scene engine files such as CS.ALL (0x47a8e0, confirmed sound table).
// The original stops at 0x15e records; a larger input is rejected rather than
// truncated. Empty names, missing terminators and trailing bytes that cannot
// form a record are errors.
std::vector<NamedRecord> parse_named_records(const std::vector<std::uint8_t>& data);

// Scene loader 0x42d52f changes CA type 0xff to 0 (on-demand), after
// 0x411dfc reads the raw table. Other named tables retain their raw bytes.
std::vector<NamedRecord> parse_actor_records(const std::vector<std::uint8_t>& data);

// engine/<chapter>.lcx, read by 0x438173 into DAT_00482460: a u16 count
// (maximum 50, the array's size) followed by that many u16 scene indices.
// 0x73 indexes this table with its raw operand and writes the result to
// DAT_00482428; 0xffff means "no entry".
std::vector<std::uint16_t> parse_scene_switch_table(const std::vector<std::uint8_t>& data);

}  // namespace byzantine
