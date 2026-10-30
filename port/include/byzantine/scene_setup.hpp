#pragma once

#include "byzantine/cc.hpp"
#include "byzantine/save.hpp"
#include "byzantine/variables.hpp"

#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace byzantine {

// Reads one metadata/asset file. Throws when the file cannot be opened.
std::vector<std::uint8_t> read_metadata_file(const std::filesystem::path& path);

// SHOW.LCC/LCV/LVM from an engine directory (the carried variable pool).
VariableMetadata read_metadata(const char* directory);

// As above, driven by the "--engine" option; nullopt when it was not supplied.
std::optional<VariableMetadata> read_variable_metadata(
    const std::map<std::string, std::string>& options);

// DAT_00460670: the examine scene index from SHOW.LCC, or -1. This is the gate
// 0xbe/0xd2 use.
void apply_engine_globals(SceneState& state, const VariableMetadata& metadata);

// The save's DAT_00482428 selects a SHOW.LCC scene row. A save written in
// another scene must not be projected through this script's mapping.
void require_matching_scene(const SaveGame& save, const VariableMetadata& metadata,
                            const SceneVariableLayout& layout);

// One lowercase media name per line (inventor/hotitems listings).
std::set<std::string> read_name_list(const std::filesystem::path& path);

// Loads the dialogue/video/panorama/CA/anims/gamepref tables from the same
// option names `byzantine-cc` uses, plus an optional "--cs" per-scene sound
// table (DAT_0047a8e0 / engine/<scene>cs).
void apply_dialogue_inputs(SceneState& state,
                           const std::map<std::string, std::string>& options);

// One scene, ready to trace: the state a `--trace-save` run starts from.
struct SceneSetup {
    SceneState state;
    std::optional<SaveGame> save;
    std::string scene;    // CC stem (or the save's scene when they match)
    std::string summary;  // one-line diagnostic
};

// Builds SceneSetup from the same options the CLI accepts: --save, --engine,
// --lcx, --lci/--ci/--lcy/--cy, --inventor/--hotitems, --co, --cf, --ca,
// --anims, --gameprefs, --cs. `cc_path` is only used for the scene name (the
// save's DAT_00482428 projection must match it).
SceneSetup load_scene_setup(const std::string& cc_path,
                            const std::map<std::string, std::string>& options,
                            const SaveGame* saved_image = nullptr);

}  // namespace byzantine
