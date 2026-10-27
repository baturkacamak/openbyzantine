#include "byzantine/records.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

std::vector<std::uint8_t> raw(const std::string& text) {
    return {text.begin(), text.end()};
}

std::vector<std::uint8_t> record(const std::string& name, const std::string& source,
                                 std::uint8_t type) {
    std::vector<std::uint8_t> data(name.begin(), name.end());
    data.push_back(0);
    data.insert(data.end(), source.begin(), source.end());
    data.push_back(0);
    data.push_back(type);
    return data;
}
}

int main() {
    try {
        using byzantine::parse_named_records;

        // Synthetic records in the 0x411dfc layout: <name>\0<source>\0<type>.
        auto data = record("voemtap2", "EA.CCG", 2);
        const auto second = record("eadrOpn", "EA.CCG", 1);
        data.insert(data.end(), second.begin(), second.end());
        const auto records = parse_named_records(data);
        require(records.size() == 2, "record count mismatch");
        require(records[0].name == "voemtap2" && records[0].source == "EA.CCG" &&
                records[0].type == 2, "first record mismatch");
        require(records[1].name == "eadrOpn" && records[1].source == "EA.CCG" &&
                records[1].type == 1, "second record mismatch");

        // The CS table contains an entry literally named "endgame" (VH scene);
        // 0x42db22 case 0xc4 compares against exactly that string.
        const auto endgame = parse_named_records(record("endgame", "VH.CCG", 0));
        require(endgame.size() == 1 && endgame[0].name == "endgame" &&
                endgame[0].type == 0, "endgame record mismatch");

        // Empty input is an empty table, not an error.
        require(parse_named_records({}).empty(), "empty table rejected");

        // Raw 0xff is NOT a dead CA slot: 0x42d52f normalizes it before
        // the scene starts. Preserve it for CS/CI/etc., normalize CA only.
        const auto untyped = record("tknuxb", "TK.CCG", 0xff);
        require(parse_named_records(untyped)[0].type == 0xff,
                "raw named record parser changed a non-CA type");
        require(byzantine::parse_actor_records(untyped)[0].type == 0,
                "CA loader did not normalize 0xff to on-demand type 0");
        for (const auto type : {0, 1, 2, 3, 4}) {
            require(byzantine::parse_actor_records(record("n", "s", type))[0].type == type,
                    "CA loader changed an explicit type");
        }

        std::vector<std::vector<std::uint8_t>> broken;
        broken.push_back(raw("name"));                                  // no terminator
        broken.push_back(raw(std::string("name\0src", 8)));              // missing source NUL
        broken.push_back(raw(std::string("name\0src\0", 9)));            // missing type byte
        broken.push_back(raw(std::string("\0src\0", 5) + '\x01'));       // empty name
        for (const auto& input : broken) {
            bool rejected = false;
            try {
                parse_named_records(input);
            } catch (const std::runtime_error&) {
                rejected = true;
            }
            require(rejected, "accepted a malformed record table");
        }

        // More than 0x15e records is rejected instead of truncated.
        std::vector<std::uint8_t> oversized;
        for (std::size_t i = 0; i <= 0x15e; ++i) {
            const auto entry = record("n", "s", 1);
            oversized.insert(oversized.end(), entry.begin(), entry.end());
        }
        bool limit_rejected = false;
        try {
            parse_named_records(oversized);
        } catch (const std::runtime_error&) {
            limit_rejected = true;
        }
        require(limit_rejected, "accepted more than 0x15e records");

        // engine/<chapter>.lcx: u16 count + count u16 scene indices (0x438173 ->
        // DAT_00482460, indexed by 0x73).
        using byzantine::parse_scene_switch_table;
        const std::vector<std::uint8_t> lcx = {3, 0, 0, 0, 25, 0, 0xff, 0xff};
        const auto targets = parse_scene_switch_table(lcx);
        require(targets.size() == 3 && targets[0] == 0 && targets[1] == 25 &&
                    targets[2] == 0xffff,
                "scene switch table mismatch");
        require(parse_scene_switch_table({0, 0}).empty(),
                "empty scene switch table rejected");
        std::vector<std::vector<std::uint8_t>> broken_switch;
        broken_switch.push_back({});
        broken_switch.push_back({1});                      // missing count low byte
        broken_switch.push_back({51, 0});                  // beyond the 50-entry array
        broken_switch.push_back({2, 0, 1, 0});             // size does not match count
        for (const auto& input : broken_switch) {
            bool rejected = false;
            try {
                parse_scene_switch_table(input);
            } catch (const std::runtime_error&) {
                rejected = true;
            }
            require(rejected, "accepted a malformed scene switch table");
        }

        std::cout << "engine record table tests passed\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
