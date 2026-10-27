#include "byzantine/cc.hpp"
#include "byzantine/compose.hpp"
#include "byzantine/jpeg.hpp"
#include "byzantine/panorama.hpp"
#include "byzantine/panorama_view.hpp"
#include "byzantine/save.hpp"
#include "byzantine/scene_setup.hpp"
#include "byzantine/variables.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
// The scene-state setup (metadata files, save projection, dialogue/video/
// panorama/CA/anims/gamepref tables) lives in scene_setup.cpp so the SDL2
// scene player builds exactly the same state as this CLI.
using byzantine::apply_dialogue_inputs;
using byzantine::apply_engine_globals;
using byzantine::read_metadata;
using byzantine::read_metadata_file;
using byzantine::read_name_list;
using byzantine::read_variable_metadata;
using byzantine::require_matching_scene;

struct Arguments {
    std::string mode;
    std::vector<std::string> positional;
    std::map<std::string, std::string> values;
};

const std::set<std::string> kModes = {
    "--hotspots", "--flow", "--labels", "--backgrounds", "--panoramas", "--census",
    "--census-detail", "--variables", "--trace", "--trace-new-game", "--trace-save",
    "--save-dump"};
const std::set<std::string> kValued = {
    "--ci", "--lci", "--cy", "--lcy", "--inventor", "--hotitems", "--cf", "--co",
    "--ca", "--anims", "--anims-overlay", "--gameprefs", "--lcc", "--cc", "--save", "--engine", "--lcx",
    "--compose-out", "--surface", "--background", "--panorama", "--panorama-x",
    "--cs", "--cd"};

Arguments parse_arguments(int argc, char** argv) {
    Arguments arguments;
    // argv[1] is normally the input file, but --save-dump also accepts the
    // documented `byzantine-cc --save-dump <save>` order.
    for (int index = 1; index < argc; ++index) {
        const std::string token(argv[index]);
        if (kModes.count(token) != 0) {
            if (!arguments.mode.empty()) throw std::runtime_error("only one mode is allowed");
            arguments.mode = token;
        } else if (kValued.count(token) != 0) {
            if (index + 1 >= argc) throw std::runtime_error(token + " needs a value");
            arguments.values[token] = argv[++index];
        } else if (token.rfind("--", 0) == 0) {
            throw std::runtime_error("unknown option: " + token);
        } else {
            arguments.positional.push_back(token);
        }
    }
    if (arguments.positional.empty()) throw std::runtime_error("no input file");
    return arguments;
}
void require_positional(const Arguments& arguments, std::size_t minimum, std::size_t maximum) {
    if (arguments.positional.size() < minimum || arguments.positional.size() > maximum) {
        throw std::runtime_error(arguments.mode + ": wrong number of arguments");
    }
}

// Blocks in file order, as consumed by FUN_00425ab4. Table occupancy is
// printed per fixed 0x15e-slot table so a save can be compared at a glance.
void dump_save(const std::vector<std::uint8_t>& bytes,
               const std::map<std::string, std::string>& options) {
    const auto save = byzantine::parse_save(bytes);
    std::cout << "save bytes=" << save.size << '\n';
    std::cout << "cd=" << save.cd_number << " chapter=" << save.chapter
              << " description=" << save.description << '\n';
    std::cout << "thumbnail bytes=" << save.thumbnail.size() << " width="
              << byzantine::kSaveThumbnailWidth << " height="
              << byzantine::kSaveThumbnailHeight << '\n';
    std::cout << "scene_index=" << save.scene_index;
    const auto chapter = options.find("--lcc");
    if (chapter != options.end()) {
        const auto names =
            byzantine::parse_chapter_scene_list(read_metadata_file(chapter->second));
        const auto name = byzantine::save_scene_name(save, names);
        std::cout << " chapter_scenes=" << names.size() << " scene=";
        std::cout << (name ? *name : "<index past the chapter list>");
    } else {
        std::cout << " scene=<supply --lcc engine/SHOW.LCC>";
    }
    std::cout << " current_scene=" << save.current_scene << '\n';

    std::size_t points = 0;
    std::size_t chains = 0;
    for (const auto& record : save.scene_records) {
        for (const auto& list : record.list4c4) points += list.size();
        for (const auto& list : record.list58c) points += list.size();
        for (const auto& chain : record.chains654) chains += chain.size();
    }
    std::cout << "scene_records=" << save.scene_records.size() << "/"
              << byzantine::kSaveSlotCount << " link_entries=" << points
              << " chain_entries=" << chains << '\n';
    for (const auto& record : save.scene_records) {
        std::cout << "scene_record slot=" << static_cast<unsigned>(record.slot)
                  << " name=" << record.name << " word08=" << record.word08 << '\n';
    }

    std::cout << "node_values=" << save.node_values.size() << '\n';
    std::cout << "dialogue_channels=" << save.dialogue_depths.size() << " depths=";
    for (std::size_t index = 0; index < save.dialogue_depths.size(); ++index) {
        std::cout << (index == 0 ? "" : ",") << save.dialogue_depths[index];
    }
    std::cout << " stack_entries=" << save.dialogue_stacks.size() << '\n';
    // The counts index DAT_00468d78; printing the live ids makes the inventory
    // strip's data (selection/examine/use evidence) readable from a save.
    for (std::size_t channel = 0; channel < save.dialogue_depths.size(); ++channel) {
        const auto depth = static_cast<std::size_t>(save.dialogue_depths[channel]);
        if (depth == 0) continue;
        std::cout << "dialogue_channel channel=" << channel << " count=" << depth << " ids=";
        for (std::size_t at = 0; at < depth; ++at) {
            std::cout << (at == 0 ? "" : ",")
                      << save.dialogue_stacks[channel * byzantine::kSaveDialogueStackDepth + at];
        }
        std::cout << '\n';
    }
    std::cout << "dialogue_index=" << save.dialogue_index
              << " dialogue_marks=" << save.dialogue_marks.size() << '\n';
    std::cout << "dialogue_selectors visibility=" << save.field_45d1c0
              << " channel=" << save.field_45d1c4
              << " scroll=" << save.field_45d1bc << '\n';
    std::cout << "state 45f210=" << save.field_45f210 << " 460668=" << save.field_460668
              << " 46066c=" << save.field_46066c << " 45f214=" << save.field_45f214
              << " 45c060=" << save.field_45c060;
    std::cout << " 45fd2c=" << save.field_45fd2c << '\n';
    if (save.field_45c060 == 0) {
        // 0x425ab4 sets DAT_00480330 = -1 and reads the word instead.
        std::cout << "panorama index=-1 state=0x" << std::hex << save.field_48253e
                  << std::dec << '\n';
    } else {
        std::cout << "panorama index=" << save.field_480330
                  << " x=" << save.field_45c064 << '\n';
    }

    std::cout << "nodes=" << save.nodes.size() << '/' << byzantine::kSaveSlotCount << '\n';
    for (const auto& node : save.nodes) {
        std::cout << "node slot=" << static_cast<unsigned>(node.slot) << " marker="
                  << static_cast<unsigned>(node.marker)
                  << " frame1c=" << node.frame_index_1c
                  << " frame20=" << node.frame_index_20
                  << " x24=" << static_cast<std::int16_t>(node.raw[0x24] | (node.raw[0x25] << 8))
                  << " y26=" << static_cast<std::int16_t>(node.raw[0x26] | (node.raw[0x27] << 8))
                  << " flags=0x" << std::hex << static_cast<unsigned>(node.raw[9])
                  << std::dec
                  << " d0a=" << static_cast<std::int16_t>(node.raw[0x0a] | (node.raw[0x0b] << 8))
                  << " d0c=" << static_cast<std::int16_t>(node.raw[0x0c] | (node.raw[0x0d] << 8))
                  << " extra=" << node.extra.size() << '\n';
    }

    std::cout << "objects=" << save.objects.size() << '/' << byzantine::kSaveSlotCount
              << '\n';
    std::vector<std::uint32_t> labels;
    for (const auto& object : save.objects) {
        labels.push_back(object.label);
        std::cout << "object slot=" << static_cast<unsigned>(object.slot)
                  << " x=" << object.x << " y=" << object.y << " w=" << object.width
                  << " h=" << object.height << " active=" << static_cast<unsigned>(object.active)
                  << " kind=0x" << std::hex << static_cast<unsigned>(object.kind) << std::dec
                  << " label=" << object.label << " name_id=" << object.name_id
                  << " argument=" << object.argument << '\n';
    }
    const auto cc = options.find("--cc");
    if (cc != options.end()) {
        const auto flow = byzantine::resolve_control_flow(
            byzantine::parse_cc(read_metadata_file(cc->second)));
        std::size_t matched = 0;
        for (const auto label : labels) {
            if (label <= 0xffff && flow.labels.count(static_cast<std::uint16_t>(label)) != 0) {
                ++matched;
            }
        }
        std::cout << "object_labels cc_labels=" << flow.labels.size() << " matched=" << matched
                  << "/" << labels.size() << '\n';
        for (const auto label : labels) {
            if (label > 0xffff ||
                flow.labels.count(static_cast<std::uint16_t>(label)) == 0) {
                std::cout << "object_label_unmatched label=" << label << '\n';
            }
        }
    }

    std::cout << "media=" << save.media.size() << '/' << byzantine::kSaveSlotCount << '\n';
    for (const auto& media : save.media) {
        std::cout << "media_record slot=" << static_cast<unsigned>(media.slot)
                  << " value=" << media.value << " media_id=" << media.media_id
                  << " frame=" << media.frame << " active="
                  << static_cast<unsigned>(media.active) << " type="
                  << static_cast<unsigned>(media.type) << '\n';
    }

    std::cout << "sounds=" << save.sounds.size() << '/' << byzantine::kSaveSlotCount
              << " sound_chain=" << save.sound_chain.size() << '\n';
    for (const auto& sound : save.sounds) {
        std::cout << "sound_record slot=" << static_cast<unsigned>(sound.slot)
                  << " word04=" << sound.word(0x04) << " word08=" << sound.word(0x08)
                  << " word0c=" << sound.word(0x0c) << " word10=" << sound.word(0x10)
                  << " word14=" << sound.word(0x14) << " word18=" << sound.word(0x18)
                  << " word30=" << sound.word(0x30) << '\n';
    }

    std::cout << "dialogue_chains=" << save.dialogue_chains.size() << '/'
              << byzantine::kSaveSlotCount << '\n';
    for (const auto& chain : save.dialogue_chains) {
        std::cout << "dialogue_chain slot=" << static_cast<unsigned>(chain.slot)
                  << " count=" << chain.count() << " ids=";
        for (std::uint32_t index = 0; index < chain.count(); ++index) {
            std::cout << (index == 0 ? "" : ",") << chain.id(index);
        }
        std::cout << '\n';
    }
    std::cout << "notebook bytes=" << save.notebook.size() << '\n';

    // 0x425ab4 -> 0x408adc restore path: validates the dialogue depths/ids and
    // the four tables against the strict SceneState contract.
    byzantine::SceneState restored;
    byzantine::load_save(restored, save);
    std::size_t stacked = 0;
    for (const auto& stack : restored.dialogue_channels) stacked += stack.size();
    std::cout << "restore objects=" << restored.objects.size()
              << " media=" << restored.media.size()
              << " sounds=" << restored.sounds.size()
              << " records=" << restored.records.size()
              << " dialogue_records=" << restored.dialogue_records.size()
              << " dialogue_stacked=" << stacked << '\n';
}
}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: byzantine-cc <input.cc> [--hotspots | --flow | --labels | "
                     "--backgrounds <input.cb> | --panoramas <input.co> | --census <input.cb> "
                     "[<scene-cs>] [--save <save.btb>] | --census-detail <input.cb> "
                     "[<scene-cs>] [--save <save.btb>] | --variables <engine-dir> | --trace "
                     "<label> <input.cb> | --trace-new-game <label> <input.cb> <engine-dir> | "
                     "--trace-save <save.btb> <label> <input.cb>] [--save <save.btb>] "
                     "[--engine <engine-dir>] "
                     "[--lci <lci> --ci <ci> --lcy <lcy> --cy <cy> "
                     "--inventor <names> --hotitems <names> --cf <cf> --ca <ca> "
                     "--co <co> --gameprefs <gamepref> --lcx <SHOW.LCX> [--cs <scene-cs>] "
                     "--anims <anims-root> [--compose-out <frame.ppm> "
                     "[--surface <WxH>] [--background <back.jpg> | "
                     "--panorama <panorama.jpg> [--panorama-x <0..w-1>]]]]\n"
                     "       byzantine-cc <save.btb> --save-dump [--lcc <SHOW.LCC>] "
                     "[--cc <scene.cc>]\n";
        return 2;
    }
    try {
        const auto arguments = parse_arguments(argc, argv);
        if (arguments.mode == "--save-dump") {
            require_positional(arguments, 1, 1);
            dump_save(read_metadata_file(arguments.positional[0]), arguments.values);
            return 0;
        }
        const auto file = read_metadata_file(arguments.positional[0]);
        const auto instructions = byzantine::parse_cc(file);
        const auto& mode = arguments.mode;
        if (mode == "--census" || mode == "--census-detail") {
            require_positional(arguments, 2, 3);
            const bool detail = mode == "--census-detail";
            // Optional per-scene CS entry: enables 0xc4's "endgame" comparison.
            std::vector<std::uint8_t> sound_data;
            if (arguments.positional.size() == 3) {
                sound_data = read_metadata_file(arguments.positional[2]);
            }
            const auto entries = byzantine::parse_cb(
                read_metadata_file(arguments.positional[1]));
            const auto flow = byzantine::resolve_control_flow(instructions);
            // An optional save supplies the object/media/sound/node/dialogue
            // tables, so a label that 0xcc/0x70 needs does not have to invent
            // them. With --engine the carried variable pool is projected through
            // the scene's LCV map as well.
            std::optional<byzantine::SaveGame> saved;
            const auto save_option = arguments.values.find("--save");
            if (save_option != arguments.values.end()) {
                saved = byzantine::parse_save(read_metadata_file(save_option->second));
            }
            const auto metadata = read_variable_metadata(arguments.values);
            const byzantine::SceneVariableLayout* variable_layout = nullptr;
            if (saved && metadata) {
                variable_layout = &byzantine::scene_variable_layout(
                    *metadata,
                    std::filesystem::path(arguments.positional[0]).stem().string());
                require_matching_scene(*saved, *metadata, *variable_layout);
            }
            std::map<std::string, std::size_t> counts;
            std::vector<std::string> rows;
            std::size_t skipped_node_commands = 0;
            std::size_t records_created = 0;
            std::size_t anims_load_failures = 0;
            std::size_t frame_overflow_logs = 0;
            for (const auto& [label, target] : flow.labels) {
                (void)target;
                // A fresh SceneState per label: this measures what one scene
                // entry can execute without inventing carried-over game state.
                byzantine::SceneState state;
                if (saved) {
                    byzantine::load_save(state, *saved);
                    if (variable_layout != nullptr) {
                        byzantine::load_save_variables(state, *saved,
                                                       variable_layout->global_ids);
                    }
                }
                if (metadata) apply_engine_globals(state, *metadata);
                if (arguments.positional.size() == 3) {
                    state.sound_records = byzantine::parse_named_records(sound_data);
                    state.sound_table_known = true;
                }
                apply_dialogue_inputs(state, arguments.values);
                std::string outcome;
                std::string reason;
                std::size_t steps = 0;
                std::size_t stopped_at = 0;
                try {
                    const auto trace =
                        byzantine::trace_scene_label(instructions, entries, state, label);
                    steps = trace.steps;
                    stopped_at = trace.stopped_at;
                    skipped_node_commands += trace.skipped_node_commands;
                    records_created += trace.records_created;
                    anims_load_failures += trace.anims_load_failures;
                    frame_overflow_logs += trace.frame_overflow_logs;
                    if (trace.unsupported_opcode) {
                        std::ostringstream key;
                        key << "0x" << std::hex << std::setw(2) << std::setfill('0')
                            << static_cast<unsigned>(*trace.unsupported_opcode);
                        outcome = key.str();
                    } else if (trace.unresolved_variable) {
                        outcome = "unresolved_variable";
                        reason = " variable=" + std::to_string(*trace.unresolved_variable);
                    } else if (trace.unresolved_random) {
                        outcome = "unresolved_random";
                    } else if (trace.unresolved_sound_name) {
                        outcome = "unresolved_sound_name";
                    } else if (trace.unresolved_dialogue ||
                               trace.unresolved_dialogue_channel) {
                        outcome = "unresolved_dialogue";
                    } else if (trace.unresolved_dialogue_inventory) {
                        outcome = "unresolved_dialogue_inventory";
                    } else if (trace.unresolved_video_name) {
                        outcome = "unresolved_video_name";
                    } else if (trace.unresolved_video_setting) {
                        outcome = "unresolved_video_setting";
                    } else if (trace.unresolved_panorama_node) {
                        outcome = "unresolved_panorama_node";
                    } else if (trace.unresolved_node_asset) {
                        outcome = "unresolved_node_asset";
                        reason = " node=" + std::to_string(*trace.unresolved_node_asset);
                    } else if (trace.missing_node_asset) {
                        outcome = "missing_node_asset";
                        reason = " node=" + std::to_string(*trace.missing_node_asset);
                    } else if (trace.missing_dialogue_chain) {
                        outcome = "missing_dialogue_chain";
                        reason = " chain=" + std::to_string(*trace.missing_dialogue_chain);
                    } else if (trace.unresolved_system_input) {
                        outcome = "unresolved_system_input";
                        reason = " sub=" + std::to_string(*trace.unresolved_system_input);
                    } else if (trace.unresolved_save_setting) {
                        outcome = "unresolved_save_setting";
                    } else if (trace.unresolved_scene_table) {
                        outcome = "unresolved_scene_table";
                        reason = " index=" + std::to_string(*trace.unresolved_scene_table);
                    } else if (trace.scene_switch_requested) {
                        outcome = "scene_switch";
                        reason = " target=" + std::to_string(*trace.scene_switch_requested);
                    } else if (trace.missing_object) {
                        outcome = "missing_object";
                        reason = " object=" + std::to_string(*trace.missing_object);
                    } else if (trace.unresolved_object_label) {
                        outcome = "unresolved_object_label";
                        reason = " label=" + std::to_string(*trace.unresolved_object_label);
                    } else if (trace.disc_request) {
                        outcome = "unresolved_disc_request";
                        reason = " disc=" + std::to_string(*trace.disc_request);
                    } else if (trace.missing_media_record) {
                        outcome = "missing_media_record";
                        reason = " media=" + std::to_string(*trace.missing_media_record);
                    } else if (trace.conversation_request) {
                        outcome = "conversation_request";
                        reason = " record=" +
                                 std::to_string(trace.conversation_request->record) +
                                 " instruction=" +
                                 std::to_string(trace.conversation_request->instruction);
                    } else if (trace.step_limit_exceeded) {
                        outcome = "step_limit";
                    } else if (trace.script_end_requested) {
                        outcome = "script_end";
                    } else {
                        outcome = "return";
                    }
                } catch (const std::exception&) {
                    outcome = "error";
                }
                ++counts[outcome];
                if (detail) {
                    rows.push_back("label=" + std::to_string(label) + " outcome=" + outcome +
                                   " steps=" + std::to_string(steps) +
                                   " stopped_at=" + std::to_string(stopped_at) + reason);
                }
            }
            std::vector<std::pair<std::string, std::size_t>> ranked(counts.begin(),
                                                                    counts.end());
            std::sort(ranked.begin(), ranked.end(), [](const auto& left, const auto& right) {
                if (left.second != right.second) return left.second > right.second;
                return left.first < right.first;
            });
            std::cout << "labels=" << flow.labels.size() << '\n';
            // 0x431b0f returns 1 for a CA asset type outside 0..3: the command
            // body is skipped and the script continues. Aggregated so the skip
            // rate stays visible instead of looking like a silent no-op.
            std::cout << "skipped_node_commands=" << skipped_node_commands
                      << " records_created=" << records_created
                      << " anims_load_failures=" << anims_load_failures
                      << " frame_overflow_logs=" << frame_overflow_logs << '\n';
            for (const auto& [outcome, count] : ranked) {
                std::cout << "outcome=" << outcome << " count=" << count << '\n';
            }
            for (const auto& row : rows) std::cout << row << '\n';
            return 0;
        }
        if (mode == "--panoramas") {
            require_positional(arguments, 2, 2);
            const auto entries = byzantine::parse_cb(
                read_metadata_file(arguments.positional[1]));
            for (const auto& panorama : byzantine::static_panoramas(instructions, entries)) {
                std::cout << "instruction=" << panorama.instruction << " id=" << panorama.id
                          << " name=" << panorama.entry.name << " source=" << panorama.entry.source
                          << " jpeg=" << panorama.jpeg_path << '\n';
            }
            return 0;
        }
        if (mode == "--variables") {
            require_positional(arguments, 2, 2);
            const auto metadata = read_metadata(arguments.positional[1].c_str());
            const auto& layout = byzantine::scene_variable_layout(
                metadata, std::filesystem::path(arguments.positional[0]).stem().string());
            for (std::size_t local = 0; local < layout.global_ids.size(); ++local) {
                const auto global = layout.global_ids[local];
                std::cout << "local=" << local << " global=" << global
                          << " name=" << metadata.names.at(global) << '\n';
            }
            return 0;
        }
        if (mode == "--backgrounds") {
            require_positional(arguments, 2, 2);
            const auto entries = byzantine::parse_cb(
                read_metadata_file(arguments.positional[1]));
            const auto backgrounds = byzantine::static_backgrounds(instructions, entries);
            for (const auto& background : backgrounds) {
                std::cout << "instruction=" << background.instruction << " id=" << background.id
                          << " name=" << background.entry.name
                          << " source=" << background.entry.source << '\n';
            }
            return 0;
        }
        if (mode == "--trace" || mode == "--trace-new-game" ||
            mode == "--trace-save") {
            // --trace:            <cc> <label> <cb> [engine-dir]
            // --trace-new-game:   <cc> <label> <cb> <engine-dir>
            // --trace-save:       <cc> <save> <label> <cb>
            // --save <file> also loads a save for plain --trace.
            const bool save_mode = mode == "--trace-save";
            require_positional(arguments, save_mode ? 4 : 3,
                               mode == "--trace-new-game" ? 4 : (save_mode ? 4 : 3));
            const std::size_t label_index = save_mode ? 2 : 1;
            const std::size_t cb_index = save_mode ? 3 : 2;
            const auto parsed_label =
                std::stoul(arguments.positional[label_index], nullptr, 0);
            if (parsed_label > 0xffff) {
                throw std::runtime_error("CC label outside 16-bit range");
            }
            const auto entries = byzantine::parse_cb(
                read_metadata_file(arguments.positional[cb_index]));
            byzantine::SceneState state;
            std::optional<byzantine::SaveGame> loaded_save;
            if (save_mode) {
                loaded_save = byzantine::parse_save(
                    read_metadata_file(arguments.positional[1]));
                byzantine::load_save(state, *loaded_save);
                std::cout << "state_origin=save file=" << arguments.positional[1]
                          << " objects=" << state.objects.size()
                          << " media=" << state.media.size()
                          << " sounds=" << state.sounds.size()
                          << " records=" << state.records.size()
                          << " dialogue_records=" << state.dialogue_records.size()
                          << '\n';
            } else if (arguments.positional.size() == 4 &&
                       mode == "--trace-new-game") {
                state = byzantine::new_game_scene_state(
                    read_metadata(arguments.positional[3].c_str()),
                    std::filesystem::path(arguments.positional[0]).stem().string());
                std::cout << "state_origin=new_game_zero_pool startup_script_executed=false\n";
            }
            const auto save_option = arguments.values.find("--save");
            if (!save_mode && save_option != arguments.values.end()) {
                loaded_save = byzantine::parse_save(
                    read_metadata_file(save_option->second));
                byzantine::load_save(state, *loaded_save);
                std::cout << "state_origin=save file=" << save_option->second
                          << " objects=" << state.objects.size()
                          << " media=" << state.media.size()
                          << " sounds=" << state.sounds.size()
                          << " records=" << state.records.size()
                          << " dialogue_records=" << state.dialogue_records.size()
                          << '\n';
            }
            const auto metadata = read_variable_metadata(arguments.values);
            if (metadata) apply_engine_globals(state, *metadata);
            if (loaded_save) {
                if (metadata) {
                    const auto& layout = byzantine::scene_variable_layout(
                        *metadata,
                        std::filesystem::path(arguments.positional[0]).stem().string());
                    require_matching_scene(*loaded_save, *metadata, layout);
                    const auto restored = byzantine::load_save_variables(
                        state, *loaded_save, layout.global_ids);
                    std::cout << "variables_restored=" << restored
                              << " declared=" << layout.global_ids.size()
                              << " scene=" << layout.scene << '\n';
                } else {
                    std::cout << "variables_restored=0 variables_source=no_engine_dir\n";
                }
            }
            apply_dialogue_inputs(state, arguments.values);
            if (loaded_save) {
                const auto records = byzantine::load_save_record_assets(state);
                if (records != 0) std::cout << "record_assets=" << records << '\n';
            }
            const auto trace = byzantine::trace_scene_label(
                instructions, entries, state, static_cast<std::uint16_t>(parsed_label));
            std::cout << "steps=" << trace.steps << " stopped_at=" << trace.stopped_at;
            if (trace.unsupported_opcode) {
                std::cout << " unsupported_opcode=0x" << std::hex
                          << static_cast<unsigned>(*trace.unsupported_opcode) << std::dec;
            }
            if (trace.unresolved_variable) {
                std::cout << " unresolved_variable=" << *trace.unresolved_variable;
            }
            if (trace.unresolved_random) {
                std::cout << " unresolved_random=true";
            }
            if (trace.unresolved_sound_name) {
                std::cout << " unresolved_sound_name=" << *trace.unresolved_sound_name;
            }
            if (trace.unresolved_dialogue) {
                std::cout << " unresolved_dialogue=" << *trace.unresolved_dialogue;
            }
            if (trace.unresolved_dialogue_channel) {
                std::cout << " unresolved_dialogue_channel="
                          << *trace.unresolved_dialogue_channel;
            }
            if (trace.unresolved_dialogue_inventory) {
                std::cout << " unresolved_dialogue_inventory=true";
            }
            if (trace.unresolved_video_name) {
                std::cout << " unresolved_video_name=" << *trace.unresolved_video_name;
            }
            if (trace.unresolved_video_setting) {
                std::cout << " unresolved_video_setting=true";
            }
            if (trace.unresolved_panorama_node) {
                std::cout << " unresolved_panorama_node=" << *trace.unresolved_panorama_node;
            }
            if (trace.missing_object) {
                std::cout << " missing_object=" << *trace.missing_object;
            }
            if (trace.unresolved_object_label) {
                std::cout << " unresolved_object_label=" << *trace.unresolved_object_label;
            }
            if (trace.unresolved_scene_table) {
                std::cout << " unresolved_scene_table=" << *trace.unresolved_scene_table;
            }
            if (trace.scene_switch_requested) {
                std::cout << " scene_switch_requested=" << *trace.scene_switch_requested;
            }
            if (trace.unresolved_node_asset) {
                std::cout << " unresolved_node_asset=" << *trace.unresolved_node_asset;
            }
            if (trace.missing_node_asset) {
                std::cout << " missing_node_asset=" << *trace.missing_node_asset;
            }
            if (trace.records_created != 0) {
                std::cout << " records_created=" << trace.records_created;
            }
            if (trace.skipped_node_commands != 0) {
                std::cout << " skipped_node_commands=" << trace.skipped_node_commands;
            }
            if (trace.anims_load_failures != 0) {
                std::cout << " anims_load_failures=" << trace.anims_load_failures;
            }
            if (trace.frame_overflow_logs != 0) {
                std::cout << " frame_overflow_logs=" << trace.frame_overflow_logs;
            }
            if (trace.missing_dialogue_chain) {
                std::cout << " missing_dialogue_chain=" << *trace.missing_dialogue_chain;
            }
            if (trace.unresolved_system_input) {
                std::cout << " unresolved_system_input=" << *trace.unresolved_system_input;
            }
            if (trace.unresolved_save_setting) {
                std::cout << " unresolved_save_setting=true";
            }
            if (trace.conversation_request) {
                std::cout << " conversation_request_record="
                          << trace.conversation_request->record
                          << " object_context="
                          << trace.conversation_request->object_context
                          << " instruction="
                          << trace.conversation_request->instruction;
            }
            if (trace.step_limit_exceeded) {
                std::cout << " step_limit_exceeded=true";
            }
            if (trace.script_end_requested) {
                std::cout << " script_end_requested=true";
            }
            if (trace.disc_request) {
                std::cout << " disc_request=" << *trace.disc_request;
            }
            if (trace.missing_media_record) {
                std::cout << " missing_media_record=" << *trace.missing_media_record;
            }
            std::cout << '\n';
            for (const auto& write : trace.object_writes) {
                std::cout << "object_write instruction=" << write.instruction
                          << " id=" << write.id << " value=" << write.value << '\n';
            }
            for (const auto& jump : trace.converse_jumps) {
                std::cout << "converse_jump instruction=" << jump.instruction
                          << " id=" << jump.id << " label=" << jump.label
                          << " target=" << jump.target << '\n';
            }
            if (trace.subtraction_underflows != 0) {
                std::cout << "subtraction_underflows=" << trace.subtraction_underflows << '\n';
            }
            for (const auto& start : trace.sound_starts) {
                std::cout << "sound_start instruction=" << start.instruction
                          << " id=" << start.id << " mode=" << start.mode
                          << " record=" << (start.record_present ? "true" : "false");
                if (start.text) std::cout << " text=" << *start.text;
                if (start.allowed) {
                    std::cout << " allowed=" << (*start.allowed ? "true" : "false");
                } else {
                    std::cout << " allowed=unknown";
                }
                std::cout << '\n';
            }
            for (const auto& write : trace.node_writes) {
                std::cout << "node_write instruction=" << write.instruction
                          << " id=" << write.id << " value04=" << write.value04
                          << " flags=" << static_cast<unsigned>(write.flags) << '\n';
            }
            for (const auto& release : trace.sound_releases) {
                std::cout << "sound_release instruction=" << release.instruction
                          << " id=" << release.id
                          << " record=" << (release.record_present ? "true" : "false")
                          << " field14=" << release.field14 << '\n';
            }
            if (trace.sound_stop_all_requests != 0) {
                std::cout << "sound_stop_all_requests=" << trace.sound_stop_all_requests
                          << '\n';
            }
            for (const auto& session : trace.sound_sessions) {
                std::cout << "sound_session instruction=" << session.instruction
                          << " id=" << session.id
                          << " record=" << (session.record_present ? "true" : "false");
                if (session.allowed) {
                    std::cout << " allowed=" << (*session.allowed ? "true" : "false");
                } else {
                    std::cout << " allowed=unknown";
                }
                for (std::size_t slot = 0; slot < session.timeouts.size(); ++slot) {
                    const auto& timeout = session.timeouts[slot];
                    if (!timeout.defined) continue;
                    std::cout << " timeout" << slot << "=" << timeout.milliseconds
                              << "/" << timeout.follow_up;
                }
                std::cout << '\n';
            }
            for (const auto& media : trace.media_definitions) {
                std::cout << "media_definition instruction=" << media.instruction
                          << " id=" << media.id << " value=" << media.value
                          << " media_id=" << media.media_id
                          << " type=" << static_cast<unsigned>(media.type) << '\n';
            }
            if (trace.media_timestamps_unavailable != 0) {
                std::cout << "media_timestamps_unavailable="
                          << trace.media_timestamps_unavailable << '\n';
            }
            for (const auto& write : trace.media_id_writes) {
                std::cout << "media_id_write instruction=" << write.instruction
                          << " id=" << write.id << " media_id=" << write.media_id
                          << '\n';
            }
            for (const auto& rotation : trace.panorama_rotations) {
                std::cout << "panorama_rotation instruction=" << rotation.instruction
                          << " id=" << rotation.id << " step=" << rotation.step << '\n';
            }
            for (const auto& command : trace.system_commands) {
                std::cout << "system_command instruction=" << command.instruction
                          << " sub=0x" << std::hex << command.sub << std::dec;
                if (command.argument) {
                    std::cout << " argument=" << *command.argument;
                }
                std::cout << '\n';
            }
            if (trace.status_write_requested) {
                std::cout << "status_write_requested=" << *trace.status_write_requested
                          << '\n';
            }
            if (trace.tempsave_requests != 0) {
                std::cout << "tempsave_requests=" << trace.tempsave_requests << '\n';
            }
            if (trace.autosave_requests != 0) {
                std::cout << "autosave_requests=" << trace.autosave_requests << '\n';
            }
            std::cout << "dialogue_visible="
                      << (state.dialogue_visible ? "true" : "false")
                      << " dialogue_input_active="
                      << (state.dialogue_input_active ? "true" : "false")
                      << " flag_4859cd=" << (state.flag_4859cd ? "true" : "false")
                      << " field_460668=" << state.field_460668 << '\n';
            for (const auto& [id, entry] : state.records) {
                if (entry.frame_count == 0 && !entry.current_frame) continue;
                std::cout << "record id=" << id << " frames=" << entry.frame_count;
                if (entry.current_frame) {
                    std::cout << " current=" << *entry.current_frame;
                } else {
                    std::cout << " current=none";
                }
                std::cout << " x=" << entry.x << " y=" << entry.y
                          << " frames_known=" << (entry.frames_known ? "true" : "false")
                          << '\n';
                // End-to-end check: the VM's current frame decodes to pixels.
                if (entry.frames_known && entry.current_frame && state.anims_library &&
                    id < state.actor_assets.size()) {
                    const auto decoded = state.anims_library->decode(
                        state.actor_assets[id].name, *entry.current_frame);
                    if (decoded) {
                        std::size_t covered = 0;
                        for (const auto value : decoded->covered) covered += value != 0;
                        std::cout << "record_frame id=" << id
                                  << " index=" << *entry.current_frame
                                  << " size=" << decoded->width << "x" << decoded->height
                                  << " covered=" << covered << "/"
                                  << decoded->pixels.size() << '\n';
                    }
                }
            }
            if (state.pending_panorama_x) {
                std::cout << "pending_panorama_x=" << *state.pending_panorama_x << '\n';
            }
            if (trace.background) {
                std::cout << "background_id=" << trace.background->id
                          << " name=" << trace.background->entry.name << '\n';
            }
            if (trace.resource_refresh_requests != 0) {
                std::cout << "resource_refresh_requests=" << trace.resource_refresh_requests
                          << " close_existing_resources="
                          << (trace.close_existing_resources ? "true" : "false") << '\n';
            }
            if (trace.dialogue_display_requests != 0 ||
                trace.dialogue_refresh_requests != 0) {
                std::cout << "dialogue_display_requests="
                          << trace.dialogue_display_requests
                          << " dialogue_refresh_requests="
                          << trace.dialogue_refresh_requests << '\n';
            }
            if (trace.sound_refresh_requests != 0) {
                std::cout << "sound_refresh_requests=" << trace.sound_refresh_requests << '\n';
            }
            for (const auto& load_item : trace.panorama_loads) {
                std::cout << "panorama_record=" << load_item.record
                          << " name=" << load_item.name
                          << " jpeg=" << load_item.jpeg_path
                          << " force=" << load_item.force
                          << " vertical=" << load_item.vertical
                          << " transition=" << load_item.transition;
                if (load_item.horizontal) {
                    std::cout << " horizontal=" << *load_item.horizontal;
                }
                if (load_item.flag_clear) {
                    std::cout << " flag_clear=" << *load_item.flag_clear;
                }
                std::cout << '\n';
            }
            if (trace.panorama_flag_clears != 0) {
                std::cout << "panorama_flag_clears=" << trace.panorama_flag_clears << '\n';
            }
            if (trace.video_skipped != 0) {
                std::cout << "video_skipped=" << trace.video_skipped << '\n';
            }
            for (const auto& video : trace.video_requests) {
                std::cout << "video_record=" << video.record << " name=" << video.name
                          << " mode=" << video.mode;
                if (video.start) std::cout << " start=" << *video.start;
                if (video.end) std::cout << " end=" << *video.end;
                if (video.transition) {
                    std::cout << " transition=" << *video.transition;
                }
                std::cout << '\n';
            }
            if (trace.dialogue_name_fallbacks != 0) {
                std::cout << "dialogue_name_fallbacks="
                          << trace.dialogue_name_fallbacks << '\n';
            }
            for (const auto& [id, record] : state.dialogue_records) {
                std::cout << "dialogue_id=" << id
                          << " state=" << static_cast<unsigned>(record.state)
                          << " inventory_loaded=" << (record.inventory_loaded ? "true" : "false")
                          << '\n';
            }
            for (const auto& [id, chain] : state.dialogue_chains) {
                std::cout << "dialogue_chain id=" << id << " count=" << chain.count
                          << " ids=";
                for (std::uint32_t index = 0; index < chain.count; ++index) {
                    std::cout << (index == 0 ? "" : ",") << chain.ids[index];
                }
                std::cout << '\n';
            }
            for (std::size_t channel = 0; channel < state.dialogue_channels.size(); ++channel) {
                if (state.dialogue_channels[channel].empty()) continue;
                std::cout << "dialogue_channel=" << channel << " stack=";
                for (const auto entry : state.dialogue_channels[channel]) {
                    std::cout << entry << ',';
                }
                std::cout << '\n';
            }
            for (const auto& text : trace.text_requests) {
                std::cout << "text_id=" << text.id << " mode=" << text.mode << '\n';
            }
            for (const auto& hotspot : trace.active_hotspots) {
                std::cout << "slot=" << hotspot.slot << " x=" << hotspot.x
                          << " y=" << hotspot.y << " width=" << hotspot.width
                          << " height=" << hotspot.height << '\n';
            }
            // Optional composition: draw every record whose current frame
            // decodes (0x43b216's kind 0..4 pass) into an explicit surface and
            // write it as PPM. No window, no blending.
            const auto compose_out = arguments.values.find("--compose-out");
            if (compose_out != arguments.values.end()) {
                std::int32_t width = 640;
                std::int32_t height = 400;
                const auto surface_option = arguments.values.find("--surface");
                if (surface_option != arguments.values.end()) {
                    const auto text = surface_option->second;
                    const auto cross = text.find('x');
                    if (cross == std::string::npos) {
                        throw std::runtime_error("--surface expects <width>x<height>");
                    }
                    width = std::stoi(text.substr(0, cross));
                    height = std::stoi(text.substr(cross + 1));
                }
                auto surface = byzantine::make_surface(width, height, 0);
                // The background/panorama stage runs before the record pass.
                // 0x41045b decodes backs\<2ch>\<name>.jpg into the scene's
                // 640x400 +0x6400 buffer (it fails unless the image is exactly
                // 640x400); 0xb9's nodes\<2ch>\<name>.jpg panorama is sampled
                // per viewport pixel through 0x4242bd.
                const auto background_option = arguments.values.find("--background");
                const auto panorama_option = arguments.values.find("--panorama");
                if (background_option != arguments.values.end() &&
                    panorama_option != arguments.values.end()) {
                    throw std::runtime_error(
                        "--background and --panorama are mutually exclusive");
                }
                if (background_option != arguments.values.end()) {
                    const auto image =
                        byzantine::decode_jpeg_file(background_option->second);
                    // 0x41045b passes an expected 640x400 to 0x40f135 and the
                    // load FAILS (returns true) on any other size, so a
                    // background of another size is an error, not a resize.
                    if (image.width != 640 || image.height != 400) {
                        throw std::runtime_error(
                            "--background must be 640x400 (0x41045b -> 0x40f135): " +
                            background_option->second);
                    }
                    const auto pixels = byzantine::rgb_to_rgb565(image);
                    const auto blit = byzantine::blit_image(
                        surface, image.width, image.height, pixels.data(), 0, 0);
                    std::cout << "background=" << background_option->second
                              << " size=" << image.width << 'x' << image.height
                              << " written=" << blit.written
                              << " clipped=" << blit.clipped << '\n';
                }
                if (panorama_option != arguments.values.end()) {
                    const auto image =
                        byzantine::decode_jpeg_file(panorama_option->second);
                    int staged_x = -1;
                    const auto staged_option =
                        arguments.values.find("--panorama-x");
                    if (staged_option != arguments.values.end()) {
                        staged_x = std::stoi(staged_option->second);
                        if (staged_x < 0 || staged_x >= image.width) {
                            throw std::runtime_error(
                                "--panorama-x must be inside 0..width-1");
                        }
                    }
                    // The same PanoramaView the player uses: 0x4236b5's seam
                    // rows, 0x401ddf's staged position, 0x4242bd's sampling.
                    byzantine::PanoramaView view(panorama_option->second, image.width,
                                                 image.height,
                                                 byzantine::rgb_to_rgb565(image),
                                                 surface.width, surface.height);
                    const auto setup = view.apply_staged_x(staged_x, false);
                    view.render(surface);
                    std::cout << "panorama=" << panorama_option->second
                              << " size=" << image.width << 'x' << image.height
                              << " padding=" << view.padding()
                              << " yaw_degrees=" << setup.horizontal_degrees
                              << " pitch_degrees=" << setup.vertical_degrees
                              << " sampled="
                              << (surface.width * surface.height) << '\n';
                }
                const auto composed = byzantine::compose_scene(state, surface);
                std::cout << "composed size=" << surface.width << 'x' << surface.height
                          << " drawn=" << composed.drawn
                          << " skipped_no_frame=" << composed.skipped_no_frame
                          << " skipped_other_kind=" << composed.skipped_other_kind
                          << " skipped_snap=" << composed.skipped_snap
                          << " snap_drawn=" << composed.snap_drawn
                          << " skipped_hidden=" << composed.skipped_hidden
                          << " pixels=" << composed.pixels_written;
                if (compose_out->second != "-") {
                    if (!byzantine::write_ppm(surface, compose_out->second)) {
                        throw std::runtime_error("cannot write the composed frame");
                    }
                    std::cout << " file=" << compose_out->second;
                }
                std::cout << '\n';
                for (const auto& draw : composed.records) {
                    std::cout << "draw id=" << draw.id
                              << " kind=" << static_cast<unsigned>(draw.kind)
                              << " x=" << draw.blit.x << " y=" << draw.blit.y
                              << " written=" << draw.blit.written
                              << " clipped=" << draw.blit.clipped
                              << (draw.blit.drawn ? "" : " offscreen") << '\n';
                }
                // Coarse luminance map so the result can be checked without a viewer.
                const std::string ramp = " .:-=+*#%@";
                const std::size_t step_y = std::max<std::size_t>(
                    1, static_cast<std::size_t>(surface.height) / 30);
                const std::size_t step_x = std::max<std::size_t>(
                    1, static_cast<std::size_t>(surface.width) / 70);
                for (std::size_t y = 0; y < static_cast<std::size_t>(surface.height);
                     y += step_y) {
                    std::string line;
                    for (std::size_t x = 0; x < static_cast<std::size_t>(surface.width);
                         x += step_x) {
                        std::uint32_t sum = 0;
                        std::size_t count = 0;
                        for (std::size_t dy = 0; dy < step_y; ++dy) {
                            for (std::size_t dx = 0; dx < step_x; ++dx) {
                                const auto yy = std::min<std::size_t>(
                                    y + dy, static_cast<std::size_t>(surface.height) - 1);
                                const auto xx = std::min<std::size_t>(
                                    x + dx, static_cast<std::size_t>(surface.width) - 1);
                                const auto pixel = surface.at(static_cast<std::int32_t>(xx),
                                                              static_cast<std::int32_t>(yy));
                                sum += 2 * ((pixel >> 11) & 0x1f) + ((pixel >> 5) & 0x3f) +
                                       2 * (pixel & 0x1f);
                                ++count;
                            }
                        }
                        line += ramp[std::min<std::size_t>(9, (sum / count) / 20)];
                    }
                    std::cout << line << '\n';
                }
            }
            return 0;
        }
        if (mode == "--flow") {
            require_positional(arguments, 1, 1);
            const auto flow = byzantine::resolve_control_flow(instructions);
            std::cout << "labels=" << flow.labels.size()
                      << " duplicate_labels=" << flow.duplicate_labels
                      << " transfers=" << flow.transfers.size()
                      << " returns=" << flow.returns << '\n';
            for (const auto& transfer : flow.transfers) {
                std::cout << "instruction=" << transfer.instruction
                          << " kind=" << (transfer.opcode == 0x76 ? "jump" : "call")
                          << " label=" << transfer.label
                          << " target=" << transfer.target_instruction << '\n';
            }
            return 0;
        }
        if (mode == "--labels") {
            require_positional(arguments, 1, 1);
            const auto flow = byzantine::resolve_control_flow(instructions);
            for (const auto& [label, target] : flow.labels) {
                std::cout << "label=" << label << " target=" << target << '\n';
            }
            return 0;
        }
        if (mode == "--hotspots") {
            require_positional(arguments, 1, 1);
            const auto hotspots = byzantine::static_hotspots(instructions);
            for (const auto& hotspot : hotspots) {
                std::cout << "instruction=" << hotspot.instruction << " slot=" << hotspot.slot
                          << " x=" << hotspot.x << " y=" << hotspot.y
                          << " width=" << hotspot.width << " height=" << hotspot.height
                          << " action=" << hotspot.action << " mode=" << hotspot.mode << '\n';
            }
            return 0;
        }
        require_positional(arguments, 1, 1);
        std::cout << "instructions: " << instructions.size() << "\n";
        for (std::size_t i = 0; i < instructions.size(); ++i) {
            std::cout << std::dec << i << " opcode=0x" << std::hex << std::setw(2)
                      << std::setfill('0') << static_cast<unsigned>(instructions[i].opcode);
            for (const auto& argument : instructions[i].arguments) {
                std::cout << " type=" << std::dec << static_cast<unsigned>(argument.type) << ":0x";
                for (const auto byte : argument.encoded_value) {
                    std::cout << std::hex << std::setw(2) << std::setfill('0')
                              << static_cast<unsigned>(byte);
                }
            }
            std::cout << '\n';
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
