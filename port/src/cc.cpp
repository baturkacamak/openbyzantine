#include "byzantine/cc.hpp"
#include "byzantine/variables.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace byzantine {
namespace {

// Recovered from BETRAYAL.EXE at 0x460466 + opcode. Ghidra's FUN_00412762
// uses this table to determine each scene instruction's argument count.
// Evidence: ev_961516329fda54629fb59b3d818cb49b28be3122a552ef037259a378fdc01a23
// and ev_0c452b2e3ab8886e9627eec37b95ff26707255cc17cf2aa7718d5ad766e61e32.
constexpr std::array<std::uint8_t, 176> kArgumentCounts = {
    1, 1, 2, 2, 1, 1, 1, 0, 6, 1, 2, 1, 1, 1, 1, 3,
    3, 3, 3, 3, 4, 1, 3, 3, 1, 1, 3, 3, 2, 1, 2, 3,
    4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 0,
    0, 0, 5, 7, 3, 2, 2, 2, 4, 1, 1, 8, 8, 0, 2, 3,
    8, 3, 2, 2, 1, 3, 8, 0, 2, 2, 3, 2, 2, 2, 2, 2,
    1, 1, 3, 8, 8, 4, 4, 2, 3, 2, 1, 1, 1, 2, 1, 2,
    3, 3, 3, 2, 3, 3, 3, 3, 5, 1, 8, 3, 3, 3, 3, 3,
    3, 2, 2, 1, 1, 0, 0, 1, 1, 3, 3, 3, 3, 3, 4, 4,
    4, 4, 0, 0, 2, 0, 0, 4, 1, 1, 8, 4, 1, 4, 4, 2,
    4, 2, 8, 2, 8, 8, 8, 0, 0, 2, 2, 2, 2, 8, 0, 3,
    0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

std::uint8_t read_byte(const std::vector<std::uint8_t>& file, std::size_t& at) {
    if (at >= file.size()) {
        throw std::runtime_error("truncated CC instruction");
    }
    return file[at++];
}

std::size_t argument_bytes(std::uint8_t type) {
    switch (type) {
    case 0: return 0;
    case 1:
    case 4: return 1;
    case 2:
    case 8:
    case 16:
    case 24:
    case 64: return 2;
    case 32: return 4;
    default: throw std::runtime_error("unknown CC argument type");
    }
}

std::uint16_t constant_word(const CcArgument& argument) {
    if ((argument.type != 2 && argument.type != 64) ||
        argument.encoded_value.size() != 2) {
        throw std::runtime_error("expected constant CC word");
    }
    return static_cast<std::uint16_t>(
        argument.encoded_value[0] | (argument.encoded_value[1] << 8));
}

std::uint16_t encoded_word(const CcArgument& argument) {
    if (argument.encoded_value.size() != 2) {
        throw std::runtime_error("expected 16-bit CC argument");
    }
    return static_cast<std::uint16_t>(
        argument.encoded_value[0] | (argument.encoded_value[1] << 8));
}

// The raw u16 the original reads from the operand slot, without the type-2
// variable resolution 0x4319b3 performs. FUN_00412762 initialises both halves
// of a slot to 0x00ff: type 0 has no data (0xffff), a one-byte type keeps the
// byte in the low half, and a two-byte type is little-endian.
std::uint16_t slot_word(const CcArgument& argument) {
    if (argument.encoded_value.empty()) return 0xffff;
    if (argument.encoded_value.size() == 1) return argument.encoded_value[0];
    return static_cast<std::uint16_t>(
        argument.encoded_value[0] | (argument.encoded_value[1] << 8));
}

std::optional<std::uint16_t> scene_value(
    const CcArgument& argument,
    const std::map<std::uint16_t, std::uint16_t>& values) {
    const auto encoded = encoded_word(argument);
    // FUN_004319b3 resolves type 2 through the scene's variable table; every
    // other two-byte operand is read literally. Within one CC script its raw
    // operand is a stable key, so the portable trace retains that key instead
    // of pretending to know the original process-global variable address.
    if (argument.type != 2) {
        return encoded;
    }
    const auto found = values.find(encoded);
    if (found == values.end()) {
        return std::nullopt;
    }
    return found->second;
}

// One variable store (0x46ee00 maps the local operand to DAT_0047e2f0). The
// local is also appended to `variable_writes`, because export_variable_pool must
// replay the writes in order when two locals of the scene share a global.
void write_variable(SceneState& state, std::uint16_t local, std::uint16_t value) {
    state.variables[local] = value;
    state.variable_writes.push_back(local);
}

}  // namespace

std::uint8_t cc_argument_count(std::uint8_t opcode) {
    // 0x460466 + opcode is indexed directly by the opcode byte (0x32..0xe1).
    if (opcode < 0x32 || opcode > 0xe1) {
        throw std::runtime_error("CC opcode outside the argument-count table");
    }
    return kArgumentCounts[opcode - 0x32];
}

std::optional<std::uint16_t> read_local_variable(const SceneState& state,
                                                 std::uint16_t local) {
    const auto found = state.variables.find(local);
    if (found == state.variables.end()) return std::nullopt;
    return found->second;
}

void write_local_variable(SceneState& state, std::uint16_t local,
                          std::uint16_t value) {
    write_variable(state, local, value);
}

namespace {

// 0x460418 / 0x460428: byte tables applied to the actor flag byte by 0x432904
// (opcode 0x85) and used by 0x4324b0 (0x64) to derive the kind byte. Objdump
// confirms byte loads (mov 0x460418(%edx),%al); scripts index them with 0..11.
constexpr std::array<std::uint8_t, 16> kActorMaskAnd = {
    0xfe, 0xfe, 0xfd, 0xfd, 0xfb, 0xfb, 0xf7, 0xf7,
    0xef, 0xef, 0xdf, 0xdf, 0xbf, 0xbf, 0x7f, 0x7f};
constexpr std::array<std::uint8_t, 16> kActorMaskOr = {
    0x00, 0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x08,
    0x00, 0x10, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80};

std::size_t actor_mask_index(const CcArgument& argument) {
    const auto index = encoded_word(argument);
    if (index >= kActorMaskAnd.size()) {
        throw std::runtime_error("actor mask index outside the recovered tables");
    }
    return index;
}

// 0x410078 -> 0x40ffee(record, frame_count) plus the first frame's offsets:
// `record->x += header.x`, `record->y += header.y`, +0x28 = the file signature
// byte, +0x18/+0x1c = the freshly allocated head node (i.e. frame 0).
void apply_anims(SceneRecordState& record, const AnimsAsset& asset) {
    record.frame_count = static_cast<std::uint16_t>(asset.frames.size());
    record.anims_signature = static_cast<std::uint16_t>(asset.signature & 0xff);
    record.frames = asset.frames;
    record.frames_known = true;
    if (asset.frames.empty()) return;
    record.x = static_cast<std::int16_t>(record.x + asset.frames[0].x_offset);
    record.y = static_cast<std::int16_t>(record.y + asset.frames[0].y_offset);
    record.current_frame = 0;
}

std::string ascii_lower(std::string text) {
    for (char& character : text) {
        if (character >= 'A' && character <= 'Z') {
            character = static_cast<char>(character - 'A' + 'a');
        }
    }
    return text;
}

struct ResolvedName {
    std::optional<std::uint16_t> id;
    // 0x408bb2/0x408c81 return 0 after logging when a name is absent from a
    // non-empty active list.
    bool fallback = false;
};

// Shared shape of 0x408bb2 (CI -> DAT_00482540) and 0x408c81 (CY -> DAT_0047a8b0):
// the operand indexes the scene record table, the record name is compared
// case-insensitively with the active list and that list index becomes the id.
// Both only walk the original array bounds: 0x15e names for dialogue, 10 for the
// channel list.
ResolvedName resolve_name_index(const std::vector<NamedRecord>& list,
                                const std::vector<NamedRecord>& table,
                                bool table_known,
                                std::uint16_t name_id,
                                const char* table_name,
                                std::size_t list_limit) {
    ResolvedName resolved;
    if (list.empty()) {
        // The original only takes this identity path when the active list was
        // never loaded; the raw id is useless without the record table.
        if (table_known) resolved.id = name_id;
        return resolved;
    }
    if (!table_known) {
        return resolved;
    }
    if (name_id >= table.size()) {
        throw std::runtime_error(std::string("script name id outside the ") + table_name +
                                 " record table");
    }
    const auto wanted = ascii_lower(table[name_id].name);
    const auto limit = std::min(list.size(), list_limit);
    for (std::size_t index = 0; index < limit; ++index) {
        if (ascii_lower(list[index].name) == wanted) {
            resolved.id = static_cast<std::uint16_t>(index);
            return resolved;
        }
    }
    resolved.fallback = true;
    resolved.id = 0;
    return resolved;
}

// 0x408662 allocates one record per global dialogue-list name; when that list was
// never loaded, 0x42c8a0 allocates the per-scene CI count instead.
std::size_t dialogue_capacity(const SceneDialogueTables& tables) {
    return tables.names.empty() ? tables.scene_names.size() : tables.names.size();
}

// 0x408bb2/0x408c81 can only return an id the original's arrays cover. A larger
// id would read a null DAT_00486f90 slot, which the game would crash on.
SceneDialogueRecord& dialogue_record(SceneState& state, std::uint16_t id) {
    if (id >= dialogue_capacity(state.dialogue_tables)) {
        throw std::runtime_error("dialogue id " + std::to_string(id) +
                                 " outside the " +
                                 std::to_string(dialogue_capacity(state.dialogue_tables)) +
                                 " allocated dialogue records");
    }
    return state.dialogue_records[id];
}

std::vector<std::uint16_t>& dialogue_channel(SceneState& state, std::uint16_t channel) {
    if (channel >= state.dialogue_channels.size()) {
        throw std::runtime_error("dialogue channel outside the ten channel stacks");
    }
    return state.dialogue_channels[channel];
}

// 0x407254 truncates the record name to 8 characters (FUN_0044c630), loads
// inventor/<name>.tga and then inventor/hotitems/<name with a leading 'h'>.tga,
// and succeeds only when both loads do. The name comes from DAT_00482540[id]
// (the global list) or, when that slot is empty, DAT_00485400[id] (the scene CI
// table). The media listing is target data, so an unevaluated load reports
// "unknown" instead of a guess.
std::optional<bool> dialogue_inventory_ready(const SceneState& state, std::uint16_t id) {
    if (!state.inventory_assets_known) return std::nullopt;
    const auto& tables = state.dialogue_tables;
    std::string name;
    if (id < tables.names.size()) {
        name = tables.names[id].name;
    } else if (id < tables.scene_names.size()) {
        name = tables.scene_names[id].name;
    }
    if (name.size() > 8) name.resize(8);
    if (name.empty()) return false;
    auto hotitem = name;
    hotitem[0] = 'h';
    return state.inventor_assets.count(ascii_lower(name)) != 0 &&
           state.hotitem_assets.count(ascii_lower(hotitem)) != 0;
}

// 0x4077b2: mark the line active, push it on the owning channel stack when its
// inventor media loads, then refresh the current channel. Returns false when the
// media load cannot be evaluated, after the state byte has been written exactly
// as the original writes it before calling 0x407254.
bool dialogue_start(SceneState& state, SceneTrace& trace, std::uint16_t channel,
                    std::uint16_t id) {
    auto& record = dialogue_record(state, id);
    if (record.state == 1) return true;
    record.state = 1;
    if (state.current_dialogue_record && *state.current_dialogue_record == id) {
        // 0x407b9c(0): the current line becomes void and is redrawn empty.
        state.current_dialogue_record.reset();
        ++trace.dialogue_display_requests;
    } else {
        bool ready = record.inventory_loaded;
        if (!ready) {
            const auto loaded = dialogue_inventory_ready(state, id);
            if (!loaded) {
                trace.unresolved_dialogue_inventory = true;
                return false;
            }
            ready = *loaded;
            if (ready) record.inventory_loaded = true;
        }
        if (ready) {
            auto& stack = dialogue_channel(state, channel);
            if (stack.size() >= 0x15e) {
                throw std::runtime_error("dialogue channel stack is full");
            }
            stack.insert(stack.begin(), id);
        }
    }
    if (state.current_dialogue_channel == channel) ++trace.dialogue_refresh_requests;
    return true;
}

// 0x407933: ignore states 0 and 2, clear the current line for state 3, remove the
// first stacked entry for this id and shrink the scroll offset when the channel
// stack drops below the visible window (DAT_0045d1bc + 6).
void dialogue_stop(SceneState& state, SceneTrace& trace, std::uint16_t channel,
                   std::uint16_t id) {
    auto& record = dialogue_record(state, id);
    if (record.state == 0 || record.state == 2) return;
    if (record.state == 3) {
        state.current_dialogue_record.reset();
        ++trace.dialogue_display_requests;
    }
    auto& stack = dialogue_channel(state, channel);
    const auto found = std::find(stack.begin(), stack.end(), id);
    if (found != stack.end()) stack.erase(found);
    if (state.dialogue_scroll != 0 &&
        static_cast<std::int32_t>(stack.size()) < state.dialogue_scroll + 6) {
        --state.dialogue_scroll;
    }
    if (state.current_dialogue_channel == channel) ++trace.dialogue_refresh_requests;
}

// 0x407ad5: make the record current (0x407b9c), return every stacked line that is
// still in state 3 to state 1, then mark this record as the pending prompt.
void dialogue_promote(SceneState& state, SceneTrace& trace, std::uint16_t id) {
    state.current_dialogue_record = id;
    ++trace.dialogue_display_requests;  // 0x407b9c -> FUN_0040df05
    for (auto& stack : state.dialogue_channels) {
        for (const auto entry : stack) {
            auto& other = dialogue_record(state, entry);
            if (other.state == 3) other.state = 1;
        }
    }
    dialogue_record(state, id).state = 3;
    ++trace.dialogue_refresh_requests;  // FUN_00407420(1)
}

}  // namespace

std::vector<CcInstruction> parse_cc(const std::vector<std::uint8_t>& file) {
    if (file.size() < 2) {
        throw std::runtime_error("truncated CC header");
    }
    const auto count = static_cast<std::uint16_t>(file[0] | (file[1] << 8));
    std::size_t at = 2;
    std::vector<CcInstruction> instructions;
    instructions.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        CcInstruction instruction;
        instruction.opcode = read_byte(file, at);
        if (instruction.opcode < 0x32 || instruction.opcode > 0xe1) {
            throw std::runtime_error("CC opcode outside the game's known range");
        }
        const auto arguments = kArgumentCounts[instruction.opcode - 0x32];
        for (std::uint8_t j = 0; j < arguments; ++j) {
            CcArgument argument;
            argument.type = read_byte(file, at);
            const auto length = argument_bytes(argument.type);
            if (at > file.size() || file.size() - at < length) {
                throw std::runtime_error("truncated CC argument");
            }
            argument.encoded_value.assign(file.begin() + at, file.begin() + at + length);
            at += length;
            instruction.arguments.push_back(std::move(argument));
        }
        instructions.push_back(std::move(instruction));
    }
    if (at != file.size()) {
        throw std::runtime_error("unexpected bytes after CC instructions");
    }
    return instructions;
}

std::vector<SceneHotspot> static_hotspots(const std::vector<CcInstruction>& script) {
    std::vector<SceneHotspot> hotspots;
    for (std::size_t index = 0; index < script.size(); ++index) {
        const auto& instruction = script[index];
        if (instruction.opcode != 0x72 || instruction.arguments.size() != 8) {
            continue;
        }
        std::array<std::uint16_t, 7> values{};
        bool constant = true;
        for (std::size_t operand = 0; operand < values.size(); ++operand) {
            const auto& argument = instruction.arguments[operand];
            if ((argument.type != 2 && argument.type != 64) ||
                argument.encoded_value.size() != 2) {
                constant = false;
                break;
            }
            values[operand] = static_cast<std::uint16_t>(
                argument.encoded_value[0] | (argument.encoded_value[1] << 8));
        }
        if (!constant) {
            continue;
        }
        SceneHotspot hotspot;
        hotspot.instruction = index;
        hotspot.slot = values[0];
        hotspot.x = static_cast<std::int16_t>(values[1]);
        hotspot.y = static_cast<std::int16_t>(values[2] + 20);
        hotspot.width = static_cast<std::int16_t>(values[3]);
        hotspot.height = static_cast<std::int16_t>(values[4]);
        hotspot.action = values[5];
        hotspot.mode = values[6];
        hotspots.push_back(hotspot);
    }
    return hotspots;
}

std::vector<SceneBackground> static_backgrounds(
    const std::vector<CcInstruction>& script,
    const std::vector<BackgroundEntry>& entries) {
    std::vector<SceneBackground> backgrounds;
    for (std::size_t index = 0; index < script.size(); ++index) {
        const auto& instruction = script[index];
        if (instruction.opcode != 0x69 || instruction.arguments.empty()) {
            continue;
        }
        const auto& argument = instruction.arguments[0];
        if ((argument.type != 2 && argument.type != 64) ||
            argument.encoded_value.size() != 2) {
            continue;
        }
        const auto id = static_cast<std::uint16_t>(
            argument.encoded_value[0] | (argument.encoded_value[1] << 8));
        if (id >= entries.size()) {
            throw std::runtime_error("CC background ID outside CB record");
        }
        backgrounds.push_back(SceneBackground{index, id, entries[id]});
    }
    return backgrounds;
}

std::vector<ScenePanorama> static_panoramas(
    const std::vector<CcInstruction>& script,
    const std::vector<BackgroundEntry>& entries) {
    std::vector<ScenePanorama> result;
    for (std::size_t index = 0; index < script.size(); ++index) {
        const auto& instruction = script[index];
        if (instruction.opcode != 0xb9) continue;
        // FUN_00436652 reads the encoded ID directly, including type 2;
        // this operand is not resolved through the variable table.
        const auto id = constant_word(instruction.arguments.at(0));
        if (id >= entries.size()) throw std::runtime_error("CC panorama ID outside CO record");
        const auto& entry = entries[id];
        if (entry.name.size() < 2 || entry.name.find_first_of("/\\:") != std::string::npos ||
            entry.name.find("..") != std::string::npos) {
            throw std::runtime_error("invalid panorama asset name");
        }
        result.push_back({index, id, entry,
                          "nodes/" + entry.name.substr(0, 2) + "/" + entry.name + ".jpg"});
    }
    return result;
}

CcControlFlow resolve_control_flow(const std::vector<CcInstruction>& script) {
    CcControlFlow flow;
    for (std::size_t index = 0; index < script.size(); ++index) {
        const auto& instruction = script[index];
        if (instruction.opcode == 0xe1) {
            const auto label = constant_word(instruction.arguments.at(0));
            if (!flow.labels.emplace(label, index + 1).second) {
                ++flow.duplicate_labels;
                flow.labels[label] = index + 1;
            }
        } else if (instruction.opcode == 0x6f || instruction.opcode == 0x83) {
            ++flow.returns;
        }
    }
    for (std::size_t index = 0; index < script.size(); ++index) {
        const auto& instruction = script[index];
        if (instruction.opcode == 0x76 || instruction.opcode == 0x9b) {
            const auto label = constant_word(instruction.arguments.at(0));
            const auto target = flow.labels.find(label);
            if (target == flow.labels.end()) {
                throw std::runtime_error("unresolved CC control-flow label");
            }
            flow.transfers.push_back(CcTransfer{index, instruction.opcode, label,
                                                target->second});
        }
    }
    return flow;
}

SceneRecordPreload preload_scene_records(SceneState& state) {
    // 0x40fd7b (see the header): free the table, then load every CA type 2/3
    // entry. 0x42d52f has already turned the file's 0xff marker into type 0, so
    // only these two types are preloaded; types 0/1 stay lazy (0x431b0f).
    SceneRecordPreload result;
    state.records.clear();
    if (!state.actor_assets_known) return result;
    for (std::size_t id = 0; id < state.actor_assets.size(); ++id) {
        const auto type = state.actor_assets[id].type;
        if (type != 2 && type != 3) continue;
        SceneRecordState record;
        if (state.anims_library) {
            const auto asset = state.anims_library->load(state.actor_assets[id].name);
            if (!asset) {
                ++result.failed;
                continue;
            }
            apply_anims(record, *asset);
        }
        state.records[static_cast<std::uint16_t>(id)] = std::move(record);
        ++result.created;
    }
    return result;
}

std::size_t load_save_record_assets(SceneState& state) {
    if (!state.anims_library || !state.actor_assets_known) return 0;
    std::size_t loaded = 0;
    for (auto& [id, record] : state.records) {
        // 0x426b9d: marker 0 is "record only", marker 1 also calls 0x40fe70.
        if (record.marker != 1) continue;
        if (id >= state.actor_assets.size()) continue;
        const auto asset = state.anims_library->load(state.actor_assets[id].name);
        if (!asset) continue;
        // 0x410078's existing-record branch (param_2[0] != NULL): fill the
        // frame list and counts, do not add the frame-0 x/y offsets again (the
        // save already has them) and keep the saved current index.
        record.frame_count = static_cast<std::uint16_t>(asset->frames.size());
        record.anims_signature = static_cast<std::uint16_t>(asset->signature & 0xff);
        record.frames = asset->frames;
        record.frames_known = true;
        if (record.current_frame && *record.current_frame >= record.frame_count) {
            record.current_frame = record.frame_count == 0
                                       ? std::nullopt
                                       : std::optional<std::uint16_t>{0};
        }
        ++loaded;
    }
    return loaded;
}

void advance_record_animation(SceneRecordState& record) {
    const auto snapshot = [&record]() {
        if (record.current_frame) {
            record.frame_index_20 = *record.current_frame;
        }
    };
    if ((record.flags & 0x04) != 0) {
        // 0x43bb77's first branch: a frozen record only refreshes the +0x20
        // frame the snap pass would draw; the frame list does not move.
        snapshot();
        return;
    }
    if (record.field06 != 0) {
        // 0x43bb77's countdown: subtract DAT_004859fc (0x42c8a0 sets it to 1)
        // and clamp a borrowed value back to 0.
        record.field06 = static_cast<std::int16_t>(record.field06 - 1);
        if (record.field06 < 0) record.field06 = 0;
        return;
    }
    record.field06 = record.field04;
    snapshot();
    const auto count = record.frame_count;
    if (!record.current_frame || count < 2 || *record.current_frame >= count) return;
    const auto index = static_cast<std::uint16_t>(*record.current_frame);
    const auto last = static_cast<std::uint16_t>(count - 1);
    if ((record.flags & 0x02) == 0) {
        // Forward leg. `node->next == record->+0x18` means the current node is
        // the tail of the circular list.
        if (index == last) {
            if ((record.flags & 0x08) != 0) record.flags |= 0x04;
            if ((record.flags & 0x01) == 0) {
                if ((record.flags & 0x04) == 0) record.current_frame = 0;
            } else {
                record.current_frame = static_cast<std::uint16_t>(count - 2);
                record.flags |= 0x02;
            }
        } else {
            record.current_frame = static_cast<std::uint16_t>(index + 1);
        }
        return;
    }
    // Reverse leg. The node index 0 is the turnaround (`node + 0x14 == 0`).
    if (index == 0) {
        if ((record.flags & 0x01) == 0) {
            if ((record.flags & 0x04) == 0) record.current_frame = last;
        } else {
            record.current_frame = 1;
            record.flags = static_cast<std::uint8_t>(record.flags & ~0x02);
        }
    } else {
        record.current_frame = static_cast<std::uint16_t>(index - 1);
    }
}

SceneCompose compose_scene(SceneState& state, Rgb16Surface& target) {
    SceneCompose result;
    // 0x43c114 + 0x40241c + 0x42b679 for a record at one position. `screen_y`
    // is the record's +0x02/+0x26 value (it carries the +0x14 surface bias).
    //
    // 0x43b130's frame node loop: it starts at `record + 0x18` (the list head)
    // instead of `record + 0x1c` (the current node) when `param_7 == 1` and the
    // animation signature at +0x28 is non-zero, then walks `node + 4` forward
    // drawing every node until it reaches the current one. That matters because
    // 0x40bbfd's command 3 leaves the destination alone: every animation frame
    // after the full head frame is a delta, so drawing only the current frame
    // on a freshly repainted layer would show fragments. The port repaints the
    // layer every frame (the original keeps one surface between frames) and
    // therefore uses the head walk, which is the original's own complete-draw
    // branch. `pixels`/`drawn` describe what the surface received.
    const auto draw_frame = [&](std::uint16_t id, const SceneRecordState& record,
                                const std::optional<std::uint16_t>& frame,
                                std::int32_t screen_x, std::int32_t screen_y,
                                bool snap) -> bool {
        SceneDraw entry;
        entry.id = id;
        entry.kind = record.kind;
        entry.snap = snap;
        if (!frame || !state.anims_library || id >= state.actor_assets.size()) {
            ++result.skipped_no_frame;
            result.records.push_back(entry);
            return false;
        }
        const auto& name = state.actor_assets[id].name;
        const auto current = *frame;
        const auto current_decoded = state.anims_library->decode(name, current);
        if (!current_decoded) {
            ++result.skipped_no_frame;
            result.records.push_back(entry);
            return false;
        }
        entry.width = current_decoded->width;
        entry.height = current_decoded->height;
        const auto x = screen_x;
        const auto y = screen_y - kPlayfieldTop;
        if (!target.inside(x, y)) {
            // 0x40241c rejects a record whose TOP-LEFT is outside the window;
            // it never draws a partially negative origin. Keep the attempt in
            // the report so an off-screen record is visible as such.
            entry.blit.x = x;
            entry.blit.y = y;
            entry.blit.covered = current_decoded->run_pixels;
            entry.blit.clipped = current_decoded->run_pixels;
            result.records.push_back(entry);
            return false;
        }
        const std::uint16_t first = record.anims_signature != 0 ? 0 : current;
        std::size_t written_total = 0;
        for (std::uint16_t node = first; node <= current; ++node) {
            const auto decoded = node == current
                                     ? current_decoded
                                     : state.anims_library->decode(name, node);
            if (!decoded) continue;
            const auto blit = byzantine::blit_frame(target, *decoded, x, y);
            written_total += blit.written;
            if (node == current) entry.blit = blit;
        }
        entry.blit.written = written_total;
        entry.blit.drawn = written_total > 0;
        entry.visible = true;
        result.pixels_written += written_total;
        if (entry.blit.drawn) ++result.drawn;
        result.records.push_back(entry);
        return true;
    };
    // 0x43b216 phase 1 (the snap pass), only when DAT_004859cd == 0 (not after
    // a save load) and the table is non-empty. A record is eligible when
    // (+0x09 & 0x20) is set, or when its CA type is 1/2 and flags bit 4 is
    // clear, and it has a +0x20 frame node. The original then draws it through
    // +0x24/+0x26, removes the matching +0x3c trail node (0x43264d), clears
    // +0x20/+0x22 and +0x29. The +0x29 byte is not carried by the port.
    if (!state.flag_4859cd && !state.records.empty()) {
        for (auto& [id, record] : state.records) {
            bool eligible = (record.flags & 0x20) != 0;
            if (!eligible && id < state.actor_assets.size()) {
                const auto type = state.actor_assets[id].type;
                eligible = (type == 1 || type == 2) && (record.flags & 4) == 0;
            }
            if (!eligible) continue;
            if (record.frame_index_20 == 0xffff) continue;  // +0x20 is NULL
            const auto frame = static_cast<std::uint16_t>(record.frame_index_20);
            if (draw_frame(id, record, frame, record.snap_x, record.snap_y, true)) {
                ++result.snap_drawn;
            }
            // 0x43264d removes the trail node at the record's current position.
            if (!record.trail.empty() &&
                record.trail.back() == std::make_pair(record.x, record.y)) {
                record.trail.pop_back();
            }
            record.frame_index_20 = 0xffff;
        }
    }
    // 0x43b216: `for (local_3c = 0; local_3c < 5; ++local_3c)` groups the table
    // by the +0x08 kind byte, and each group walks ids from the highest down.
    // The normal pass only reaches a record when `(+0x09 & 0x20) == 0` and
    // `(+0x09 & 0x40) == 0`: 0x20 marks a record for the separate snap pass
    // (which needs a non-zero +0x20 that 0x40ffee leaves at 0 and this port does
    // not model) and 0x40 skips it entirely. Drawing either here would paint
    // records the original never shows (the CA preload's 17 records all keep the
    // 0x20 default until a 0x64 writes their kind byte).
    for (std::uint8_t kind = 0; kind < 5; ++kind) {
        for (auto it = state.records.rbegin(); it != state.records.rend(); ++it) {
            auto& record = it->second;
            if (record.kind != kind) continue;
            if ((record.flags & 0x20) != 0) {
                ++result.skipped_snap;
                continue;
            }
            if ((record.flags & 0x40) != 0) {
                ++result.skipped_hidden;
                continue;
            }
            // 0x43b216 draws the record and then runs 0x43bb77 on the records
            // it actually drew: the frame only advances while the record is
            // visible. The head walk above rebuilds the complete picture from
            // the full frame, so the next step can draw a delta onto it.
            if (draw_frame(it->first, record, record.current_frame, record.x, record.y, false)) {
                advance_record_animation(record);
            }
        }
    }
    for (const auto& [id, record] : state.records) {
        (void)id;
        if (record.kind >= 5) ++result.skipped_other_kind;
    }
    return result;
}

// Runs `script` from `start_pc` until it returns/ends. Both public entry points
// below differ only in how they pick that address.
static SceneTrace trace_scene_from(const std::vector<CcInstruction>& script,
                                   const std::vector<BackgroundEntry>& entries,
                                   SceneState& state, std::size_t start_pc,
                                   std::size_t step_budget) {
    const auto flow = resolve_control_flow(script);
    std::map<std::uint16_t, SceneHotspot> hotspots;
    for (const auto& hotspot : static_hotspots(script)) {
        hotspots.emplace(hotspot.slot, hotspot);
    }
    std::vector<std::size_t> return_stack;
    SceneTrace trace;
    std::size_t pc = start_pc;
    // 0x431b0f(id, opcode): the guard every DAT_004649e8 command shares. The
    // original returns 0 only when the record exists, creating it through
    // 0x40fe70 for a CA type of 0/1; 0x40fd7b already created types 2/3 at
    // scene start. Any other type leaves the slot NULL and the dispatcher skips
    // the body. The port reproduces that decision from the shipped CA table;
    // the animation frames the load would supply are still not modelled.
    const auto resolve_record = [&](std::uint16_t id,
                                    std::size_t at) -> SceneRecordState* {
        const auto found = state.records.find(id);
        if (found != state.records.end()) return &found->second;
        if (!state.actor_assets_known) {
            trace.unresolved_node_asset = id;
            trace.stopped_at = at;
            pc = script.size();
            return nullptr;
        }
        if (id >= state.actor_assets.size()) {
            // 0x431b0f dereferences DAT_0047dd70[id] without a NULL check.
            trace.missing_node_asset = id;
            trace.stopped_at = at;
            pc = script.size();
            return nullptr;
        }
        const auto type = state.actor_assets[id].type;
        if (type <= 3) {
            // 0x40fd7b (2/3) or 0x431b0f -> 0x40fe70 (0/1) create the record
            // with 0x40ffee's defaults and, when the asset loads, its frame
            // count, first-frame offsets and frame geometry.
            if (state.anims_library) {
                const auto asset = state.anims_library->load(
                    state.actor_assets[id].name);
                if (!asset) {
                    // 0x40fe70 logs through 0x40ac43/0x40ac38 and leaves the
                    // slot NULL, so the command body is skipped.
                    ++trace.anims_load_failures;
                    return nullptr;
                }
                auto& record = state.records[id];
                apply_anims(record, *asset);
                ++trace.records_created;
                return &record;
            }
            ++trace.records_created;
            return &state.records[id];
        }
        // Neither creation path applies: the original logs and skips the body.
        ++trace.skipped_node_commands;
        return nullptr;
    };
    constexpr std::size_t kStepLimit = 10000;
    const std::size_t limit =
        step_budget != 0 && step_budget < kStepLimit ? step_budget : kStepLimit;
    while (pc < script.size() && trace.steps < limit) {
        const auto& instruction = script[pc];
        const auto current = pc;
        ++trace.steps;
        ++pc;
        switch (instruction.opcode) {
        case 0xe1:
            break;
        case 0x72: {
            // 0x42db22 case 0x72 creates DAT_00464300[slot] when the slot is
            // NULL: x/y+0x14/w/h, active=0, label=operand 5, kind=operand 6,
            // name id from operand 7. The static hotspot table already parsed
            // the same instruction; using it keeps the click rectangle and the
            // object table consistent. A save object or an earlier creation is
            // never overwritten (the original logs 0x40abb4 then).
            if (instruction.arguments.size() == 8) {
                const auto& slot_argument = instruction.arguments[0];
                if ((slot_argument.type == 2 || slot_argument.type == 64) &&
                    slot_argument.encoded_value.size() == 2) {
                    const auto slot = encoded_word(slot_argument);
                    if (state.objects.find(slot) == state.objects.end()) {
                        const auto found = hotspots.find(slot);
                        if (found != hotspots.end()) {
                            SceneObjectState object;
                            object.x = found->second.x;
                            object.y = found->second.y;
                            object.width = found->second.width;
                            object.height = found->second.height;
                            object.active = 0;
                            object.kind = static_cast<std::uint8_t>(found->second.mode);
                            object.label = found->second.action;
                            object.argument = 0xffffffffu;
                            object.name_id = 0xffffffffu;
                            // 0x42db22 case 0x72 reads operand 7 only when its
                            // type byte is 2. Kinds 0x13/0x14 store that word as
                            // a dialogue-chain id; every other kind passes it
                            // through 0x408bb2 (the per-scene CI name table) and
                            // stores the resulting dialogue-record id in +0x10.
                            // 0x407c5a then compares +0x10 with the selected
                            // inventory record. Without the CI table the id stays
                            // "unset" instead of being guessed.
                            const auto& name_argument = instruction.arguments[7];
                            if (name_argument.type == 2 &&
                                name_argument.encoded_value.size() == 2) {
                                const auto name_id = encoded_word(name_argument);
                                if (object.kind == 0x13 || object.kind == 0x14) {
                                    object.name_id = name_id;
                                } else if (state.dialogue_tables.scene_names_known &&
                                           name_id <
                                               state.dialogue_tables.scene_names.size()) {
                                    const auto resolved = resolve_name_index(
                                        state.dialogue_tables.names,
                                        state.dialogue_tables.scene_names, true, name_id,
                                        "CI", 0x15e);
                                    if (resolved.id) {
                                        object.name_id = *resolved.id;
                                        if (resolved.fallback) ++trace.dialogue_name_fallbacks;
                                    }
                                }
                            }
                            state.objects.emplace(slot, object);
                        }
                    }
                }
            }
            break;
        }
        case 0x7a:
            // 0x42db22 case 0x7a only copies operand 0 into the dispatcher's
            // scratch word; it has no other side effect. The hotspot rectangle
            // is the preceding 0x72 and the enable/disable is 0x6d/0x6e, so a
            // scene entry that ends each 0x72 with 0x7a needs this case to
            // reach the rest of the script.
            break;
        case 0x68: {
            // 0x42db22 case 0x68 (0x42dad9): the alternate-mode selector
            // DAT_00464878, which 0x42c8a0 initialises to -1 before the scene
            // loop. Without a transition snapshot (DAT_004603d0) the mode is 0
            // only when operand 1 is a type-2 zero; every other operand shape
            // leaves it at -1. The DAT_00460660 transition branch is not
            // modelled. 0x42dad9's redraw helpers are renderer work; the write
            // itself is retained and reported instead of being guessed.
            std::int32_t mode = -1;
            const auto& operand = instruction.arguments.at(1);
            if (operand.type == 2 && encoded_word(operand) == 0) mode = 0;
            if (mode != state.alternate_mode) {
                state.alternate_mode = mode;
                ++trace.alternate_mode_changes;
            }
            break;
        }
        case 0x69: {
            const auto id = constant_word(instruction.arguments.at(0));
            // 0x431ec2 passes operand 1 to 0x41045b. Code 4 is the examine
            // close-up: the image is inventor/examine/<stem>.jpg derived from
            // DAT_0045d1cc, not a CB background.
            std::uint16_t transition = 0xffff;
            if (instruction.arguments.size() > 1) {
                const auto& argument = instruction.arguments[1];
                if ((argument.type == 2 || argument.type == 64) &&
                    argument.encoded_value.size() == 2) {
                    transition = encoded_word(argument);
                }
            }
            if (transition == 4) {
                if (!state.current_use_item) {
                    trace.missing_examine_item = true;
                    trace.stopped_at = current;
                    pc = script.size();
                    break;
                }
                trace.examine_image = *state.current_use_item;
                trace.background.reset();
                break;
            }
            if (id >= entries.size()) {
                throw std::runtime_error("CC background ID outside CB record");
            }
            trace.background = SceneBackground{current, id, entries[id]};
            break;
        }
        case 0x67: {
            const auto value = scene_value(instruction.arguments.at(0), state.variables);
            if (!value) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(0));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            // The executable maps this per-scene operand through its variable
            // table before the write. A trace remains in one script, so using
            // the same encoded operand preserves all later local references.
            write_variable(state, encoded_word(instruction.arguments.at(1)), *value);
            break;
        }
        case 0x79: {
            // 0x42db22 case 0x79: DAT_0045f490 is the scene-exit reason, written
            // only when it is still 0. The case also sets the dispatcher result
            // (local_8) to 1 in main-script mode, and 0x42d594 turns 1 into
            // "store DAT_00484e28 and stop"; the current script therefore ends
            // here. 0x41e289 resumes that saved address when the caller script
            // continues, so this is a return to the caller, not a fallthrough.
            if (state.field_45f490 == 0) state.field_45f490 = 1;
            trace.script_end_requested = true;
            trace.stopped_at = current;
            pc = script.size();
            break;
        }
        case 0x7d: {
            // RANDOM (0x42db22 case 0x7d, logger string 0x45d420). Operand 1
            // is the range: type 2 -> another scene variable, type 0x40 ->
            // constant, any other type -> 0xffff. Operand 0 is the destination.
            // 0x431e89 calls _rand() only for a nonzero range, then stores the
            // low 16 bits of rand() % range; range 0 stores 0 without advancing
            // the stream.
            const auto& range_argument = instruction.arguments.at(1);
            std::uint16_t range = 0xffff;
            if (range_argument.type == 2) {
                const auto found = state.variables.find(encoded_word(range_argument));
                if (found == state.variables.end()) {
                    trace.unresolved_variable = encoded_word(range_argument);
                    trace.stopped_at = current;
                    pc = script.size();
                    break;
                }
                range = found->second;
            } else if (range_argument.type == 0x40) {
                range = constant_word(range_argument);
            }
            if (!state.random_state) {
                trace.unresolved_random = true;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            std::uint16_t roll = 0;
            if (range != 0) {
                state.random_state = *state.random_state * 0x343fdu + 0x269ec3u;
                roll = static_cast<std::uint16_t>((*state.random_state & 0x7fff0000u) >> 16);
            }
            write_variable(state, encoded_word(instruction.arguments.at(0)),
                           range == 0 ? 0 : static_cast<std::uint16_t>(roll % range));
            break;
        }
        case 0x7f: {
            // 0x42db22 case 0x7f: operand 0 is the RAW DAT_00464f60 slot and
            // operand 1 the RAW destination local. The value stored is
            // (dword +0x00) - (dword +0x04) truncated to 16 bits, i.e. the
            // media value minus its frame counter. Objdump at 0x430cfe shows a
            // 32-bit load and `sub` with `mov WORD [ebp-0x10],ax`; Ghidra's
            // (short) casts are not the operation. The dereference happens
            // BEFORE the NULL check, so an absent record faults in the original
            // and is an explicit stop here instead of a skipped write.
            const auto id = encoded_word(instruction.arguments.at(0));
            const auto found = state.media.find(id);
            if (found == state.media.end()) {
                trace.missing_media_record = id;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            const auto destination = encoded_word(instruction.arguments.at(1));
            write_variable(state, destination, static_cast<std::uint16_t>(
                static_cast<std::uint32_t>(found->second.value) - found->second.frame));
            break;
        }
        case 0x6a: {
            // 0x42db22 case 0x6a: define DAT_00464f60[id]. Operand 0 is the raw
            // slot, operand 1 is resolved through 0x4319b3, operand 2 is the raw
            // media id, and operand 3 only contributes its low byte as the type
            // when its type byte is 2. The record is allocated when absent; the
            // fields below are written unconditionally (an existing record is
            // reused). timeGetTime() is real process state and is not invented.
            const auto id = encoded_word(instruction.arguments.at(0));
            const auto value = scene_value(instruction.arguments.at(1), state.variables);
            if (!value) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            SceneMediaState& media = state.media[id];
            media.value = *value;
            media.media_id = encoded_word(instruction.arguments.at(2));
            media.active = 1;
            media.frame = 0;
            media.timestamp = 0;
            ++trace.media_timestamps_unavailable;
            media.type = instruction.arguments.at(3).type == 2
                ? static_cast<std::uint8_t>(encoded_word(instruction.arguments.at(3)) & 0xff)
                : 0;
            trace.media_definitions.push_back(
                SceneMediaDefinition{current, id, *value, media.media_id, media.type});
            break;
        }
        case 0x80: {
            // 0x42db22 case 0x80: DAT_00464f60[raw(0)]->+0x0c = raw(1). A
            // missing slot takes the 0x40abb4 path instead of a write.
            const auto id = encoded_word(instruction.arguments.at(0));
            const auto found = state.media.find(id);
            if (found == state.media.end()) {
                ++trace.missing_media_requests;
            } else {
                found->second.media_id = encoded_word(instruction.arguments.at(1));
                trace.media_id_writes.push_back(
                    SceneMediaIdWrite{current, id, found->second.media_id});
            }
            break;
        }
        case 0x6c: {
            // 0x42db22 case 0x6c: clear DAT_00464f60[id]+0x10. A missing object
            // takes the 0x40abb4 error path instead of a write.
            const auto id = encoded_word(instruction.arguments.at(0));
            const auto found = state.media.find(id);
            if (found == state.media.end()) {
                ++trace.missing_media_requests;
            } else {
                found->second.active = 0;
            }
            break;
        }
        case 0x70: {
            // 0x42db22 case 0x70 CONVERSE. The original runs the nested
            // conversation runner 0x432d6e and uses its result as the branch:
            //   0xffff -> pc stays after the 0x70
            //   0xfffe -> 0x42db22 sets the dispatcher's "stop" result
            //   other  -> an object id; pc becomes DAT_004654e0[object->+0x0c]
            // Operand 0 is the conversation-record index into engine/<scene>cd
            // (DAT_0046e870), NOT an object slot; 0x410f09 maps its name into
            // DAT_00485a10 and DAT_00482ad0 holds the loaded .ccb state. Operand
            // 1 is the object id the conversation starts with (0x432d6e's third
            // argument). The conversation itself needs its CCB/CCF/BYZ data and
            // player input, so the VM pauses here and the caller resumes with
            // apply_conversation_result(). The old operand-as-object lookup was
            // wrong and is removed.
            trace.conversation_request = SceneTrace::ConversationRequest{
                current, slot_word(instruction.arguments.at(0)),
                slot_word(instruction.arguments.at(1))};
            trace.stopped_at = current;
            pc = script.size();
            break;
        }
        case 0xcc: {
            // 0x42db22 case 0xcc: DAT_00464300[raw(0)]->+0x14 = raw(1). Both
            // operands are read through 0x436652 (the encoded word), NOT
            // resolved through 0x4319b3, so a type-2 operand here is still a
            // literal slot/value. The original dereferences the table with no
            // NULL check; an absent slot is an explicit error, never an
            // implicit allocation. The second operand's short result widens
            // through the original's uint assignment, so 0x8000..0xffff become
            // 0xffff8000..0xffffffff.
            const auto id = encoded_word(instruction.arguments.at(0));
            const auto encoded = encoded_word(instruction.arguments.at(1));
            const auto found = state.objects.find(id);
            if (found == state.objects.end()) {
                trace.missing_object = id;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            const auto value = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(static_cast<std::int16_t>(encoded)));
            found->second.argument = value;
            trace.object_writes.push_back(SceneObjectWrite{current, id, value});
            break;
        }
        case 0x65: {
            // 0x42db22 case 0x65: define DAT_00486220[id]. Operands 1 and 2 are
            // resolved through the variable table; 3, 4, 5 and 6 are raw.
            const auto id = encoded_word(instruction.arguments.at(0));
            SceneSoundState sound;
            sound.defined = true;
            const auto flag = scene_value(instruction.arguments.at(1), state.variables);
            if (!flag) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            sound.field14 = *flag == 0 ? 1 : 0;
            const auto field10 = scene_value(instruction.arguments.at(2), state.variables);
            if (!field10) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(2));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            sound.field10 = *field10;
            sound.field0c = encoded_word(instruction.arguments.at(3));
            sound.field08 = encoded_word(instruction.arguments.at(4));
            sound.field18 = encoded_word(instruction.arguments.at(5));
            // record[7] is operand 6's type byte; the value is its raw word.
            sound.enabled = instruction.arguments.at(6).type == 2
                ? encoded_word(instruction.arguments.at(6)) : 1;
            // The original clears enabled again when +0x18 == 0 and the runtime
            // sound-device check (FUN_0041d440(2)) reports disabled. That check
            // is environment state and is NOT evaluated here.
            state.sounds[id] = sound;
            break;
        }
        case 0xc1: {
            // 0x42db22 case 0xc1: write DAT_00486220[id]+0x30 from a resolved
            // operand; the sentinel 3000 is stored as -1.
            const auto value = scene_value(instruction.arguments.at(1), state.variables);
            if (!value) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            auto& sound = state.sounds[encoded_word(instruction.arguments.at(0))];
            sound.field30 = *value == 3000 ? 0xffff : *value;
            break;
        }
        case 0xbe:
        case 0xd2: {
            // 0x42db22 cases 0xbe/0xd2 -> 0x42515f(opcode): 0xbe writes
            // savegame\tempsave.btb and 0xd2 copies it to autosave.btb. The gate
            // is DAT_00460670 != -1 (the chapter has an examine scene) and
            // gamepref[0] == 1. Writing is a non-goal, so the request is
            // recorded and the script continues (the original has no script
            // state side effect beyond the flag below).
            if (!state.examine_scene_index) {
                trace.unresolved_save_setting = true;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (*state.examine_scene_index < 0) break;  // no examine scene
            if (!state.gameprefs) {
                trace.unresolved_save_setting = true;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if ((*state.gameprefs)[0] != 1) break;
            if (instruction.opcode == 0xbe) {
                ++trace.tempsave_requests;
            } else {
                ++trace.autosave_requests;
            }
            break;
        }
        case 0xbf: {
            // 0x42db22 case 0xbf: write DAT_00486220[id]+0x28 and +0x2c from
            // resolved operands 1/2, and +0x1c from operand 3 when that operand
            // is a scene variable (type 2). A constant operand 3 is ignored, as
            // in the original's byte-type test.
            std::optional<std::uint16_t> field28 =
                scene_value(instruction.arguments.at(1), state.variables);
            std::optional<std::uint16_t> field2c =
                scene_value(instruction.arguments.at(2), state.variables);
            std::optional<std::uint16_t> field1c;
            if (instruction.arguments.at(3).type == 2) {
                field1c = scene_value(instruction.arguments.at(3), state.variables);
            }
            if (!field28 || !field2c || (instruction.arguments.at(3).type == 2 && !field1c)) {
                const auto& unresolved = !field28 ? instruction.arguments.at(1)
                    : (!field2c ? instruction.arguments.at(2) : instruction.arguments.at(3));
                trace.unresolved_variable = encoded_word(unresolved);
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            auto& sound = state.sounds[encoded_word(instruction.arguments.at(0))];
            sound.field28 = *field28;
            sound.field2c = *field2c;
            if (field1c) sound.field1c = *field1c;
            break;
        }
        case 0xc0: {
            // 0x42db22 case 0xc0: write +0x1c from resolved operand 1, then
            // +0x20 and +0x24 when operands 2/3 are scene variables, and push the
            // new parameters to the audio objects through 0x427d1a.
            std::optional<std::uint16_t> field1c =
                scene_value(instruction.arguments.at(1), state.variables);
            std::optional<std::uint16_t> field20;
            std::optional<std::uint16_t> field24;
            if (instruction.arguments.at(2).type == 2) {
                field20 = scene_value(instruction.arguments.at(2), state.variables);
                if (instruction.arguments.at(3).type == 2) {
                    field24 = scene_value(instruction.arguments.at(3), state.variables);
                }
            }
            if (!field1c || (instruction.arguments.at(2).type == 2 && !field20) ||
                (instruction.arguments.at(3).type == 2 && !field24)) {
                const auto& unresolved = !field1c ? instruction.arguments.at(1)
                    : (!field20 ? instruction.arguments.at(2) : instruction.arguments.at(3));
                trace.unresolved_variable = encoded_word(unresolved);
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            auto& sound = state.sounds[encoded_word(instruction.arguments.at(0))];
            sound.field1c = *field1c;
            if (field20) sound.field20 = *field20;
            if (field24) sound.field24 = *field24;
            ++trace.sound_refresh_requests;
            break;
        }
        case 0xc2: {
            // 0x42db22 case 0xc2: write +0x20 and +0x24 from resolved operands
            // 1/2, +0x1c from a type-2 operand 3, then refresh the audio objects.
            std::optional<std::uint16_t> field20 =
                scene_value(instruction.arguments.at(1), state.variables);
            std::optional<std::uint16_t> field24 =
                scene_value(instruction.arguments.at(2), state.variables);
            std::optional<std::uint16_t> field1c;
            if (instruction.arguments.at(3).type == 2) {
                field1c = scene_value(instruction.arguments.at(3), state.variables);
            }
            if (!field20 || !field24 || (instruction.arguments.at(3).type == 2 && !field1c)) {
                const auto& unresolved = !field20 ? instruction.arguments.at(1)
                    : (!field24 ? instruction.arguments.at(2) : instruction.arguments.at(3));
                trace.unresolved_variable = encoded_word(unresolved);
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            auto& sound = state.sounds[encoded_word(instruction.arguments.at(0))];
            sound.field20 = *field20;
            sound.field24 = *field24;
            if (field1c) sound.field1c = *field1c;
            ++trace.sound_refresh_requests;
            break;
        }
        case 0xc4: {
            // 0x42db22 case 0xc4: activate every type-2 sound operand, start it
            // through 0x427de1(id, 1) (non-blocking; +0x14 selects Play loop)
            // and flag the "endgame" record by name.
            for (std::size_t i = 0; i < instruction.arguments.size(); ++i) {
                const auto& argument = instruction.arguments.at(i);
                if (argument.type != 2) {
                    continue;
                }
                const auto id = encoded_word(argument);
                auto& sound = state.sounds[id];
                if (sound.field14 != 0) {
                    sound.active = true;
                }
                if (!state.sound_table_known) {
                    trace.unresolved_sound_name = id;
                    trace.stopped_at = current;
                    pc = script.size();
                    break;
                }
                if (id < state.sound_records.size() &&
                    state.sound_records[id].name == "endgame") {
                    state.endgame_requested = true;
                    continue;
                }
                SceneSoundStart start;
                start.instruction = current;
                start.step = trace.steps;
                start.opcode = 0xc4;
                start.id = id;
                start.record_present = sound.defined;
                start.loop = sound.field14 != 0;
                if (sound.defined) {
                    if (sound.field18 != 0) {
                        start.allowed = true;
                    } else if (state.gameprefs) {
                        start.allowed = (*state.gameprefs)[2] != 0;
                    }
                }
                trace.sound_starts.push_back(std::move(start));
            }
            break;
        }
        case 0xbb: {
            // 0x42db22 case 0xbb -> 0x4073f0(raw(0)): select the dialogue channel
            // whose CY name resolves through 0x408c81, reset the scroll to 0 and
            // redraw (0x407420(0)). The operand is a raw CY name id, exactly as
            // 0xab's operand 2.
            const auto channel_name = encoded_word(instruction.arguments.at(0));
            const auto channel = resolve_name_index(
                state.dialogue_tables.channels, state.dialogue_tables.scene_channels,
                state.dialogue_tables.scene_channels_known, channel_name, "CY", 10);
            if (!channel.id) {
                trace.unresolved_dialogue_channel = channel_name;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (channel.fallback) ++trace.dialogue_name_fallbacks;
            state.current_dialogue_channel = *channel.id;
            state.dialogue_scroll = 0;
            ++trace.dialogue_refresh_requests;
            break;
        }
        case 0xc6: {
            // 0x42db22 case 0xc6: create/reset DAT_00486a10[id] (count +0x64 = 0,
            // 25 ids = 0xffffffff) and append resolved CI name ids 1..7.
            const auto id = encoded_word(instruction.arguments.at(0));
            auto& chain = state.dialogue_chains[id];
            for (std::size_t i = 1; i < instruction.arguments.size(); ++i) {
                const auto& argument = instruction.arguments.at(i);
                if (argument.type != 2) continue;
                const auto name_id = encoded_word(argument);
                const auto resolved = resolve_name_index(
                    state.dialogue_tables.names, state.dialogue_tables.scene_names,
                    state.dialogue_tables.scene_names_known, name_id, "CI", 0x15e);
                if (!resolved.id) {
                    trace.unresolved_dialogue = name_id;
                    trace.stopped_at = current;
                    pc = script.size();
                    break;
                }
                if (resolved.fallback) ++trace.dialogue_name_fallbacks;
                if (chain.count >= chain.ids.size()) {
                    throw std::runtime_error("dialogue chain exceeds its 25 entries");
                }
                chain.ids[chain.count++] = *resolved.id;
            }
            break;
        }
        case 0xc7: {
            // 0x42db22 case 0xc7: remove the resolved ids from an existing
            // DAT_00486a10 chain, compacting it and shrinking +0x64.
            const auto id = encoded_word(instruction.arguments.at(0));
            const auto found = state.dialogue_chains.find(id);
            if (found == state.dialogue_chains.end()) {
                trace.missing_dialogue_chain = id;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            auto& chain = found->second;
            for (std::size_t i = 1; i < instruction.arguments.size(); ++i) {
                const auto& argument = instruction.arguments.at(i);
                if (argument.type != 2) continue;
                const auto name_id = encoded_word(argument);
                const auto resolved = resolve_name_index(
                    state.dialogue_tables.names, state.dialogue_tables.scene_names,
                    state.dialogue_tables.scene_names_known, name_id, "CI", 0x15e);
                if (!resolved.id) {
                    trace.unresolved_dialogue = name_id;
                    trace.stopped_at = current;
                    pc = script.size();
                    break;
                }
                if (resolved.fallback) ++trace.dialogue_name_fallbacks;
                for (std::size_t index = 0; index < chain.count; ++index) {
                    if (chain.ids[index] != *resolved.id) continue;
                    for (std::size_t move = index; move + 1 < chain.count; ++move) {
                        chain.ids[move] = chain.ids[move + 1];
                    }
                    chain.ids[--chain.count] = 0xffffffffu;
                    break;  // 0x408bb2 returns one id per operand
                }
            }
            break;
        }
        case 0xc8: {
            // 0x42db22 case 0xc8 (decimal case 200): a sub-command chosen by
            // the resolved operand 0. 0x1e reseeds _rand from local time (the
            // stream is not reproducible), 0x1f queries input through
            // 0x42a579, 0x20 resets/activates the input flag, 0x32/0x33 write
            // DAT_00460668, and 0x36..0x3a request a status.000 write through
            // 0x427389. 0x21..0x31/0x34/0x35 are no-ops.
            const auto sub = scene_value(instruction.arguments.at(0), state.variables);
            if (!sub) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(0));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            SceneSystemCommand command;
            command.instruction = current;
            command.sub = *sub;
            if (*sub == 0x1f || *sub == 0x20) {
                const auto& argument = instruction.arguments.at(1);
                if (argument.type == 2 || argument.type == 0x40) {
                    command.argument = scene_value(argument, state.variables);
                } else if (argument.type == 0) {
                    // The node's unused operand slot is the 0xffff initialiser.
                    command.argument = 0xffff;
                }
            }
            trace.system_commands.push_back(command);
            if (*sub == 0x1e) {
                // 0x42a6aa: srand(local time); _rand(); 0x42913b(); input 0.
                state.dialogue_input_active = false;
                trace.unresolved_random = true;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (*sub == 0x1f) {
                // 0x42a579 returns the event/coordinate triplet; the original
                // only runs it when operands 2/3/4 are all scene variables.
                const bool typed = instruction.arguments.size() > 4 &&
                    instruction.arguments.at(2).type == 2 &&
                    instruction.arguments.at(3).type == 2 &&
                    instruction.arguments.at(4).type == 2;
                if (!typed) break;  // original no-op
                trace.unresolved_system_input = *sub;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (*sub == 0x20) {
                state.dialogue_input_active = true;
                break;
            }
            if (*sub == 0x32) {
                state.field_460668 = 1;
                break;
            }
            if (*sub == 0x33) {
                state.field_460668 = 2;
                break;
            }
            if (*sub == 0x3a) {
                state.field_45f490 = 2;
                // falls through to the 0x36..0x39 status request
            }
            if (*sub >= 0x36 && *sub <= 0x3a) {
                trace.status_write_requested = *sub;
                break;
            }
            // 0x21..0x31, 0x34, 0x35: no-op.
            break;
        }
        case 0xcb: {
            // 0x42db22 case 0xcb -> 0x402285(resolved(0), step). step is 10 by
            // default and only read when operand 1's type byte is 0x40.
            const auto id = scene_value(instruction.arguments.at(0), state.variables);
            if (!id) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(0));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            std::uint16_t step = 10;
            if (instruction.arguments.at(1).type == 0x40) {
                step = encoded_word(instruction.arguments.at(1));
            }
            trace.panorama_rotations.push_back(
                ScenePanoramaRotation{current, *id, step});
            break;
        }
        case 0xbc: {
            // 0x42db22 case 0xbc: deactivate every type-2 sound operand. The
            // original splits on +0x2c before calling 0x4281b1/0x4277ed; that
            // renderer/audio distinction is not modelled, only the flag and the
            // voice stop the player applies.
            for (std::size_t i = 0; i < instruction.arguments.size(); ++i) {
                const auto& argument = instruction.arguments.at(i);
                if (argument.type != 2) {
                    continue;
                }
                const auto id = encoded_word(argument);
                auto& sound = state.sounds[id];
                sound.active = false;
                SceneSoundStop stop;
                stop.instruction = current;
                stop.step = trace.steps;
                stop.id = id;
                stop.record_present = sound.defined;
                trace.sound_stops.push_back(stop);
            }
            break;
        }
        case 0xcd: {
            // 0x42db22 case 0xcd: FUN_00417a17 draws the text string
            // DAT_00483640[id] (handle through 0x44c580/0x44ba00) into the
            // locked surface. Record the request; no renderer here.
            trace.text_requests.push_back(SceneTextRequest{current,
                encoded_word(instruction.arguments.at(0)),
                encoded_word(instruction.arguments.at(1))});
            break;
        }
        case 0xd1: {
            // 0x42db22 case 0xd1 -> FUN_00428cbd(raw(0), raw(1), text, 0, -1, 0,
            // 0xffffffff, 0, 0xffffffff). raw(0) is the DAT_00486220 id, raw(1)
            // is the mode (0 = run the dialogue window while playing), and
            // operand 2 is a text id only when its type byte is 2. The original
            // blocks until playback/dialogue ends; the port records the request
            // and the +0x18/gamepref[2] gate instead of opening an audio device.
            SceneSoundStart start;
            start.instruction = current;
            start.step = trace.steps;
            start.id = encoded_word(instruction.arguments.at(0));
            start.mode = encoded_word(instruction.arguments.at(1));
            if (instruction.arguments.at(2).type == 2) {
                start.text = encoded_word(instruction.arguments.at(2));
            }
            const auto record = state.sounds.find(start.id);
            start.record_present = record != state.sounds.end() && record->second.defined;
            if (start.record_present) {
                if (record->second.field18 != 0) {
                    start.allowed = true;
                } else if (state.gameprefs) {
                    start.allowed = (*state.gameprefs)[2] != 0;
                }
            }
            trace.sound_starts.push_back(std::move(start));
            break;
        }
        case 0xcf: {
            // 0x42db22 case 0xcf: for every operand whose type byte is 2 call
            // 0x427a01(raw operand word). 0x427a01 queues a 0x5c-byte playback
            // session only when 0x428143 reports "not already playing",
            // DAT_00485c90[id] holds a DirectSound object (created by 0x427de1)
            // and the record's +0x14 is nonzero. The device state is not
            // modelled, so each request keeps the record fields it would copy.
            for (const auto& argument : instruction.arguments) {
                if (argument.type != 2) continue;
                SceneSoundRelease release;
                release.instruction = current;
                release.step = trace.steps;
                release.id = encoded_word(argument);
                const auto record = state.sounds.find(release.id);
                release.record_present =
                    record != state.sounds.end() && record->second.defined;
                if (release.record_present) release.field14 = record->second.field14;
                trace.sound_releases.push_back(release);
            }
            break;
        }
        case 0xd0: {
            // 0x42db22 case 0xd0 -> 0x4278ab: walk DAT_0046019c, stop and free
            // every playback session and set the list head to NULL. The port
            // has no session list, so the request is counted instead of being
            // treated as a no-op.
            ++trace.sound_stop_all_requests;
            trace.sound_stop_alls.push_back(SceneSoundStopAll{current, trace.steps});
            break;
        }
        case 0xd3: {
            // 0x42db22 case 0xd3: the nine-argument 0x428cbd form
            // (id, 0, -1, p4, p5, p6, p7, p8, p9). Mode 0 runs the dialogue
            // window with no text; p4/p6/p8 are millisecond timeouts and
            // p5/p7/p9 the follow-up node records 0x431ec2(record, 0x66, 0)
            // switches to when their timeout elapses. Operands 3 and 5 only
            // contribute when their type byte is 0x40. The transition itself is
            // not executed (the panorama layer is request-only), so the session
            // is retained with its gate.
            SceneSoundSession session;
            session.instruction = current;
            session.step = trace.steps;
            session.id = encoded_word(instruction.arguments.at(0));
            const auto first = scene_value(instruction.arguments.at(1), state.variables);
            if (!first) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            session.timeouts[0].defined = true;
            session.timeouts[0].milliseconds = *first;
            session.timeouts[0].follow_up = encoded_word(instruction.arguments.at(2));
            std::array<std::optional<std::uint16_t>, 2> declared;
            bool unresolved = false;
            for (std::size_t slot = 0; slot < declared.size(); ++slot) {
                const auto& operand = instruction.arguments.at(3 + slot * 2);
                if (operand.type != 0x40) continue;
                declared[slot] = scene_value(operand, state.variables);
                if (!declared[slot]) {
                    trace.unresolved_variable = encoded_word(operand);
                    trace.stopped_at = current;
                    pc = script.size();
                    unresolved = true;
                    break;
                }
            }
            if (unresolved) break;
            for (std::size_t slot = 0; slot < declared.size(); ++slot) {
                if (!declared[slot]) continue;
                auto& timeout = session.timeouts[slot + 1];
                timeout.defined = true;
                timeout.milliseconds = *declared[slot];
                timeout.follow_up = encoded_word(instruction.arguments.at(4 + slot * 2));
            }
            const auto record = state.sounds.find(session.id);
            session.record_present =
                record != state.sounds.end() && record->second.defined;
            if (session.record_present) {
                if (record->second.field18 != 0) {
                    session.allowed = true;
                } else if (state.gameprefs) {
                    session.allowed = (*state.gameprefs)[2] != 0;
                }
            }
            trace.sound_sessions.push_back(std::move(session));
            break;
        }
        case 0xd4: {
            // DAT_004859cd = 1 (the save loader sets the same flag after a load).
            state.flag_4859cd = true;
            break;
        }
        case 0xd5: {
            // DAT_004859cd = 0.
            state.flag_4859cd = false;
            break;
        }
        case 0xab: {
            // 0x42db22 case 0xab: operand 0 is a RAW CI name id resolved by
            // 0x408bb2, operand 1 the raw mode, operand 2 a RAW CY name id
            // resolved by 0x408c81. Mode 1 starts the line through 0x4077b2,
            // modes 0 and 2 end it through 0x407933, and the record's first byte
            // is then written with the mode regardless of the branch taken.
            const auto name_id = encoded_word(instruction.arguments.at(0));
            const auto mode = encoded_word(instruction.arguments.at(1));
            const auto channel_name = encoded_word(instruction.arguments.at(2));
            const auto resolved = resolve_name_index(
                state.dialogue_tables.names, state.dialogue_tables.scene_names,
                state.dialogue_tables.scene_names_known, name_id, "CI", 0x15e);
            if (!resolved.id) {
                trace.unresolved_dialogue = name_id;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (resolved.fallback) ++trace.dialogue_name_fallbacks;
            const auto channel = resolve_name_index(
                state.dialogue_tables.channels, state.dialogue_tables.scene_channels,
                state.dialogue_tables.scene_channels_known, channel_name, "CY", 10);
            if (!channel.id) {
                trace.unresolved_dialogue_channel = channel_name;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (channel.fallback) ++trace.dialogue_name_fallbacks;
            bool executed = true;
            if (mode == 1) {
                executed = dialogue_start(state, trace, *channel.id, *resolved.id);
            } else if (mode == 0 || mode == 2) {
                dialogue_stop(state, trace, *channel.id, *resolved.id);
            }
            if (!executed) {
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            dialogue_record(state, *resolved.id).state =
                static_cast<std::uint8_t>(mode);
            break;
        }
        case 0xb0:
        case 0xb1:
        case 0xb2:
        case 0xb3: {
            // 0x42db22 cases 0xb0..0xb3 compare the resolved record's first byte
            // with the RAW operand 1 and jump to operand 2's label when they are
            // equal (0xb0/0xb2) or different (0xb1/0xb3). 0xb2/0xb3 are the call
            // variants: FUN_0043194f saves a return address before the jump. The
            // byte is zero-extended, so a compare value above 255 never matches.
            const auto name_id = encoded_word(instruction.arguments.at(0));
            const auto expected = encoded_word(instruction.arguments.at(1));
            const auto resolved = resolve_name_index(
                state.dialogue_tables.names, state.dialogue_tables.scene_names,
                state.dialogue_tables.scene_names_known, name_id, "CI", 0x15e);
            if (!resolved.id) {
                trace.unresolved_dialogue = name_id;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (resolved.fallback) ++trace.dialogue_name_fallbacks;
            const auto state_byte = dialogue_record(state, *resolved.id).state;
            const bool matches = state_byte == expected;
            const bool invert = instruction.opcode == 0xb1 || instruction.opcode == 0xb3;
            if (invert ? !matches : matches) {
                const auto target = flow.labels.find(
                    constant_word(instruction.arguments.at(2)));
                if (target == flow.labels.end()) {
                    throw std::runtime_error("unresolved CC dialogue label");
                }
                if (instruction.opcode >= 0xb2) {
                    if (return_stack.size() >= 24) {
                        throw std::runtime_error("CC call stack limit exceeded");
                    }
                    return_stack.push_back(pc);
                }
                pc = target->second;
            }
            break;
        }
        case 0xb4: {
            // DAT_0045d1c0 = 0; FUN_004075a1(1) clears/redraws the message area.
            state.dialogue_visible = false;
            ++trace.dialogue_refresh_requests;
            break;
        }
        case 0xb5: {
            // DAT_0045d1c0 = 1; FUN_00407420(1) redraws the message window.
            state.dialogue_visible = true;
            ++trace.dialogue_refresh_requests;
            break;
        }
        case 0xb6: {
            // 0x42db22 case 0xb6: start the line on its channel (0x4077b2) and
            // then call 0x407ad5, which makes it current, returns every stacked
            // state-3 line to state 1 and marks this record 3.
            const auto name_id = encoded_word(instruction.arguments.at(0));
            const auto channel_name = encoded_word(instruction.arguments.at(1));
            const auto resolved = resolve_name_index(
                state.dialogue_tables.names, state.dialogue_tables.scene_names,
                state.dialogue_tables.scene_names_known, name_id, "CI", 0x15e);
            if (!resolved.id) {
                trace.unresolved_dialogue = name_id;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (resolved.fallback) ++trace.dialogue_name_fallbacks;
            const auto channel = resolve_name_index(
                state.dialogue_tables.channels, state.dialogue_tables.scene_channels,
                state.dialogue_tables.scene_channels_known, channel_name, "CY", 10);
            if (!channel.id) {
                trace.unresolved_dialogue_channel = channel_name;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (channel.fallback) ++trace.dialogue_name_fallbacks;
            if (!dialogue_start(state, trace, *channel.id, *resolved.id)) {
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            dialogue_promote(state, trace, *resolved.id);
            break;
        }
        case 0xbd: {
            // 0x42db22 case 0xbd: play the AVI named by the CF record at
            // DAT_00484330 (engine/<scene>cf, 0x412f77). Operand 0 is the raw
            // record index, operand 1 the raw mode, and operands 2/3 are read
            // only when their type byte is 0x40. FUN_0040d031 skips modes 2 and 3
            // when gamepref[5] / gamepref[4] are zero (FUN_0041d440), which is
            // why the preference array is required rather than assumed.
            const auto record_id = encoded_word(instruction.arguments.at(0));
            const auto video_mode = encoded_word(instruction.arguments.at(1));
            if (!state.video_table_known) {
                trace.unresolved_video_name = record_id;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (record_id >= state.video_records.size()) {
                throw std::runtime_error(
                    "video record id outside the CF table");
            }
            const bool gated = video_mode == 2 || video_mode == 3;
            if (gated && !state.gameprefs) {
                trace.unresolved_video_setting = true;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (gated && (*state.gameprefs)[video_mode == 2 ? 5 : 4] == 0) {
                ++trace.video_skipped;
                break;
            }
            SceneVideoRequest request;
            request.instruction = current;
            request.record = record_id;
            request.name = state.video_records[record_id].name;
            request.path = "avi/" + request.name + ".avi";
            request.mode = video_mode;
            if (instruction.arguments.at(2).type == 0x40) {
                request.start = encoded_word(instruction.arguments.at(2));
            }
            if (instruction.arguments.at(3).type == 0x40) {
                request.end = encoded_word(instruction.arguments.at(3));
            }
            // DAT_00460664 hands a panorama transition code to the next 0xb9.
            // Only the unconditional modes are recorded; mode 3's value depends
            // on DAT_00464878, which is save/UI state.
            if (video_mode == 0) request.transition = 0x65;
            else if (video_mode == 2) request.transition = 0x6a;
            else if (video_mode == 4) request.transition = 0x67;
            // Modes 1, 5 and the default leave DAT_00460664 untouched.
            if (request.transition) {
                state.pending_panorama_transition = *request.transition;
            }
            trace.video_requests.push_back(std::move(request));
            break;
        }
        case 0x64: {
            // 0x4324b0 via 0x42db22 case 0x64. Operand 1 is a RAW kind-table
            // index even though its CC type byte is 2; operands 2..4 are only
            // absent or constant in all 950 extracted occurrences.
            const auto& kind_argument = instruction.arguments.at(1);
            if (kind_argument.type == 0) {
                throw std::runtime_error("0x64 without a kind operand is unsupported");
            }
            // 0x42db22 `case 100:` (Ghidra prints 0x64 in decimal here) runs
            // the same 0x431b0f guard as the other record commands.
            auto* record = resolve_record(encoded_word(instruction.arguments.at(0)),
                                          current);
            if (!record) break;
            const auto kind = kActorMaskOr[actor_mask_index(kind_argument)];
            if (kind != 0x20 || (record->flags & 0x20) == 0) {
                // Objdump 0x43251d: `mov BYTE PTR [ecx+0x9],al` writes the
                // 0x460428 byte to +0x09, the same byte 0x85/0x86 mask and
                // 0x84 reads as its trail guard. +0x08 is only written by 0x95.
                record->flags = kind;
                if (instruction.arguments.at(2).type != 0) {
                    record->field04 = static_cast<std::int16_t>(
                        constant_word(instruction.arguments.at(2)));
                }
                if (instruction.arguments.at(3).type != 0) {
                    record->x = static_cast<std::int16_t>(
                        constant_word(instruction.arguments.at(3)));
                }
                if (instruction.arguments.at(4).type != 0) {
                    record->y = static_cast<std::int16_t>(
                        constant_word(instruction.arguments.at(4)) + 0x14);
                }
                record->field06 = 0;
                // 0x4324b0 (objdump 0x432594): +0x1c = +0x18, +0x20/+0x22 = 0,
                // +0x24/+0x26 = x/y — the frame list restarts at its head.
                record->snap_x = record->x;
                record->snap_y = record->y;
                if (record->frame_count != 0) record->current_frame = 0;
                ++record->frame_restart_requests;
            }
            break;
        }
        case 0x86: {
            // 0x42db22 case 0x86: 0x431b0f ensures the DAT_004649e8 record
            // exists (0x40fd7b/0x40fe70 from the CA asset), then 0x43299d
            // writes its +0x04 from the RAW operand 1 and 0x432904 applies the
            // 0x460418/0x460428 mask pair to +0x09 for operands 2..7 whose type
            // byte is 2.
            const auto id = encoded_word(instruction.arguments.at(0));
            auto* record = resolve_record(id, current);
            if (!record) break;
            for (std::size_t i = 2; i < instruction.arguments.size(); ++i) {
                const auto& mask = instruction.arguments.at(i);
                if (mask.type == 2) (void)actor_mask_index(mask);
            }
            record->field04 = static_cast<std::int16_t>(
                encoded_word(instruction.arguments.at(1)));
            for (std::size_t i = 2; i < instruction.arguments.size(); ++i) {
                const auto& mask = instruction.arguments.at(i);
                if (mask.type != 2) continue;
                const auto index = actor_mask_index(mask);
                record->flags = static_cast<std::uint8_t>(
                    (kActorMaskAnd[index] & record->flags) | kActorMaskOr[index]);
            }
            trace.node_writes.push_back(SceneNodeWrite{current, id,
                record->field04, record->flags});
            break;
        }
        case 0x8d: {
            // 0x42db22 case 0x8d: resolve operand 0, then 0x431a69 -> 0x40c7b4
            // makes CD `disc` the current one. 0x40c7b4 enumerates CD-ROM
            // drives (0x40c911), verifies the volume (0x40c9ce/0x40c70e) and
            // may raise the "Now accessing CD n" message box, so its result is
            // host state that a scene trace cannot invent. 0x431a69 maps disc 7
            // to "1, else 2". 0x40c7b4 updates DAT_0045da78 and returns 0 on
            // success, which 0x431a69 turns into 0xffff (the caller continues);
            // failure returns 1 (the caller ends the script) and sets
            // DAT_0045f490 = 3, or 1 for the disc-7 fallback.
            // 0x409513 that follows unconditionally is the debug-mode
            // instruction logger (DAT_00485c6c), which the port does not model.
            const auto disc = scene_value(instruction.arguments.at(0), state.variables);
            if (!disc) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(0));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            trace.disc_request = *disc;
            trace.stopped_at = current;
            pc = script.size();
            break;
        }
        case 0x8a:
        case 0x8b: {
            // 0x42db22 cases 0x8a/0x8b: 0x431b0f ensures the DAT_004649e8
            // record exists, then 0x432b09 appends an extra-list entry or
            // 0x432c65 writes +0x2c.
            const auto id = encoded_word(instruction.arguments.at(0));
            auto* record = resolve_record(id, current);
            if (!record) break;
            if (instruction.opcode == 0x8b) {
                // 0x432c65: record +0x2c = raw operand 1.
                record->field_2c = encoded_word(instruction.arguments.at(1));
                break;
            }
            // 0x432b09: entry +0x04 = resolved operand 1, entry +0x00 = raw
            // operand 2. The save's (first, second) pair is (offset +0, +4).
            const auto second = scene_value(instruction.arguments.at(1), state.variables);
            if (!second) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (record->extra.size() >= 0x7f) {
                throw std::runtime_error("node extra list exceeds the saved 127 entries");
            }
            SceneNodeExtra extra;
            extra.control = 1;  // present marker; the save stream stores a count
            extra.first = encoded_word(instruction.arguments.at(2));
            extra.second = *second;
            record->extra.push_back(extra);
            break;
        }
        case 0x85: {
            // 0x432904(id, record, 1): every present operand 1..7 whose type is
            // 2 applies the 0x460418/0x460428 mask pair to the flag byte.
            auto* record = resolve_record(encoded_word(instruction.arguments.at(0)),
                                          current);
            if (!record) break;
            for (std::size_t i = 1; i < instruction.arguments.size(); ++i) {
                const auto& mask = instruction.arguments.at(i);
                if (mask.type != 2) {
                    continue;
                }
                const auto index = actor_mask_index(mask);
                record->flags = static_cast<std::uint8_t>(
                    (kActorMaskAnd[index] & record->flags) | kActorMaskOr[index]);
            }
            break;
        }
        case 0x95: {
            // 0x42db22 case 0x95: 0x431b0f guard first, then write +0x08 from a
            // resolved operand 1.
            auto* record = resolve_record(encoded_word(instruction.arguments.at(0)),
                                          current);
            if (!record) break;
            const auto value = scene_value(instruction.arguments.at(1), state.variables);
            if (!value) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            record->kind = static_cast<std::uint8_t>(*value);
            break;
        }
        case 0x84: {
            // 0x4326eb: append the current position to the +0x3c trail list
            // (unless flags bit 0x20 suppresses it), then write x/y and add
            // 0x14 to y. Values are resolved before any write so a rejected
            // update is atomic; the original would already hold them. The
            // 0x431b0f guard runs first, so a skipped command never reads them.
            auto* record = resolve_record(encoded_word(instruction.arguments.at(0)),
                                          current);
            if (!record) break;
            std::optional<std::uint16_t> new_x;
            std::optional<std::uint16_t> new_y;
            if (instruction.arguments.at(1).type == 2 ||
                instruction.arguments.at(1).type == 0x40) {
                new_x = scene_value(instruction.arguments.at(1), state.variables);
                if (!new_x) {
                    trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                    trace.stopped_at = current;
                    pc = script.size();
                    break;
                }
            }
            if (instruction.arguments.at(2).type == 2 ||
                instruction.arguments.at(2).type == 0x40) {
                new_y = scene_value(instruction.arguments.at(2), state.variables);
                if (!new_y) {
                    trace.unresolved_variable = encoded_word(instruction.arguments.at(2));
                    trace.stopped_at = current;
                    pc = script.size();
                    break;
                }
            }
            if ((record->flags & 0x20) == 0) {
                const std::pair<std::int16_t, std::int16_t> position{record->x, record->y};
                bool present = false;
                for (const auto& entry : record->trail) {
                    if (entry == position) {
                        present = true;
                        break;
                    }
                }
                if (!present) {
                    record->trail.push_back(position);
                }
            }
            if (new_x) {
                record->x = static_cast<std::int16_t>(*new_x);
            }
            if (new_y) {
                record->y = static_cast<std::int16_t>(*new_y);
            }
            record->y = static_cast<std::int16_t>(record->y + 0x14);
            break;
        }
        case 0x91: {
            // 0x4325d3 -> 0x40cf73 walks the +0x18 frame list; 0xffff clears
            // the current node. The list length comes from the loaded animation
            // asset, so record the request instead of inventing an index. The
            // 0x431b0f guard runs first.
            auto* record = resolve_record(encoded_word(instruction.arguments.at(0)),
                                          current);
            if (!record) break;
            const auto count = scene_value(instruction.arguments.at(1), state.variables);
            if (!count) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (record->frame_count == 0) {
                // No frame count (no asset, no save): keep the request only.
                if (*count == 0xffff) {
                    ++record->frame_restart_requests;
                } else {
                    ++record->frame_advance_requests;
                }
                break;
            }
            // 0x40cf73: 0xffff clears +0x1c; otherwise start at the head and
            // advance `count` times along node->next, logging through
            // 0x40ac43/0x40ac38 when the list ends (the node stays put).
            if (*count == 0xffff) {
                record->current_frame.reset();
            } else if (*count >= record->frame_count) {
                ++trace.frame_overflow_logs;
                record->current_frame =
                    static_cast<std::uint16_t>(record->frame_count - 1);
            } else {
                record->current_frame = *count;
            }
            break;
        }
        case 0x73: {
            // 0x42db22 case 0x73: DAT_00482460[raw(0)] -> DAT_00482428. The array
            // is 50 entries initialised to 0xffff; entries from the LCX count to
            // 49 are valid "no entry", beyond 49 is an out-of-bounds read. The
            // DAT_004603d0 override (a transition snapshot) is not modelled.
            const auto index = encoded_word(instruction.arguments.at(0));
            if (!state.scene_switch_table_known) {
                trace.unresolved_scene_table = index;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (index >= 50) {
                throw std::runtime_error("CC scene switch index outside the 50-entry table");
            }
            if (index >= state.scene_switch_targets.size() ||
                state.scene_switch_targets[index] == 0xffff) {
                break;  // initialised "no entry": DAT_00482428 is unchanged
            }
            trace.scene_switch_requested = state.scene_switch_targets[index];
            trace.stopped_at = current;
            pc = script.size();
            break;
        }
        case 0x74:
        case 0x75: {
            // FUN_0042db22: ADD/SUB resolve operand 0 as a value and
            // operand 1 as the scene variable destination. SUB checks unsigned
            // underflow; ADD writes the low 16 bits back to storage.
            const auto amount = scene_value(instruction.arguments.at(0), state.variables);
            const auto destination = encoded_word(instruction.arguments.at(1));
            const auto previous = state.variables.find(destination);
            if (!amount || previous == state.variables.end()) {
                trace.unresolved_variable = !amount
                    ? encoded_word(instruction.arguments.at(0)) : destination;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (instruction.opcode == 0x75 && previous->second < *amount) {
                // 0x40ac43/0x40ac38: the original logs and skips the write.
                ++trace.subtraction_underflows;
                break;
            }
            previous->second = static_cast<std::uint16_t>(instruction.opcode == 0x74
                ? previous->second + *amount : previous->second - *amount);
            state.variable_writes.push_back(destination);
            break;
        }
        case 0x6d:
        case 0x6e:
            for (const auto& argument : instruction.arguments) {
                if (argument.type != 2) {
                    continue;
                }
                const auto slot = constant_word(argument);
                if (hotspots.find(slot) == hotspots.end()) {
                    throw std::runtime_error("CC hotspot slot not declared");
                }
                state.hotspot_enabled[slot] = instruction.opcode == 0x6d;
                const auto object = state.objects.find(slot);
                if (object != state.objects.end()) {
                    object->second.active = instruction.opcode == 0x6d ? 1 : 0;
                }
            }
            break;
        case 0x71:
        case 0x77:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9d:
        case 0x9e:
        case 0x9f:
        case 0xa0:
        case 0xa1:
        case 0xa2: {
            const auto left = scene_value(instruction.arguments.at(0), state.variables);
            const auto right = scene_value(instruction.arguments.at(1), state.variables);
            if (!left || !right) {
                const auto& unresolved = !left ? instruction.arguments.at(0)
                                                : instruction.arguments.at(1);
                trace.unresolved_variable = encoded_word(unresolved);
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            // FUN_00431a36 resolves operand 0 into local_28 and operand 1
            // into local_14. The dispatcher compares their unsigned low
            // words; the apparent reversed local names must not reverse the
            // script operand order. 0x9d..0xa2 save a return address.
            bool should_jump = false;
            switch (instruction.opcode) {
            case 0x71: case 0x9d: should_jump = *left == *right; break;
            case 0x77: case 0x9e: should_jump = *left != *right; break;
            case 0x96: case 0x9f: should_jump = *left > *right; break;
            case 0x97: case 0xa0: should_jump = *left >= *right; break;
            case 0x98: case 0xa1: should_jump = *left < *right; break;
            case 0x99: case 0xa2: should_jump = *left <= *right; break;
            }
            if (should_jump) {
                const auto target = flow.labels.find(constant_word(instruction.arguments.at(2)));
                if (target == flow.labels.end()) {
                    throw std::runtime_error("unresolved CC conditional label");
                }
                if (instruction.opcode >= 0x9d && instruction.opcode <= 0xa2) {
                    if (return_stack.size() >= 24) {
                        throw std::runtime_error("CC call stack limit exceeded");
                    }
                    return_stack.push_back(pc);
                }
                pc = target->second;
            }
            break;
        }
        case 0x90:
            // FUN_0042db22 reads its literal selector, conditionally closes
            // active entries (FUN_00432303), then reconciles every loaded
            // resource (FUN_00432401) and raises the engine refresh flag.
            // A renderer/resource manager has not been reconstructed yet, but
            // this intent is enough for a static scene trace to continue.
            if (constant_word(instruction.arguments.at(0)) == 0) {
                trace.close_existing_resources = true;
            }
            ++trace.resource_refresh_requests;
            break;
        case 0xb9: {
            // 0x42db22 case 0xb9: load the panorama recorded at DAT_00483d90[id]
            // (CO.ALL). Operand 0 is the RAW record id, operands 1/2 are resolved
            // through the variable table and operand 3 is used only when its type
            // is 2. The transition code comes from operand 3 through 0x432133 or,
            // when operand 3 is absent, from DAT_00460664, which is consumed.
            // 0x40186e builds "nodes/<2>/<name>.jpg" and 0x401ddf turns the staged
            // DAT_0045c064 back into an angle; neither is rebuilt here.
            std::uint16_t transition = 100;
            if (instruction.arguments.at(3).type == 2) {
                // 0x432133(id, 1): 1 -> 0x67, 2 -> 0x65, 3 -> 0x66, else 100.
                const auto id = encoded_word(instruction.arguments.at(3));
                transition = id == 1 ? 0x67 : (id == 2 ? 0x65 : (id == 3 ? 0x66 : 100));
            } else if (state.pending_panorama_transition) {
                transition = *state.pending_panorama_transition;
                state.pending_panorama_transition.reset();
            }
            const auto force = scene_value(instruction.arguments.at(1), state.variables);
            if (!force) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(1));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            const auto vertical = scene_value(instruction.arguments.at(2), state.variables);
            if (!vertical) {
                trace.unresolved_variable = encoded_word(instruction.arguments.at(2));
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            const auto record_id = encoded_word(instruction.arguments.at(0));
            if (!state.panorama_table_known) {
                trace.unresolved_panorama_node = record_id;
                trace.stopped_at = current;
                pc = script.size();
                break;
            }
            if (record_id >= state.panorama_records.size()) {
                throw std::runtime_error("panorama record id outside the CO table");
            }
            const auto& entry = state.panorama_records[record_id];
            if (entry.name.size() < 2 || entry.name.find_first_of("/\\:") != std::string::npos ||
                entry.name.find("..") != std::string::npos) {
                throw std::runtime_error("invalid panorama asset name");
            }
            ScenePanoramaLoad load;
            load.instruction = current;
            load.record = record_id;
            load.name = entry.name;
            load.jpeg_path = "nodes/" + entry.name.substr(0, 2) + "/" + entry.name + ".jpg";
            load.horizontal = state.pending_panorama_x;
            load.force = *force;
            load.vertical = *vertical;
            load.transition = transition;
            if (*force != 0) {
                // 0x42db22 clears DAT_0047e2f0[0x46ee00[raw(1) + scene*600]]; the
                // table meaning is not recovered, so the index is retained.
                load.flag_clear = encoded_word(instruction.arguments.at(1));
                ++trace.panorama_flag_clears;
            }
            // The original clears DAT_0045c064 inside 0x401ddf, i.e. after the
            // JPEG opened. The port treats a known node record as loadable.
            state.pending_panorama_x.reset();
            trace.panorama_loads.push_back(std::move(load));
            break;
        }
        case 0xba: {
            // Both original dispatchers pass an unsigned 16-bit raw operand
            // to FUN_00401c21, which clamps it to 0..0x9bf.
            const auto x = constant_word(instruction.arguments.at(0));
            state.pending_panorama_x = x > 0x9bf ? 0x9bf : x;
            break;
        }
        case 0xb8:
            // FUN_00438cf9 replaces the transition table with the current
            // enabled bits, then disables those hotspots. A second suspension
            // replaces the first snapshot, rather than nesting it.
            state.suspended_hotspots.clear();
            for (auto& [slot, is_enabled] : state.hotspot_enabled) {
                if (is_enabled) state.suspended_hotspots[slot] = true;
                is_enabled = false;
            }
            break;
        case 0xb7:
            // FUN_00438d78 consumes saved set bits and re-enables their live
            // hotspots. Other currently enabled hotspots remain enabled.
            for (const auto& [slot, was_enabled] : state.suspended_hotspots) {
                if (was_enabled && hotspots.find(slot) != hotspots.end()) {
                    state.hotspot_enabled[slot] = true;
                }
            }
            state.suspended_hotspots.clear();
            break;
        case 0x76:
        case 0x9b: {
            const auto target = flow.labels.find(constant_word(instruction.arguments.at(0)));
            if (target == flow.labels.end()) {
                throw std::runtime_error("unresolved CC control-flow label");
            }
            if (instruction.opcode == 0x9b) {
                if (return_stack.size() >= 24) {
                    throw std::runtime_error("CC call stack limit exceeded");
                }
                return_stack.push_back(pc);
            }
            pc = target->second;
            break;
        }
        case 0x6f:
        case 0x83:
            if (return_stack.empty()) {
                trace.stopped_at = current;
                pc = script.size();
            } else {
                pc = return_stack.back();
                return_stack.pop_back();
            }
            break;
        default:
            trace.unsupported_opcode = instruction.opcode;
            trace.stopped_at = current;
            pc = script.size();
            break;
        }
    }
    if (step_budget != 0 && step_budget < kStepLimit && trace.steps >= step_budget &&
        pc < script.size()) {
        trace.paused_by_budget = true;
        trace.stopped_at = pc;
    } else if (trace.steps == kStepLimit && pc < script.size()) {
        trace.step_limit_exceeded = true;
        trace.stopped_at = pc;
    }
    // 0x40c02c iterates DAT_00464300 in ascending id order and tests the
    // object's own +0x08 active byte. The save and 0x6d/0x6e both write that
    // byte, so the object table is authoritative; hotspot_enabled is only a
    // fallback for a static hotspot whose object was not created (it should not
    // happen, but the port must not lose a clickable rectangle).
    for (const auto& [slot, hotspot] : hotspots) {
        bool active = false;
        const auto object = state.objects.find(slot);
        if (object != state.objects.end()) active = object->second.active != 0;
        if (!active) {
            const auto enabled = state.hotspot_enabled.find(slot);
            if (enabled != state.hotspot_enabled.end()) active = enabled->second;
        }
        if (active) trace.active_hotspots.push_back(hotspot);
    }
    return trace;
}

SceneTrace trace_scene_label(const std::vector<CcInstruction>& script,
                             const std::vector<BackgroundEntry>& entries,
                             SceneState& state,
                             std::uint16_t label, std::size_t step_budget) {
    const auto flow = resolve_control_flow(script);
    const auto start = flow.labels.find(label);
    if (start == flow.labels.end()) {
        throw std::runtime_error("unknown CC scene label");
    }
    return trace_scene_from(script, entries, state, start->second, step_budget);
}

SceneTrace trace_scene_entry(const std::vector<CcInstruction>& script,
                             const std::vector<BackgroundEntry>& entries,
                             SceneState& state, std::size_t step_budget) {
    // 0x42c8a0 reloads DAT_00482428's scene after a 0x73 switch and 0x42d594
    // starts at DAT_0047a238, the instruction-list head, so execution begins at
    // instruction 0 (a 0x68 and the 0x72/0x7a hotspot declarations), not at a
    // named label.
    return trace_scene_from(script, entries, state, 0, step_budget);
}

SceneTrace trace_scene_label(const std::vector<CcInstruction>& script,
                             const std::vector<BackgroundEntry>& entries,
                             std::uint16_t label) {
    SceneState state;
    return trace_scene_label(script, entries, state, label);
}

SceneTrace trace_scene_from_instruction(const std::vector<CcInstruction>& script,
                                        const std::vector<BackgroundEntry>& entries,
                                        SceneState& state, std::size_t instruction,
                                        std::size_t step_budget) {
    if (instruction > script.size()) {
        throw std::runtime_error("conversation resume instruction outside the script");
    }
    return trace_scene_from(script, entries, state, instruction, step_budget);
}

std::size_t apply_conversation_result(const std::vector<CcInstruction>& script,
                                      const SceneState& state,
                                      std::size_t instruction,
                                      std::uint16_t result,
                                      SceneConverseJump* jump) {
    if (instruction >= script.size() || script[instruction].opcode != 0x70) {
        throw std::runtime_error("conversation result applied to a non-0x70 instruction");
    }
    if (result == 0xffff) return instruction + 1;
    if (result == 0xfffe) {
        throw std::runtime_error("conversation aborted (0xfffe) without a script continuation");
    }
    const auto object = state.objects.find(result);
    if (object == state.objects.end()) {
        throw std::runtime_error("conversation returned object " + std::to_string(result) +
                                 " which the scene does not define");
    }
    const auto label = object->second.label;
    if (label > 0xffff) {
        throw std::runtime_error("conversation object label outside 16-bit range");
    }
    const auto flow = resolve_control_flow(script);
    const auto target = flow.labels.find(static_cast<std::uint16_t>(label));
    if (target == flow.labels.end()) {
        throw std::runtime_error("conversation object " + std::to_string(result) +
                                 " has no CC label " + std::to_string(label));
    }
    if (jump != nullptr) {
        *jump = SceneConverseJump{instruction, result, label, target->second};
    }
    return target->second;
}

std::string trace_blocker(const SceneTrace& trace) {
    // Every field below is a stop: 0x42db22 sets the flag and the dispatcher
    // rewinds pc to the end of the script. Counters that only report a skipped
    // or logged command (skipped_node_commands, anims_load_failures,
    // missing_media_requests, subtraction_underflows, dialogue_name_fallbacks,
    // video_skipped, media_timestamps_unavailable, refresh/display requests) are
    // deliberately absent: the original continues after them.
    if (trace.unsupported_opcode) {
        std::ostringstream text;
        text << "unsupported_opcode=0x" << std::hex
             << static_cast<int>(*trace.unsupported_opcode) << std::dec;
        return text.str();
    }
    if (trace.unresolved_variable) {
        return "unresolved_variable=" + std::to_string(*trace.unresolved_variable);
    }
    if (trace.unresolved_random) return "unresolved_random";
    if (trace.unresolved_sound_name) {
        return "unresolved_sound_name=" + std::to_string(*trace.unresolved_sound_name);
    }
    if (trace.missing_object) return "missing_object=" + std::to_string(*trace.missing_object);
    if (trace.missing_examine_item) return "missing_examine_item";
    if (trace.unresolved_object_label) {
        return "unresolved_object_label=" + std::to_string(*trace.unresolved_object_label);
    }
    if (trace.unresolved_dialogue) {
        return "unresolved_dialogue=" + std::to_string(*trace.unresolved_dialogue);
    }
    if (trace.unresolved_dialogue_channel) {
        return "unresolved_dialogue_channel=" +
               std::to_string(*trace.unresolved_dialogue_channel);
    }
    if (trace.unresolved_dialogue_inventory) return "unresolved_dialogue_inventory";
    if (trace.unresolved_video_name) {
        return "unresolved_video_name=" + std::to_string(*trace.unresolved_video_name);
    }
    if (trace.unresolved_video_setting) return "unresolved_video_setting";
    if (trace.unresolved_node_asset) {
        return "unresolved_node_asset=" + std::to_string(*trace.unresolved_node_asset);
    }
    if (trace.missing_node_asset) {
        return "missing_node_asset=" + std::to_string(*trace.missing_node_asset);
    }
    if (trace.disc_request) return "disc_request=" + std::to_string(*trace.disc_request);
    if (trace.missing_media_record) {
        return "missing_media_record=" + std::to_string(*trace.missing_media_record);
    }
    if (trace.missing_dialogue_chain) {
        return "missing_dialogue_chain=" + std::to_string(*trace.missing_dialogue_chain);
    }
    if (trace.unresolved_system_input) {
        return "unresolved_system_input=" + std::to_string(*trace.unresolved_system_input);
    }
    if (trace.unresolved_save_setting) return "unresolved_save_setting";
    if (trace.unresolved_scene_table) {
        return "unresolved_scene_table=" + std::to_string(*trace.unresolved_scene_table);
    }
    if (trace.unresolved_panorama_node) {
        return "unresolved_panorama_node=" + std::to_string(*trace.unresolved_panorama_node);
    }
    if (trace.step_limit_exceeded) return "step_limit_exceeded";
    return {};
}

void load_save(SceneState& state, const SaveGame& save) {
    // 0x425ab4 frees the four 0x15e tables before reading them; replacing the
    // maps reproduces that "the save is the whole table" behaviour instead of
    // merging into state from another scene.
    state.objects.clear();
    state.media.clear();
    state.sounds.clear();
    state.records.clear();

    for (const auto& record : save.objects) {
        SceneObjectState object;
        object.x = record.x;
        object.y = record.y;
        object.width = record.width;
        object.height = record.height;
        object.active = record.active;
        object.kind = record.kind;
        object.label = record.label;
        object.name_id = record.name_id;
        object.argument = record.argument;
        // record.uninitialized_0a is intentionally not read: no recovered
        // writer, and 0x72's allocation is not zeroed.
        state.objects[record.slot] = object;
    }

    for (const auto& record : save.media) {
        SceneMediaState media;
        media.value = record.value;
        media.frame = record.frame;
        media.timestamp = record.timestamp;
        media.media_id = record.media_id;
        media.active = record.active;
        media.type = record.type;
        // +0x12 is not carried (no writer; stale heap bytes in shipped saves).
        state.media[record.slot] = media;
    }

    for (const auto& record : save.sounds) {
        SceneSoundState sound;
        sound.defined = true;
        sound.enabled = record.word(0x04);
        sound.field08 = record.word(0x08);
        sound.field0c = record.word(0x0c);
        sound.field10 = record.word(0x10);
        sound.field14 = record.word(0x14);
        sound.field18 = record.word(0x18);
        sound.field1c = record.word(0x1c);
        sound.field20 = record.word(0x20);
        sound.field24 = record.word(0x24);
        sound.field28 = record.word(0x28);
        sound.field2c = record.word(0x2c);
        sound.field30 = record.word(0x30);  // 0xffff is the -1 sentinel
        sound.active = record.raw.at(0x34) != 0;
        state.sounds[record.slot] = sound;
    }

    for (const auto& record : save.nodes) {
        SceneRecordState entry;
        entry.marker = record.marker;
        // 0x426b9d copies the 0x40-byte record field by field (+0,+2,+4,+6,+8,
        // +9,+0xa,+0xc,+0xe,+0x10,+0x12,+0x14,+0x24,+0x26,+0x29). The fields
        // an opcode reads are restored here; the rest keep 0x40ffee's values.
        // For marker 1 the loader also calls 0x40fe70, which re-initialises the
        // record from the animation asset; that asset load is not modelled, so
        // the saved bytes stay (the same documented approximation as the frame
        // indices below).
        entry.x = static_cast<std::int16_t>(
            static_cast<std::uint16_t>(record.raw.at(0x00) | (record.raw.at(0x01) << 8)));
        entry.y = static_cast<std::int16_t>(
            static_cast<std::uint16_t>(record.raw.at(0x02) | (record.raw.at(0x03) << 8)));
        entry.field04 = static_cast<std::int16_t>(
            static_cast<std::uint16_t>(record.raw.at(0x04) | (record.raw.at(0x05) << 8)));
        entry.field06 = static_cast<std::int16_t>(
            static_cast<std::uint16_t>(record.raw.at(0x06) | (record.raw.at(0x07) << 8)));
        entry.kind = record.raw.at(0x08);
        entry.flags = record.raw.at(0x09);
        // +0x14 is in 0x426b9d's copy list (local_c[10]).
        entry.frame_count = static_cast<std::uint16_t>(
            record.raw.at(0x14) | (record.raw.at(0x15) << 8));
        entry.frame_index_1c = record.frame_index_1c;
        entry.frame_index_20 = record.frame_index_20;
        entry.snap_x = static_cast<std::int16_t>(
            static_cast<std::uint16_t>(record.raw.at(0x24) | (record.raw.at(0x25) << 8)));
        entry.snap_y = static_cast<std::int16_t>(
            static_cast<std::uint16_t>(record.raw.at(0x26) | (record.raw.at(0x27) << 8)));
        // 0x426b9d feeds the first recovered dword to 0x40cf73(record, index):
        // 0xffff clears +0x1c, otherwise the head is advanced `index` times.
        if (record.frame_index_1c != 0xffff && entry.frame_count != 0) {
            entry.current_frame = static_cast<std::uint16_t>(
                std::min<std::uint32_t>(record.frame_index_1c,
                                        entry.frame_count - 1));
        }
        entry.field_2c = record.field_2c;
        entry.field_30 = record.field_30;
        for (const auto& extra : record.extra) {
            entry.extra.push_back(SceneNodeExtra{extra.control, extra.first, extra.second});
        }
        state.records[record.slot] = entry;
    }

    // DAT_00486a10 dialogue chains (block 24): count at +0x64, 25 ids at +0x00.
    state.dialogue_chains.clear();
    for (const auto& record : save.dialogue_chains) {
        SceneDialogueChain chain;
        chain.count = record.count();
        if (chain.count > chain.ids.size()) {
            throw std::runtime_error("save dialogue chain exceeds its 25 entries");
        }
        for (std::size_t index = 0; index < chain.ids.size(); ++index) {
            chain.ids[index] = record.id(index);
        }
        for (std::size_t index = 0; index < chain.count; ++index) {
            if (chain.ids[index] >= save.dialogue_marks.size()) {
                throw std::runtime_error(
                    "save dialogue chain id has no allocated record");
            }
        }
        state.dialogue_chains[record.slot] = chain;
    }
    state.field_460668 = save.field_460668;
    // 0x425ab4 sets DAT_004859cd = 1 after a fully successful load.
    state.flag_4859cd = true;

    // 0x408adc reads the dialogue selectors, the ten depths, the ten 350-entry
    // stacks, the current record index and the 0x4089b0 state bytes.
    if (save.field_45d1c4 >= kSaveDialogueChannels) {
        throw std::runtime_error(
            "save dialogue channel selector outside the ten channels");
    }
    // DAT_0045d1c0 (dialog visibility) has a consumer now: 0xb4/0xb5 compare no
    // flag, they write it, and the save restore feeds the state.
    state.dialogue_visible = save.field_45d1c0 != 0;
    state.current_dialogue_channel = static_cast<std::uint16_t>(save.field_45d1c4);
    state.dialogue_scroll = static_cast<std::int32_t>(save.field_45d1bc);
    state.dialogue_channels = {};
    for (std::size_t channel = 0; channel < kSaveDialogueChannels; ++channel) {
        const auto depth = save.dialogue_depths[channel];
        if (depth > kSaveDialogueStackDepth) {
            throw std::runtime_error(
                "save dialogue depth outside the 350-entry channel stack");
        }
        auto& stack = state.dialogue_channels[channel];
        stack.reserve(depth);
        for (std::size_t index = 0; index < depth; ++index) {
            // 0x4077b2 stores the newest entry at index 0, so the saved order
            // is already top-first; the loader casts the u32 ids to u16.
            const auto id = save.dialogue_stacks[channel * kSaveDialogueStackDepth + index];
            if (id > 0xffff) {
                throw std::runtime_error("save dialogue stack id outside 16 bits");
            }
            // 0x40892d writes one state byte per allocated DAT_00486f90 record
            // and stops at the first NULL slot, so that count is the record
            // capacity. A stacked id beyond it has no record to display.
            if (id >= save.dialogue_marks.size()) {
                throw std::runtime_error("save dialogue stack id has no allocated record");
            }
            stack.push_back(static_cast<std::uint16_t>(id));
        }
    }
    state.dialogue_records.clear();
    for (std::size_t index = 0; index < save.dialogue_marks.size(); ++index) {
        SceneDialogueRecord record;
        record.state = save.dialogue_marks[index].second;
        state.dialogue_records[static_cast<std::uint16_t>(index)] = record;
    }
    // 0x408adc: index -1 clears DAT_0045d1c8; any other index selects
    // DAT_00486f90[index], and an unallocated slot is NULL, i.e. no current line.
    state.current_dialogue_record.reset();
    if (save.dialogue_index >= 0 &&
        static_cast<std::size_t>(save.dialogue_index) < save.dialogue_marks.size()) {
        state.current_dialogue_record = static_cast<std::uint16_t>(save.dialogue_index);
    }
}

std::size_t load_save_variables(SceneState& state, const SaveGame& save,
                                const std::vector<std::uint16_t>& declared_global_ids) {
    // 0x42c8a0 initialises every one of the 50 scene rows with 300 identity
    // entries; 0x438173 then overwrites the first `count` entries with the LCV
    // global ids. 0x4319b3 reads DAT_0047e2f0 through that row.
    if (save.node_values.size() != kSaveNodeValueCount) {
        throw std::runtime_error("save variable pool is not the 3000-word table");
    }
    apply_variable_pool(state, save.node_values,
                        SceneVariableLayout{std::string(), declared_global_ids});
    return 300;
}

}  // namespace byzantine
