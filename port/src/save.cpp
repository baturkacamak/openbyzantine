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
// `raw_count` keeps the byte as written: an empty list is stored as 0xff in
// the shipped images (the writer copies the signed in-memory count), and a
// byte-identical writer must not normalise it to 0.
std::vector<SaveLinkEntry> read_link_list(Reader& reader, std::uint8_t* raw_count) {
    std::vector<SaveLinkEntry> entries;
    const auto raw = reader.u8();
    if (raw_count != nullptr) *raw_count = raw;
    const auto count = static_cast<std::int8_t>(raw);
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
SaveSceneRecord read_scene_record(Reader& reader, std::uint16_t slot) {
    SaveSceneRecord record;
    record.slot = slot;
    record.name = reader.cstring("scene record name", 256);
    record.word08 = reader.u16();
    record.bytes0a = reader.fixed<5>();
    record.bytes0f = reader.fixed<0x32>();
    record.bytes41 = reader.fixed<0x32>();
    record.bytes73 = reader.fixed<0x32>();
    for (std::size_t index = 0; index < record.list4c4.size(); ++index) {
        record.list4c4[index] = read_link_list(reader, &record.list4c4_count[index]);
    }
    for (std::size_t index = 0; index < record.list58c.size(); ++index) {
        record.list58c[index] = read_link_list(reader, &record.list58c_count[index]);
    }
    for (auto& pointer : record.pointersA8) pointer = read_scene_pointer(reader);
    for (auto& pointer : record.pointers170) pointer = read_scene_pointer(reader);
    for (auto& pointer : record.pointers238) pointer = read_scene_pointer(reader);
    for (auto& pair : record.slots3c8) pair = read_scene_pair(reader);
    record.bytes490 = reader.fixed<0x32>();
    for (auto& chain : record.chains654) chain = read_scene_chain(reader);
    return record;
}

// FUN_00426b9d: fixed node record, two frame indices, then the extra list.
SaveNodeSlot read_node(Reader& reader, std::uint8_t marker, std::uint16_t slot) {
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

class Writer {
public:
    void u8(std::uint8_t value) { bytes_.push_back(value); }
    void u16(std::uint16_t value) {
        bytes_.push_back(static_cast<std::uint8_t>(value & 0xff));
        bytes_.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
    }
    void u32(std::uint32_t value) {
        for (int shift = 0; shift < 32; shift += 8) {
            bytes_.push_back(static_cast<std::uint8_t>((value >> shift) & 0xff));
        }
    }
    void i32(std::int32_t value) { u32(static_cast<std::uint32_t>(value)); }
    void bytes(const std::uint8_t* data, std::size_t size) {
        bytes_.insert(bytes_.end(), data, data + size);
    }
    template <std::size_t N>
    void fixed(const std::array<std::uint8_t, N>& value) {
        bytes(value.data(), value.size());
    }
    void cstring(const std::string& text, const char* what) {
        if (text.find('\0') != std::string::npos || text.size() > 256) {
            throw std::runtime_error(std::string("save writer: invalid ") + what);
        }
        bytes_.insert(bytes_.end(), text.begin(), text.end());
        u8(0);
    }
    std::vector<std::uint8_t> take() { return std::move(bytes_); }

private:
    std::vector<std::uint8_t> bytes_;
};


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
        save.scene_records.push_back(read_scene_record(reader, static_cast<std::uint16_t>(slot)));
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
        save.nodes.push_back(read_node(reader, marker, static_cast<std::uint16_t>(slot)));
    }

    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        if (!slot_present(reader, "object")) continue;
        SaveObjectRecord object;
        object.slot = static_cast<std::uint16_t>(slot);
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
        sound.slot = static_cast<std::uint16_t>(slot);
        sound.raw = reader.fixed<0x3c>();
        save.sounds.push_back(sound);
    }
    while (reader.u8() != 0xff) {
        save.sound_chain.push_back(reader.fixed<0x5c>());
    }

    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        if (!slot_present(reader, "media")) continue;
        SaveMediaRecord media;
        media.slot = static_cast<std::uint16_t>(slot);
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
        chain.slot = static_cast<std::uint16_t>(slot);
        chain.raw = reader.fixed<0x68>();
        save.dialogue_chains.push_back(chain);
    }

    const auto tail = reader.rest();
    validate_notebook(tail);
    save.notebook.assign(tail.begin(), tail.end());
    return save;
}

std::vector<std::uint8_t> write_save(const SaveGame& save) {
    // FUN_00424dc0 writes the same blocks in the same order as FUN_00425ab4
    // reads them; every variable-length table is marshalled in slot order and
    // uses the writer's own marker convention. The function refuses values the
    // original cannot write (wrong table sizes, an absent marker in a fixed
    // table, a key outside the 0x15e slots) instead of emitting a broken image.
    Writer writer;
    writer.u32(save.cd_number);
    writer.cstring(save.chapter, "chapter name");
    writer.cstring(save.description, "save description");
    if (save.thumbnail.size() != kSaveThumbnailBytes) {
        throw std::runtime_error("save writer: thumbnail is not 0xdc80 bytes");
    }
    writer.bytes(save.thumbnail.data(), save.thumbnail.size());
    writer.u16(save.scene_index);

    std::array<const SaveSceneRecord*, kSaveSlotCount> scene_records{};
    for (const auto& record : save.scene_records) {
        if (record.slot >= kSaveSlotCount) {
            throw std::runtime_error("save writer: scene record slot outside 0x15e");
        }
        if (scene_records[record.slot] != nullptr) {
            throw std::runtime_error("save writer: duplicate scene record slot");
        }
        scene_records[record.slot] = &record;
    }
    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        const auto* record = scene_records[slot];
        if (record == nullptr) {
            writer.u8(0xff);
            continue;
        }
        writer.u8(0x00);
        writer.cstring(record->name, "scene record name");
        writer.u16(record->word08);
        writer.fixed(record->bytes0a);
        writer.fixed(record->bytes0f);
        writer.fixed(record->bytes41);
        writer.fixed(record->bytes73);
        // FUN_0042658f's count is signed: 0x80..0xff mean "no entries". Keep
        // the parsed byte when it still describes the entries, so an absent
        // list stays 0xff exactly as the writer copied it.
        for (const auto* lists :
             {&record->list4c4, &record->list58c}) {
            for (std::size_t index = 0; index < lists->size(); ++index) {
                const auto& entries = (*lists)[index];
                const auto raw = (lists == &record->list4c4)
                                     ? record->list4c4_count[index]
                                     : record->list58c_count[index];
                const bool consistent =
                    (raw < 0x80 && raw == entries.size()) ||
                    (raw >= 0x80 && entries.empty());
                if (!consistent && entries.size() > 0x7f) {
                    throw std::runtime_error(
                        "save writer: link list exceeds 127 entries");
                }
                writer.u8(consistent ? raw
                                     : static_cast<std::uint8_t>(entries.size()));
                for (const auto& entry : entries) {
                    writer.u8(entry.value);
                    writer.u8(entry.code);
                }
            }
        }
        for (const auto* pointers : {&record->pointersA8, &record->pointers170,
                                     &record->pointers238}) {
            for (const auto& pointer : *pointers) {
                if (!pointer.present) {
                    writer.u8(0xff);
                    continue;
                }
                writer.u8(pointer.script_id);
                writer.u8(pointer.source_list);
                writer.u8(pointer.position);
            }
        }
        for (const auto& pair : record->slots3c8) {
            writer.u8(pair.present ? pair.first : 0xff);
            writer.u8(pair.second);
        }
        writer.fixed(record->bytes490);
        for (const auto& chain : record->chains654) {
            if (chain.size() > 0xff) {
                throw std::runtime_error("save writer: scene chain exceeds 255 entries");
            }
            writer.u8(static_cast<std::uint8_t>(chain.size()));
            for (const auto& entry : chain) {
                writer.u8(entry.name);
                writer.u8(entry.table_index);
            }
        }
    }

    if (save.node_values.size() != kSaveNodeValueCount) {
        throw std::runtime_error("save writer: variable pool is not the 3000-word table");
    }
    for (const auto value : save.node_values) writer.u16(value);

    writer.u32(save.field_45d1c0);
    writer.u32(save.field_45d1c4);
    writer.u32(save.field_45d1bc);
    for (const auto depth : save.dialogue_depths) {
        if (depth > kSaveDialogueStackDepth) {
            throw std::runtime_error("save writer: inventory depth outside 350");
        }
        writer.u32(depth);
    }
    for (const auto id : save.dialogue_stacks) writer.u32(id);
    writer.i32(save.dialogue_index);
    if (save.dialogue_marks.size() > kSaveSlotCount) {
        throw std::runtime_error("save writer: dialogue marks exceed 0x15e");
    }
    for (const auto& [flag, value] : save.dialogue_marks) {
        writer.u8(flag);
        writer.u8(value);
    }
    writer.u8(0xff);

    writer.u32(save.field_45f210);
    writer.u32(save.field_460668);
    writer.u32(save.field_46066c);
    if (save.field_45f214.empty()) {
        writer.u8(0);
    } else {
        writer.cstring(save.field_45f214, "0x45f214 string");
    }
    writer.u32(save.field_45c060);
    if (save.field_45c060 == 0) {
        writer.u16(save.field_48253e);
    } else {
        writer.u32(save.field_480330);
        writer.u32(save.field_45c064);
    }

    std::array<const SaveNodeSlot*, kSaveSlotCount> nodes{};
    for (const auto& node : save.nodes) {
        if (node.slot >= kSaveSlotCount) {
            throw std::runtime_error("save writer: node slot outside 0x15e");
        }
        if (nodes[node.slot] != nullptr) {
            throw std::runtime_error("save writer: duplicate node slot");
        }
        nodes[node.slot] = &node;
    }
    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        const auto* node = nodes[slot];
        if (node == nullptr) {
            writer.u8(0xff);
            continue;
        }
        // 0x4269ec writes 1 when the CA asset type is 0/1, 0 otherwise.
        writer.u8(node->marker == 0 ? 0 : 1);
        writer.fixed(node->raw);
        writer.u32(node->frame_index_1c);
        writer.u32(node->frame_index_20);
        writer.u32(node->field_2c);
        writer.u32(node->field_30);
        // 0x4269ec writes a count byte, then count * (u32, u32), but 0x426b9d
        // reads a *control* byte and 8 bytes while it is 1..127. The shipped
        // records always have an empty list, where both agree. To keep a list
        // with one or more entries loadable by the original reader, write its
        // framing: 1 + 8 bytes per entry, then a 0 terminator. The original
        // writer's count form would desynchronise 0x426b9d.
        if (node->extra.empty()) {
            writer.u8(0);
        } else {
            for (const auto& entry : node->extra) {
                writer.u8(1);
                writer.u32(entry.first);
                writer.u32(entry.second);
            }
            writer.u8(0);
        }
    }

    std::array<const SaveObjectRecord*, kSaveSlotCount> objects{};
    for (const auto& object : save.objects) {
        if (object.slot >= kSaveSlotCount) {
            throw std::runtime_error("save writer: object slot outside 0x15e");
        }
        if (objects[object.slot] != nullptr) {
            throw std::runtime_error("save writer: duplicate object slot");
        }
        objects[object.slot] = &object;
    }
    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        const auto* object = objects[slot];
        if (object == nullptr) {
            writer.u8(0xff);
            continue;
        }
        writer.u8(0x00);
        writer.fixed(object->raw);
    }

    writer.u32(save.field_45fd2c);
    std::array<const SaveSoundRecord*, kSaveSlotCount> sounds{};
    for (const auto& sound : save.sounds) {
        if (sound.slot >= kSaveSlotCount) {
            throw std::runtime_error("save writer: sound slot outside 0x15e");
        }
        if (sounds[sound.slot] != nullptr) {
            throw std::runtime_error("save writer: duplicate sound slot");
        }
        sounds[sound.slot] = &sound;
    }
    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        const auto* sound = sounds[slot];
        if (sound == nullptr) {
            writer.u8(0xff);
            continue;
        }
        writer.u8(0x00);
        writer.fixed(sound->raw);
    }
    // 0x427135 writes 0x00 before each chain record and 0xff after the last.
    for (const auto& entry : save.sound_chain) {
        writer.u8(0x00);
        writer.bytes(entry.data(), entry.size());
    }
    writer.u8(0xff);

    std::array<const SaveMediaRecord*, kSaveSlotCount> media{};
    for (const auto& record : save.media) {
        if (record.slot >= kSaveSlotCount) {
            throw std::runtime_error("save writer: media slot outside 0x15e");
        }
        if (media[record.slot] != nullptr) {
            throw std::runtime_error("save writer: duplicate media slot");
        }
        media[record.slot] = &record;
    }
    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        const auto* record = media[slot];
        if (record == nullptr) {
            writer.u8(0xff);
            continue;
        }
        writer.u8(0x00);
        writer.fixed(record->raw);
    }

    writer.u32(save.current_scene);

    std::array<const SaveDialogueChain*, kSaveSlotCount> chains{};
    for (const auto& chain : save.dialogue_chains) {
        if (chain.slot >= kSaveSlotCount) {
            throw std::runtime_error("save writer: dialogue chain slot outside 0x15e");
        }
        if (chains[chain.slot] != nullptr) {
            throw std::runtime_error("save writer: duplicate dialogue chain slot");
        }
        chains[chain.slot] = &chain;
    }
    for (std::size_t slot = 0; slot < kSaveSlotCount; ++slot) {
        const auto* chain = chains[slot];
        if (chain == nullptr) {
            writer.u8(0xff);
            continue;
        }
        writer.u8(0x00);
        writer.fixed(chain->raw);
    }

    // 0x42118c copies savegame/notebook.000 line by line; the parser's
    // validation is the same one the reader applies.
    const std::vector<std::uint8_t> notebook(save.notebook.begin(),
                                             save.notebook.end());
    validate_notebook(notebook);
    writer.bytes(notebook.data(), notebook.size());
    return writer.take();
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
