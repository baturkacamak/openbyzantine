#pragma once

#include "byzantine/cc.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace byzantine {

struct SceneVariableLayout {
    std::string scene;
    std::vector<std::uint16_t> global_ids;
};

struct VariableMetadata {
    std::vector<std::string> names;
    std::vector<SceneVariableLayout> scenes;
};

// SHOW.LCC scene order, SHOW.LCV per-scene maps, SHOW.LVM global names.
// FUN_00438173 reads these; FUN_0042c8a0 establishes a zeroed global pool.
VariableMetadata parse_variable_metadata(const std::vector<std::uint8_t>& lcc,
                                         const std::vector<std::uint8_t>& lcv,
                                         const std::vector<std::uint8_t>& lvm);
const SceneVariableLayout& scene_variable_layout(const VariableMetadata& metadata,
                                                 std::string_view scene);

// Explicit fresh-game snapshot of declared local variables only. This does
// not execute the startup script or reconstruct a mid-game scene's history.
SceneState new_game_scene_state(const VariableMetadata& metadata, std::string_view scene);

}  // namespace byzantine
