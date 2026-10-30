#include "byzantine/scene_setup.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace byzantine {

std::vector<std::uint8_t> read_metadata_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("cannot open metadata: " + path.string());
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

VariableMetadata read_metadata(const char* directory) {
    const std::filesystem::path root(directory);
    return parse_variable_metadata(read_metadata_file(root / "SHOW.LCC"),
                                   read_metadata_file(root / "SHOW.LCV"),
                                   read_metadata_file(root / "SHOW.LVM"));
}

std::optional<VariableMetadata> read_variable_metadata(
    const std::map<std::string, std::string>& options) {
    const auto engine = options.find("--engine");
    if (engine == options.end()) return std::nullopt;
    return read_metadata(engine->second.c_str());
}

void apply_engine_globals(SceneState& state, const VariableMetadata& metadata) {
    state.examine_scene_index = -1;
    for (std::size_t index = 0; index < metadata.scenes.size(); ++index) {
        if (metadata.scenes[index].scene == "examine") {
            state.examine_scene_index = static_cast<std::int32_t>(index);
            break;
        }
    }
}

void require_matching_scene(const SaveGame& save, const VariableMetadata& metadata,
                            const SceneVariableLayout& layout) {
    if (save.scene_index >= metadata.scenes.size()) {
        throw std::runtime_error("save scene index outside the engine scene list");
    }
    const auto& saved_scene = metadata.scenes[save.scene_index].scene;
    if (saved_scene != layout.scene) {
        throw std::runtime_error("save scene " + saved_scene +
                                 " does not match trace scene " + layout.scene);
    }
}

// One lowercase media name per line. 0x4077b2's dialogue stack push depends on
// the inventor/hotitems TGAs that exist on the shipped CDs, so the caller
// supplies exactly the shipped names instead of guessing a media list.
std::set<std::string> read_name_list(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("cannot open name list: " + path.string());
    std::set<std::string> names;
    std::string name;
    while (file >> name) {
        for (char& character : name) {
            character = static_cast<char>(
                std::tolower(static_cast<unsigned char>(character)));
        }
        names.insert(name);
    }
    return names;
}

void apply_dialogue_inputs(SceneState& state,
                           const std::map<std::string, std::string>& options) {
    const auto load = [&options](const char* key, std::vector<NamedRecord>& target,
                                 bool* known) {
        const auto found = options.find(key);
        if (found == options.end()) return;
        target = parse_named_records(read_metadata_file(found->second));
        if (known != nullptr) *known = true;
    };
    auto& tables = state.dialogue_tables;
    load("--lci", tables.names, nullptr);
    load("--ci", tables.scene_names, &tables.scene_names_known);
    load("--lcy", tables.channels, nullptr);
    load("--cy", tables.scene_channels, &tables.scene_channels_known);
    const auto inventor = options.find("--inventor");
    const auto hotitems = options.find("--hotitems");
    if (inventor != options.end() && hotitems != options.end()) {
        state.inventor_assets = read_name_list(inventor->second);
        state.hotitem_assets = read_name_list(hotitems->second);
        state.inventory_assets_known = true;
    }
    const auto panorama = options.find("--co");
    if (panorama != options.end()) {
        state.panorama_records = parse_cb(read_metadata_file(panorama->second));
        state.panorama_table_known = true;
    }
    const auto switch_table = options.find("--lcx");
    if (switch_table != options.end()) {
        state.scene_switch_targets =
            parse_scene_switch_table(read_metadata_file(switch_table->second));
        state.scene_switch_table_known = true;
    }
    const auto video = options.find("--cf");
    if (video != options.end()) {
        state.video_records = parse_named_records(read_metadata_file(video->second));
        state.video_table_known = true;
    }
    const auto conversations = options.find("--cd");
    if (conversations != options.end()) {
        state.conversation_records =
            parse_conversation_records(read_metadata_file(conversations->second));
        state.conversation_table_known = true;
    }
    const auto anims = options.find("--anims");
    if (anims != options.end()) {
        // 0x40fe70 resolves "anims\<name[0]><name[1]>\<name>.rle"; the library
        // indexes the extracted tree case-insensitively.
        const auto preferred = options.find("--anims-overlay");
        state.anims_library = std::make_shared<AnimsLibrary>(anims->second,
            preferred == options.end() ? "" : preferred->second);
    }
    const auto assets = options.find("--ca");
    if (assets != options.end()) {
        // engine/<scene>ca (DAT_0047dd70): the animation asset table whose +0x08
        // type byte decides whether a DAT_004649e8 record can exist.
        state.actor_assets = parse_actor_records(read_metadata_file(assets->second));
        state.actor_assets_known = true;
    }
    const auto preferences = options.find("--gameprefs");
    if (preferences != options.end()) {
        // savegame/gamepref_%03d: seven little-endian dwords (0x41f753).
        const auto data = read_metadata_file(preferences->second);
        if (data.size() != 28) {
            throw std::runtime_error("gamepref file must hold seven 32-bit words");
        }
        std::array<std::uint32_t, 7> values{};
        for (std::size_t index = 0; index < values.size(); ++index) {
            values[index] = static_cast<std::uint32_t>(data[index * 4]) |
                            (static_cast<std::uint32_t>(data[index * 4 + 1]) << 8) |
                            (static_cast<std::uint32_t>(data[index * 4 + 2]) << 16) |
                            (static_cast<std::uint32_t>(data[index * 4 + 3]) << 24);
        }
        state.gameprefs = values;
    }
    // 0xc4 compares the sound name with "endgame", so it needs the per-scene
    // sound table. The CLI supplies it through the census positional; the scene
    // player supplies the same file as "--cs".
    const auto sounds = options.find("--cs");
    if (sounds != options.end()) {
        state.sound_records = parse_named_records(read_metadata_file(sounds->second));
        state.sound_table_known = true;
    }
}

SceneSetup load_scene_setup(const std::string& cc_path,
                            const std::map<std::string, std::string>& options,
                            const SaveGame* saved_image) {
    SceneSetup setup;
    setup.scene = std::filesystem::path(cc_path).stem().string();
    const auto save_option = options.find("--save");
    if (saved_image != nullptr || save_option != options.end()) {
        setup.save = saved_image ? *saved_image : parse_save(read_metadata_file(save_option->second));
        load_save(setup.state, *setup.save);
        setup.summary = "state_origin=save objects=" +
                        std::to_string(setup.state.objects.size()) +
                        " media=" + std::to_string(setup.state.media.size()) +
                        " sounds=" + std::to_string(setup.state.sounds.size()) +
                        " records=" + std::to_string(setup.state.records.size());
    } else {
        setup.summary = "state_origin=empty";
    }
    const auto metadata = read_variable_metadata(options);
    if (metadata) apply_engine_globals(setup.state, *metadata);
    if (setup.save) {
        if (metadata) {
            const auto& layout = scene_variable_layout(*metadata, setup.scene);
            require_matching_scene(*setup.save, *metadata, layout);
            const auto restored =
                load_save_variables(setup.state, *setup.save, layout.global_ids);
            setup.scene = layout.scene;
            setup.summary += " variables_restored=" + std::to_string(restored) +
                             " declared=" + std::to_string(layout.global_ids.size());
        } else if (!options.count("--engine")) {
            throw std::runtime_error(
                "--save needs --engine for the carried variable pool projection");
        }
    } else if (metadata) {
        // 0x42c8a0 initialises DAT_0047e2f0 to 3000 zero words before the first
        // scene runs. Without a save the port must do the same, otherwise every
        // identity local (past the declared LCV count) is "unresolved" instead
        // of the original's zero. A save still replaces this with its own pool.
        const auto& layout = scene_variable_layout(*metadata, setup.scene);
        apply_variable_pool(setup.state, std::vector<std::uint16_t>(3000, 0), layout);
        setup.summary += " variables_zero_pool=300 declared=" +
                         std::to_string(layout.global_ids.size());
    }
    apply_dialogue_inputs(setup.state, options);
    if (setup.save) {
        // Reattach marker-0 CA type 2/3 preloads and marker-1 lazy assets.
        // The anims/CA inputs are only available now.
        const auto records = load_save_record_assets(setup.state);
        if (records != 0) {
            setup.summary += " record_assets=" + std::to_string(records);
        }
    }
    return setup;
}

}  // namespace byzantine
