#include "byzantine/save.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace byzantine {
namespace {

// Strict little-endian reader. Every accessor reports the offset that ran out,
// so a truncated or misaligned image fails loudly instead of guessing.
class Reader {
public:
    explicit Reader(const std::vector<std::uint8_t>& bytes) : bytes_(bytes) {}

    std::size_t position() const { return position_; }
    std::size_t remaining() const { return bytes_.size() - position_; }
    bool at_end() const { return position_ == bytes_.size(); }

    const std::uint8_t* take(std::size_t count) {
        if (count > remaining()) {
            throw std::runtime_error("save: truncated at offset " +
                                     std::to_string(position_) + " (need " +
                                     std::to_string(count) + " bytes, have " +
                                     std::to_string(remaining()) + ")");
        }
        const auto* start = bytes_.data() + position_;
        position_ += count;
        return start;
    }

    std::uint8_t u8() { return *take(1); }

    std::uint16_t u16() {
        const auto* data = take(2);
        return static_cast<std::uint16_t>(data[0] | (data[1] << 8));
    }

    std::uint32_t u32() {
        const auto* data = take(4);
        return static_cast<std::uint32_t>(data[0]) |
               (static_cast<std::uint32_t>(data[1]) << 8) |
               (static_cast<std::uint32_t>(data[2]) << 16) |
               (static_cast<std::uint32_t>(data[3]) << 24);
    }

    std::int32_t i32() { return static_cast<std::int32_t>(u32()); }

    template <std::size_t N>
    std::array<std::uint8_t, N> fixed() {
        const auto* data = take(N);
        std::array<std::uint8_t, N> result{};
        std::copy(data, data + N, result.begin());
        return result;
    }

    // The original uses fputs/fread of NUL-terminated strings, so a missing
    // terminator is a real defect. limit is a parser safety bound, not a rule
    // taken from the game.
    std::string cstring(const char* what, std::size_t limit = 260) {
        std::string text;
        while (true) {
            if (at_end()) {
                throw std::runtime_error(std::string("save: unterminated ") + what +
                                         " ending at offset " +
                                         std::to_string(position_));
            }
            const auto value = u8();
            if (value == 0) return text;
            if (text.size() >= limit) {
                throw std::runtime_error(std::string("save: ") + what + " at offset " +
                                         std::to_string(position_) +
                                         " exceeds the parser limit of " +
                                         std::to_string(limit) + " bytes");
            }
            text.push_back(static_cast<char>(value));
        }
    }

    std::vector<std::uint8_t> rest() {
        const auto* data = bytes_.data() + position_;
        std::vector<std::uint8_t> tail(data, bytes_.data() + bytes_.size());
        position_ = bytes_.size();
        return tail;
    }

private:
    const std::vector<std::uint8_t>& bytes_;
    std::size_t position_ = 0;
};

// The three fixed tables whose absent marker is 0xff and whose present marker
// is 0 (DAT_00464300, DAT_00464f60, DAT_00486220). Anything else means the
// stream is misaligned: FUN_00424dc0 only ever writes those two values.
bool slot_present(Reader& reader, const char* table) {
    const auto marker = reader.u8();
    if (marker == 0xff) return false;
    if (marker == 0x00) return true;
    throw std::runtime_error(std::string("save: invalid ") + table + " slot marker " +
                             std::to_string(marker) + " at offset " +
                             std::to_string(reader.position() - 1));
}

// FUN_0042658f: one signed count byte, then count * (value, code) pairs. The
// original compares the count as a signed char, so 0x80..0xfe consume nothing.
std::vector<SaveLinkEntry> read_link_list(Reader& reader) {
    std::vector<SaveLinkEntry> entries;
    const auto count = static_cast<std::int8_t>(reader.u8());
    for (int index = 0; index < count; ++index) {
        SaveLinkEntry entry;
        entry.value = reader.u8();
        entry.code = reader.u8();
        entries.push_back(entry);
    }
    return entries;
}

// FUN_00426662: 0xff means absent, otherwise a script id plus a list selector.
SaveScenePointer read_scene_pointer(Reader& reader) {
    SaveScenePointer pointer;
    const auto first = reader.u8();
    if (first == 0xff) return pointer;
    pointer.present = true;
    pointer.script_id = first;
    pointer.source_list = reader.u8();
    pointer.position = reader.u8();
    return pointer;
}

// FUN_00426788: always two bytes; a leading 0xff marks the slot empty.
SaveScenePairSlot read_scene_pair(Reader& reader) {
    SaveScenePairSlot slot;
    const auto first = reader.u8();
    slot.second = reader.u8();
    if (first != 0xff) {
        slot.present = true;
        slot.first = first;
    }
    return slot;
}

// FUN_00426892: an unsigned count byte, then count * (name, table index).
std::vector<SaveSceneChainEntry> read_scene_chain(Reader& reader) {
    const auto count = reader.u8();
    std::vector<SaveSceneChainEntry> entries;
    entries.reserve(count);
    for (std::size_t index = 0; index < count; ++index) {
        SaveSceneChainEntry entry;
        entry.name = reader.u8();
        entry.table_index = reader.u8();
        entries.push_back(entry);
    }
    return entries;
}

// FUN_00426261: one per-scene record, in the same order FUN_00425218 writes it.
SaveSceneRecord read_scene_record(Reader& reader, std::uint8_t slot) {
    SaveSceneRecord record;
    record.slot = slot;
    record.name = reader.cstring("scene record name", 256);
    record.word08 = reader.u16();
    record.bytes0a = reader.fixed<5>();
    record.bytes0f = reader.fixed<0x32>();
    record.bytes41 = reader.fixed<0x32>();
    record.bytes73 = reader.fixed<0x32>();
    for (auto& list : record.list4c4) list = read_link_list(reader);
    for (auto& list : record.list58c) list = read_link_list(reader);
    for (auto& pointer : record.pointersA8) pointer = read_scene_pointer(reader);
    for (auto& pointer : record.pointers170) pointer = read_scene_pointer(reader);
    for (auto& pointer : record.pointers238) pointer = read_scene_pointer(reader);
    for (auto& pair : record.slots3c8) pair = read_scene_pair(reader);
    record.bytes490 = reader.fixed<0x32>();
    for (auto& chain : record.chains654) chain = read_scene_chain(reader);
    return record;
}

// FUN_00426b9d: fixed node record, two frame indices, then the extra list.
SaveNodeSlot read_node(Reader& reader, std::uint8_t marker, std::uint8_t slot) {
    SaveNodeSlot node;
    node.present = true;
    node.slot = slot;
    node.marker = marker;
    node.raw = reader.fixed<0x40>();
    node.frame_index_1c = reader.u32();
    node.frame_index_20 = reader.u32();
    node.field_2c = reader.u32();
    node.field_30 = reader.u32();
    while (true) {
        const auto control = reader.u8();
        // 0x426e2d tests the signed byte against zero: 0 and 0x80..0xff stop.
        if (control == 0 || control >= 0x80) break;
        SaveNodeExtra extra;
        extra.control = control;
        extra.first = reader.u32();
        extra.second = reader.u32();
        node.extra.push_back(extra);
    }
    return node;
}

// The notebook tail is savegame/notebook.000 copied line by line (0x42118c
// reads it back with fgets and writes each line with a newline). Every shipped
// save satisfies both rules below; a NUL or another control byte means the
// tail is not that text.
void validate_notebook(const std::vector<std::uint8_t>& tail) {
    for (std::size_t index = 0; index < tail.size(); ++index) {
        const auto value = tail[index];
        const bool allowed = value == 0x09 || value == 0x0a || value == 0x0d ||
                             value >= 0x20;
        if (!allowed) {
            throw std::runtime_error("save: notebook byte " + std::to_string(value) +
                                     " at offset " + std::to_string(index) +
                                     " of the trailing text region");
        }
    }
    if (!tail.empty() && tail.back() != 0x0a) {
        throw std::runtime_error("save: trailing notebook text does not end with a newline");
    }
}

}  // namespace

SaveGame parse_save(const std::vector<std::uint8_t>& bytes) {
    Reader reader(bytes);
    SaveGame save;
    save.size = bytes.size();

    save.cd_number = reader.u32();
    save.chapter = reader.cstring("chapter name", 256);
    save.description = reader.cstring("save description", 256);
    // FUN_004209d2 seeks 0xdc80 bytes forward: the thumbnail FUN_00420888
    // writes as 0xa8 rows of 0xa8 16-bit pixels.
    const auto* thumbnail = reader.take(kSaveThumbnailBytes);
    save.thumbnail.assign(thumbnail, thumbnail + kSaveThumbnailBytes);
    save.scene_index = reader.u16();

    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        if (!slot_present(reader, "scene record")) continue;
        save.scene_records.push_back(read_scene_record(reader, static_cast<std::uint8_t>(slot)));
    }

    save.node_values.resize(kSaveNodeValueCount);
    for (auto& value : save.node_values) value = reader.u16();

    // 0x425ab4 calls FUN_00408adc here; 0x408adc reads the three channel
    // selectors, then DAT_00468cc0 (10 dword depths) and DAT_00468d78
    // (10 * 0x578 bytes of dialogue ids).
    save.field_45d1c0 = reader.u32();
    save.field_45d1c4 = reader.u32();
    save.field_45d1bc = reader.u32();
    for (auto& depth : save.dialogue_depths) depth = reader.u32();
    for (auto& id : save.dialogue_stacks) id = reader.u32();

    save.dialogue_index = reader.i32();

    // FUN_004089b0: (flag, value) pairs while flag != 0xff, at most 0x15e.
    for (std::size_t index = 0; index < kSaveSlotCount; ++index) {
        const auto flag = reader.u8();
        if (flag == 0xff) break;
        save.dialogue_marks.emplace_back(flag, reader.u8());
    }

    save.field_45f210 = reader.u32();
    save.field_460668 = reader.u32();
    save.field_46066c = reader.u32();

    const auto first = reader.u8();
    if (first != 0) {
        save.field_45f214.push_back(static_cast<char>(first));
        save.field_45f214 += reader.cstring("0x45f214 string", 256);
    }

    save.field_45c060 = reader.u32();
    if (save.field_45c060 == 0) {
        save.field_48253e = reader.u16();
    } else {
        save.field_480330 = reader.u32();
        save.field_45c064 = reader.u32();
    }

    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        const auto marker = reader.u8();
        if (marker == 0xff) continue;
        save.nodes.push_back(read_node(reader, marker, static_cast<std::uint8_t>(slot)));
    }

    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        if (!slot_present(reader, "object")) continue;
        SaveObjectRecord object;
        object.slot = static_cast<std::uint8_t>(slot);
        object.raw = reader.fixed<0x18>();
        object.x = static_cast<std::int16_t>(object.raw[0] | (object.raw[1] << 8));
        object.y = static_cast<std::int16_t>(object.raw[2] | (object.raw[3] << 8));
        object.width = static_cast<std::int16_t>(object.raw[4] | (object.raw[5] << 8));
        object.height = static_cast<std::int16_t>(object.raw[6] | (object.raw[7] << 8));
        object.active = object.raw[8];
        object.kind = object.raw[9];
        object.uninitialized_0a =
            static_cast<std::uint16_t>(object.raw[0x0a] | (object.raw[0x0b] << 8));
        object.label = static_cast<std::uint32_t>(object.raw[0x0c]) |
                       (static_cast<std::uint32_t>(object.raw[0x0d]) << 8) |
                       (static_cast<std::uint32_t>(object.raw[0x0e]) << 16) |
                       (static_cast<std::uint32_t>(object.raw[0x0f]) << 24);
        object.name_id = static_cast<std::uint32_t>(object.raw[0x10]) |
                         (static_cast<std::uint32_t>(object.raw[0x11]) << 8) |
                         (static_cast<std::uint32_t>(object.raw[0x12]) << 16) |
                         (static_cast<std::uint32_t>(object.raw[0x13]) << 24);
        object.argument = static_cast<std::uint32_t>(object.raw[0x14]) |
                          (static_cast<std::uint32_t>(object.raw[0x15]) << 8) |
                          (static_cast<std::uint32_t>(object.raw[0x16]) << 16) |
                          (static_cast<std::uint32_t>(object.raw[0x17]) << 24);
        save.objects.push_back(object);
    }

    save.field_45fd2c = reader.u32();

    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        if (!slot_present(reader, "sound")) continue;
        SaveSoundRecord sound;
        sound.slot = static_cast<std::uint8_t>(slot);
        sound.raw = reader.fixed<0x3c>();
        save.sounds.push_back(sound);
    }
    while (reader.u8() != 0xff) {
        save.sound_chain.push_back(reader.fixed<0x5c>());
    }

    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        if (!slot_present(reader, "media")) continue;
        SaveMediaRecord media;
        media.slot = static_cast<std::uint8_t>(slot);
        media.raw = reader.fixed<0x14>();
        media.value = static_cast<std::uint16_t>(media.raw[0] | (media.raw[1] << 8));
        media.frame = static_cast<std::uint32_t>(media.raw[4]) |
                      (static_cast<std::uint32_t>(media.raw[5]) << 8) |
                      (static_cast<std::uint32_t>(media.raw[6]) << 16) |
                      (static_cast<std::uint32_t>(media.raw[7]) << 24);
        media.timestamp = static_cast<std::uint32_t>(media.raw[8]) |
                          (static_cast<std::uint32_t>(media.raw[9]) << 8) |
                          (static_cast<std::uint32_t>(media.raw[0x0a]) << 16) |
                          (static_cast<std::uint32_t>(media.raw[0x0b]) << 24);
        media.media_id = static_cast<std::uint32_t>(media.raw[0x0c]) |
                         (static_cast<std::uint32_t>(media.raw[0x0d]) << 8) |
                         (static_cast<std::uint32_t>(media.raw[0x0e]) << 16) |
                         (static_cast<std::uint32_t>(media.raw[0x0f]) << 24);
        media.active = media.raw[0x10];
        media.type = media.raw[0x11];
        save.media.push_back(media);
    }

    save.current_scene = reader.u32();

    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        if (reader.u8() == 0xff) continue;
        SaveDialogueChain chain;
        chain.slot = static_cast<std::uint8_t>(slot);
        chain.raw = reader.fixed<0x68>();
        save.dialogue_chains.push_back(chain);
    }

    const auto tail = reader.rest();
    validate_notebook(tail);
    save.notebook.assign(tail.begin(), tail.end());
    return save;
}

std::vector<std::string> parse_chapter_scene_list(const std::vector<std::uint8_t>& bytes) {
    std::vector<std::string> names;
    std::string current;
    const auto flush = [&names, &current]() {
        if (!current.empty() && current.back() == '\r') current.pop_back();
        // 0x438173 lowercases every character that is not the line terminator.
        for (char& character : current) {
            const auto value = static_cast<unsigned char>(character);
            if (value >= 'A' && value <= 'Z') character = static_cast<char>(value + 0x20);
        }
        if (current.size() > kSaveSceneNameLimit) {
            throw std::runtime_error("chapter scene name '" + current +
                                     "' is longer than 8 characters");
        }
        names.push_back(current);
        current.clear();
    };
    for (const auto value : bytes) {
        if (value == 0x0a) {
            flush();
        } else {
            current.push_back(static_cast<char>(value));
        }
    }
    if (!current.empty()) flush();
    // The terminating newline of the last line must not add an extra entry.
    if (!names.empty() && names.back().empty() && !bytes.empty() && bytes.back() == 0x0a) {
        names.pop_back();
    }
    return names;
}

std::optional<std::string> save_scene_name(const SaveGame& save,
                                           const std::vector<std::string>& chapter_scenes) {
    if (save.scene_index >= chapter_scenes.size()) return std::nullopt;
    return chapter_scenes[save.scene_index];
}

}  // namespace byzantine
