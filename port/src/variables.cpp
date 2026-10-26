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
}  // namespace byzantine
