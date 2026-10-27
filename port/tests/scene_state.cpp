#include "byzantine/cc.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
byzantine::CcArgument word(unsigned type, unsigned value) {
    return {static_cast<std::uint8_t>(type),
            {static_cast<std::uint8_t>(value), static_cast<std::uint8_t>(value >> 8)}};
}
std::vector<std::uint8_t> read_file(const char* path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("missing scene fixture");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
// DAT_0047dd70 (engine/<scene>ca): `count` animation assets of one type byte.
// 0x40fd7b pre-loads types 2/3, 0x431b0f loads 0/1 on demand, and any other
// type leaves the DAT_004649e8 slot NULL so the command body is skipped.
byzantine::SceneState with_assets(std::size_t count, std::uint8_t type) {
    byzantine::SceneState state;
    state.actor_assets.assign(count, byzantine::NamedRecord{"anim", "T.CCG", type});
    state.actor_assets_known = true;
    return state;
}
}

int main(int argc, char** argv) {
    try {
        using byzantine::CcInstruction;
        // 0x68 writes DAT_00464878 (0x42dad9): 0 only when operand 1 is a
        // type-2 zero, -1 for every other shape. 0x7a is a dispatcher no-op.
        const std::vector<CcInstruction> alternate{
            {0xe1, {word(2, 10)}}, {0x68, {word(2, 0), word(2, 0)}}, {0x6f, {}},
            {0xe1, {word(2, 20)}}, {0x68, {word(2, 1), word(64, 0)}}, {0x6f, {}},
            {0xe1, {word(2, 30)}}, {0x68, {word(2, 1), word(2, 1)}}, {0x6f, {}}
        };
        {
            byzantine::SceneState mode_state;
            require(mode_state.alternate_mode == -1, "alternate mode not initialised to -1");
            const auto zero = byzantine::trace_scene_label(alternate, {}, mode_state, 10);
            require(!zero.unsupported_opcode && mode_state.alternate_mode == 0 &&
                    zero.alternate_mode_changes == 1, "0x68 type-2 zero did not select mode 0");
            const auto other_type = byzantine::trace_scene_label(alternate, {}, mode_state, 20);
            require(!other_type.unsupported_opcode && mode_state.alternate_mode == -1 &&
                    other_type.alternate_mode_changes == 1,
                    "0x68 non-type-2 operand did not restore -1");
            const auto nonzero = byzantine::trace_scene_label(alternate, {}, mode_state, 30);
            require(!nonzero.unsupported_opcode && mode_state.alternate_mode == -1 &&
                    nonzero.alternate_mode_changes == 0,
                    "0x68 nonzero operand changed the mode");
        }
        // A scene entry runs from instruction 0 and the 0x72/0x7a declaration
        // pairs must not stop it, which is what 0x42c8a0 does after a 0x73.
        {
            const std::vector<CcInstruction> entry_script{
                {0x7a, {word(2, 7), {0, {}}}},
                {0x72, {word(2, 0), word(64, 10), word(64, 20), word(64, 30),
                        word(64, 40), word(2, 50), word(2, 60), {0, {}}}},
                {0x7a, {word(2, 0), word(2, 3)}},
                {0x67, {word(64, 5), word(2, 3)}},
                {0xe1, {word(2, 10)}},
                {0x6f, {}}
            };
            byzantine::SceneState entry_state;
            const auto entry = byzantine::trace_scene_entry(entry_script, {}, entry_state);
            require(!entry.unsupported_opcode && entry.steps == 6 &&
                    entry_state.variables.at(3) == 5,
                    "scene entry did not start at instruction 0 or 0x7a stopped it");
        }
        // 0x72 also allocates DAT_00464300[slot] with the same rect (y is
        // operand 2 + 0x14), kind = operand 6, label = operand 5 and the
        // resolved name id; 0x6d/0x6e then write +0x08. VB's map entry uses
        // 0x72 for slot 4 and failed on missing_object before this.
        {
            const std::vector<CcInstruction> object_script{
                {0x72, {word(2, 4), word(64, 10), word(64, 20), word(64, 30),
                        word(64, 40), word(2, 50), word(2, 8), word(2, 7)}},
                {0x6d, {word(2, 4)}},
                {0x6f, {}},
                {0xe1, {word(2, 20)}},
                {0x72, {word(2, 4), word(64, 1), word(64, 1), word(64, 1),
                        word(64, 1), word(2, 1), word(2, 1), word(2, 1)}},
                {0x6e, {word(2, 4)}},
                {0x6f, {}}
            };
            byzantine::SceneState object_state;
            const auto created = byzantine::trace_scene_entry(object_script, {}, object_state);
            const auto object = object_state.objects.find(4);
            require(!created.unsupported_opcode && object != object_state.objects.end(),
                    "0x72 did not create the declared object");
            require(object->second.x == 10 && object->second.y == 40 &&
                        object->second.width == 30 && object->second.height == 40 &&
                        object->second.kind == 8 && object->second.label == 50 &&
                        object->second.name_id == 0xffffffffu &&
                        object->second.active == 1 &&
                        object->second.argument == 0xffffffffu,
                    "0x72 object fields do not match the declared hotspot");
            // The second 0x72 must not overwrite the earlier object (the
            // original only creates a NULL slot), while 0x6e still disables it.
            const auto reuse =
                byzantine::trace_scene_label(object_script, {}, object_state, 20);
            require(!reuse.unsupported_opcode &&
                        object->second.x == 10 && object->second.width == 30 &&
                        object->second.kind == 8 && object->second.label == 50 &&
                        object->second.active == 0,
                    "0x72 overwrote an existing object or 0x6e left it active");
            // Kind 0x13/0x14 store operand 7 directly; other kinds would need
            // FUN_00408bb2's string-table resolution, which is not modelled.
            const std::vector<CcInstruction> named_script{
                {0x72, {word(2, 6), word(64, 1), word(64, 2), word(64, 3),
                        word(64, 4), word(2, 9), word(2, 0x13), word(2, 4)}},
                {0x6f, {}}
            };
            byzantine::SceneState named_state;
            const auto named = byzantine::trace_scene_entry(named_script, {}, named_state);
            const auto named_object = named_state.objects.find(6);
            require(!named.unsupported_opcode && named_object != named_state.objects.end() &&
                        named_object->second.kind == 0x13 &&
                        named_object->second.name_id == 4,
                    "0x72 kind 0x13 did not keep the direct name id");
            // Every other kind passes operand 7 through 0x408bb2: the per-scene
            // CI name id resolves to the SHOW.LCI record index stored in +0x10.
            // CLOCK's object 26 is kind 0x12 with CI id 0 ("ISTAR" at record 21).
            const std::vector<CcInstruction> resolved_script{
                {0x72, {word(2, 6), word(64, 1), word(64, 2), word(64, 3),
                        word(64, 4), word(2, 9), word(2, 0x12), word(2, 0)}},
                {0x6f, {}}
            };
            byzantine::SceneState resolved_state;
            resolved_state.dialogue_tables.scene_names.resize(1);
            resolved_state.dialogue_tables.scene_names[0].name = "ISTAR";
            resolved_state.dialogue_tables.scene_names_known = true;
            resolved_state.dialogue_tables.names.resize(22);
            for (std::size_t index = 0; index < 21; ++index) {
                resolved_state.dialogue_tables.names[index].name =
                    "other" + std::to_string(index);
            }
            resolved_state.dialogue_tables.names[21].name = "ISTAR";
            const auto resolved =
                byzantine::trace_scene_entry(resolved_script, {}, resolved_state);
            const auto resolved_object = resolved_state.objects.find(6);
            require(!resolved.unsupported_opcode &&
                        resolved_object != resolved_state.objects.end() &&
                        resolved_object->second.kind == 0x12 &&
                        resolved_object->second.name_id == 21,
                    "0x72 kind 0x12 did not resolve the CI name id to a record id");
            // Without the CI table the id must stay unset, never guessed.
            byzantine::SceneState unresolved_state;
            const auto unresolved =
                byzantine::trace_scene_entry(resolved_script, {}, unresolved_state);
            const auto unresolved_object = unresolved_state.objects.find(6);
            require(!unresolved.unsupported_opcode &&
                        unresolved_object != unresolved_state.objects.end() &&
                        unresolved_object->second.name_id == 0xffffffffu,
                    "0x72 guessed a name id without the CI table");
            // 0x40c02c tests DAT_00464300[slot]+0x08, so an object active from
            // a save must be hittable even when no 0x6d ran in this trace.
            const std::vector<CcInstruction> active_script{
                {0x72, {word(2, 2), word(64, 1), word(64, 2), word(64, 3),
                        word(64, 4), word(2, 30), word(2, 8), word(2, 0)}},
                {0x6f, {}}
            };
            byzantine::SceneState active_state;
            byzantine::SceneObjectState saved_object;
            saved_object.active = 1;
            saved_object.kind = 8;
            saved_object.label = 30;
            saved_object.name_id = 0xffffffffu;
            active_state.objects[2] = saved_object;
            const auto active_trace =
                byzantine::trace_scene_entry(active_script, {}, active_state);
            const bool saw_two = std::any_of(
                active_trace.active_hotspots.begin(), active_trace.active_hotspots.end(),
                [](const byzantine::SceneHotspot& hotspot) { return hotspot.slot == 2; });
            require(saw_two,
                    "a save-active object was missing from the click/active set");
        }
        // 0x69 with transition code 4 is the examine close-up: it requests
        // inventor/examine/<item>.jpg from DAT_0045d1cc and must not require a
        // CB background entry (EXAMINE's CB can be empty).
        {
            const std::vector<CcInstruction> examine_script{
                {0x69, {word(2, 0), word(2, 4)}}, {0x6f, {}}};
            byzantine::SceneState examine_state;
            examine_state.current_use_item = 11;
            const auto requested =
                byzantine::trace_scene_entry(examine_script, {}, examine_state);
            require(requested.examine_image && *requested.examine_image == 11 &&
                        !requested.missing_examine_item && !requested.background,
                    "0x69 transition 4 did not request the selected examine image");
            byzantine::SceneState missing_state;
            const auto missing =
                byzantine::trace_scene_entry(examine_script, {}, missing_state);
            require(missing.missing_examine_item &&
                        byzantine::trace_blocker(missing) == "missing_examine_item",
                    "0x69 transition 4 without DAT_0045d1cc was accepted");
        }
        // A trace that stopped on missing data/settings is not a success. Every
        // such field must be classified, including the two the player used to
        // ignore (unresolved_dialogue_channel, unresolved_save_setting).
        {
            const auto blocked = [](const byzantine::SceneTrace& trace) {
                return !byzantine::trace_blocker(trace).empty();
            };
            byzantine::SceneTrace base;
            require(!blocked(base), "a clean trace was classified as a blocker");
            base.script_end_requested = true;
            require(!blocked(base), "0x79 script end was classified as a blocker");
            base = {};
            base.scene_switch_requested = 7;
            require(!blocked(base), "0x73 scene switch was classified as a blocker");
            base = {};
            base.unresolved_dialogue_channel = 3;
            require(blocked(base), "unresolved dialogue channel accepted as success");
            base = {};
            base.unresolved_save_setting = true;
            require(blocked(base), "unresolved save setting accepted as success");
            base = {};
            base.unresolved_dialogue_inventory = true;
            require(blocked(base), "unresolved dialogue inventory accepted as success");
            base = {};
            base.unresolved_video_setting = true;
            require(blocked(base), "unresolved video setting accepted as success");
            base = {};
            base.missing_media_record = 5;
            require(blocked(base), "missing media record accepted as success");
            base = {};
            base.disc_request = 1;
            require(blocked(base), "disc request accepted as success");
            base = {};
            base.step_limit_exceeded = true;
            require(blocked(base), "step limit accepted as success");
            base = {};
            base.skipped_node_commands = 4;   // logged and continued in the original
            base.sound_refresh_requests = 2;
            require(!blocked(base), "a continuing counter was classified as a blocker");
            byzantine::SceneTrace unsupported;
            unsupported.unsupported_opcode = 0x8f;
            require(byzantine::trace_blocker(unsupported).find("unsupported_opcode") !=
                        std::string::npos,
                    "unsupported opcode did not name itself");
        }
        // 0x40fd7b pre-loads the CA type 2/3 records at scene start; types 0/1
        // stay lazy and the file's 0xff marker (0x42d52f turns it into type 0)
        // is not preloaded. A failed asset load leaves the slot NULL.
        {
            byzantine::SceneState preload_state;
            preload_state.actor_assets = {
                {"lazy0", "T.CCG", 0}, {"lazy1", "T.CCG", 1},
                {"pre2", "T.CCG", 2}, {"pre3", "T.CCG", 3},
                {"marker", "T.CCG", 0xff}};
            preload_state.actor_assets_known = true;
            const auto preload = byzantine::preload_scene_records(preload_state);
            require(preload.created == 2 && preload.failed == 0 &&
                    preload_state.records.size() == 2 &&
                    preload_state.records.count(2) == 1 &&
                    preload_state.records.count(3) == 1 &&
                    preload_state.records.count(0) == 0 &&
                    preload_state.records.count(1) == 0 &&
                    preload_state.records.count(4) == 0,
                    "scene record preload selected the wrong CA types");
            byzantine::SceneState missing;
            missing.actor_assets = {{"gone", "T.CCG", 3}};
            missing.actor_assets_known = true;
            missing.anims_library = std::make_shared<byzantine::AnimsLibrary>(
                "output/no-such-anims-tree");
            const auto failed = byzantine::preload_scene_records(missing);
            require(failed.created == 0 && failed.failed == 1 &&
                    missing.records.empty(),
                    "a failed preload asset did not leave the slot NULL");
            byzantine::SceneState unknown;
            const auto none = byzantine::preload_scene_records(unknown);
            require(none.created == 0 && none.failed == 0 && unknown.records.empty(),
                    "preload invented a CA table");
        }
        // 0xba reads the raw operand, even for type 2. Retain it between
        // actions and clamp unsigned words exactly as the original caller.
        for (const unsigned x : {0u, 747u, 2495u, 2496u, 65535u}) {
            const std::vector<CcInstruction> camera{
                {0xe1, {word(2, 10)}}, {0xba, {word(2, x)}}, {0x6f, {}},
                {0xe1, {word(2, 20)}}, {0x6f, {}}
            };
            byzantine::SceneState state;
            require(!state.pending_panorama_x, "invented initial panorama position");
            const auto trace = byzantine::trace_scene_label(camera, {}, state, 10);
            require(!trace.unsupported_opcode && !trace.unresolved_variable &&
                    state.pending_panorama_x == (x > 2495 ? 2495 : x),
                    "panorama position operand/clamp mismatch");
            byzantine::trace_scene_label(camera, {}, state, 20);
            require(state.pending_panorama_x == (x > 2495 ? 2495 : x),
                    "pending panorama position lost between actions");
        }
        const std::vector<CcInstruction> panorama_script{
            {0xb9, {word(2, 1), word(2, 31), word(64, 0), {0, {}}}}
        };
        const auto panoramas = byzantine::static_panoramas(
            panorama_script, {{"ea00", "EA.CCG"}, {"ea01", "EA.CCG"}});
        require(panoramas.size() == 1 && panoramas[0].id == 1 &&
                panoramas[0].jpeg_path == "nodes/ea/ea01.jpg",
                "panorama node/path binding mismatch");
        bool rejected_node = false;
        try { byzantine::static_panoramas(panorama_script, {{"ea00", "EA.CCG"}}); }
        catch (const std::runtime_error&) { rejected_node = true; }
        require(rejected_node, "accepted out-of-range panorama ID");
        // Separate entry points model two clicks: an assignment performed by
        // the first must affect a branch reached through the second.
        const std::vector<CcInstruction> script{
            {0xe1, {word(2, 10)}},
            {0x67, {word(64, 42), word(2, 3)}},
            {0x6f, {}},
            {0xe1, {word(2, 20)}},
            {0x71, {word(2, 3), word(64, 42), word(2, 30)}},
            {0xff, {}},
            {0xe1, {word(2, 30)}},
            {0x69, {word(2, 0)}},
            {0x6f, {}}
        };
        byzantine::SceneState state;
        byzantine::trace_scene_label(script, {}, state, 10);
        const auto continued = byzantine::trace_scene_label(script, {{"test", ""}}, state, 20);
        require(continued.background.has_value(), "assigned value lost between entry points");
        require(!continued.unsupported_opcode, "conditional branch took the wrong path");
        const auto fresh = byzantine::trace_scene_label(script, {{"test", ""}}, 20);
        require(fresh.unresolved_variable == 3, "fresh state invented a variable value");

        // Step budget: a caller can pause a label after N instructions and replay
        // it from the same entry state with a larger budget (the scene player
        // uses this to wait for a sound without a second VM).
        {
            const std::vector<CcInstruction> budget_script{
                {0xe1, {word(2, 10)}},
                {0x67, {word(64, 11), word(2, 1)}},
                {0x67, {word(64, 22), word(2, 2)}},
                {0x69, {word(2, 0)}},
                {0x6f, {}}
            };
            byzantine::SceneState entry;
            const auto paused = byzantine::trace_scene_label(
                budget_script, {{"test", ""}}, entry, 10, 1);
            require(paused.steps == 1 && paused.paused_by_budget,
                    "step budget did not pause");
            require(entry.variables.at(1) == 11 && entry.variables.count(2) == 0,
                    "paused state is not exactly after two instructions");
            require(!paused.background.has_value(), "paused trace ran past its budget");
            byzantine::SceneState resumed;
            const auto finished = byzantine::trace_scene_label(
                budget_script, {{"test", ""}}, resumed, 10, 100);
            require(!finished.paused_by_budget && finished.background.has_value() &&
                        resumed.variables.at(2) == 22,
                    "budgeted replay did not reach the same end state");
            byzantine::SceneState whole;
            byzantine::trace_scene_label(budget_script, {{"test", ""}}, whole, 10);
            require(whole.variables == resumed.variables &&
                        whole.records.size() == resumed.records.size(),
                    "budgeted replay differs from an unbudgeted run");
        }

        const std::vector<CcInstruction> arithmetic{
            {0xe1, {word(2, 1)}},
            {0x74, {word(64, 2), word(2, 3)}},
            {0x75, {word(2, 4), word(2, 3)}},
            {0x6f, {}}
        };
        byzantine::SceneState arithmetic_state;
        arithmetic_state.variables = {{3, 65535}, {4, 1}};
        byzantine::trace_scene_label(arithmetic, {}, arithmetic_state, 1);
        require(arithmetic_state.variables.at(3) == 0,
                "16-bit wraparound or variable-sourced subtraction failed");
        byzantine::SceneState unknown_state;
        const auto unknown = byzantine::trace_scene_label(arithmetic, {}, unknown_state, 1);
        require(unknown.unresolved_variable == 3 && unknown_state.variables.empty(),
                "arithmetic invented an unset destination");
        arithmetic_state.variables = {{3, 0}, {4, 3}};
        // 0x75's underflow path is 0x40ac43/0x40ac38: log and skip the write,
        // then continue. It must not abort the trace.
        const auto underflow_trace =
            byzantine::trace_scene_label(arithmetic, {}, arithmetic_state, 1);
        require(underflow_trace.subtraction_underflows == 1 &&
                    !underflow_trace.unsupported_opcode &&
                    arithmetic_state.variables.at(3) == 2,
                "subtraction underflow changed the destination or stopped the trace");

        // 0x7d RANDOM: 0x431e89 calls the CRT _rand stream (0x44d070) only for
        // a nonzero range and stores rand() % range. The seed is real game
        // state (0x415a4e uses 0xbeef, 0x42a6aa uses local time), so an unset
        // stream must stop the trace instead of inventing values.
        const std::vector<CcInstruction> random_script{
            {0xe1, {word(2, 10)}},
            {0x7d, {word(2, 5), word(64, 100)}},
            {0x6f, {}}
        };
        byzantine::SceneState unseeded;
        const auto blocked_random = byzantine::trace_scene_label(random_script, {}, unseeded, 10);
        require(blocked_random.unresolved_random && !blocked_random.unsupported_opcode &&
                unseeded.variables.find(5) == unseeded.variables.end(),
                "RANDOM executed without a known seed");

        byzantine::SceneState random_state;
        random_state.random_state = 0xbeef;
        const auto first_random = byzantine::trace_scene_label(random_script, {}, random_state, 10);
        require(!first_random.unresolved_random && !first_random.unsupported_opcode &&
                random_state.variables.at(5) == 84, "RANDOM first roll mismatch");
        // Independent LCG check: seed 0xbeef then 0x6fa8ddf6, 0x26035ce1,
        // 0x896b4c20, 0xc87d3a63 -> 84, 31, 11, 57 for range 100.
        for (int i = 0; i < 3; ++i) {
            byzantine::trace_scene_label(random_script, {}, random_state, 10);
        }
        require(random_state.variables.at(5) == 57,
                "RANDOM LCG sequence mismatch");

        // Range from another scene variable (type 2): rand()%7 == 3 for the
        // first seeded draw. A later zero range stores 0 and must NOT advance
        // the stream (0x431e89 skips _rand() for a zero range).
        const std::vector<CcInstruction> variable_range{
            {0xe1, {word(2, 10)}},
            {0x67, {word(64, 7), word(2, 7)}},
            {0x7d, {word(2, 5), word(2, 7)}},
            {0x7d, {word(2, 5), word(64, 0)}},
            {0x6f, {}}
        };
        byzantine::SceneState ranged_state;
        ranged_state.random_state = 0xbeef;
        byzantine::trace_scene_label(variable_range, {}, ranged_state, 10);
        require(ranged_state.variables.at(5) == 0 &&
                ranged_state.random_state == 0x6fa8ddf6u,
                "zero range advanced the stream or wrote the wrong value");

        byzantine::SceneState variable_only;
        variable_only.random_state = 0xbeef;
        const std::vector<CcInstruction> no_zero_range{
            {0xe1, {word(2, 10)}},
            {0x67, {word(64, 7), word(2, 7)}},
            {0x7d, {word(2, 5), word(2, 7)}},
            {0x6f, {}}
        };
        byzantine::trace_scene_label(no_zero_range, {}, variable_only, 10);
        require(variable_only.variables.at(5) == 3, "RANDOM variable range mismatch");

        byzantine::SceneState missing_range;
        missing_range.random_state = 0xbeef;
        const std::vector<CcInstruction> missing_script{
            {0xe1, {word(2, 10)}},
            {0x7d, {word(2, 5), word(2, 99)}},
            {0x6f, {}}
        };
        const auto missing_trace = byzantine::trace_scene_label(missing_script, {}, missing_range, 10);
        require(missing_trace.unresolved_variable == 99 && !missing_trace.unresolved_random &&
                missing_range.random_state == 0xbeef && missing_range.variables.empty(),
                "RANDOM read an unknown range variable");

        // A range operand that is neither type 2 nor type 0x40 reads as 0xffff.
        byzantine::SceneState odd_range;
        odd_range.random_state = 0xbeef;
        const std::vector<CcInstruction> odd_script{
            {0xe1, {word(2, 10)}},
            {0x7d, {word(2, 5), {1, {9}}}},
            {0x6f, {}}
        };
        byzantine::trace_scene_label(odd_script, {}, odd_range, 10);
        require(odd_range.variables.at(5) == 28584,
                "RANDOM non-constant range must use 0xffff");

        // Actor/animation records (DAT_004649e8). Defaults come from 0x40ffee:
        // y=0x14, +0x04=10, flags=0x20. 0x64/0x85 use the 0x460418/0x460428
        // byte tables with the RAW type-2 operand; 0x84 appends to the +0x3c
        // trail list unless bit 0x20 is set; 0x91 records frame-list requests.
        const auto absent = byzantine::CcArgument{0, {}};
        const std::vector<CcInstruction> actor_script{
            {0xe1, {word(2, 10)}},
            {0x64, {word(2, 1), word(2, 5), word(64, 7), word(64, 100), word(64, 50)}},
            {0x85, {word(2, 1), word(2, 1), word(2, 3), word(2, 4), absent, absent,
                    absent, absent}},
            {0x95, {word(2, 1), word(64, 9)}},
            {0x84, {word(2, 1), word(64, 33), word(64, 44)}},
            {0x6f, {}}
        };
        auto actor_state = with_assets(4, 3);
        const auto actor_trace = byzantine::trace_scene_label(actor_script, {}, actor_state, 10);
        require(!actor_trace.unsupported_opcode && !actor_trace.unresolved_variable &&
                !actor_trace.unresolved_random, "actor commands stopped the trace");
        const auto& actor = actor_state.records.at(1);
        // 0x64 writes its 0x460428 byte to +0x09 (objdump 0x43251d), so the
        // 0x85 masks continue from 0x04, not from the 0x20 default. Clearing
        // bit 0x20 also re-enables 0x84's +0x3c trail append, so the pre-0x84
        // position (100, 70) is recorded.
        require(actor.x == 33 && actor.y == 64 && actor.field04 == 7 &&
                actor.kind == 9 && actor.flags == 0x03 &&
                actor.frame_restart_requests == 1 && actor.trail.size() == 1 &&
                actor.trail[0] == std::pair<std::int16_t, std::int16_t>{100, 70} &&
                actor_trace.records_created == 1 &&
                actor_trace.skipped_node_commands == 0,
                "actor 0x64/0x85/0x95/0x84 state mismatch");

        // 0x64 guard: kind index 11 yields 0x20, and flags already holds 0x20,
        // so the whole update is skipped.
        const std::vector<CcInstruction> guard_script{
            {0xe1, {word(2, 10)}},
            {0x64, {word(2, 1), word(2, 11), word(64, 7), word(64, 100), word(64, 50)}},
            {0x6f, {}}
        };
        auto guard_state = with_assets(4, 3);
        byzantine::trace_scene_label(guard_script, {}, guard_state, 10);
        const auto& guarded = guard_state.records.at(1);
        require(guarded.kind == 0 && guarded.x == 0 && guarded.y == 0x14 &&
                guarded.field04 == 10 && guarded.frame_restart_requests == 0,
                "0x64 kind/flag guard not applied");

        // Clearing bit 0x20 re-enables trail recording; repeated coordinates
        // must not append twice.
        const std::vector<CcInstruction> trail_script{
            {0xe1, {word(2, 10)}},
            {0x85, {word(2, 1), word(2, 10), absent, absent, absent, absent, absent, absent}},
            {0x84, {word(2, 1), word(64, 5), word(64, 6)}},
            {0x84, {word(2, 1), word(64, 5), word(64, 6)}},
            {0x84, {word(2, 1), word(64, 5), word(64, 6)}},
            {0x6f, {}}
        };
        auto trail_state = with_assets(4, 3);
        byzantine::trace_scene_label(trail_script, {}, trail_state, 10);
        const auto& trailed = trail_state.records.at(1);
        require(trailed.trail.size() == 2 &&
                trailed.trail[0] == std::pair<std::int16_t, std::int16_t>{0, 0x14} &&
                trailed.trail[1] == std::pair<std::int16_t, std::int16_t>{5, 26},
                "0x84 trail append/deduplicate mismatch");

        // 0x91 selects/animation-advances frames; 0xffff restarts the list.
        const std::vector<CcInstruction> frame_script{
            {0xe1, {word(2, 10)}},
            {0x91, {word(2, 1), word(64, 3)}},
            {0x91, {word(2, 1), word(64, 0xffff)}},
            {0x6f, {}}
        };
        auto frame_state = with_assets(4, 3);
        byzantine::trace_scene_label(frame_script, {}, frame_state, 10);
        require(frame_state.records.at(1).frame_advance_requests == 1 &&
                frame_state.records.at(1).frame_restart_requests == 1,
                "0x91 frame-list requests mismatch");

        // A mask index outside the 0x00..0x0f tables is rejected instead of
        // reading past the recovered bytes.
        bool rejected_mask = false;
        try {
            byzantine::trace_scene_label(
                {{0xe1, {word(2, 10)}},
                 {0x85, {word(2, 1), word(2, 16), absent, absent, absent, absent, absent,
                         absent}},
                 {0x6f, {}}},
                {}, actor_state, 10);
        } catch (const std::runtime_error&) {
            rejected_mask = true;
        }
        require(rejected_mask, "accepted an actor mask index outside the tables");

        // 0x65 defines DAT_00486220[id]; 0xc4 activates type-2 sound operands
        // and flags the record named "endgame" (0x42db22 case 0xc4); 0xbc
        // clears the active flag again.
        const std::vector<CcInstruction> sound_script{
            {0xe1, {word(2, 10)}},
            {0x65, {word(2, 3), word(64, 0), word(64, 7), word(64, 20), word(64, 30),
                    word(64, 40), word(2, 9)}},
            {0xc4, {word(2, 3), absent, absent, absent, absent, absent, absent, absent}},
            {0xbc, {word(2, 3), absent, absent, absent, absent, absent, absent, absent}},
            {0x6f, {}}
        };
        byzantine::SceneState sound_state;
        sound_state.sound_records = {{"x", "EA.CCG", 0}, {"y", "EA.CCG", 1},
                                     {"z", "EA.CCG", 1}, {"endgame", "VH.CCG", 0}};
        sound_state.sound_table_known = true;
        const auto sound_trace = byzantine::trace_scene_label(sound_script, {}, sound_state, 10);
        require(!sound_trace.unsupported_opcode && !sound_trace.unresolved_variable &&
                !sound_trace.unresolved_sound_name, "sound commands stopped the trace");
        require(sound_state.sounds.at(3).defined &&
                sound_state.sounds.at(3).field14 == 1 &&
                sound_state.sounds.at(3).field10 == 7 &&
                sound_state.sounds.at(3).field0c == 20 &&
                sound_state.sounds.at(3).field08 == 30 &&
                sound_state.sounds.at(3).field18 == 40 &&
                sound_state.sounds.at(3).enabled == 9 &&
                !sound_state.sounds.at(3).active && sound_state.endgame_requested,
                "0x65/0xc4 sound state mismatch");
        // 0xc4 queues an ambient start (0x427de1, non-blocking) with the +0x14
        // loop flag; 0xbc queues the matching stop. Use a non-endgame record so
        // the start is not diverted to the endgame flag.
        byzantine::SceneState ambient_state;
        ambient_state.sound_records = {{"ambient", "AM.CCG", 0}};
        ambient_state.sound_table_known = true;
        byzantine::SceneSoundState ambient_sound;
        ambient_sound.defined = true;
        ambient_sound.field14 = 1;
        ambient_sound.field18 = 1;
        ambient_state.sounds[3] = ambient_sound;
        const auto ambient_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xc4, {word(2, 3), absent, absent, absent, absent, absent, absent, absent}},
             {0xbc, {word(2, 3), absent, absent, absent, absent, absent, absent, absent}},
             {0x6f, {}}},
            {}, ambient_state, 10);
        require(ambient_trace.sound_starts.size() == 1 &&
                    ambient_trace.sound_starts[0].opcode == 0xc4 &&
                    ambient_trace.sound_starts[0].id == 3 &&
                    ambient_trace.sound_starts[0].loop &&
                    ambient_trace.sound_starts[0].record_present &&
                    ambient_trace.sound_stops.size() == 1 &&
                    ambient_trace.sound_stops[0].id == 3 &&
                    !ambient_state.sounds.at(3).active,
                "0xc4 ambient start / 0xbc stop trace mismatch");

        // Without the per-scene CS table the "endgame" comparison is not
        // evaluable and must stop instead of being skipped.
        byzantine::SceneState unknown_sound;
        const std::vector<CcInstruction> activate_only{
            {0xe1, {word(2, 10)}},
            {0xc4, {word(2, 5), absent, absent, absent, absent, absent, absent, absent}},
            {0x6f, {}}
        };
        const auto unknown_trace = byzantine::trace_scene_label(activate_only, {}, unknown_sound, 10);
        require(unknown_trace.unresolved_sound_name == 5 &&
                !unknown_sound.endgame_requested, "0xc4 ran without the sound table");

        // 0x6c clears DAT_00464f60[id]+0x10; a missing object records the
        // original 0x40abb4 error path instead of writing.
        byzantine::SceneState media_state;
        media_state.media[2].active = 1;
        const auto media_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0x6c, {word(2, 2)}},
             {0x6c, {word(2, 9)}},
             {0x6f, {}}},
            {}, media_state, 10);
        require(media_state.media.at(2).active == 0 &&
                media_trace.missing_media_requests == 1 &&
                media_state.media.count(9) == 0,
                "0x6c media object handling mismatch");

        // 0x6a defines DAT_00464f60[id] from resolved operand 1, raw operand 2
        // and operand 3's low byte as the type.
        byzantine::SceneState media_define_state;
        const auto media_define_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0x6a, {word(2, 1), word(64, 5), word(2, 77), word(2, 0x0109)}},
             {0x6a, {word(2, 2), word(64, 6), word(2, 88), {0, {}}}},
             {0x6f, {}}},
            {}, media_define_state, 10);
        require(!media_define_trace.unsupported_opcode &&
                media_define_state.media.at(1).value == 5 &&
                media_define_state.media.at(1).media_id == 77 &&
                media_define_state.media.at(1).type == 9 &&
                media_define_state.media.at(1).active == 1 &&
                media_define_state.media.at(1).frame == 0 &&
                media_define_state.media.at(1).timestamp == 0 &&
                media_define_state.media.at(2).type == 0 &&
                media_define_trace.media_definitions.size() == 2 &&
                media_define_trace.media_timestamps_unavailable == 2,
            "0x6a media definition mismatch");

        // 0xd1 retains the 0x428cbd start request and the +0x18/gamepref[2] gate
        // without opening an audio device.
        byzantine::SceneState sound_start_state;
        byzantine::SceneSoundState start_sound;
        start_sound.defined = true;
        start_sound.field18 = 1;
        sound_start_state.sounds[2] = start_sound;
        sound_start_state.gameprefs = std::array<std::uint32_t, 7>{};
        const auto start_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xd1, {word(2, 2), word(2, 0), word(2, 7)}},
             {0xd1, {word(2, 9), word(2, 0), {0, {}}}},
             {0x6f, {}}},
            {}, sound_start_state, 10);
        require(start_trace.sound_starts.size() == 2 &&
                start_trace.sound_starts[0].record_present &&
                start_trace.sound_starts[0].allowed == true &&
                start_trace.sound_starts[0].text == 7 &&
                start_trace.sound_starts[0].mode == 0 &&
                !start_trace.sound_starts[1].record_present &&
                !start_trace.sound_starts[1].allowed,
            "0xd1 sound start mismatch");
        byzantine::SceneState start_gated_state;
        byzantine::SceneSoundState gated_sound;
        gated_sound.defined = true;
        gated_sound.field18 = 0;
        start_gated_state.sounds[2] = gated_sound;
        start_gated_state.gameprefs = std::array<std::uint32_t, 7>{};
        const auto gated_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xd1, {word(2, 2), word(2, 1), {0, {}}}},
             {0x6f, {}}},
            {}, start_gated_state, 10);
        require(gated_trace.sound_starts.size() == 1 &&
                gated_trace.sound_starts[0].allowed == false,
            "0xd1 gamepref gate mismatch");

        // 0xb5 shows the window, 0xbb selects a channel through 0x408c81 and
        // resets the scroll, 0xb4 hides it again. All three request a redraw.
        byzantine::SceneState visibility_state;
        visibility_state.dialogue_tables.channels = {
            {"a", "EA.CCG", 0}, {"b", "EA.CCG", 0}, {"c", "EA.CCG", 0}};
        visibility_state.dialogue_tables.scene_channels = {
            {"a", "EA.CCG", 0}, {"b", "EA.CCG", 0}, {"c", "EA.CCG", 0}};
        visibility_state.dialogue_tables.scene_channels_known = true;
        visibility_state.dialogue_scroll = 5;
        const auto visibility_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xb5, {}}, {0xbb, {word(2, 1)}}, {0xb4, {}},
             {0x6f, {}}},
            {}, visibility_state, 10);
        require(!visibility_state.dialogue_visible &&
                visibility_state.current_dialogue_channel == 1 &&
                visibility_state.dialogue_scroll == 0 &&
                visibility_trace.dialogue_refresh_requests == 3,
            "0xb4/0xb5/0xbb dialogue window mismatch");

        // 0x73 reports the DAT_00482460 target and stops; 0xffff is a valid
        // "no entry" and an absent table is a named outcome.
        byzantine::SceneState switch_state;
        switch_state.scene_switch_targets = {0, 25, 0xffff};
        switch_state.scene_switch_table_known = true;
        const auto switch_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x73, {word(2, 1)}}, {0x6f, {}}},
            {}, switch_state, 10);
        require(switch_trace.scene_switch_requested == 25 &&
                switch_trace.stopped_at == 1,
            "0x73 did not stop on the scene switch");
        byzantine::SceneState no_switch;
        no_switch.scene_switch_targets = {0, 0xffff};
        no_switch.scene_switch_table_known = true;
        const auto no_switch_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x73, {word(2, 1)}}, {0x6f, {}}},
            {}, no_switch, 10);
        require(!no_switch_trace.scene_switch_requested &&
                !no_switch_trace.unsupported_opcode,
            "0x73 treated a 0xffff entry as a switch");
        byzantine::SceneState switch_no_table;
        const auto no_table_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x73, {word(2, 1)}}, {0x6f, {}}},
            {}, switch_no_table, 10);
        require(no_table_trace.unresolved_scene_table == 1,
            "0x73 accepted a missing scene switch table");

        // 0x80 rewrites an existing DAT_00464f60 record's +0x0c; missing keeps
        // the 0x40abb4 path.
        byzantine::SceneState media_id_state;
        media_id_state.media[2].media_id = 1;
        const auto media_id_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x80, {word(2, 2), word(2, 77)}},
             {0x80, {word(2, 9), word(2, 1)}}, {0x6f, {}}},
            {}, media_id_state, 10);
        require(media_id_state.media.at(2).media_id == 77 &&
                media_id_trace.media_id_writes.size() == 1 &&
                media_id_trace.missing_media_requests == 1 &&
                media_id_state.media.count(9) == 0,
            "0x80 media id write mismatch");

        // 0xd4/0xd5 write DAT_004859cd.
        byzantine::SceneState flag_state;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xd4, {}}, {0xd5, {}}, {0x6f, {}}},
            {}, flag_state, 10);
        require(!flag_state.flag_4859cd, "0xd4/0xd5 flag mismatch");
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xd4, {}}, {0x6f, {}}}, {}, flag_state, 10);
        require(flag_state.flag_4859cd, "0xd4 did not set DAT_004859cd");

        // 0x8b writes node +0x2c; 0x8a appends (raw(2), resolved(1)); a node
        // that the save did not provide needs the animation asset.
        byzantine::SceneState node_cmd_state;
        node_cmd_state.records[3] = byzantine::SceneRecordState{};
        node_cmd_state.variables[5] = 42;
        const auto node_cmd_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0x8b, {word(2, 3), word(2, 0x1234)}},
             {0x8a, {word(2, 3), word(2, 5), word(2, 7)}},
             {0x8a, {word(2, 9), word(2, 5), word(2, 7)}},
             {0x6f, {}}},
            {}, node_cmd_state, 10);
        require(node_cmd_state.records.at(3).field_2c == 0x1234 &&
                node_cmd_state.records.at(3).extra.size() == 1 &&
                node_cmd_state.records.at(3).extra[0].first == 7 &&
                node_cmd_state.records.at(3).extra[0].second == 42 &&
                node_cmd_trace.unresolved_node_asset == 9,
            "0x8a/0x8b node command mismatch");

        // 0x86 writes node +0x04 from the raw operand and applies the
        // 0x460418/0x460428 pair to +0x09 for type-2 operands 2..7; a node the
        // save did not provide needs the animation asset.
        byzantine::SceneState node_value_state;
        node_value_state.records[4] = byzantine::SceneRecordState{};
        const auto node_value_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0x86, {word(2, 4), word(64, 5), word(2, 1), absent, absent, absent,
                     absent, absent}},
             {0x86, {word(2, 9), word(64, 1), absent, absent, absent, absent, absent,
                     absent}},
             {0x6f, {}}},
            {}, node_value_state, 10);
        require(node_value_state.records.at(4).field04 == 5 &&
                node_value_state.records.at(4).flags == 0x21 &&
                node_value_trace.node_writes.size() == 1 &&
                node_value_trace.node_writes[0].id == 4 &&
                node_value_trace.node_writes[0].value04 == 5 &&
                node_value_trace.node_writes[0].flags == 0x21 &&
                node_value_trace.unresolved_node_asset == 9 &&
                node_value_trace.stopped_at == 2,
                "0x86 node value/flag write mismatch");

        // 0x431b0f decides existence from the CA asset table, not from the
        // script: 0x40fd7b pre-loads types 2/3 and 0x431b0f loads 0/1, so a
        // 0xff entry leaves the record NULL, the original logs through
        // 0x40ac43/0x40ac38 and the dispatcher skips the body (the script
        // continues). A save-restored record exists regardless of the type.
        auto skip_state = with_assets(4, 0xff);
        const auto skip_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0x64, {word(2, 1), word(2, 5), word(64, 7), word(64, 100), word(64, 50)}},
             {0x85, {word(2, 1), word(2, 1), absent, absent, absent, absent, absent,
                     absent}},
             {0x6f, {}}},
            {}, skip_state, 10);
        require(skip_trace.skipped_node_commands == 2 &&
                skip_state.records.empty() && !skip_trace.unresolved_node_asset &&
                skip_trace.steps == 3 && skip_trace.stopped_at == 3,
                "0x431b0f did not skip a 0xff asset record");

        // A slot beyond the CA table is a NULL dereference in 0x431b0f: stop.
        auto short_state = with_assets(3, 2);
        const auto short_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x64, {word(2, 9), word(2, 5)}}, {0x6f, {}}},
            {}, short_state, 10);
        require(short_trace.missing_node_asset == 9 && short_state.records.empty(),
                "0x431b0f accepted an id outside the CA table");

        // The save wins: a record that block 17 restored is present even when
        // the CA entry type would never create one.
        auto restored_state = with_assets(4, 0xff);
        restored_state.records[2] = byzantine::SceneRecordState{};
        const auto restored_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0x85, {word(2, 2), word(2, 1), absent, absent, absent, absent, absent,
                     absent}},
             {0x6f, {}}},
            {}, restored_state, 10);
        require(restored_state.records.at(2).flags == 0x21 &&
                restored_trace.skipped_node_commands == 0,
                "a save-restored record was skipped because of its CA type");

        // The frame list: 0x40ffee stores the frame count at +0x14 and 0x410078
        // adds the first frame's offsets to x/y. 0x91 -> 0x40cf73 clears +0x1c
        // for 0xffff, otherwise advances the head `count` times and logs through
        // 0x40ac43/0x40ac38 when it runs off the end (the node stays put).
        auto frame_list_state = with_assets(4, 2);
        frame_list_state.records[1] = byzantine::SceneRecordState{};
        frame_list_state.records.at(1).frame_count = 5;
        frame_list_state.records.at(1).frames_known = true;
        frame_list_state.records.at(1).current_frame = 0;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x91, {word(2, 1), word(64, 3)}},
             {0x6f, {}}},
            {}, frame_list_state, 10);
        require(frame_list_state.records.at(1).current_frame == 3,
                "0x91 did not select the requested frame");
        const auto overflow = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x91, {word(2, 1), word(64, 9)}},
             {0x6f, {}}},
            {}, frame_list_state, 10);
        require(overflow.frame_overflow_logs == 1 &&
                frame_list_state.records.at(1).current_frame == 4,
                "0x91 did not clamp/log past the last frame");
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x91, {word(2, 1), word(64, 0xffff)}},
             {0x6f, {}}},
            {}, frame_list_state, 10);
        require(!frame_list_state.records.at(1).current_frame,
                "0x91 0xffff did not clear the current frame");
        // 0x64 restarts the list: +0x1c = +0x18.
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0x64, {word(2, 1), word(2, 5), word(64, 7), word(64, 100), word(64, 50)}},
             {0x6f, {}}},
            {}, frame_list_state, 10);
        require(frame_list_state.records.at(1).current_frame == 0,
                "0x64 did not restart the frame list");

        // Creation from the animation asset: the CA name resolves
        // "anims/<name[0]><name[1]>/<name>.rle" through AnimsLibrary, the frame
        // count and first-frame offsets are applied, and a missing file is a
        // failed 0x40fe70 (the command body is skipped, not invented).
        const auto root = std::filesystem::temp_directory_path() / "byz-scene-anims";
        std::filesystem::remove_all(root);
        std::filesystem::create_directories(root / "anims" / "ab");
        {
            const auto path = root / "anims" / "ab" / "ABANIM.RLE";
            std::ofstream out(path, std::ios::binary);
            // 4x2 frame at offset (100,10) whose payload is a valid run stream:
            // row 0 = one literal at x 0, row 1 = a 4-pixel gap, then the
            // closing marker (row == height). 6 words.
            const std::array<std::uint8_t, 26 + 12> frame_header{
                0, 0,                          // +0x00
                0x04, 0x00,                    // +0x02 width 4
                0x02, 0x00,                    // +0x04 height 2
                0x64, 0x00,                    // +0x06 x offset 100
                0x0a, 0x00,                    // +0x08 y offset 10
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // +0x0a..+0x15
                0x06, 0, 0, 0,                 // +0x16 words = 6
                0x00, 0x40, 0x01, 0x00, 0x11, 0x11,  // row 0: marker + 1 literal
                0x00, 0x40, 0x04, 0xc0, 0x00, 0x40}; // row 1: marker + gap, close
            const std::array<std::uint8_t, 10> file_header{
                7, 0, 0, 0, 0, 0, 0, 0, 1, 0};  // sig 7, frame count 1
            out.write(reinterpret_cast<const char*>(file_header.data()),
                      static_cast<std::streamsize>(file_header.size()));
            out.write(reinterpret_cast<const char*>(frame_header.data()),
                      static_cast<std::streamsize>(frame_header.size()));
        }
        auto anim_state = with_assets(4, 2);
        anim_state.actor_assets[1] = byzantine::NamedRecord{"abanim", "T.CCG", 2};
        anim_state.anims_library =
            std::make_shared<byzantine::AnimsLibrary>(root.string());
        const auto anim_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0x85, {word(2, 1), word(2, 1), absent, absent, absent, absent, absent,
                     absent}},
             {0x6f, {}}},
            {}, anim_state, 10);
        require(anim_trace.records_created == 1 &&
                anim_state.records.at(1).frames_known &&
                anim_state.records.at(1).frame_count == 1 &&
                anim_state.records.at(1).current_frame == 0 &&
                anim_state.records.at(1).x == 100 &&
                anim_state.records.at(1).y == 0x14 + 10 &&
                anim_state.records.at(1).anims_signature == 7 &&
                anim_state.records.at(1).flags == 0x21,
                "record creation from the animation asset mismatch");
        // Scene composition: 0x43b216 groups records by the +0x08 kind and walks
        // each group from the highest id down, drawing the current frame at the
        // record's x/y. The destination is the 640x480 scene surface, so the
        // 640x400 playfield starts at kPlayfieldTop and a record's stored y
        // (which carries the same +0x14 bias) is reduced by it. A kind outside
        // 0..4 is never drawn by that loop.
        auto compose_state = anim_state;  // keeps the anims library and assets
        // The creation test's 0x64 left the snap flag set (0x21); a room label
        // clears it before the record is drawn.
        compose_state.records.at(1).flags = 0x01;
        // Both drawn records resolve the same synthetic asset by CA name.
        compose_state.actor_assets[2] = byzantine::NamedRecord{"abanim", "T.CCG", 2};
        compose_state.records[2] = byzantine::SceneRecordState{};
        compose_state.records.at(2).kind = 0;
        compose_state.records.at(2).flags = 0x04;  // normal pass draws this
        compose_state.records.at(2).current_frame = 0;
        compose_state.records.at(2).x = 1;
        compose_state.records.at(2).y = 1 + byzantine::kPlayfieldTop;
        compose_state.records[3] = byzantine::SceneRecordState{};
        compose_state.records.at(3).kind = 7;  // outside 0x43b216's 0..4 loop
        compose_state.records.at(3).flags = 0x04;
        compose_state.records.at(3).current_frame = 0;
        // 0x20 is the snap-pass flag (0x40ffee's default) and 0x40 hides the
        // record in the normal pass too; neither may be drawn here.
        compose_state.records[4] = byzantine::SceneRecordState{};  // flags 0x20
        compose_state.records.at(4).kind = 0;
        compose_state.records.at(4).current_frame = 0;
        compose_state.records[5] = byzantine::SceneRecordState{};
        compose_state.records.at(5).kind = 0;
        compose_state.records.at(5).flags = 0x40;
        compose_state.records.at(5).current_frame = 0;
        auto target = byzantine::make_surface(8, 8, 0);
        const auto composed = byzantine::compose_scene(compose_state, target);
        // Record 1 sits at (100,30) from the creation test, i.e. fully off an
        // 8x8 surface: it is attempted (descending id) but writes nothing.
        require(composed.drawn == 1 && composed.skipped_other_kind == 1 &&
                    composed.skipped_snap == 1 && composed.skipped_hidden == 1 &&
                    composed.records.size() == 2 &&
                    composed.records[0].id == 2 && composed.records[1].id == 1 &&
                    composed.records[0].blit.drawn &&
                    !composed.records[1].blit.drawn &&
                    composed.records[1].blit.written == 0 &&
                    composed.pixels_written == composed.records[0].blit.written,
                "compose_scene ordering/counters mismatch");
        require(composed.records[0].blit.y == 1, "compose_scene playfield origin");
        require(target.at(1, 1) == 0x2211, "composed pixel missing");  // 0x1111 after 0x4164a0
        // 0x43b216 phase 1: a flag-0x20 record with a +0x20 frame is drawn
        // once at +0x24/+0x26 and the +0x20 pointer is cleared. DAT_004859cd
        // (set after a save load) disables the whole pass.
        {
            auto snap_state = compose_state;
            snap_state.actor_assets.resize(7);
            snap_state.actor_assets[6] = byzantine::NamedRecord{"abanim", "T.CCG", 1};
            byzantine::SceneRecordState snap;
            snap.kind = 0;
            snap.flags = 0x20;
            snap.frame_index_20 = 0;
            snap.snap_x = 3;
            snap.snap_y = byzantine::kPlayfieldTop + 4;
            snap_state.records[6] = snap;
            snap_state.flag_4859cd = false;
            auto snap_target = byzantine::make_surface(8, 8, 0);
            const auto snapped = byzantine::compose_scene(snap_state, snap_target);
            require(snapped.snap_drawn == 1 && snapped.drawn == 2 &&
                        snap_target.at(3, 4) == 0x2211 &&
                        snap_state.records.at(6).frame_index_20 == 0xffff,
                    "0x43b216 snap pass did not draw/clear the +0x20 frame");
            byzantine::SceneRecordState snap_again;
            snap_again.kind = 0;
            snap_again.flags = 0x20;
            snap_again.frame_index_20 = 0;
            snap_again.snap_x = 3;
            snap_again.snap_y = byzantine::kPlayfieldTop + 4;
            auto loaded_state = compose_state;
            loaded_state.records[6] = snap_again;
            loaded_state.flag_4859cd = true;  // save load: DAT_004859cd = 1
            auto loaded_target = byzantine::make_surface(8, 8, 0);
            const auto loaded = byzantine::compose_scene(loaded_state, loaded_target);
            require(loaded.snap_drawn == 0 && loaded_target.at(3, 4) == 0 &&
                        loaded_state.records.at(6).frame_index_20 == 0,
                    "snap pass ran after a load (DAT_004859cd) ");
        }
        // 0x40241c rejects a record whose top-left is above the playfield: the
        // original never draws through a negative origin.
        auto above_state = compose_state;
        above_state.records.at(2).y = byzantine::kPlayfieldTop - 1;
        auto above_target = byzantine::make_surface(8, 8, 0);
        const auto above = byzantine::compose_scene(above_state, above_target);
        require(above.drawn == 0 && above.records[0].blit.written == 0 &&
                    above.records[0].blit.clipped == 1 &&
                    above_target.at(1, 1) == 0,
                "compose_scene drew a record above the playfield");
        std::filesystem::remove_all(root);

        // 0x43bb77: the frame list is circular, +0x04 is the delay and +0x06
        // the countdown. Verify the forward walk, the wrap, the frozen flag,
        // the ping-pong turnaround and the stop-at-last bit.
        {
            byzantine::SceneRecordState rec;
            rec.frames_known = true;
            rec.frame_count = 4;
            rec.current_frame = 0;
            rec.field04 = 2;
            rec.field06 = 0;
            rec.flags = 0;
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 1 && rec.field06 == 2 &&
                        rec.frame_index_20 == 0,
                    "0x43bb77 first step mismatch");
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 1 && rec.field06 == 1,
                    "0x43bb77 countdown mismatch");
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 1 && rec.field06 == 0,
                    "0x43bb77 countdown clamp mismatch");
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 2 && rec.field06 == 2,
                    "0x43bb77 second step mismatch");
            rec.current_frame = 3;
            rec.field06 = 0;
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 0, "0x43bb77 circular wrap mismatch");
            rec.flags = 0x04;
            rec.current_frame = 2;
            rec.frame_index_20 = 0xffff;
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 2 && rec.frame_index_20 == 2,
                    "0x43bb77 frozen record advanced");
            rec.flags = 0x01;
            rec.current_frame = 3;
            rec.field06 = 0;
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 2 && (rec.flags & 0x02) != 0,
                    "0x43bb77 ping-pong turnaround mismatch");
            rec.field06 = 0;
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 1, "0x43bb77 reverse step mismatch");
            rec.field06 = 0;
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 0, "0x43bb77 reverse tail mismatch");
            rec.field06 = 0;
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 1 && (rec.flags & 0x02) == 0,
                    "0x43bb77 forward resume mismatch");
            rec.flags = 0x08;
            rec.current_frame = 3;
            rec.field06 = 0;
            byzantine::advance_record_animation(rec);
            require(rec.current_frame == 3 && (rec.flags & 0x04) != 0,
                    "0x43bb77 stop-at-last mismatch");
        }

        // 0x79 ends the script and only raises the exit reason from 0 to 1.
        byzantine::SceneState end_state;
        const auto end_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x79, {}}, {0x6f, {}}}, {}, end_state, 10);
        require(end_trace.script_end_requested && end_trace.stopped_at == 1 &&
                end_state.field_45f490 == 1 && end_trace.steps == 1,
                "0x79 script end mismatch");
        end_state.field_45f490 = 3;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x79, {}}}, {}, end_state, 10);
        require(end_state.field_45f490 == 3,
                "0x79 overwrote a nonzero scene exit reason");

        // 0x8d records the requested CD; the drive check itself is host state.
        byzantine::SceneState disc_state;
        const auto disc_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x8d, {word(64, 3)}}}, {}, disc_state, 10);
        require(disc_trace.disc_request == 3 && disc_trace.stopped_at == 1,
                "0x8d disc request mismatch");
        const auto disc_variable = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x8d, {word(2, 9)}}}, {}, disc_state, 10);
        require(disc_variable.unresolved_variable == 9,
                "0x8d invented a disc variable");

        // 0x7f stores media value - frame in the raw destination local, in
        // 16-bit arithmetic; an absent record faults in the original.
        byzantine::SceneState media_delta_state;
        media_delta_state.media[5] = byzantine::SceneMediaState{};
        media_delta_state.media.at(5).value = 100;
        media_delta_state.media.at(5).frame = 40;
        const auto media_delta_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x7f, {word(2, 5), word(2, 25)}}, {0x6f, {}}},
            {}, media_delta_state, 10);
        require(media_delta_state.variables.at(25) == 60 &&
                !media_delta_trace.missing_media_record,
                "0x7f media delta mismatch");
        media_delta_state.media.at(5).value = 0;
        media_delta_state.media.at(5).frame = 1;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x7f, {word(2, 5), word(2, 25)}}},
            {}, media_delta_state, 10);
        require(media_delta_state.variables.at(25) == 0xffff,
                "0x7f did not truncate the subtraction to 16 bits");
        const auto media_missing_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0x7f, {word(2, 6), word(2, 26)}}},
            {}, media_delta_state, 10);
        require(media_missing_trace.missing_media_record == 6 &&
                media_delta_state.variables.count(26) == 0,
                "0x7f invented an absent media record");

        // 0xcf records one release per type-2 operand; 0xd0 counts the stop-all.
        byzantine::SceneState release_state;
        release_state.sounds[1] = byzantine::SceneSoundState{};
        release_state.sounds.at(1).defined = true;
        release_state.sounds.at(1).field14 = 1;
        const auto release_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xcf, {word(2, 1), word(64, 2), absent, absent, absent, absent, absent,
                     absent}},
             {0xd0, {}}, {0x6f, {}}},
            {}, release_state, 10);
        require(release_trace.sound_releases.size() == 1 &&
                release_trace.sound_releases[0].id == 1 &&
                release_trace.sound_releases[0].record_present &&
                release_trace.sound_releases[0].field14 == 1 &&
                release_trace.sound_releases[0].step != 0 &&
                release_trace.sound_stop_all_requests == 1 &&
                release_trace.sound_stop_alls.size() == 1 &&
                release_trace.sound_stop_alls[0].step != 0 &&
                !release_trace.unsupported_opcode,
                "0xcf/0xd0 sound release mismatch");

        // 0xd3 calls the nine-argument 0x428cbd form: mode 0, no text, and up to
        // three (timeout, follow-up record) pairs; only 0x40 operands 3/5
        // contribute. The +0x18/gamepref[2] gate is shared with 0xd1.
        byzantine::SceneState session_state;
        session_state.sounds[24] = byzantine::SceneSoundState{};
        session_state.sounds.at(24).defined = true;
        session_state.sounds.at(24).field18 = 1;
        const auto session_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xd3, {word(2, 24), word(64, 15000), word(2, 18), word(64, 18000),
                     word(2, 19), absent, absent}},
             {0x6f, {}}},
            {}, session_state, 10);
        require(session_trace.sound_sessions.size() == 1 &&
                session_trace.sound_sessions[0].id == 24 &&
                session_trace.sound_sessions[0].record_present &&
                session_trace.sound_sessions[0].allowed == true &&
                session_trace.sound_sessions[0].timeouts[0].defined &&
                session_trace.sound_sessions[0].timeouts[0].milliseconds == 15000 &&
                session_trace.sound_sessions[0].timeouts[0].follow_up == 18 &&
                session_trace.sound_sessions[0].timeouts[1].milliseconds == 18000 &&
                session_trace.sound_sessions[0].timeouts[1].follow_up == 19 &&
                !session_trace.sound_sessions[0].timeouts[2].defined,
                "0xd3 timed sound session mismatch");

        // 0xbe/0xd2: examine scene + gamepref[0] == 1; no save is written.
        byzantine::SceneState save_gate_state;
        save_gate_state.examine_scene_index = 8;
        std::array<std::uint32_t, 7> save_prefs{};
        save_prefs[0] = 1;
        save_gate_state.gameprefs = save_prefs;
        const auto save_gate_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xbe, {}}, {0xd2, {}}, {0x6f, {}}},
            {}, save_gate_state, 10);
        require(save_gate_trace.tempsave_requests == 1 &&
                save_gate_trace.autosave_requests == 1 &&
                !save_gate_trace.unresolved_save_setting &&
                !save_gate_trace.unsupported_opcode,
            "0xbe/0xd2 save request mismatch");
        save_prefs[0] = 0;
        save_gate_state.gameprefs = save_prefs;
        const auto no_save_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xbe, {}}, {0x6f, {}}}, {}, save_gate_state, 10);
        require(no_save_trace.tempsave_requests == 0,
                "0xbe ignored gamepref[0] == 0");
        byzantine::SceneState save_unknown_gate;
        const auto save_unknown_gate_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xbe, {}}, {0x6f, {}}}, {}, save_unknown_gate, 10);
        require(save_unknown_gate_trace.unresolved_save_setting,
                "0xbe accepted an unknown examine/gamepref gate");

        // 0xc6 creates/appends a DAT_00486a10 chain, 0xc7 removes one id.
        byzantine::SceneState chain_state;
        chain_state.dialogue_tables.names = {{"a", "", 0}, {"b", "", 0}, {"c", "", 0}};
        chain_state.dialogue_tables.scene_names = {{"a", "", 0}, {"b", "", 0}, {"c", "", 0}};
        chain_state.dialogue_tables.scene_names_known = true;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xc6, {word(2, 0), word(2, 0), word(2, 1), word(2, 2)}},
             {0xc7, {word(2, 0), word(2, 1)}}, {0x6f, {}}},
            {}, chain_state, 10);
        require(chain_state.dialogue_chains.at(0).count == 2 &&
                chain_state.dialogue_chains.at(0).ids[0] == 0 &&
                chain_state.dialogue_chains.at(0).ids[1] == 2,
            "0xc6/0xc7 dialogue chain mismatch");
        const auto missing_chain = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xc7, {word(2, 9), word(2, 1)}}, {0x6f, {}}},
            {}, chain_state, 10);
        require(missing_chain.missing_dialogue_chain == 9,
                "0xc7 accepted a missing dialogue chain");

        // 0xc8 sub-commands: flags, input waits, status requests, no-ops.
        byzantine::SceneState system_state;
        system_state.variables[5] = 0x32;
        system_state.variables[6] = 0;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xc8, {word(2, 5), word(2, 6)}}, {0x6f, {}}},
            {}, system_state, 10);
        require(system_state.field_460668 == 1, "0xc8 sub 0x32 mismatch");
        system_state.variables[5] = 0x33;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xc8, {word(2, 5), word(2, 6)}}, {0x6f, {}}},
            {}, system_state, 10);
        require(system_state.field_460668 == 2, "0xc8 sub 0x33 mismatch");
        system_state.variables[5] = 0x36;
        const auto status_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xc8, {word(2, 5), word(2, 6)}}, {0x6f, {}}},
            {}, system_state, 10);
        require(status_trace.status_write_requested == 0x36,
                "0xc8 status request mismatch");
        system_state.variables[5] = 0x1e;
        const auto reseed_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xc8, {word(2, 5), word(2, 6)}}, {0x6f, {}}},
            {}, system_state, 10);
        require(reseed_trace.unresolved_random,
                "0xc8 sub 0x1e invented the time-seeded RNG stream");
        system_state.variables[5] = 0x1f;
        const auto input_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xc8, {word(2, 5), word(2, 6), word(2, 0), word(2, 1), word(2, 2)}},
             {0x6f, {}}},
            {}, system_state, 10);
        require(input_trace.unresolved_system_input == 0x1f,
                "0xc8 sub 0x1f invented input values");
        system_state.variables[5] = 0x21;
        const auto no_op_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xc8, {word(2, 5), word(2, 6)}}, {0x6f, {}}},
            {}, system_state, 10);
        require(!no_op_trace.unsupported_opcode && !no_op_trace.unresolved_variable,
                "0xc8 no-op sub-command stopped the trace");

        // 0xcb records a 0x402285 panorama rotation request.
        byzantine::SceneState rotation_state;
        const auto rotation_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xcb, {word(64, 1234), word(64, 5)}},
             {0x6f, {}}},
            {}, rotation_state, 10);
        require(rotation_trace.panorama_rotations.size() == 1 &&
                rotation_trace.panorama_rotations[0].id == 1234 &&
                rotation_trace.panorama_rotations[0].step == 5,
            "0xcb panorama rotation mismatch");
        byzantine::SceneState default_rotation;
        const auto default_rotation_trace = byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}}, {0xcb, {word(64, 1234), {0, {}}}}, {0x6f, {}}},
            {}, default_rotation, 10);
        require(default_rotation_trace.panorama_rotations[0].step == 10,
            "0xcb default step mismatch");



        // 0xc1 writes +0x30 and maps the 3000 sentinel to -1.
        byzantine::SceneState sentinel_state;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xc1, {word(2, 4), word(64, 3000)}},
             {0xc1, {word(2, 4), word(64, 50)}},
             {0x6f, {}}},
            {}, sentinel_state, 10);
        require(sentinel_state.sounds.at(4).field30 == 50,
                "0xc1 value mismatch");
        byzantine::SceneState sentinel_only;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xc1, {word(2, 4), word(64, 3000)}},
             {0x6f, {}}},
            {}, sentinel_only, 10);
        require(sentinel_only.sounds.at(4).field30 == 0xffff,
                "0xc1 3000 sentinel mismatch");

        // 0xbc clears the active flag of every type-2 operand.
        byzantine::SceneState deactivate_state;
        byzantine::SceneSoundState deactivated;
        deactivated.defined = true;
        deactivated.field14 = 1;
        deactivated.active = true;
        deactivate_state.sounds[7] = deactivated;
        byzantine::trace_scene_label(
            {{0xe1, {word(2, 10)}},
             {0xbc, {word(2, 7), absent, absent, absent, absent, absent, absent, absent}},
             {0x6f, {}}},
            {}, deactivate_state, 10);
        require(!deactivate_state.sounds.at(7).active, "0xbc did not deactivate");

        // 0xcd records a text-draw request (0x417a17) instead of dropping it.
        const std::vector<CcInstruction> text_script{
            {0xe1, {word(2, 10)}},
            {0xcd, {word(64, 7), word(64, 1)}},
            {0x6f, {}}
        };
        byzantine::SceneState text_state;
        const auto text_trace = byzantine::trace_scene_label(text_script, {}, text_state, 10);
        require(text_trace.text_requests.size() == 1 &&
                text_trace.text_requests[0].id == 7 &&
                text_trace.text_requests[0].mode == 1 &&
                text_trace.text_requests[0].instruction == 1,
                "0xcd text request mismatch");

        // Conditional calls must resume after the call, including when a
        // second condition is false. An unconditional jump would skip the write.
        for (const unsigned opcode : {0x9d, 0x9e}) {
            for (const bool taken : {false, true}) {
                const bool compare_equal = opcode == 0x9d ? taken : !taken;
                const std::vector<CcInstruction> calls{
                    {0xe1, {word(2, 1)}},
                    {static_cast<std::uint8_t>(opcode),
                        {word(64, 7), word(64, compare_equal ? 7 : 8), word(2, 2)}},
                    {0x67, {word(64, 99), word(2, 9)}},
                    {0x6f, {}},
                    {0xe1, {word(2, 2)}},
                    {0x67, {word(64, 11), word(2, 8)}},
                    {0x6f, {}}
                };
                byzantine::SceneState call_state;
                const auto result = byzantine::trace_scene_label(calls, {}, call_state, 1);
                require(!result.unsupported_opcode && !result.unresolved_variable,
                        "conditional call unexpectedly stopped");
                require(call_state.variables.at(9) == 99, "conditional call lost return address");
                require((call_state.variables.count(8) != 0) == taken,
                        "conditional call selected wrong branch");
            }
        }

        // Truth tables include the sign boundary: the EXE compares unsigned
        // low words, so 65535 and 32768 must stay greater than 0 and 32767.
        struct OrderedCase {
            unsigned left, right;
            bool greater, greater_equal, less, less_equal;
        };
        const OrderedCase ordered_cases[]{
            {0, 0, false, true, false, true},
            {0, 65535, false, false, true, true},
            {65535, 0, true, true, false, false},
            {32768, 32767, true, true, false, false},
            {32767, 32768, false, false, true, true},
            {7, 7, false, true, false, true}
        };
        for (const unsigned opcode : {0x96, 0x97, 0x98, 0x99, 0x9f, 0xa0, 0xa1, 0xa2}) {
            const bool is_call = opcode >= 0x9f;
            const auto relation = opcode - (is_call ? 0x9f : 0x96);
            for (const auto& values : ordered_cases) {
                const bool expected[]{values.greater, values.greater_equal,
                                      values.less, values.less_equal};
                for (unsigned source = 0; source < 3; ++source) {
                    const std::vector<CcInstruction> ordered{
                        {0xe1, {word(2, 1)}},
                        {static_cast<std::uint8_t>(opcode),
                            {word(source == 1 ? 2 : 64, source == 1 ? 42 : values.left),
                             word(source == 2 ? 2 : 64, source == 2 ? 43 : values.right),
                             word(2, 2)}},
                        {0x67, {word(64, 99), word(2, 9)}},
                        {0x6f, {}},
                        {0xe1, {word(2, 2)}},
                        {0x67, {word(64, 11), word(2, 8)}},
                        {0x6f, {}}
                    };
                    byzantine::SceneState ordered_state;
                    ordered_state.variables = {{42, static_cast<std::uint16_t>(values.left)},
                                               {43, static_cast<std::uint16_t>(values.right)}};
                    const auto result = byzantine::trace_scene_label(ordered, {}, ordered_state, 1);
                    require(!result.unsupported_opcode && !result.unresolved_variable,
                            "ordered comparison unexpectedly stopped");
                    require((ordered_state.variables.count(8) != 0) == expected[relation],
                            "ordered comparison selected wrong branch");
                    require((ordered_state.variables.count(9) != 0) ==
                                (is_call || !expected[relation]),
                            "ordered jump/call continuation is incorrect");
                    if (source != 0) {
                        byzantine::SceneState missing;
                        const auto unresolved = byzantine::trace_scene_label(ordered, {}, missing, 1);
                        require(unresolved.unresolved_variable == (source == 1 ? 42 : 43) &&
                                missing.variables.empty() && unresolved.stopped_at == 1,
                                "ordered comparison invented missing state");
                    }
                }
            }
        }

        // Suspend/restore spans separate actions. Restoring merges the saved
        // bits, consumes them once, and must not revive a disabled hotspot on
        // a later restore. A second suspend replaces the old snapshot.
        const std::vector<CcInstruction> transitions{
            {0x72, {word(2, 1), word(64, 0), word(64, 0), word(64, 10),
                    word(64, 10), word(2, 10), word(64, 1), {0, {}}}},
            {0x72, {word(2, 2), word(64, 20), word(64, 0), word(64, 10),
                    word(64, 10), word(2, 10), word(64, 1), {0, {}}}},
            {0xe1, {word(2, 10)}}, {0xb8, {}}, {0x6f, {}},
            {0xe1, {word(2, 20)}}, {0xb7, {}}, {0x6f, {}},
            {0xe1, {word(2, 30)}}, {0x6e, {word(2, 1)}}, {0x6f, {}}
        };
        byzantine::SceneState transition_state;
        transition_state.hotspot_enabled = {{1, true}, {2, false}};
        const auto suspended = byzantine::trace_scene_label(transitions, {}, transition_state, 10);
        require(suspended.active_hotspots.empty(), "suspend left hotspots enabled");
        transition_state.hotspot_enabled[2] = true;
        const auto restored = byzantine::trace_scene_label(transitions, {}, transition_state, 20);
        require(restored.active_hotspots.size() == 2 && transition_state.suspended_hotspots.empty(),
                "restore lost saved or newly enabled hotspots");
        byzantine::trace_scene_label(transitions, {}, transition_state, 30);
        const auto consumed = byzantine::trace_scene_label(transitions, {}, transition_state, 20);
        require(consumed.active_hotspots.size() == 1 && consumed.active_hotspots[0].slot == 2,
                "restore reused consumed snapshot");
        byzantine::trace_scene_label(transitions, {}, transition_state, 10);
        byzantine::trace_scene_label(transitions, {}, transition_state, 10);
        require(byzantine::trace_scene_label(transitions, {}, transition_state, 20).active_hotspots.empty(),
                "nested suspend retained obsolete snapshot");

        // Real object +8 is authoritative, even if the compatibility mirror
        // is missing or stale. TK switches from slots 1/2 to 3/4/5 through
        // this path: retaining object.active made the old area win every hit.
        byzantine::SceneState live_transitions;
        live_transitions.objects[1].active = 1;  // saved active; no mirror
        live_transitions.objects[2].active = 0;
        live_transitions.hotspot_enabled[2] = true;  // stale mirror
        const auto live_suspended = byzantine::trace_scene_label(
            transitions, {}, live_transitions, 10);
        require(live_suspended.active_hotspots.empty() &&
                    live_transitions.objects[1].active == 0 &&
                    live_transitions.suspended_hotspots.size() == 1 &&
                    live_transitions.suspended_hotspots.count(1) == 1,
                "suspend did not use authoritative object active bytes");
        live_transitions.objects[2].active = 1;  // new view enabled separately
        const auto live_restored = byzantine::trace_scene_label(
            transitions, {}, live_transitions, 20);
        require(live_restored.active_hotspots.size() == 2 &&
                    live_transitions.objects[1].active == 1 &&
                    live_transitions.objects[2].active == 1,
                "restore lost an object or overwrote the new view's active set");
        byzantine::trace_scene_label(transitions, {}, live_transitions, 30);
        live_transitions.hotspot_enabled[1] = true;  // must not revive object
        const auto live_consumed = byzantine::trace_scene_label(
            transitions, {}, live_transitions, 20);
        require(live_consumed.active_hotspots.size() == 1 &&
                    live_consumed.active_hotspots.front().slot == 2,
                "stale mirror revived a disabled object after restore");

        // Dialogue records (DAT_00486f90), channel stacks and the CI/CY name
        // mapping used by 0xab/0xb0/0xb1/0xb6. Synthetic tables keep every
        // expectation hand-checkable: the global lists define record ids, the
        // per-scene tables define the script's name ids.
        byzantine::SceneDialogueTables dialogue_tables;
        dialogue_tables.names = {{"iAlpha", "", 255}, {"iBeta", "", 255},
                                 {"iGamma", "", 255}, {"iDelta", "", 255}};
        // CI order is the script's name id: 0 -> record 0, 1 -> record 2,
        // 2 -> absent name (falls back to record 0), 3 -> record 1.
        dialogue_tables.scene_names = {{"iAlpha", "", 255}, {"iGAMMA", "", 255},
                                       {"imissing", "", 255}, {"iBeta", "", 255}};
        dialogue_tables.scene_names_known = true;
        dialogue_tables.channels = {{"real", "", 255}, {"temp", "", 255},
                                    {"vArtEph", "", 255}};
        dialogue_tables.scene_channels = {{"TEMP", "", 255}, {"real", "", 255},
                                          {"unknown", "", 255}};
        dialogue_tables.scene_channels_known = true;
        byzantine::SceneState dialogue_state;
        dialogue_state.dialogue_tables = dialogue_tables;
        // 0xab: operand 0 = CI name id, operand 1 = raw mode, operand 2 = CY name
        // id. Name id 0 resolves to record 0 ("iAlpha"), channel id 1 to channel 0.
        const std::vector<CcInstruction> dialogue_script{
            {0xe1, {word(2, 10)}},
            {0xab, {word(2, 0), word(64, 1), word(2, 1)}},
            {0x6f, {}},
            {0xe1, {word(2, 20)}},
            {0xab, {word(2, 0), word(64, 0), word(2, 1)}},
            {0x6f, {}}
        };
        // The inventor media listing decides 0x407254, so an unknown listing must
        // stop the trace after the state byte has already been written.
        const auto blocked_start =
            byzantine::trace_scene_label(dialogue_script, {}, dialogue_state, 10);
        require(blocked_start.unresolved_dialogue_inventory &&
                !blocked_start.unresolved_dialogue &&
                dialogue_state.dialogue_records.at(0).state == 1 &&
                dialogue_state.dialogue_channels[0].empty(),
                "dialogue start invented the inventor media result");
        dialogue_state.inventor_assets = {"ialpha", "ibeta", "igamma", "idelta"};
        dialogue_state.hotitem_assets = {"halpha", "hbeta", "hgamma", "hdelta"};
        dialogue_state.inventory_assets_known = true;
        // The blocked run already wrote the state byte, so replay from a clean
        // record set: 0x4077b2 returns early for a line that is already state 1.
        dialogue_state.dialogue_records.clear();
        const auto started =
            byzantine::trace_scene_label(dialogue_script, {}, dialogue_state, 10);
        require(!started.unresolved_dialogue && !started.unresolved_dialogue_inventory &&
                dialogue_state.dialogue_records.at(0).state == 1 &&
                dialogue_state.dialogue_channels[0].size() == 1 &&
                dialogue_state.dialogue_channels[0][0] == 0 &&
                started.dialogue_refresh_requests == 1,
                "dialogue start mode/stack mismatch");
        // Mode 0 ends the line: 0x407933 removes the stacked id, shrinks the
        // scroll offset below its six-line window and refreshes the channel.
        dialogue_state.dialogue_scroll = 3;
        const auto stopped =
            byzantine::trace_scene_label(dialogue_script, {}, dialogue_state, 20);
        require(dialogue_state.dialogue_records.at(0).state == 0 &&
                dialogue_state.dialogue_channels[0].empty() &&
                dialogue_state.dialogue_scroll == 2 &&
                stopped.dialogue_refresh_requests == 1,
                "dialogue stop left state or scroll behind");
        // Mode 2 also stops, but 0x407933's early return ignores a record that
        // already holds 0 or 2; 0xab then still writes the mode byte afterwards.
        dialogue_state.dialogue_records[0].state = 2;
        dialogue_state.dialogue_channels[0] = {0};
        byzantine::trace_scene_label(dialogue_script, {}, dialogue_state, 20);
        require(dialogue_state.dialogue_channels[0].size() == 1 &&
                dialogue_state.dialogue_records.at(0).state == 0,
                "dialogue stop ignored the state-2 early return");
        // 0x408bb2 falls back to record 0 after logging when a CI name is absent
        // from the active list, and the missing CY name does the same.
        const std::vector<CcInstruction> fallback_script{
            {0xe1, {word(2, 10)}},
            {0xab, {word(2, 2), word(64, 1), word(2, 2)}},
            {0x6f, {}}
        };
        dialogue_state.dialogue_records.clear();
        dialogue_state.current_dialogue_record.reset();
        dialogue_state.dialogue_channels[0].clear();
        const auto fallback =
            byzantine::trace_scene_label(fallback_script, {}, dialogue_state, 10);
        require(!fallback.unresolved_dialogue && fallback.dialogue_name_fallbacks == 2 &&
                dialogue_state.dialogue_records.at(0).state == 1 &&
                dialogue_state.dialogue_channels[0].size() == 1,
                "dialogue name fallback did not resolve to record 0");
        // A CI name id outside the scene table would read a null record slot.
        const std::vector<CcInstruction> outside_script{
            {0xe1, {word(2, 10)}},
            {0xab, {word(2, 4), word(64, 1), word(2, 1)}},
            {0x6f, {}}
        };
        bool rejected_name = false;
        try { byzantine::trace_scene_label(outside_script, {}, dialogue_state, 10); }
        catch (const std::runtime_error&) { rejected_name = true; }
        require(rejected_name, "accepted a CI name id outside the record table");
        // 0xb0 jumps when the record byte equals the raw operand and 0xb1 when it
        // differs; both branches are real jumps, not requests.
        const std::vector<CcInstruction> branch_script{
            {0xe1, {word(2, 10)}},
            {0xb0, {word(2, 0), word(64, 0), word(2, 20), word(64, 0)}},
            {0x67, {word(64, 1), word(2, 5)}},
            {0x6f, {}},
            {0xe1, {word(2, 20)}},
            {0x67, {word(64, 2), word(2, 5)}},
            {0x6f, {}}
        };
        byzantine::SceneState branch_state;
        branch_state.dialogue_tables = dialogue_tables;
        branch_state.dialogue_records[0].state = 0;
        byzantine::trace_scene_label(branch_script, {}, branch_state, 10);
        require(branch_state.variables.at(5) == 2, "0xb0 equality jump was not taken");
        branch_state.variables.clear();
        branch_state.dialogue_records[0].state = 1;
        byzantine::trace_scene_label(branch_script, {}, branch_state, 10);
        require(branch_state.variables.at(5) == 1,
                "0xb0 jumped although the record byte differed");
        const std::vector<CcInstruction> differ_script{
            {0xe1, {word(2, 10)}},
            {0xb1, {word(2, 0), word(64, 1), word(2, 20), word(64, 0)}},
            {0x67, {word(64, 1), word(2, 5)}},
            {0x6f, {}},
            {0xe1, {word(2, 20)}},
            {0x67, {word(64, 2), word(2, 5)}},
            {0x6f, {}}
        };
        branch_state.variables.clear();
        branch_state.dialogue_records[0].state = 0;
        byzantine::trace_scene_label(differ_script, {}, branch_state, 10);
        require(branch_state.variables.at(5) == 2, "0xb1 inequality jump was not taken");
        branch_state.variables.clear();
        branch_state.dialogue_records[0].state = 1;
        byzantine::trace_scene_label(differ_script, {}, branch_state, 10);
        require(branch_state.variables.at(5) == 1,
                "0xb1 jumped although the record byte matched");
        // 0xb6 starts the line, makes it current (0x407b9c) and then 0x407ad5
        // returns every stacked state-3 line to 1 before marking this record 3.
        const std::vector<CcInstruction> prompt_script{
            {0xe1, {word(2, 30)}},
            {0xb6, {word(2, 0), word(2, 1)}},
            {0x6f, {}},
            {0xe1, {word(2, 40)}},
            {0xb6, {word(2, 3), word(2, 1)}},
            {0x6f, {}},
            {0xe1, {word(2, 50)}},
            {0xb6, {word(2, 0), word(2, 1)}},
            {0x6f, {}}
        };
        byzantine::SceneState prompt_state;
        prompt_state.dialogue_tables = dialogue_tables;
        prompt_state.inventor_assets = {"ialpha", "ibeta"};
        prompt_state.hotitem_assets = {"halpha", "hbeta"};
        prompt_state.inventory_assets_known = true;
        byzantine::trace_scene_label(prompt_script, {}, prompt_state, 30);
        require(prompt_state.dialogue_records.at(0).state == 3 &&
                prompt_state.current_dialogue_record == 0 &&
                prompt_state.dialogue_channels[0] == std::vector<std::uint16_t>{0},
                "0xb6 did not promote the started line");
        byzantine::trace_scene_label(prompt_script, {}, prompt_state, 40);
        require(prompt_state.dialogue_records.at(1).state == 3 &&
                prompt_state.dialogue_records.at(0).state == 1 &&
                prompt_state.current_dialogue_record == 1,
                "0xb6 did not return the previous prompt to state 1");
        byzantine::trace_scene_label(prompt_script, {}, prompt_state, 50);
        require(prompt_state.dialogue_records.at(0).state == 3 &&
                prompt_state.dialogue_records.at(1).state == 1,
                "0xb6 did not re-promote from the channel stack");

        // 0xb2/0xb3 are the call variants of the dialogue comparison: the taken
        // branch saves a return address (0x43194f), so the following 0x6f comes
        // back to the instruction after the call.
        const std::vector<CcInstruction> call_script{
            {0xe1, {word(2, 10)}},
            {0xb2, {word(2, 0), word(64, 0), word(2, 20), word(64, 0)}},
            {0x67, {word(64, 1), word(2, 5)}},
            {0x6f, {}},
            {0xe1, {word(2, 20)}},
            {0x67, {word(64, 2), word(2, 5)}},
            {0x6f, {}}
        };
        byzantine::SceneState call_state;
        call_state.dialogue_tables = dialogue_tables;
        call_state.dialogue_records[0].state = 0;
        const auto called = byzantine::trace_scene_label(call_script, {}, call_state, 10);
        require(call_state.variables.at(5) == 1 && called.stopped_at == 3,
                "0xb2 did not return to the instruction after the call");
        call_state.variables.clear();
        call_state.dialogue_records[0].state = 1;
        byzantine::trace_scene_label(call_script, {}, call_state, 10);
        require(call_state.variables.at(5) == 1,
                "0xb2 called although the record byte differed");
        const std::vector<CcInstruction> call_differ{
            {0xe1, {word(2, 10)}},
            {0xb3, {word(2, 0), word(64, 1), word(2, 20), word(64, 0)}},
            {0x67, {word(64, 1), word(2, 5)}},
            {0x6f, {}},
            {0xe1, {word(2, 20)}},
            {0x67, {word(64, 2), word(2, 5)}},
            {0x6f, {}}
        };
        call_state.variables.clear();
        byzantine::trace_scene_label(call_differ, {}, call_state, 10);
        require(call_state.variables.at(5) == 1,
                "0xb3 did not return to the instruction after the call");

        // 0xbd plays the AVI named by the CF record (DAT_00484330). Modes 2 and 3
        // are gated on gamepref[5]/[4]; DAT_00460664 is handed to 0xb9 for the
        // unconditional modes only.
        byzantine::SceneState video_state;
        video_state.video_records = {{"exCam", "EXAMINE.CCG", 255},
                                     {"hsain1", "EXAMINE.CCG", 255}};
        video_state.video_table_known = true;
        video_state.gameprefs = std::array<std::uint32_t, 7>{1, 1, 1, 1, 1, 1, 0};
        const std::vector<CcInstruction> video_script{
            {0xe1, {word(2, 10)}},
            {0xbd, {word(2, 0), word(64, 0), word(64, 0x1234), word(64, 0x5678)}},
            {0x6f, {}}
        };
        const auto played = byzantine::trace_scene_label(video_script, {}, video_state, 10);
        require(played.video_requests.size() == 1 && !played.unresolved_video_name &&
                played.video_requests[0].name == "exCam" &&
                played.video_requests[0].path == "avi/exCam.avi" &&
                played.video_requests[0].mode == 0 &&
                played.video_requests[0].start == 0x1234 &&
                played.video_requests[0].end == 0x5678 &&
                played.video_requests[0].transition == 0x65,
                "0xbd video request mismatch");
        // A constant operand 2/3 is only read when its type byte is 0x40.
        const std::vector<CcInstruction> video_no_operands{
            {0xe1, {word(2, 10)}},
            {0xbd, {word(2, 1), word(64, 4), {0, {}}, {0, {}}}},
            {0x6f, {}}
        };
        byzantine::trace_scene_label(video_no_operands, {}, video_state, 10);
        require(video_state.video_records.size() == 2, "video fixture changed");
        // Mode 3 keeps DAT_00460664 unset: its value depends on DAT_00464878.
        const std::vector<CcInstruction> video_mode3{
            {0xe1, {word(2, 10)}},
            {0xbd, {word(2, 1), word(64, 3), {0, {}}, {0, {}}}},
            {0x6f, {}}
        };
        const auto mode3 = byzantine::trace_scene_label(video_mode3, {}, video_state, 10);
        require(mode3.video_requests.size() == 1 &&
                mode3.video_requests[0].name == "hsain1" &&
                !mode3.video_requests[0].transition &&
                !mode3.video_requests[0].start,
                "0xbd mode 3 request mismatch");
        // A zero gamepref entry skips the whole command; without the pref array
        // the gate cannot be evaluated.
        byzantine::SceneState gated_state;
        gated_state.video_records = video_state.video_records;
        gated_state.video_table_known = true;
        // Mode 2 reads gamepref[5] and mode 3 reads gamepref[4] (FUN_0041d440).
        gated_state.gameprefs = std::array<std::uint32_t, 7>{1, 1, 1, 1, 1, 0, 0};        const std::vector<CcInstruction> video_mode2{
            {0xe1, {word(2, 10)}},
            {0xbd, {word(2, 0), word(64, 2), {0, {}}, {0, {}}}},
            {0x6f, {}}
        };
        const auto skipped = byzantine::trace_scene_label(video_mode2, {}, gated_state, 10);
        require(skipped.video_skipped == 1 && skipped.video_requests.empty(),
                "0xbd ignored the gamepref gate");
        gated_state.gameprefs.reset();
        const auto unknown_gate = byzantine::trace_scene_label(video_mode2, {}, gated_state, 10);
        require(unknown_gate.unresolved_video_setting &&
                unknown_gate.stopped_at == 1,
                "0xbd invented the gamepref gate");
        byzantine::SceneState no_video_table;
        const auto no_table = byzantine::trace_scene_label(video_script, {}, no_video_table, 10);
        require(no_table.unresolved_video_name == 0, "0xbd invented the CF record name");
        bool rejected_video = false;
        try {
            const std::vector<CcInstruction> outside_video{
                {0xe1, {word(2, 10)}},
                {0xbd, {word(2, 5), word(64, 0), {0, {}}, {0, {}}}},
                {0x6f, {}}
            };
            byzantine::trace_scene_label(outside_video, {}, video_state, 10);
        } catch (const std::runtime_error&) { rejected_video = true; }
        require(rejected_video, "accepted a video id outside the CF table");

        // 0xbf/0xc0/0xc2 rewrite DAT_00486220 audio parameters. Operands are
        // resolved through the scene variable table and 0xc0/0xc2 then push the
        // result through 0x427d1a, which is counted instead of played.
        const std::vector<CcInstruction> audio_script{
            {0xe1, {word(2, 10)}},
            {0xbf, {word(2, 0), word(64, 0x1111), word(64, 0x2222), word(2, 7)}},
            {0x6f, {}}
        };
        byzantine::SceneState audio_state;
        audio_state.variables[7] = 0x3333;
        byzantine::trace_scene_label(audio_script, {}, audio_state, 10);
        require(audio_state.sounds.at(0).field28 == 0x1111 &&
                audio_state.sounds.at(0).field2c == 0x2222 &&
                audio_state.sounds.at(0).field1c == 0x3333,
                "0xbf audio parameter mismatch");
        // A constant operand 3 never writes +0x1c (the original tests type == 2).
        const std::vector<CcInstruction> audio_constant{
            {0xe1, {word(2, 10)}},
            {0xbf, {word(2, 0), word(64, 0x4444), word(64, 0x5555), word(64, 0x6666)}},
            {0x6f, {}}
        };
        byzantine::trace_scene_label(audio_constant, {}, audio_state, 10);
        require(audio_state.sounds.at(0).field28 == 0x4444 &&
                audio_state.sounds.at(0).field1c == 0x3333,
                "0xbf wrote +0x1c from a constant operand");
        // A missing variable stops before any parameter is written.
        const std::vector<CcInstruction> audio_missing{
            {0xe1, {word(2, 10)}},
            {0xbf, {word(2, 0), word(64, 0x7777), word(64, 0x8888), word(2, 11)}},
            {0x6f, {}}
        };
        const auto blocked_audio = byzantine::trace_scene_label(audio_missing, {}, audio_state, 10);
        require(blocked_audio.unresolved_variable == 11 &&
                audio_state.sounds.at(0).field28 == 0x4444,
                "0xbf applied a partially resolved update");
        const std::vector<CcInstruction> spread_script{
            {0xe1, {word(2, 10)}},
            {0xc0, {word(2, 0), word(64, 0x9999), word(2, 8), word(2, 9)}},
            {0xc2, {word(2, 0), word(64, 0xaaaa), word(64, 0xbbbb), word(2, 7)}},
            {0xc0, {word(2, 0), word(64, 0xcccc), word(64, 0x1234), {0, {}}}},
            {0x6f, {}}
        };
        byzantine::SceneState spread_state;
        spread_state.variables = {{7, 0x3333}, {8, 0xdddd}, {9, 0xeeee}};
        const auto spread = byzantine::trace_scene_label(spread_script, {}, spread_state, 10);
        const auto& sound = spread_state.sounds.at(0);
        require(sound.field1c == 0xcccc && sound.field20 == 0xaaaa &&
                sound.field24 == 0xbbbb && spread.sound_refresh_requests == 3,
                "0xc0/0xc2 audio parameter or refresh mismatch");

        // 0xb9 loads the CO.ALL panorama node named by its raw operand 0. The
        // staged 0xba position and the DAT_00460664 transition code (set by 0xbd)
        // are consumed here; 0x40186e's JPEG/projection work stays outside.
        const std::vector<byzantine::BackgroundEntry> panorama_records{
            {"ea00", "EA.CCG"}, {"ea02", "EA.CCG"}};
        const std::vector<CcInstruction> node_script{
            {0xe1, {word(2, 10)}},
            {0xba, {word(2, 747)}},
            {0xb9, {word(2, 1), word(64, 0), word(64, 0), {0, {}}}},
            {0x6f, {}}
        };
        byzantine::SceneState panorama_state;
        panorama_state.panorama_records = panorama_records;
        panorama_state.panorama_table_known = true;
        const auto loaded =
            byzantine::trace_scene_label(node_script, {}, panorama_state, 10);
        require(!loaded.unresolved_panorama_node && loaded.panorama_loads.size() == 1 &&
                loaded.panorama_loads[0].record == 1 &&
                loaded.panorama_loads[0].name == "ea02" &&
                loaded.panorama_loads[0].jpeg_path == "nodes/ea/ea02.jpg" &&
                loaded.panorama_loads[0].horizontal == 747 &&
                loaded.panorama_loads[0].transition == 100 &&
                !panorama_state.pending_panorama_x,
                "0xb9 panorama load mismatch");
        // DAT_00460664 is consumed when operand 3 is not a scene variable, while
        // a type-2 operand 3 takes the 0x432133 mapping and leaves it pending.
        panorama_state.pending_panorama_transition = 0x67;
        const auto with_pending =
            byzantine::trace_scene_label(node_script, {}, panorama_state, 10);
        require(with_pending.panorama_loads.size() == 1 &&
                with_pending.panorama_loads[0].transition == 0x67 &&
                !panorama_state.pending_panorama_transition,
                "0xb9 did not consume the pending transition code");
        panorama_state.pending_panorama_transition = 0x67;
        const std::vector<CcInstruction> panorama_mapped{
            {0xe1, {word(2, 10)}},
            {0xb9, {word(2, 0), word(64, 0), word(64, 0), word(2, 2)}},
            {0x6f, {}}
        };
        const auto mapped =
            byzantine::trace_scene_label(panorama_mapped, {}, panorama_state, 10);
        require(mapped.panorama_loads.size() == 1 &&
                mapped.panorama_loads[0].transition == 0x65 &&
                panorama_state.pending_panorama_transition == 0x67,
                "0xb9 transition mapping or pending code mismatch");
        // A nonzero force clears the DAT_0047e2f0 entry selected by the raw
        // operand; only the index is recoverable.
        panorama_state.pending_panorama_transition.reset();
        const std::vector<CcInstruction> panorama_force{
            {0xe1, {word(2, 10)}},
            {0xb9, {word(2, 0), word(64, 1), word(2, 5), word(64, 100)}},
            {0x6f, {}}
        };
        panorama_state.variables[5] = 3;
        const auto forced = byzantine::trace_scene_label(panorama_force, {}, panorama_state, 10);
        require(forced.panorama_loads.size() == 1 &&
                forced.panorama_loads[0].force == 1 &&
                forced.panorama_loads[0].vertical == 3 &&
                forced.panorama_loads[0].transition == 100 &&
                forced.panorama_loads[0].flag_clear == 1 &&
                forced.panorama_flag_clears == 1,
                "0xb9 force/vertical/flag-clear mismatch");
        // The transition is consumed before the operand resolutions: a missing
        // variable still clears DAT_00460664, exactly like the original order.
        byzantine::SceneState blocked_panorama;
        blocked_panorama.panorama_records = panorama_records;
        blocked_panorama.panorama_table_known = true;
        blocked_panorama.pending_panorama_transition = 0x66;
        const auto blocked = byzantine::trace_scene_label(panorama_force, {}, blocked_panorama, 10);
        require(blocked.unresolved_variable == 5 &&
                !blocked_panorama.pending_panorama_transition,
                "0xb9 resolved operands before consuming the pending transition");
        byzantine::SceneState no_panorama_table;
        const auto no_nodes =
            byzantine::trace_scene_label(node_script, {}, no_panorama_table, 10);
        require(no_nodes.unresolved_panorama_node == 1,
                "0xb9 invented the CO record name");
        bool rejected_panorama = false;
        try {
            const std::vector<CcInstruction> outside_panorama{
                {0xe1, {word(2, 10)}},
                {0xb9, {word(2, 7), word(64, 0), word(64, 0), {0, {}}}},
                {0x6f, {}}
            };
            byzantine::trace_scene_label(outside_panorama, {}, panorama_state, 10);
        } catch (const std::runtime_error&) { rejected_panorama = true; }
        require(rejected_panorama, "accepted a panorama id outside the CO table");

        if (argc >= 3) {
            const auto ea = byzantine::parse_cc(read_file(argv[1]));
            const auto cb = byzantine::parse_cb(read_file(argv[2]));
            byzantine::SceneState ea_state;
            const auto start = byzantine::trace_scene_label(ea, cb, ea_state, 475);
            require(start.active_hotspots.size() == 2, "EA start hotspot count changed");
            const auto closeup = byzantine::trace_scene_label(ea, cb, ea_state, 595);
            require(closeup.background && closeup.background->entry.name == "EAEmShCU",
                    "EA closeup selected wrong background");
            require(closeup.active_hotspots.size() == 1 && closeup.active_hotspots[0].slot == 94,
                    "EA transition retained stale enabled hotspots");
            const auto back = byzantine::trace_scene_label(
                ea, cb, ea_state, closeup.active_hotspots[0].action);
            // The conditional call is decoded, but entry 475 does not set up
            // variable 5 used by the panorama return. Preserve that unknown.
            require(!back.unsupported_opcode && back.unresolved_variable == 5,
                    "EA return must report missing panorama state");

            // Original EA label 428 branches on variable 26 < 9. Explicitly
            // seeded values test this command, not the game's missing setup.
            byzantine::SceneState comparison_state;
            comparison_state.variables[26] = 8;
            const auto less = byzantine::trace_scene_label(ea, cb, comparison_state, 428);
            require(!less.unsupported_opcode && !less.unresolved_variable &&
                    less.stopped_at == 475,
                    "EA less-than jump failed to reach its return");
            comparison_state.variables[26] = 9;
            const auto equal = byzantine::trace_scene_label(ea, cb, comparison_state, 428);
            // Instruction 470 is 0xb1 on dialogue name id 2, which needs the
            // scene's CI record table; without it the trace must not guess the
            // record byte or the branch.
            require(equal.unresolved_dialogue == 2 && equal.stopped_at == 470,
                    "EA dialogue branch must report the missing CI table");

            // Label 464 calls label 422 only for values below 2. Both paths now
            // complete: 0xc1 and 0x6c are implemented.
            comparison_state.variables[26] = 1;
            const auto called = byzantine::trace_scene_label(ea, cb, comparison_state, 464);
            require(!called.unsupported_opcode && !called.unresolved_variable &&
                    called.stopped_at == 521,
                    "EA conditional call selected wrong target");
            comparison_state.variables[26] = 2;
            const auto skipped = byzantine::trace_scene_label(ea, cb, comparison_state, 464);
            require(!skipped.unsupported_opcode && !skipped.unresolved_variable &&
                    skipped.stopped_at == 521,
                    "EA conditional call incorrectly included equality");
        }
        if (argc >= 5) {
            const auto star = byzantine::parse_cc(read_file(argv[3]));
            const auto cb = byzantine::parse_cb(read_file(argv[4]));
            const auto hotspots = byzantine::static_hotspots(star);
            require(!hotspots.empty(), "STAR fixture has no hotspots");
            byzantine::SceneState star_state;
            const auto slot = hotspots.front().slot;
            star_state.suspended_hotspots[slot] = true;
            const auto restored = byzantine::trace_scene_label(star, cb, star_state, 1282);
            require(!restored.unsupported_opcode && !restored.unresolved_variable &&
                    restored.active_hotspots.size() == 1 && restored.active_hotspots[0].slot == slot &&
                    star_state.suspended_hotspots.empty(),
                    "original STAR restore label did not consume the saved hotspot");
        }
        if (argc >= 6) {
            const auto ea = byzantine::parse_cc(read_file(argv[1]));
            const auto nodes = byzantine::parse_cb(read_file(argv[5]));
            const auto bindings = byzantine::static_panoramas(ea, nodes);
            bool found = false;
            for (const auto& binding : bindings) {
                if (binding.instruction == 317) {
                    found = binding.id == 0 && binding.jpeg_path == "nodes/ea/ea00.jpg";
                }
            }
            require(found, "EA return panorama binding changed");
            const auto cb = byzantine::parse_cb(read_file(argv[2]));
            byzantine::SceneState state;
            const auto trace = byzantine::trace_scene_label(ea, cb, state, 1754);
            require(!trace.unsupported_opcode && !trace.unresolved_variable &&
                    state.pending_panorama_x == 747,
                    "original EA camera setter failed");
        }
        std::cout << "scene state tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
