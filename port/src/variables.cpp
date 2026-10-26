#include "byzantine/variables.hpp"

#include <algorithm>
#include <set>
#include <stdexcept>

namespace byzantine {
namespace {
std::string lower_scene(std::string name) {
    for (auto& c : name) {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + ('a' - 'A'));
    }
    return name;
}

std::uint16_t read_word(const std::vector<std::uint8_t>& bytes, std::size_t& at) {
    if (at > bytes.size() || bytes.size() - at < 2) {
        throw std::runtime_error("truncated variable metadata word");
    }
    const auto value = static_cast<std::uint16_t>(bytes[at] | (bytes[at + 1] << 8));
    at += 2;
    return value;
}
}

VariableMetadata parse_variable_metadata(const std::vector<std::uint8_t>& lcc,
                                         const std::vector<std::uint8_t>& lcv,
                                         const std::vector<std::uint8_t>& lvm) {
    VariableMetadata result;
    std::set<std::string> scene_names;
    std::size_t at = 0;
    while (at < lcc.size()) {
        const auto start = at;
        while (at < lcc.size() && lcc[at] != '\n') ++at;
        std::string name(lcc.begin() + start, lcc.begin() + at);
        if (!name.empty() && name.back() == '\r') name.pop_back();
        if (at < lcc.size()) ++at;
        name = lower_scene(name);
        if (name.empty() || name.size() > 8 || name.find('\0') != std::string::npos ||
            !scene_names.insert(name).second || result.scenes.size() >= 50) {
            throw std::runtime_error("invalid or duplicate LCC scene name");
        }
        result.scenes.push_back({name, {}});
    }
    if (result.scenes.empty()) throw std::runtime_error("empty LCC scene list");

    at = 0;
    const auto names = read_word(lvm, at);
    if (names > 3000) throw std::runtime_error("LVM exceeds global variable pool");
    for (std::size_t i = 0; i < names; ++i) {
        const auto start = at;
        while (at < lvm.size() && lvm[at] != 0) ++at;
        if (at == lvm.size()) throw std::runtime_error("unterminated LVM variable name");
        result.names.emplace_back(lvm.begin() + start, lvm.begin() + at);
        ++at;
    }
    if (at != lvm.size()) throw std::runtime_error("unexpected bytes after LVM names");

    at = 0;
    for (auto& scene : result.scenes) {
        const auto count = read_word(lcv, at);
        if (count > 300) throw std::runtime_error("LCV exceeds per-scene variable table");
        for (std::size_t i = 0; i < count; ++i) {
            const auto id = read_word(lcv, at);
            if (id >= result.names.size()) throw std::runtime_error("LCV global ID outside LVM names");
            scene.global_ids.push_back(id);
        }
    }
    if (at != lcv.size()) throw std::runtime_error("unexpected bytes after LCV scene maps");
    return result;
}

const SceneVariableLayout& scene_variable_layout(const VariableMetadata& metadata,
                                                 std::string_view scene) {
    const auto name = lower_scene(std::string(scene));
    const auto found = std::find_if(metadata.scenes.begin(), metadata.scenes.end(),
                                  [&](const SceneVariableLayout& entry) { return entry.scene == name; });
    if (found == metadata.scenes.end()) throw std::runtime_error("scene missing from LCC: " + std::string(scene));
    return *found;
}

SceneState new_game_scene_state(const VariableMetadata& metadata, std::string_view scene) {
    SceneState state;
    const auto& layout = scene_variable_layout(metadata, scene);
    for (std::size_t i = 0; i < layout.global_ids.size(); ++i) {
        state.variables.emplace(static_cast<std::uint16_t>(i), 0);
    }
    return state;
}

namespace {
// The 0x46ee00 row entry for one local operand: an LCV global id, or the
// identity past the declared count (0x42c8a0 initialises all 300 that way).
std::size_t mapped_global(std::size_t local, const SceneVariableLayout& layout) {
    return local < layout.global_ids.size() ? layout.global_ids[local] : local;
}
}  // namespace

std::vector<std::uint16_t> export_variable_pool(const SceneState& state,
                                                const SceneVariableLayout& layout) {
    if (layout.global_ids.size() > 300) {
        throw std::runtime_error("variable layout exceeds the 300-entry scene table");
    }
    // Start from the carried pool (zeros when 0x42c8a0's clear is all that
    // happened). A known pool is the arrival state, so only the locals the
    // script actually wrote are replayed, in write order: that keeps an aliased
    // global at the value the script last stored instead of letting the identity
    // tail overwrite it. Without a pool the projection itself is the only
    // information, so all 300 locals are written out.
    std::vector<std::uint16_t> pool(3000, 0);
    const bool pool_known = !state.variable_pool.empty();
    if (pool_known) {
        if (state.variable_pool.size() != pool.size()) {
            throw std::runtime_error("carried variable pool is not the 3000-word table");
        }
        pool = state.variable_pool;
    }
    const auto store = [&](std::uint16_t local, std::uint16_t value) {
        const auto global = mapped_global(local, layout);
        if (global >= pool.size()) {
            throw std::runtime_error("variable mapping outside the 3000-word pool");
        }
        pool[global] = value;
    };
    if (pool_known) {
        for (const auto local : state.variable_writes) {
            const auto found = state.variables.find(local);
            if (found != state.variables.end()) store(local, found->second);
        }
    } else {
        for (std::size_t local = 0; local < 300; ++local) {
            const auto found = state.variables.find(static_cast<std::uint16_t>(local));
            if (found != state.variables.end()) {
                store(static_cast<std::uint16_t>(local), found->second);
            }
        }
    }
    return pool;
}

void apply_variable_pool(SceneState& state, const std::vector<std::uint16_t>& pool,
                         const SceneVariableLayout& layout) {
    if (layout.global_ids.size() > 300) {
        throw std::runtime_error("variable layout exceeds the 300-entry scene table");
    }
    if (pool.size() != 3000) {
        throw std::runtime_error("variable pool is not the 3000-word table");
    }
    state.variable_pool = pool;
    // A freshly imported pool IS the arrival state: nothing has been written
    // since, so the replay log starts empty.
    state.variable_writes.clear();
    state.variables.clear();
    for (std::uint16_t local = 0; local < 300; ++local) {
        const auto global = mapped_global(local, layout);
        state.variables[local] = pool[global];
    }
}
}  // namespace byzantine
