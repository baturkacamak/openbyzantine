#include "byzantine/variables.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace {
using Bytes = std::vector<std::uint8_t>;
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
Bytes read_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("missing variable metadata fixture");
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
void rejects(const Bytes& lcc, const Bytes& lcv, const Bytes& lvm) {
    bool rejected = false;
    try { byzantine::parse_variable_metadata(lcc, lcv, lvm); }
    catch (const std::runtime_error&) { rejected = true; }
    require(rejected, "accepted malformed variable metadata");
}
}
int main(int argc, char** argv) {
    try {
        // Two scenes share global 0 at different local IDs. Preserve raw name
        // spelling and normalize scene lookup, including DOS line endings.
        const Bytes lcc{'A', '\r', '\n', 'B', '\n'};
        const Bytes lcv{2, 0, 1, 0, 0, 0, 1, 0, 0, 0};
        const Bytes lvm{2, 0, 'S', 'h', 'a', 'r', 'e', 'd', 0, 'X', 0};
        const auto metadata = byzantine::parse_variable_metadata(lcc, lcv, lvm);
        require(metadata.names[0] == "Shared", "variable name changed");
        require(byzantine::scene_variable_layout(metadata, "A").global_ids[1] ==
                byzantine::scene_variable_layout(metadata, "b").global_ids[0],
                "shared global variable mapping lost");
        const auto initial = byzantine::new_game_scene_state(metadata, "a");
        require(initial.variables.size() == 2 && initial.variables.at(0) == 0 &&
                initial.variables.at(1) == 0 && !initial.variables.count(2) &&
                initial.hotspot_enabled.empty(), "fresh snapshot invented scene execution");
        bool unknown_scene = false;
        try { byzantine::scene_variable_layout(metadata, "missing"); }
        catch (const std::runtime_error&) { unknown_scene = true; }
        require(unknown_scene, "unknown scene accepted");
        for (std::size_t i = 0; i < lcv.size(); ++i) {
            rejects(lcc, Bytes(lcv.begin(), lcv.begin() + i), lvm);
        }
        for (std::size_t i = 0; i < lvm.size(); ++i) {
            rejects(lcc, lcv, Bytes(lvm.begin(), lvm.begin() + i));
        }
        rejects({}, lcv, lvm);
        rejects({'a', '\n', 'A', '\n'}, lcv, lvm);
        rejects({'a', 0, '\n'}, lcv, lvm);
        rejects({'1','2','3','4','5','6','7','8','9'}, lcv, lvm);
        auto bad = lcv;
        bad.push_back(0);
        rejects(lcc, bad, lvm);
        bad = lcv; bad[2] = 2;  // global ID beyond the two names
        rejects(lcc, bad, lvm);
        bad = lcv; bad[0] = 45; bad[1] = 1;  // 301 local entries
        rejects(lcc, bad, lvm);
        bad = lvm; bad.push_back(0);
        rejects(lcc, lcv, bad);
        bad = lvm; bad[0] = 185; bad[1] = 11; // 3001 globals
        rejects(lcc, lcv, bad);
        // DAT_0047e2f0 crosses a scene switch through each scene's own LCV row:
        // A maps locals 0/1 to globals 400/2, B maps them to 2/401. A write A
        // makes is visible to B through their shared global, and every untouched
        // pool word survives the round trip.
        const byzantine::SceneVariableLayout layout_a{"a", {400, 2}};
        const byzantine::SceneVariableLayout layout_b{"b", {2, 401}};
        byzantine::SceneState state_a;
        state_a.variable_pool.assign(3000, 0);
        state_a.variable_pool[400] = 11;
        state_a.variable_pool[2] = 22;
        state_a.variable_pool[7] = 77;
        byzantine::apply_variable_pool(state_a, state_a.variable_pool, layout_a);
        require(state_a.variables.size() == 300 && state_a.variables.at(0) == 11 &&
                state_a.variables.at(1) == 22 && state_a.variables.at(7) == 77,
                "carried pool projection mismatch");
        // 0x42c8a0 clears the whole 3000-word pool before the first scene. A
        // no-save entry now applies that zero pool, so locals past the declared
        // LCV count (identity) are 0 instead of "unresolved".
        {
            byzantine::SceneState zeroed;
            byzantine::apply_variable_pool(zeroed, std::vector<std::uint16_t>(3000, 0),
                                           layout_a);
            require(zeroed.variable_pool.size() == 3000 &&
                        zeroed.variables.size() == 300 &&
                        zeroed.variables.at(0) == 0 &&
                        zeroed.variables.at(1) == 0 &&
                        zeroed.variables.at(299) == 0,
                    "zero pool did not initialise all 300 locals");
        }
        state_a.variables[1] = 99;  // A local 1 -> global 2
        state_a.variable_writes.push_back(1);
        const auto pool = byzantine::export_variable_pool(state_a, layout_a);
        require(pool.size() == 3000 && pool[400] == 11 && pool[2] == 99 && pool[7] == 77 &&
                pool[3] == 0, "export lost a written or an untouched pool word");
        // A scene that writes nothing must hand its arrival pool on unchanged.
        byzantine::SceneState untouched = state_a;
        untouched.variable_writes.clear();
        require(byzantine::export_variable_pool(untouched, layout_a) == state_a.variable_pool,
                "export changed a pool the scene never wrote");
        byzantine::SceneState state_b;
        byzantine::apply_variable_pool(state_b, pool, layout_b);
        require(state_b.variables.at(0) == 99 && state_b.variables.at(1) == 0 &&
                state_b.variable_pool.at(7) == 77,
                "B did not read A's shared global through its own mapping");
        state_b.variables[0] = 123;  // B local 0 -> global 2
        state_b.variable_writes.push_back(0);
        const auto back_pool = byzantine::export_variable_pool(state_b, layout_b);
        byzantine::SceneState back;
        byzantine::apply_variable_pool(back, back_pool, layout_a);
        require(back.variables.at(1) == 123 && back.variables.at(0) == 11 &&
                back.variable_pool.at(7) == 77 && back.variable_pool.at(3) == 0,
                "return trip lost B's write or an untouched global");
        // 0x46ee00 can map a declared local and an identity local onto the same
        // global. Replaying the writes keeps the value the script stored.
        const byzantine::SceneVariableLayout aliased{"c", {50, 3}};
        byzantine::SceneState alias_state;
        std::vector<std::uint16_t> alias_pool(3000, 0);
        alias_pool[50] = 7;
        byzantine::apply_variable_pool(alias_state, alias_pool, aliased);
        require(alias_state.variables.at(0) == 7 && alias_state.variables.at(50) == 7,
                "aliased local did not share one global");
        alias_state.variables[0] = 9;  // declared local 0 -> global 50
        alias_state.variable_writes.push_back(0);
        require(byzantine::export_variable_pool(alias_state, aliased)[50] == 9,
                "identity tail overwrote the aliased global the script wrote");
        bool rejected_pool = false;
        try { byzantine::apply_variable_pool(back, {1, 2, 3}, layout_a); }
        catch (const std::runtime_error&) { rejected_pool = true; }
        require(rejected_pool, "accepted a short carried pool");
        rejected_pool = false;
        try { byzantine::export_variable_pool(back, byzantine::SceneVariableLayout{"a", std::vector<std::uint16_t>(301, 0)}); }
        catch (const std::runtime_error&) { rejected_pool = true; }
        require(rejected_pool, "accepted a 301-entry scene row");
        if (argc >= 2) {
            const std::filesystem::path root(argv[1]);
            const auto original = byzantine::parse_variable_metadata(
                read_file(root / "SHOW.LCC"), read_file(root / "SHOW.LCV"),
                read_file(root / "SHOW.LVM"));
            require(original.scenes.size() == 44 && original.names.size() == 1049,
                    "original variable metadata inventory changed");
            const auto& ea = byzantine::scene_variable_layout(original, "EA");
            const auto& start = byzantine::scene_variable_layout(original, "START");
            const auto& al = byzantine::scene_variable_layout(original, "AL");
            require(ea.global_ids[5] == 8 && original.names[8] == "lastNode" &&
                    start.global_ids[8] == 8 && al.global_ids[1] == 8,
                    "lastNode alias mapping incorrect");
            require(ea.global_ids[26] == 74 && original.names[74] == "flowChk",
                    "EA flowChk mapping incorrect");
            const auto state = byzantine::new_game_scene_state(original, "EA");
            require(state.variables.at(5) == 0 && state.variables.at(26) == 0 &&
                    state.variables.size() == ea.global_ids.size(),
                    "original new-game snapshot incomplete");

            // 0x425ab4 writes the 3000-word DAT_0047e2f0 pool; 0x46ee00 maps the
            // scene's 300 local entries through the LCV overrides (EA 5 -> 8,
            // 26 -> 74) and identity beyond the declared count.
            byzantine::SaveGame save;
            save.node_values.resize(byzantine::kSaveNodeValueCount);
            for (std::size_t i = 0; i < save.node_values.size(); ++i) {
                save.node_values[i] = static_cast<std::uint16_t>(1000 + i);
            }
            byzantine::SceneState restored;
            const auto restored_count =
                byzantine::load_save_variables(restored, save, ea.global_ids);
            require(restored_count == 300 && restored.variables.size() == 300 &&
                    restored.variables.at(5) == 1008 &&
                    restored.variables.at(26) == 1074 &&
                    restored.variables.at(133) == 1133 &&
                    restored.variables.at(299) == 1299,
                    "saved variable pool projection incorrect");
            require(restored.variables.at(5) != save.node_values[5],
                    "saved variable pool was copied without the LCV mapping");
            if (argc == 4) {
                const auto script = byzantine::parse_cc(read_file(argv[2]));
                const auto backgrounds = byzantine::parse_cb(read_file(argv[3]));
                auto fresh = state;
                const auto view = byzantine::trace_scene_label(script, backgrounds, fresh, 475);
                require(view.background && view.background->entry.name == "EAEMShMs",
                        "fresh-state EA view changed");
                const auto closeup = byzantine::trace_scene_label(script, backgrounds, fresh, 595);
                require(closeup.background && closeup.background->entry.name == "EAEmShCU",
                        "fresh-state EA closeup changed");
                const auto back = byzantine::trace_scene_label(script, backgrounds, fresh, 602);
                require(!back.unresolved_variable && back.unresolved_panorama_node &&
                        back.stopped_at == 317,
                        "fresh-state EA return did not reach panorama loader");
                const auto unknown = byzantine::trace_scene_label(script, backgrounds, 602);
                require(unknown.unresolved_variable == 5,
                        "ordinary trace silently acquired new-game defaults");
                auto panorama_state = state;
                const auto panorama = byzantine::trace_scene_label(
                    script, backgrounds, panorama_state, 254);
                require(!panorama.unresolved_variable && panorama.unresolved_panorama_node &&
                        panorama.stopped_at == 2074 && panorama_state.pending_panorama_x == 747,
                        "fresh EA 254 camera setup did not reach ea02 loader");
                // With the CO record supplied the loader itself completes, keeps
                // the requested 747 pixel offset and consumes the staged value.
                auto loaded_state = state;
                loaded_state.panorama_records = {{"ea00", "EA.CCG"}, {"ea01", "EA.CCG"},
                                                 {"ea03", "EA.CCG"}, {"ea02", "EA.CCG"}};
                loaded_state.panorama_table_known = true;
                const auto loaded = byzantine::trace_scene_label(
                    script, backgrounds, loaded_state, 254);
                require(!loaded.unresolved_variable && !loaded.unresolved_panorama_node &&
                        loaded.panorama_loads.size() == 1 &&
                        loaded.panorama_loads[0].name == "ea02" &&
                        loaded.panorama_loads[0].jpeg_path == "nodes/ea/ea02.jpg" &&
                        loaded.panorama_loads[0].horizontal == 747 &&
                        !loaded_state.pending_panorama_x,
                        "EA 254 panorama load did not consume the camera position");
            }
        }
        std::cout << "variable metadata tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
