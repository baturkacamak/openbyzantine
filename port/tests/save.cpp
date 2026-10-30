#include "byzantine/cc.hpp"
#include "byzantine/save.hpp"
#include "byzantine/runtime_save.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot read " + path.string());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

bool rejected(const std::vector<std::uint8_t>& bytes) {
    try {
        byzantine::parse_save(bytes);
    } catch (const std::runtime_error&) {
        return true;
    }
    return false;
}

byzantine::CcArgument word(unsigned value) {
    return {2, {static_cast<std::uint8_t>(value), static_cast<std::uint8_t>(value >> 8)}};
}

// 0xcc writes DAT_00464300[id]+0x14; 0x70 replaces pc with
// DAT_004654e0[object->+0x0c]. Both are fed here from a SaveGame built in
// memory, so the restore path and both branches are tested without a fixture.
void check_restore_and_object_opcodes() {
    using byzantine::CcInstruction;

    byzantine::SaveGame save;
    byzantine::SaveObjectRecord object;
    object.slot = 2;
    object.x = 10;
    object.y = 20;
    object.width = 30;
    object.height = 40;
    object.active = 1;
    object.kind = 8;
    object.uninitialized_0a = 0xbeef;  // must stay uninterpreted
    object.label = 70;
    object.name_id = 5;
    object.argument = 0xffffffffu;
    save.objects.push_back(object);
    object.slot = 5;
    object.label = 999;  // deliberately not a label of the test script
    save.objects.push_back(object);

    byzantine::SaveMediaRecord media;
    media.slot = 3;
    media.value = 12;
    media.frame = 4;
    media.timestamp = 5;
    media.media_id = 6;
    media.active = 1;
    media.type = 2;
    save.media.push_back(media);

    byzantine::SaveSoundRecord sound;
    sound.slot = 4;
    const auto put16 = [&sound](std::size_t offset, std::uint16_t value) {
        sound.raw[offset] = static_cast<std::uint8_t>(value);
        sound.raw[offset + 1] = static_cast<std::uint8_t>(value >> 8);
    };
    put16(0x04, 1);
    put16(0x08, 2);
    put16(0x0c, 3);
    put16(0x10, 4);
    put16(0x14, 5);
    put16(0x18, 6);
    put16(0x30, 0xffff);
    sound.raw[0x34] = 1;
    save.sounds.push_back(sound);

    byzantine::SaveNodeSlot node;
    node.slot = 6;
    node.marker = 1;
    node.frame_index_1c = 7;
    node.frame_index_20 = 0xffff;
    node.field_2c = 8;
    node.field_30 = 9;
    node.extra.push_back(byzantine::SaveNodeExtra{2, 11, 12});
    // 0x426b9d copies +0x14 (the frame count) and feeds the first recovered
    // dword to 0x40cf73(record, index) as the current frame.
    node.raw[0x14] = 9;
    node.raw[0x15] = 0;
    save.nodes.push_back(node);

    // DAT_0047e2f0: local 0/1/2 are declared, every later local is identity.
    save.node_values.assign(byzantine::kSaveNodeValueCount, 0);
    save.node_values[3] = 333;
    save.node_values[1] = 111;
    save.node_values[74] = 740;
    save.node_values[200] = 2000;
    save.node_values[299] = 2990;

    // 0x408adc dialogue state: channel 0 has two stacked ids (top first), the
    // current record is 1, and ten allocated records carry their state bytes.
    save.field_45d1c4 = 0;
    save.field_45d1bc = 0;
    save.field_45d1c0 = 1;
    save.dialogue_depths[0] = 2;
    save.dialogue_stacks[0] = 7;
    save.dialogue_stacks[1] = 9;
    for (std::uint8_t record = 0; record < 10; ++record) {
        save.dialogue_marks.emplace_back(0, record == 0 ? 1 : (record == 1 ? 3 : 0));
    }
    save.dialogue_index = 1;
    byzantine::SaveDialogueChain chain;
    chain.slot = 0;
    chain.raw.fill(0xff);  // 0xffffffff in the unused id slots
    chain.raw[0] = 2;
    chain.raw[1] = 0;
    chain.raw[2] = 0;
    chain.raw[3] = 0;
    chain.raw[4] = 3;
    chain.raw[5] = 0;
    chain.raw[6] = 0;
    chain.raw[7] = 0;
    chain.raw[0x64] = 2;
    chain.raw[0x65] = 0;
    chain.raw[0x66] = 0;
    chain.raw[0x67] = 0;
    save.dialogue_chains.push_back(chain);
    save.field_460668 = 2;

    byzantine::SceneState state;
    state.objects[9].label = 1;  // the save replaces, not merges
    state.media[9].active = 1;
    byzantine::load_save(state, save);
    require(state.objects.size() == 2 && state.objects.count(9) == 0,
            "load_save merged object records");
    require(state.media.size() == 1 && state.media.count(1) == 0,
            "load_save merged media records");
    const auto& restored = state.objects.at(2);
    require(restored.x == 10 && restored.y == 20 && restored.width == 30 &&
                restored.height == 40 && restored.active == 1 && restored.kind == 8 &&
                restored.label == 70 && restored.name_id == 5 &&
                restored.argument == 0xffffffffu,
            "load_save object field mismatch");
    require(state.media.at(3).value == 12 && state.media.at(3).active == 1 &&
                state.media.at(3).type == 2, "load_save media field mismatch");
    require(state.sounds.at(4).defined && state.sounds.at(4).enabled == 1 &&
                state.sounds.at(4).field08 == 2 && state.sounds.at(4).field0c == 3 &&
                state.sounds.at(4).field10 == 4 && state.sounds.at(4).field14 == 5 &&
                state.sounds.at(4).field18 == 6 && state.sounds.at(4).field30 == 0xffff &&
                state.sounds.at(4).active,
            "load_save sound field mismatch");
    require(state.records.at(6).marker == 1 && state.records.at(6).frame_index_1c == 7 &&
                state.records.at(6).frame_index_20 == 0xffff &&
                state.records.at(6).frame_count == 9 &&
                state.records.at(6).current_frame == 7 &&
                !state.records.at(6).frames_known &&
                state.records.at(6).field_2c == 8 && state.records.at(6).field_30 == 9 &&
                state.records.at(6).extra.size() == 1 &&
                state.records.at(6).extra[0].control == 2,
            "load_save node field mismatch");

    // 0x408adc: stacks are restored top-first, record states come from repeated
    // (0x00, state) pairs and the current record is the saved index.
    require(state.dialogue_channels[0].size() == 2 &&
                state.dialogue_channels[0][0] == 7 &&
                state.dialogue_channels[0][1] == 9 &&
                state.current_dialogue_channel == 0 &&
                state.dialogue_scroll == 0 &&
                state.dialogue_visible &&
                state.current_dialogue_record.has_value() &&
                *state.current_dialogue_record == 1 &&
                state.dialogue_records.size() == 10 &&
                state.dialogue_records.at(0).state == 1 &&
                state.dialogue_records.at(1).state == 3 &&
                state.dialogue_records.at(2).state == 0,
            "load_save dialogue state mismatch");
    require(state.dialogue_chains.size() == 1 &&
                state.dialogue_chains.at(0).count == 2 &&
                state.dialogue_chains.at(0).ids[0] == 2 &&
                state.dialogue_chains.at(0).ids[1] == 3 &&
                state.dialogue_chains.at(0).ids[2] == 0xffffffffu &&
                state.field_460668 == 2 &&
                state.flag_4859cd,
            "load_save dialogue chain/flag mismatch");

    // -1 clears the current line; an index at or past the allocated record
    // count selects a NULL DAT_00486f90 slot, which is also no current line.
    byzantine::SaveGame no_current = save;
    no_current.dialogue_index = -1;
    byzantine::SceneState no_current_state;
    byzantine::load_save(no_current_state, no_current);
    require(!no_current_state.current_dialogue_record,
            "load_save kept a current record for index -1");
    no_current.dialogue_index = 10;
    byzantine::load_save(no_current_state, no_current);
    require(!no_current_state.current_dialogue_record,
            "load_save kept a current record for an unallocated slot");

    // A depth or a stacked id outside the file's own limits is corrupt state.
    bool deep_rejected = false;
    byzantine::SaveGame deep = save;
    deep.dialogue_depths[0] = byzantine::kSaveDialogueStackDepth + 1;
    try { byzantine::load_save(no_current_state, deep); }
    catch (const std::runtime_error&) { deep_rejected = true; }
    require(deep_rejected, "load_save accepted a dialogue depth beyond 350");
    bool orphan_rejected = false;
    byzantine::SaveGame orphan = save;
    orphan.dialogue_stacks[0] = 10;  // ten records: ids 0..9 only
    try { byzantine::load_save(no_current_state, orphan); }
    catch (const std::runtime_error&) { orphan_rejected = true; }
    require(orphan_rejected, "load_save accepted a dialogue id with no record");

    // 0x46ee00: declared locals take their LCV global id, the remaining 300-row
    // entries are identity. SceneState keys stay the raw local operand.
    const auto populated = byzantine::load_save_variables(
        state, save, {3, 1, 2999});
    require(populated == 300 && state.variables.size() == 300,
            "load_save_variables did not fill the 300-entry scene row");
    require(state.variables.at(0) == 333 && state.variables.at(1) == 111 &&
                state.variables.at(2) == 0 &&
                state.variables.at(3) == 333 &&
                state.variables.at(74) == 740 &&
                state.variables.at(200) == 2000 &&
                state.variables.at(299) == 2990,
            "load_save_variables LCV/identity mapping mismatch");

    // 0x70 pauses at the conversation record; the caller applies the result and
    // resumes inside the same script. Operand 0 is the engine/<scene>cd record
    // index (2), not an object slot; the result object selects the label.
    const std::vector<CcInstruction> script{
        {0xe1, {word(10)}},       // 0
        {0x70, {word(2), word(0)}},  // 1: conversation record 2
        {0x6f, {}},               // 2
        {0xe1, {word(70)}},       // 3
        {0x69, {word(0)}},        // 4
        {0xcc, {word(2), word(0x1234)}},  // 5
        {0xcc, {word(9), word(7)}},       // 6: slot 9 is absent
        {0x6f, {}}                // 7
    };
    const auto paused = byzantine::trace_scene_label(script, {{"bg", ""}}, state, 10);
    require(paused.conversation_request.has_value() &&
                paused.conversation_request->record == 2 &&
                paused.conversation_request->object_context == 0 &&
                paused.conversation_request->instruction == 1,
            "0x70 did not report the conversation request");
    require(!paused.background, "0x70 continued past the conversation");
    byzantine::SceneConverseJump jump;
    const auto resume = byzantine::apply_conversation_result(script, state, 1, 2, &jump);
    require(resume == 4 && jump.id == 2 && jump.label == 70 && jump.target == 4,
            "conversation result did not select the object label");
    const auto resumed =
        byzantine::trace_scene_from_instruction(script, {{"bg", ""}}, state, resume);
    require(resumed.background && resumed.background->id == 0,
            "resumed segment did not run the target label");
    require(state.objects.at(2).argument == 0x1234,
            "0xcc did not write the object argument");
    require(resumed.missing_object.has_value() && *resumed.missing_object == 9,
            "0xcc on an absent object did not report the NULL dereference");

    // A missing object must never be created implicitly.
    byzantine::SceneState empty;
    const auto missing = byzantine::trace_scene_label(
        {{0xe1, {word(10)}}, {0xcc, {word(9), word(1)}}, {0x6f, {}}}, {}, empty, 10);
    require(missing.missing_object.has_value() && *missing.missing_object == 9 &&
                empty.objects.empty(),
            "0xcc allocated an absent object");

    // 0x70 with an absent result object and with an object label outside the
    // script: applying the result must refuse instead of guessing.
    const auto missing_converse = byzantine::trace_scene_label(
        {{0xe1, {word(10)}}, {0x70, {word(9), word(0)}}, {0x6f, {}}}, {}, empty, 10);
    require(missing_converse.conversation_request.has_value() &&
                missing_converse.conversation_request->record == 9,
            "0x70 did not report record 9");
    bool missing_threw = false;
    try {
        (void)byzantine::apply_conversation_result(
            {{0xe1, {word(10)}}, {0x70, {word(9), word(0)}}, {0x6f, {}}}, empty, 1, 9);
    } catch (const std::runtime_error&) {
        missing_threw = true;
    }
    require(missing_threw, "a conversation result object was invented");

    bool label_threw = false;
    try {
        (void)byzantine::apply_conversation_result(script, state, 1, 5);
    } catch (const std::runtime_error&) {
        label_threw = true;
    }
    require(label_threw, "a conversation object label outside the script was accepted");

    // The raw operand widens as the original's short->uint assignment does.
    byzantine::SceneState negative;
    byzantine::load_save(negative, save);
    byzantine::trace_scene_label(
        {{0xe1, {word(10)}}, {0xcc, {word(2), word(0x8000)}}, {0x6f, {}}}, {}, negative,
        10);
    require(negative.objects.at(2).argument == 0xffff8000u,
            "0xcc did not sign-extend the encoded short");
}

void check_writer_node_extras() {
    // FUN_004269ec writes a count; FAM_00426b9d reads control bytes. The writer
    // emits the reader's framing (1 + 8 bytes per entry, 0 terminator), so a
    // multi-entry list is loadable and parse_save sees it again.
    byzantine::SaveGame save;
    save.thumbnail.assign(byzantine::kSaveThumbnailBytes, 0);
    save.node_values.assign(byzantine::kSaveNodeValueCount, 0);
    byzantine::SaveNodeSlot node;
    node.present = true;
    node.slot = 3;
    node.marker = 0;
    node.frame_index_1c = 0xffff;
    node.frame_index_20 = 0xffff;
    node.field_2c = 0xffffffffu;
    node.field_30 = 0xffffffffu;
    node.extra.push_back(byzantine::SaveNodeExtra{1, 0x12345678u, 0x9abcdef0u});
    node.extra.push_back(byzantine::SaveNodeExtra{1, 0x01020304u, 0x05060708u});
    save.nodes.push_back(node);
    const auto bytes = byzantine::write_save(save);
    const auto parsed = byzantine::parse_save(bytes);
    require(parsed.nodes.size() == 1 && parsed.nodes[0].extra.size() == 2,
            "node extra list with two entries did not round-trip");
    require(parsed.nodes[0].extra[0].first == 0x12345678u &&
                parsed.nodes[0].extra[0].second == 0x9abcdef0u &&
                parsed.nodes[0].extra[1].first == 0x01020304u &&
                parsed.nodes[0].extra[1].second == 0x05060708u,
            "node extra list values changed");
}

void check_chapter_list() {
    using byzantine::parse_chapter_scene_list;
    using byzantine::save_scene_name;

    // 0x438173 lowercases every line and the original opens the file in text
    // mode, so CRLF arrives as LF.
    const std::string text = "START\r\nAL\r\nexamine\r\n";
    const std::vector<std::uint8_t> data(text.begin(), text.end());
    const auto names = parse_chapter_scene_list(data);
    require(names.size() == 3, "chapter list entry count");
    require(names[0] == "start" && names[1] == "al" && names[2] == "examine",
            "chapter list lowercasing");
    // No trailing empty entry from the final newline, and no entry when empty.
    require(parse_chapter_scene_list({}).empty(), "empty chapter list");

    bool long_name_rejected = false;
    const std::string long_text = "ninechars\r\n";
    try {
        parse_chapter_scene_list(
            std::vector<std::uint8_t>(long_text.begin(), long_text.end()));
    } catch (const std::runtime_error&) {
        long_name_rejected = true;
    }
    require(long_name_rejected, "accepted a scene name longer than 8 characters");

    // DAT_00482428 past the end must not invent a name.
    byzantine::SaveGame save;
    save.scene_index = 3;
    require(!save_scene_name(save, names).has_value(), "invented a scene name past the list");
    save.scene_index = 1;
    require(save_scene_name(save, names).value() == "al", "chapter index mapping");
}

void check_real_save(const std::filesystem::path& path, const std::vector<std::string>& scenes,
                     const std::filesystem::path& cc_root) {
    const auto bytes = read_file(path);
    const auto save = byzantine::parse_save(bytes);
    const auto tag = path.filename().string();

    require(save.size == bytes.size(), tag + ": size mismatch");
    // FUN_00424dc0 round-trip: the writer must reproduce the reader's image
    // byte for byte on the shipped save set.
    const auto written = byzantine::write_save(save);
    if (written != bytes) {
        std::size_t at = 0;
        while (at < written.size() && at < bytes.size() && written[at] == bytes[at]) ++at;
        std::cerr << tag << ": first write_save difference at " << at << " orig="
                  << (at < bytes.size() ? std::to_string(bytes[at]) : "-") << " new="
                  << (at < written.size() ? std::to_string(written[at]) : "-")
                  << " sizes=" << bytes.size() << '/' << written.size() << '\n';
        std::cerr << "  orig: ";
        for (std::size_t i = at > 16 ? at - 16 : 0; i < std::min(bytes.size(), at + 16); ++i)
            std::cerr << std::hex << static_cast<int>(bytes[i]) << ' ';
        std::cerr << "\n  new:  ";
        for (std::size_t i = at > 16 ? at - 16 : 0; i < std::min(written.size(), at + 16); ++i)
            std::cerr << std::hex << static_cast<int>(written[i]) << ' ';
        std::cerr << std::dec << '\n';
    }
    require(written == bytes, tag + ": write_save is not byte-identical");
    const auto reparsed = byzantine::parse_save(written);
    require(reparsed.scene_index == save.scene_index &&
                reparsed.objects.size() == save.objects.size() &&
                reparsed.media.size() == save.media.size() &&
                reparsed.sounds.size() == save.sounds.size() &&
                reparsed.nodes.size() == save.nodes.size() &&
                reparsed.dialogue_chains.size() == save.dialogue_chains.size(),
            tag + ": write_save/parse_save round-trip lost a table");
    require(save.thumbnail.size() == byzantine::kSaveThumbnailBytes,
            tag + ": thumbnail block size");
    require(save.node_values.size() == byzantine::kSaveNodeValueCount,
            tag + ": node value table size");
    require(save.dialogue_stacks.size() ==
                byzantine::kSaveDialogueChannels * byzantine::kSaveDialogueStackDepth,
            tag + ": dialogue stack size");
    require(save.chapter == "show", tag + ": unexpected chapter " + save.chapter);
    // FUN_00424dc0 forces 1 to 2 before writing, so the shipped set is 2..6.
    require(save.cd_number >= 2 && save.cd_number <= 6,
            tag + ": unexpected CD number " + std::to_string(save.cd_number));
    require(!save.description.empty(), tag + ": empty description");

    const auto scene = byzantine::save_scene_name(save, scenes);
    require(scene.has_value(), tag + ": scene index past the chapter list");
    const auto cc_path = cc_root / [&scene] {
        std::string upper = *scene;
        for (char& character : upper) character = static_cast<char>(std::toupper(
            static_cast<unsigned char>(character)));
        return upper;
    }();
    require(std::filesystem::exists(cc_path), tag + ": no CC script for scene " + *scene);

    // The self-validating check: every DAT_00464300 +0x0c value is a CC label,
    // because 0x70 CONVERSE indexes DAT_004654e0 with it (0x42db22 case 0x70).
    const auto flow = byzantine::resolve_control_flow(byzantine::parse_cc(read_file(cc_path)));
    require(!flow.labels.empty(), tag + ": empty CC label set");
    require(!save.objects.empty(), tag + ": no object records");
    for (const auto& object : save.objects) {
        require(object.label <= 0xffff, tag + ": object label outside 16 bits");
        if (flow.labels.count(static_cast<std::uint16_t>(object.label)) == 0) {
            throw std::runtime_error(tag + ": object slot " +
                                     std::to_string(object.slot) + " label " +
                                     std::to_string(object.label) +
                                     " is not a label of scene " + *scene);
        }
    }

    // The restored tables must line up exactly with the parsed records.
    byzantine::SceneState restored;
    byzantine::load_save(restored, save);
    require(restored.objects.size() == save.objects.size() &&
                restored.media.size() == save.media.size() &&
                restored.sounds.size() == save.sounds.size() &&
                restored.records.size() == save.nodes.size(),
            tag + ": load_save table sizes differ from the parsed save");
    require(restored.dialogue_records.size() == save.dialogue_marks.size(),
            tag + ": load_save dialogue record count differs from the marks");
    require(restored.dialogue_chains.size() == save.dialogue_chains.size(),
            tag + ": load_save dialogue chain count differs");
    for (const auto& chain : save.dialogue_chains) {
        const auto found = restored.dialogue_chains.find(chain.slot);
        require(found != restored.dialogue_chains.end() &&
                    found->second.count == chain.count(),
                tag + ": load_save dialogue chain content differs");
    }
    require(restored.field_460668 == save.field_460668 && restored.flag_4859cd,
            tag + ": load_save mid-block/load flag differs");
    require(restored.current_dialogue_channel == save.field_45d1c4 &&
                restored.dialogue_scroll == static_cast<std::int32_t>(save.field_45d1bc) &&
                restored.dialogue_visible == (save.field_45d1c0 != 0),
            tag + ": load_save dialogue selectors differ");
    std::size_t stacked = 0;
    for (std::size_t channel = 0; channel < byzantine::kSaveDialogueChannels; ++channel) {
        require(restored.dialogue_channels[channel].size() == save.dialogue_depths[channel],
                tag + ": load_save dialogue depth differs");
        stacked += restored.dialogue_channels[channel].size();
    }
    require(stacked <= byzantine::kSaveDialogueChannels * byzantine::kSaveDialogueStackDepth,
            tag + ": load_save dialogue stacks exceed their capacity");
    if (save.dialogue_index >= 0 &&
        static_cast<std::size_t>(save.dialogue_index) < save.dialogue_marks.size()) {
        require(restored.current_dialogue_record.has_value() &&
                    *restored.current_dialogue_record ==
                        static_cast<std::uint16_t>(save.dialogue_index),
                tag + ": load_save lost the current dialogue record");
    } else {
        require(!restored.current_dialogue_record,
                tag + ": load_save invented a current dialogue record");
    }

    // Real-save acceptance for the two object opcodes. VT label 356 runs six
    // 0xcc writes; EA label 1779 reaches a 0x70 whose operand 0 is the
    // conversation-record index 3 (not object 3). The conversation itself is
    // interactive, so the trace pauses; applying a scripted result object 3
    // resumes at object 3's +0x0c label 544.
    if (*scene == "vt") {
        const auto script = byzantine::parse_cc(read_file(cc_path));
        const auto trace = byzantine::trace_scene_label(script, {}, restored, 356);
        require(trace.object_writes.size() == 6 && !trace.missing_object,
                tag + ": VT label 356 did not perform the six 0xcc writes");
    }
    if (*scene == "ea") {
        const auto script = byzantine::parse_cc(read_file(cc_path));
        // The jump target runs 0x69, so the test supplies placeholder CB records
        // rather than depending on a CB fixture the save test does not receive.
        const std::vector<byzantine::BackgroundEntry> entries(1024, {"bg", ""});
        const auto trace = byzantine::trace_scene_label(script, entries, restored, 1779);
        require(trace.conversation_request.has_value() &&
                    trace.conversation_request->record == 3 &&
                    trace.conversation_request->instruction == 2246,
                tag + ": EA label 1779 did not pause at conversation record 3");
        byzantine::SceneConverseJump jump;
        const auto resume = byzantine::apply_conversation_result(
            script, restored, trace.conversation_request->instruction, 3, &jump);
        require(jump.id == 3 && jump.label == 544,
                tag + ": EA conversation result did not select object 3's label 544");
        const auto resumed = byzantine::trace_scene_from_instruction(
            script, entries, restored, resume);
        require(resumed.background.has_value(),
                tag + ": EA resumed conversation segment did not run its label");
    }

    // Truncation must fail, never silently parse a prefix.
    require(rejected(std::vector<std::uint8_t>(bytes.begin(), bytes.begin() + 50000)),
            tag + ": accepted a 50000-byte prefix");
    require(rejected(std::vector<std::uint8_t>(bytes.begin(), bytes.end() - 1)),
            tag + ": accepted an image with the final newline removed");

    // Extra bytes after the notebook text are not text: 0x42118c only writes
    // whole lines, so a trailing NUL is corruption.
    auto extended = bytes;
    extended.push_back(0);
    require(rejected(extended), tag + ": accepted a trailing NUL byte");

    std::cout << "save " << tag << " bytes=" << save.size << " cd=" << save.cd_number
              << " scene=" << *scene << " objects=" << save.objects.size()
              << " media=" << save.media.size() << " sounds=" << save.sounds.size()
              << " nodes=" << save.nodes.size()
              << " scene_records=" << save.scene_records.size()
              << " notebook=" << save.notebook.size() << " labels=" << flow.labels.size()
              << " ok\n";
}
}  // namespace

void check_runtime_companion() {
    byzantine::RuntimeSave saved;
    saved.btb={1,2,3};saved.view.scene="test";
    saved.view.camera={{123.25,-7.5}};
    saved.view.morris.initialize();saved.view.morris.nodes[4].flags|=2;
    saved.view.conversation[3].disabled.insert({1,2,3,4});
    saved.view.conversation[3].slot_2a[4]=byzantine::ConversationSlotValue{5,{1,2,3,4}};
    saved.closeup=4;saved.parent_btb={4,5};saved.parent.scene="parent";
    const auto bytes=byzantine::write_runtime_save(saved);
    const auto restored=byzantine::read_runtime_save(bytes);
    require(byzantine::write_runtime_save(restored)==bytes,"runtime companion lost data");
    require(restored.view.camera==saved.view.camera && restored.parent_btb==saved.parent_btb,
            "runtime camera/parent not preserved");
    for(auto bad:{std::vector<std::uint8_t>{},std::vector<std::uint8_t>{99,0,0,0}}) {
        bool refused=false;try {byzantine::read_runtime_save(bad);}catch(const std::exception&){refused=true;}
        require(refused,"invalid companion accepted");
    }
    for(std::size_t length:{std::size_t(8),bytes.size()-1}) {
        bool refused=false;try {byzantine::read_runtime_save({bytes.begin(),bytes.begin()+length});}
        catch(const std::exception&){refused=true;}
        require(refused,"truncated companion accepted");
    }
}

int main(int argc, char** argv) {
    try {
        check_runtime_companion();
        check_chapter_list();
        check_writer_node_extras();
        check_restore_and_object_opcodes();

        // Real saves are supplied by the caller; without them the layout checks
        // above still run and the file-dependent part is skipped.
        if (argc < 4) {
            std::cout << "save tests passed (real save fixtures skipped)\n";
            return 0;
        }
        const std::filesystem::path save_dir(argv[1]);
        if (!std::filesystem::is_directory(save_dir) ||
            !std::filesystem::exists(argv[2]) || !std::filesystem::is_directory(argv[3])) {
            std::cout << "save tests passed (save fixtures or tables missing; skipped)\n";
            return 0;
        }
        const auto scenes = byzantine::parse_chapter_scene_list(read_file(argv[2]));
        const std::filesystem::path cc_root(argv[3]);
        require(!scenes.empty(), "empty chapter scene list");

        std::vector<std::filesystem::path> saves;
        for (const auto& entry : std::filesystem::directory_iterator(save_dir)) {
            if (!entry.is_regular_file()) continue;
            auto extension = entry.path().extension().string();
            for (char& character : extension) {
                character = static_cast<char>(std::tolower(
                    static_cast<unsigned char>(character)));
            }
            if (extension == ".btb") saves.push_back(entry.path());
        }
        std::sort(saves.begin(), saves.end());
        require(!saves.empty(), "no .btb files in the save directory");

        // At least three different sizes are required by the step's acceptance
        // criteria; report the spread instead of assuming it.
        std::set<std::uintmax_t> sizes;
        for (const auto& path : saves) {
            sizes.insert(std::filesystem::file_size(path));
            check_real_save(path, scenes, cc_root);
        }
        require(sizes.size() >= 3, "fewer than three distinct save sizes");

        std::cout << "save tests passed (" << saves.size() << " saves, " << sizes.size()
                  << " distinct sizes)\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
