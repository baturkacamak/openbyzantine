#pragma once

// savegame/NN.BTB reader.
//
// Reconstructed from the save loader FUN_00425ab4 (0x425ab4-0x426260) and the
// matching writer FUN_00424dc0 (0x424dc0-0x42515e) in the saved export set.
// Every block below is emitted and consumed in the same order, so the write
// path independently confirms the read order. Offset comments are file
// offsets inside their block, not C++ member offsets, unless stated.
//
// Not modelled here: applying the state (0x408adc display refresh, the
// 0x464300 object table ownership, 0xcc/0x70). This is a parser only.

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace byzantine {

// 0x15e slots in every fixed table (DAT_00464300, DAT_00464f60, DAT_00486220,
// DAT_00486a10, DAT_004649e8, DAT_00482ad0).
constexpr std::size_t kSaveSlotCount = 0x15e;
// engine/SHOW.LCC holds one lowercase scene name per line (0x438173).
constexpr std::size_t kSaveSceneNameLimit = 8;
// 0x420888 writes 0xa8 rows of 0xa8 words.
constexpr std::size_t kSaveThumbnailBytes = 0xdc80;
constexpr std::size_t kSaveThumbnailWidth = 0xa8;
constexpr std::size_t kSaveThumbnailHeight = 0xa8;
// DAT_00468cc0 (10 dwords) and DAT_00468d78 (10 * 0x578 bytes) in 0x408adc.
constexpr std::size_t kSaveDialogueChannels = 10;
constexpr std::size_t kSaveDialogueStackDepth = 0x578 / 4;
// DAT_0047e2f0 is 3000 words, written by 0x424dc0 as 3000 records of 2 bytes.
constexpr std::size_t kSaveNodeValueCount = 3000;

// DAT_00482ad0[i]: one per-scene equipment/object record (0x1ea4 bytes in the
// original). Only the file-carried part is modelled. The 50-entry link lists
// are stored as written: an empty vector means the 0xff "absent" marker.
struct SaveLinkEntry {
    std::uint8_t value = 0;  // +0x10 in the linked record
    std::uint8_t code = 0;   // +0x11 in the linked record
};
struct SaveScenePointer {
    bool present = false;
    std::uint8_t script_id = 0;  // record +0x00
    std::uint8_t source_list = 0;  // index into the +0x58c list
    std::uint8_t position = 0;     // steps along that list
};
struct SaveScenePairSlot {
    bool present = false;
    std::uint8_t first = 0;
    std::uint8_t second = 0;
};
struct SaveSceneChainEntry {
    std::uint8_t name = 0;
    std::uint8_t table_index = 0;
};
struct SaveSceneRecord {
    std::uint8_t slot = 0;  // 0..0x15d
    std::string name;       // copied to record +0x1e94
    std::uint16_t word08 = 0;  // +0x08, fread 2
    std::array<std::uint8_t, 5> bytes0a{};  // +0x0a..+0x0e
    std::array<std::uint8_t, 0x32> bytes0f{};
    std::array<std::uint8_t, 0x32> bytes41{};
    std::array<std::uint8_t, 0x32> bytes73{};
    std::array<std::vector<SaveLinkEntry>, 0x32> list4c4;  // FUN_0042658f
    std::array<std::vector<SaveLinkEntry>, 0x32> list58c;  // FUN_0042658f
    std::array<SaveScenePointer, 0x32> pointersA8;   // FUN_00426662
    std::array<SaveScenePointer, 0x32> pointers170;  // FUN_00426662
    std::array<SaveScenePointer, 0x32> pointers238;  // FUN_00426662
    std::array<SaveScenePairSlot, 0x32> slots3c8;    // FUN_00426788
    std::array<std::uint8_t, 0x32> bytes490{};
    std::array<std::vector<SaveSceneChainEntry>, 0x32> chains654;  // FUN_00426892
};

// DAT_00464300[i], 0x18 bytes. Layout recovered from 0x42db22 case 0x72.
struct SaveObjectRecord {
    std::uint8_t slot = 0;
    std::int16_t x = 0;       // +0x00
    std::int16_t y = 0;       // +0x02, already includes 0x72's +0x14 bias
    std::int16_t width = 0;   // +0x04
    std::int16_t height = 0;  // +0x06
    std::uint8_t active = 0;  // +0x08 (0x72 writes 0, ON/OFF rewrite it)
    std::uint8_t kind = 0;    // +0x09 (0x13/0x14/0x17/... hotspot kinds)
    // +0x0a..+0x0b: no recovered writer. 0x72 skips these two bytes and the
    // 0x18-byte allocation is not zeroed, so saved values are stale heap data.
    std::uint16_t uninitialized_0a = 0;
    std::uint32_t label = 0;    // +0x0c, DAT_004654e0 index (CONVERSE 0x70)
    std::uint32_t name_id = 0;  // +0x10, 0xffffffff when 0x72 did not resolve
    std::uint32_t argument = 0;  // +0x14, 0xffffffff default, 0xcc overwrites
    std::array<std::uint8_t, 0x18> raw{};
};

// DAT_00464f60[i], 0x14 bytes. Layout from 0x42db22 case 0x6a; 0x6c clears
// +0x10.
struct SaveMediaRecord {
    std::uint8_t slot = 0;
    std::uint16_t value = 0;    // +0x00
    std::uint32_t frame = 0;    // +0x04, initialised to 0
    std::uint32_t timestamp = 0;  // +0x08, timeGetTime() at definition
    std::uint32_t media_id = 0;   // +0x0c, operand 2
    std::uint8_t active = 0;      // +0x10
    std::uint8_t type = 0;        // +0x11
    std::array<std::uint8_t, 0x14> raw{};
};

// DAT_00486220[i], 0x3c bytes, consumed by 0x65/0xbc/0xbf/0xc0/0xc1/0xc2.
struct SaveSoundRecord {
    std::uint8_t slot = 0;
    std::array<std::uint8_t, 0x3c> raw{};
    std::uint16_t word(std::size_t offset) const {
        return static_cast<std::uint16_t>(raw.at(offset) |
                                          (raw.at(offset + 1) << 8));
    }
};

// One 8-byte entry of the node's extra list.
struct SaveNodeExtra {
    std::uint8_t control = 0;  // read before the 8 bytes, 1..127 continues
    std::uint32_t first = 0;
    std::uint32_t second = 0;
};

// DAT_004649e8[i], 0x40 bytes, plus the two frame indices and the extra list.
struct SaveNodeSlot {
    bool present = false;
    std::uint8_t slot = 0;
    std::uint8_t marker = 0;  // 1 requests FUN_0040fe70 in the original
    std::array<std::uint8_t, 0x40> raw{};
    std::uint32_t frame_index_1c = 0;  // 0xffff means "none" (0x40cf73)
    std::uint32_t frame_index_20 = 0;  // 0xffff means "none" (0x426ed0)
    std::uint32_t field_2c = 0;
    std::uint32_t field_30 = 0;
    // 0x426b9d reads a control byte and then 8 bytes while it is 1..127.
    // FUN_004269ec writes a count byte plus count*8 bytes instead, so a count
    // of 2 or more would desynchronise the original reader; every shipped
    // save in the CD1 set has control 0 here. The parser follows the reader.
    std::vector<SaveNodeExtra> extra;
};

// DAT_00486a10[i], 0x68 bytes: per-object dialogue chains (0x407c5a).
struct SaveDialogueChain {
    std::uint8_t slot = 0;
    std::array<std::uint8_t, 0x68> raw{};
    std::uint32_t count() const {  // +0x64
        return static_cast<std::uint32_t>(raw.at(0x64)) |
               (static_cast<std::uint32_t>(raw.at(0x65)) << 8) |
               (static_cast<std::uint32_t>(raw.at(0x66)) << 16) |
               (static_cast<std::uint32_t>(raw.at(0x67)) << 24);
    }
    std::uint32_t id(std::size_t index) const {
        const auto base = index * 4;
        return static_cast<std::uint32_t>(raw.at(base)) |
               (static_cast<std::uint32_t>(raw.at(base + 1)) << 8) |
               (static_cast<std::uint32_t>(raw.at(base + 2)) << 16) |
               (static_cast<std::uint32_t>(raw.at(base + 3)) << 24);
    }
};

struct SaveGame {
    std::size_t size = 0;          // file length
    std::uint32_t cd_number = 0;   // DAT_0045da78, checked by 0x40c7b4
    std::string chapter;           // DAT_004859c0, e.g. "show" (0x437cad)
    std::string description;       // DAT_00484e30, save name (0x421d5e)
    std::vector<std::uint8_t> thumbnail;  // 168x168 RGB16, 0xdc80 bytes
    std::uint16_t scene_index = 0;  // DAT_00482428, index into engine/SHOW.LCC
    std::vector<SaveSceneRecord> scene_records;   // present slots, in file order
    std::vector<std::uint16_t> node_values;        // DAT_0047e2f0, 3000 words
    // 0x408adc preamble (FUN_00408a1e writes the same three dwords).
    std::uint32_t field_45d1c0 = 0;  // 0x4075fe/0x4083d4: dialog visibility
    std::uint32_t field_45d1c4 = 0;  // 0x4073f0: selected channel
    std::uint32_t field_45d1bc = 0;  // 0x407527: scroll position in the channel
    std::array<std::uint32_t, kSaveDialogueChannels> dialogue_depths{};  // 42
    std::array<std::uint32_t, kSaveDialogueChannels * kSaveDialogueStackDepth>
        dialogue_stacks{};  // DAT_00468d78
    std::int32_t dialogue_index = -1;  // DAT_0045d1c8 (0xffffffff = none)
    // 0x4089b0: (flag, value) pairs while flag != 0xff, at most 0x15e.
    std::vector<std::pair<std::uint8_t, std::uint8_t>> dialogue_marks;
    std::uint32_t field_45f210 = 0;
    std::uint32_t field_460668 = 0;
    std::uint32_t field_46066c = 0;
    std::string field_45f214;  // optional NUL-terminated string
    std::uint32_t field_45c060 = 0;
    std::uint16_t field_48253e = 0xffff;  // when field_45c060 == 0
    std::uint32_t field_480330 = 0;       // when field_45c060 != 0
    std::uint32_t field_45c064 = 0;       // panorama x, 0x401ddf
    std::vector<SaveNodeSlot> nodes;                  // present slots only
    std::vector<SaveObjectRecord> objects;            // DAT_00464300
    std::uint32_t field_45fd2c = 0;
    std::vector<SaveSoundRecord> sounds;              // DAT_00486220
    std::vector<std::array<std::uint8_t, 0x5c>> sound_chain;  // DAT_0046019c
    std::vector<SaveMediaRecord> media;               // DAT_00464f60
    std::uint32_t current_scene = 0;  // DAT_00464878, compared by 0x425ab4
    std::vector<SaveDialogueChain> dialogue_chains;   // DAT_00486a10
    std::string notebook;  // the tail, copied to savegame/notebook.000
};

// Parses one savegame/NN.BTB image. Throws std::runtime_error with the failing
// offset when the image is truncated, a count/marker is invalid, or the
// trailing notebook region is not the line text that 0x4210af/0x42118c write.
SaveGame parse_save(const std::vector<std::uint8_t>& bytes);

// engine/SHOW.LCC as read by 0x438173: one scene name per line, lowercased,
// CR/LF trimmed (the original opens in text mode), at most 8 characters.
std::vector<std::string> parse_chapter_scene_list(
    const std::vector<std::uint8_t>& bytes);

// DAT_00482428 looked up in that list. Returns nullopt when the index is past
// the end, so the caller can report the missing chapter table instead of
// inventing a name.
std::optional<std::string> save_scene_name(const SaveGame& save,
                                           const std::vector<std::string>& chapter_scenes);

}  // namespace byzantine
