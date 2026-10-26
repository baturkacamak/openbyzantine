// Conversation (.ccb/.ccf/.byz) parser and 0x432d6e/0x43688b/0x436b01 runtime.
//
// The fixtures are synthetic: the shipped dialogue text is game data and is not
// committed. The real conversation is exercised by the acceptance run in
// analysis/rea/CONVERSATION.md (MR label 413 -> IMhMR2a).

#include "byzantine/all.hpp"
#include "byzantine/conversation.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

int failures = 0;

void check(bool condition, const std::string& what) {
    if (!condition) {
        std::cerr << "FAIL " << what << '\n';
        ++failures;
    }
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

struct Writer {
    std::vector<std::uint8_t> bytes;
    void u8(std::uint8_t value) { bytes.push_back(value); }
    void u16(std::uint16_t value) {
        bytes.push_back(static_cast<std::uint8_t>(value & 0xff));
        bytes.push_back(static_cast<std::uint8_t>(value >> 8));
    }
};

struct Operand {
    std::uint8_t type;
    std::uint16_t value;
};

struct Instr {
    std::uint8_t opcode;
    std::vector<Operand> operands;
};

void write_argument(Writer& writer, const Operand& operand) {
    writer.u8(operand.type);
    switch (operand.type) {
    case 0:
        break;
    case 1:
    case 4:
        writer.u8(static_cast<std::uint8_t>(operand.value & 0xff));
        break;
    case 2:
    case 8:
    case 16:
    case 24:
    case 64:
        writer.u8(static_cast<std::uint8_t>(operand.value & 0xff));
        writer.u8(static_cast<std::uint8_t>(operand.value >> 8));
        break;
    case 32:
        writer.u8(static_cast<std::uint8_t>(operand.value & 0xff));
        writer.u8(static_cast<std::uint8_t>(operand.value >> 8));
        writer.u16(0);
        break;
    default:
        throw std::runtime_error("bad fixture argument type");
    }
}

void write_instruction(Writer& writer, const Instr& instruction) {
    writer.u8(instruction.opcode);
    for (const auto& operand : instruction.operands) write_argument(writer, operand);
}

void write_list(Writer& writer, const std::vector<Instr>& instructions) {
    writer.u16(static_cast<std::uint16_t>(instructions.size()));
    for (const auto& instruction : instructions) write_instruction(writer, instruction);
}

struct EntryFixture {
    std::uint16_t value;
    std::vector<Instr> list;
};

// One group record. `empty` groups consume a slot but do not count toward the
// declared non-empty group count.
struct GroupFixture {
    std::vector<EntryFixture> entries;
};

void write_groups(Writer& writer, const std::vector<GroupFixture>& groups) {
    std::size_t nonempty = 0;
    for (const auto& group : groups) {
        if (!group.entries.empty()) ++nonempty;
    }
    writer.u8(static_cast<std::uint8_t>(nonempty));
    for (const auto& group : groups) {
        writer.u8(static_cast<std::uint8_t>(group.entries.size()));
        for (const auto& entry : group.entries) {
            writer.u16(entry.value);
            writer.u16(0xffff);
            write_list(writer, entry.list);
            writer.u8(0);  // no sub-entries
        }
    }
}

// list1 = 0x3a no-op header, list2 = debug 0x3e.
void write_header(Writer& writer) {
    write_list(writer, {Instr{0x3a, {{2, 0}, {2, 1}, {2, 2}, {2, 3}, {0, 0}, {0, 0}}}});
    write_list(writer, {Instr{0x3e, {{32, 0}}}});
}

std::vector<std::string> lines(std::initializer_list<const char*> values) {
    std::vector<std::string> out;
    for (const auto* value : values) out.emplace_back(value);
    return out;
}

EntryFixture entry(std::uint16_t value, std::vector<Instr> list) {
    return EntryFixture{value, std::move(list)};
}

GroupFixture group(std::initializer_list<EntryFixture> entries) {
    return GroupFixture{std::vector<EntryFixture>(entries)};
}

byzantine::ConversationProgram build_program() {
    Writer writer;
    write_header(writer);
    // group 0: line 1, set local 5, jump to group 1
    // group 1: line 2, no list
    // group 2: line 4, add to local 5, end
    write_groups(writer, {group({entry(1, {Instr{0x67, {{64, 1}, {2, 5}}},
                                          Instr{0x32, {{1, 1}}}})}),
                          group({entry(2, {})}),
                          group({entry(4, {Instr{0x74, {{64, 2}, {2, 5}}},
                                           Instr{0x39, {}}})})});
    // answers for group 1: line 3 ends, line 4 jumps to group 2
    write_groups(writer, {group({}), group({entry(3, {Instr{0x39, {}}}),
                                            entry(4, {Instr{0x32, {{1, 2}}}})})});
    const auto clips = lines({"null", "abcmeh00", "abcmeh01", "abcpro00", "abcpro01"});
    const auto texts = lines({"null", "first line", "second line", "yes", "no"});
    return byzantine::parse_conversation_program(writer.bytes, clips, texts, "synthetic");
}

byzantine::SceneState zero_state() {
    byzantine::SceneState state;
    for (int local = 0; local < 300; ++local) state.variables[local] = 0;
    return state;
}

void test_lines_and_records() {
    {
        const std::string file = "one\r\ntwo\n\nthree";
        const std::vector<std::uint8_t> bytes(file.begin(), file.end());
        const auto parsed = byzantine::parse_conversation_lines(bytes, "test");
        check(parsed.size() == 4, "CCF/BYZ CRLF split");
        check(parsed[0] == "one" && parsed[2].empty() && parsed[3] == "three",
              "CCF/BYZ content");
    }
    {
        const std::string file = "one\n";
        const std::vector<std::uint8_t> bytes(file.begin(), file.end());
        check(byzantine::parse_conversation_lines(bytes, "test").size() == 1,
              "trailing newline is not a record");
    }
    {
        Writer writer;
        const std::string a = "IMhMR1a";
        const std::string b = "IMhMr1a.ccd";
        for (const char ch : a) writer.u8(static_cast<std::uint8_t>(ch));
        writer.u8(0);
        for (const char ch : b) writer.u8(static_cast<std::uint8_t>(ch));
        writer.u8(0);
        writer.u8(0xff);
        const auto records = byzantine::parse_conversation_records(writer.bytes);
        check(records.size() == 1 && records[0].name == "IMhMR1a" &&
                  records[0].file == "IMhMr1a.ccd",
              "conversation record pair");
    }
    {
        Writer writer;
        writer.u8('a');
        writer.u8(0);
        writer.u8('b');
        writer.u8(0);
        writer.u8(0xfe);
        bool threw = false;
        try {
            (void)byzantine::parse_conversation_records(writer.bytes);
        } catch (const std::exception&) {
            threw = true;
        }
        check(threw, "conversation record separator is validated");
    }
}

void test_program_and_runtime() {
    auto program = build_program();
    check(program.trailing_bytes == 0, "no trailing bytes");
    check(program.lines.size() == 50 && program.answers.size() == 50,
          "group arrays are 50 slots");
    check(program.lines[0].size() == 1 && program.lines[1].size() == 1 &&
              program.lines[2].size() == 1,
          "line groups");
    check(program.answers[0].empty() && program.answers[1].size() == 2,
          "empty group consumes a slot");

    auto state = zero_state();
    byzantine::ConversationSession session(program, state);
    session.start();
    auto event = session.next();
    check(event.kind == byzantine::ConversationEventKind::line &&
              event.line.index == 1 && event.line.show_text,
          "first line");
    session.line_shown();
    event = session.next();
    check(event.kind == byzantine::ConversationEventKind::line &&
              event.line.index == 2,
          "second line");
    session.line_shown();
    event = session.next();
    check(event.kind == byzantine::ConversationEventKind::answers &&
              event.answers.size() == 2,
          "answer list");
    check(event.answers[0].line == 3 && event.answers[1].line == 4, "answer lines");
    session.choose(0);
    event = session.next();
    check(event.kind == byzantine::ConversationEventKind::finished &&
              event.result == 0xffff,
          "first answer ends the conversation");
    check(state.variables[5] == 1, "0x67 wrote local 5");
    check(state.variable_writes.size() == 1 && state.variable_writes[0] == 5,
          "0x67 logged the local for the pool export");

    auto state2 = zero_state();
    byzantine::ConversationSession session2(program, state2);
    session2.start();
    (void)session2.next();
    session2.line_shown();
    (void)session2.next();
    session2.line_shown();
    (void)session2.next();
    session2.choose(1);
    auto event2 = session2.next();
    check(event2.kind == byzantine::ConversationEventKind::line &&
              event2.line.index == 4,
          "second answer branches to group 2");
    session2.line_shown();
    event2 = session2.next();
    check(event2.kind == byzantine::ConversationEventKind::finished, "group 2 ends");
    check(state2.variables[5] == 3, "0x74 added to local 5");

    auto state3 = zero_state();
    byzantine::ConversationSession session3(program, state3);
    session3.start();
    (void)session3.next();
    session3.line_shown();
    (void)session3.next();
    session3.line_shown();
    (void)session3.next();
    session3.click_object(7);
    const auto event3 = session3.next();
    check(event3.kind == byzantine::ConversationEventKind::finished &&
              event3.result == 7,
          "object click returns the object id");
    check(state3.variables[5] == 1, "object click keeps the conversation's writes");
}

void test_memory_reentry() {
    auto program = build_program();
    const auto run_to_answers = [&](byzantine::ConversationSession& session) {
        (void)session.next();
        session.line_shown();
        (void)session.next();
        session.line_shown();
        return session.next();
    };

    // A selected answer stays unspeak across a second 0x70. The completed run
    // also sets state+0xe, so the re-entry starts from the default line group.
    auto state = zero_state();
    auto& memory = state.conversation_memory[0];
    {
        byzantine::ConversationSession session(program, state, &memory);
        session.start();
        auto event = run_to_answers(session);
        check(event.kind == byzantine::ConversationEventKind::answers &&
                  event.answers.size() == 2,
              "memory fixture first list");
        session.choose(0);
        event = session.next();
        check(event.kind == byzantine::ConversationEventKind::finished,
              "memory fixture first run ends");
        check(memory.done, "completed conversation sets state+0xe");
        check(memory.unspeak.size() == 1, "selected answer is remembered");
    }
    {
        byzantine::ConversationSession session(program, state, &memory);
        session.start();
        auto event = run_to_answers(session);
        check(event.kind == byzantine::ConversationEventKind::answers &&
                  event.answers.size() == 1 && event.answers[0].line == 4,
              "asked answer stays hidden on the same-scene re-entry");
        session.cancel();
        check(memory.done, "cancel does not clear the completed state+0xe");
    }

    // A cancel before any choice keeps state+0xe clear; the next 0x70 shows the
    // same two answers and the scene stays in control.
    auto state2 = zero_state();
    auto& memory2 = state2.conversation_memory[0];
    {
        byzantine::ConversationSession session(program, state2, &memory2);
        session.start();
        auto event = run_to_answers(session);
        check(event.answers.size() == 2, "cancel fixture first list");
        const auto cancelled = session.cancel();
        check(cancelled.kind == byzantine::ConversationEventKind::finished &&
                  cancelled.result == 0xfffe,
              "cancel returns 0xfffe");
        check(!memory2.done, "cancel leaves state+0xe clear");
        check(memory2.unspeak.empty(), "cancel before a choice remembers no answer");
    }
    {
        byzantine::ConversationSession session(program, state2, &memory2);
        session.start();
        auto event = run_to_answers(session);
        check(event.kind == byzantine::ConversationEventKind::answers &&
                  event.answers.size() == 2,
              "cancelled conversation restarts its list");
    }
}

void test_conditions() {
    // group 0 line, list disables answer line 3 in groups2[1] and enables it
    // only when local 7 == 2.
    Writer writer;
    write_header(writer);
    write_groups(writer, {group({entry(1, {Instr{0x37, {{8, 1}}},
                                          Instr{0x42, {{2, 7}, {64, 2}, {8, 1}}},
                                          Instr{0x32, {{1, 1}}}})}),
                          group({entry(2, {})})});
    write_groups(writer, {group({}), group({entry(3, {Instr{0x39, {}}}),
                                            entry(4, {Instr{0x39, {}}})})});
    auto state = zero_state();
    const auto clips = lines({"null", "a", "b", "c", "d"});
    const auto texts = lines({"null", "a", "b", "c", "d"});
    auto program = byzantine::parse_conversation_program(writer.bytes, clips, texts, "cond");
    byzantine::ConversationSession session(program, state);
    session.start();
    (void)session.next();
    session.line_shown();
    (void)session.next();
    session.line_shown();
    auto event = session.next();
    check(event.kind == byzantine::ConversationEventKind::answers &&
              event.answers.size() == 1 && event.answers[0].line == 4,
          "0x37 disabled the first answer");
    check(state.variables[7] == 0, "unrelated local untouched");

    state.variables[7] = 2;
    byzantine::ConversationSession session2(program, state);
    session2.start();
    (void)session2.next();
    session2.line_shown();
    (void)session2.next();
    session2.line_shown();
    event = session2.next();
    check(event.kind == byzantine::ConversationEventKind::answers &&
              event.answers.size() == 2,
          "0x42 re-enabled the answer when the variable matched");
}

void test_unsupported_and_rejections() {
    {
        Writer writer;
        write_header(writer);
        write_groups(writer, {{{{1, {Instr{0x69, {{2, 0}, {2, 1}}}}}}}});
        write_groups(writer, {{{{2, {}}}}});
        auto program = byzantine::parse_conversation_program(
            writer.bytes, lines({"null", "a", "b"}), lines({"null", "a", "b"}), "bad");
        auto state = zero_state();
        byzantine::ConversationSession session(program, state);
        session.start();
        auto event = session.next();
        check(event.kind == byzantine::ConversationEventKind::line, "line before opcode");
        session.line_shown();
        event = session.next();
        check(event.kind == byzantine::ConversationEventKind::error &&
                  event.error.find("0x69") != std::string::npos,
              "unsupported conversation opcode is an explicit error");
    }
    {
        Writer writer;
        write_header(writer);
        write_groups(writer, {{{{1, {}}}}});
        write_groups(writer, {});
        writer.bytes.push_back(0xaa);  // trailing byte is read by the original? no:
        // FUN_004119e3 stops after groups2; trailing bytes are allowed.
        auto program = byzantine::parse_conversation_program(
            writer.bytes, lines({"null", "a"}), lines({"null", "a"}), "trail");
        check(program.trailing_bytes == 1, "trailing bytes are counted");
    }
    {
        Writer writer;
        write_header(writer);
        writer.u8(1);  // one declared non-empty group, but no group record follows
        bool threw = false;
        try {
            (void)byzantine::parse_conversation_program(writer.bytes, {}, {}, "trunc");
        } catch (const std::exception&) {
            threw = true;
        }
        check(threw, "a missing non-empty group is rejected");
    }
    {
        Writer writer;
        write_header(writer);
        writer.u8(1);  // one non-empty group
        for (int i = 0; i < 50; ++i) {
            writer.u8(0);  // empty group
        }
        writer.u8(1);
        writer.u16(1);
        writer.u16(0xffff);
        write_list(writer, {});
        writer.u8(0);
        bool threw = false;
        try {
            (void)byzantine::parse_conversation_program(writer.bytes, {}, {}, "slots");
        } catch (const std::exception&) {
            threw = true;
        }
        check(threw, "a group beyond slot 49 is rejected");
    }
    {
        Writer writer;
        write_header(writer);
        writer.u8(1);
        writer.u8(1);
        writer.u16(1);
        writer.u16(0xffff);
        write_list(writer, {Instr{0x36, {{2, 0}}}});
        writer.u8(0);
        // Remove the operand's second data byte, leaving a truncated type 2.
        writer.bytes.pop_back();
        bool threw = false;
        try {
            (void)byzantine::parse_conversation_program(writer.bytes, {}, {}, "trunc2");
        } catch (const std::exception&) {
            threw = true;
        }
        check(threw, "a truncated operand is rejected");
    }
}

void test_real_byz(const std::string& path) {
    if (path.empty()) return;
    const auto archive = byzantine::parse_all(read_file(path));
    const byzantine::AllEntry* entry = nullptr;
    for (const auto& candidate : archive.entries) {
        if (candidate.name == "IMHMR2A") entry = &candidate;
    }
    check(entry != nullptr, "Byz.all holds IMHMR2A");
    if (entry == nullptr) return;
    std::vector<std::uint8_t> data(read_file(path));
    data = std::vector<std::uint8_t>(data.begin() + static_cast<std::ptrdiff_t>(
                                                          archive.data_offset + entry->start),
                                     data.begin() + static_cast<std::ptrdiff_t>(
                                                         archive.data_offset + entry->end));
    const auto parsed = byzantine::parse_conversation_lines(data, "IMHMR2A");
    check(parsed.size() >= 30, "IMHMR2A has at least 30 BYZ records");
    check(!parsed.empty() && parsed[0] == "null", "IMHMR2A line 0 is the null marker");
    bool found_greeting = false;
    for (const auto& line : parsed) {
        if (line.find("Kapal") != std::string::npos) found_greeting = true;
    }
    check(found_greeting, "IMHMR2A holds the shipped greeting line");
}

}  // namespace

int main(int argc, char** argv) {
    try {
        test_lines_and_records();
        test_program_and_runtime();
        test_memory_reentry();
        test_conditions();
        test_unsupported_and_rejections();
        test_real_byz(argc > 1 ? argv[1] : "");
    } catch (const std::exception& error) {
        std::cerr << "unexpected exception: " << error.what() << '\n';
        return 2;
    }
    if (failures != 0) {
        std::cerr << failures << " conversation test(s) failed\n";
        return 1;
    }
    std::cout << "conversation-test: all checks passed\n";
    return 0;
}
