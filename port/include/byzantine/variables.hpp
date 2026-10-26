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

// DAT_0047e2f0, the 3000-word variable pool. 0x42c8a0 clears it once, before
// its scene loop, and every scene reads and writes it through its own
// DAT_0046ee00 row (local -> global; identity past the LCV count). A scene
// switch therefore carries the pool, not the per-scene local numbering:
// `export_variable_pool` writes the current scene's 300 locals back into their
// global slots and `apply_variable_pool` projects the pool into another scene's
// 300 locals. Both are exact inverses for a local that the scene can address.
std::vector<std::uint16_t> export_variable_pool(const SceneState& state,
                                                const SceneVariableLayout& layout);
void apply_variable_pool(SceneState& state, const std::vector<std::uint16_t>& pool,
                         const SceneVariableLayout& layout);

}  // namespace byzantine
