#pragma once

#include "byzantine/cc.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace byzantine {

// One operand of a conversation (0x43340a) instruction, as parsed by
// FUN_00412762. The original instruction node stores the 8 argument-type bytes
// at +1..+8 and then eight 4-byte operand slots at +0x0a, +0x0e, ...
//
// FUN_00436652 combines two 16-bit fields of a slot:
//     word = *(u16*)(slot + 2) * 0x100 + *(u16*)(slot + 0)
// The parser initialises both fields to 0x00ff. A one-byte operand therefore
// reads back as 0xff00 | byte, while a two-byte operand reads back as the
// little-endian 16-bit value. `value` and `high` keep the two halves because
// several opcodes read them separately (FUN_004367eb uses `value` as an array
// index and `high` as the number of `next` hops).
struct ConversationArgument {
    std::uint8_t type = 0;
    std::uint16_t value = 0xffff;  // *(u16*)(slot + 0)
    std::uint16_t high = 0xffff;   // *(u16*)(slot + 2)

    // FUN_00436652
    std::uint16_t word() const {
        return static_cast<std::uint16_t>(static_cast<std::uint16_t>(high * 0x100) + value);
    }
};

struct ConversationInstruction {
    std::uint8_t opcode = 0;
    std::vector<ConversationArgument> arguments;
};

// One entry of a conversation group list (FUN_00412cc4, 0x14 bytes).
//   +0x00 value   (BYZ/CCF line index; also FUN_004367eb's array index)
//   +0x02 link    (purpose not recovered; read and retained)
//   +0x04 list    (FUN_00412762 instruction list)
//   +0x08 next    (next entry in the same group)
//   +0x0c subs    (sub-entry chain; FUN_004367eb counts hops through it for the
//                  0x4367eb `high` operand? no: sub-entries are a separate chain)
//   +0x10 enabled (1 at allocation)
//   +0x11 unspeak (0 at allocation; 0x4f sets it, a selected answer sets it)
struct ConversationEntry {
    std::uint16_t value = 0xffff;
    std::uint16_t link = 0xffff;
    std::vector<ConversationInstruction> instructions;
    std::vector<ConversationEntry> subs;
    bool enabled = true;
    bool unspeak = false;
};

// One group array (state+0x4c4 for lines, state+0x58c for answers). The
// original has 50 pointer slots; empty groups leave a null slot and are still
// consumed from the file until the declared non-empty group count is reached.
using ConversationGroups = std::vector<std::vector<ConversationEntry>>;

struct ConversationProgram {
    std::string name;
    std::vector<ConversationInstruction> lists[2];
    ConversationGroups lines;    // 0x43340a state+0x131
    ConversationGroups answers;  // 0x43340a state+0x163
    std::vector<std::string> clips;  // .ccf
    std::vector<std::string> texts;  // .byz
    // FUN_004119e3 stops after the declared structures; the shipped files can
    // carry further bytes (IMHMR1A: 314). They are retained as a count instead
    // of being treated as instructions.
    std::size_t trailing_bytes = 0;
};

std::vector<ConversationRecord> parse_conversation_records(
    const std::vector<std::uint8_t>& file);

// FUN_004123f9: one null-terminated line per record, CR/LF stripped. The
// original caps the array at 0x2ee records; more is rejected here.
std::vector<std::string> parse_conversation_lines(const std::vector<std::uint8_t>& file,
                                                  const char* what);

// FUN_004119e3 + FUN_00412762 + FUN_00412aea. Strict: a truncated operand, an
// unknown argument type, a group slot beyond 49, or a declared non-empty group
// that is never read is an error. `clips`/`texts` are the paired .ccf/.byz
// records and may be empty when only the bytecode is parsed.
ConversationProgram parse_conversation_program(const std::vector<std::uint8_t>& ccb,
                                               const std::vector<std::string>& clips,
                                               const std::vector<std::string>& texts,
                                               std::string name);

// A line (0x424780): the BYZ record and its paired CCF voice clip. `null`
// (case-insensitive) means "no line and no media"; "#" means no visible text
// but the media clip still plays.
struct ConversationLine {
    std::uint16_t index = 0xffff;
    std::string text;
    std::string clip;
    std::string speaker;  // three characters after the clip's first three
    bool show_text = false;
    bool media_allowed = false;
};

struct ConversationAnswer {
    std::size_t choice = 0;      // index into the presented list
    std::uint16_t line = 0xffff; // BYZ/CCF index
    std::string text;
    std::string clip;
    bool show_text = false;
    bool media_allowed = false;
    // Stable pointer into the parsed program; choose() uses it to mark exactly
    // the selected entry (a line index can occur twice in one group).
    ConversationEntry* entry = nullptr;
};

enum class ConversationEventKind { line, answers, finished, error };

struct ConversationEvent {
    ConversationEventKind kind = ConversationEventKind::error;
    ConversationLine line;
    std::vector<ConversationAnswer> answers;
    // finished: 0xffff is the normal 0x432d6e result (no label branch); any
    // other value is the object id the player clicked while the answer list was
    // up (0x436b01 -> 0x70 -> DAT_004654e0[object->+0x0c]).
    std::uint16_t result = 0xffff;
    std::string error;
    std::size_t group = 0;
};

// Runs one conversation program against a SceneState. The state machine follows
// 0x432d6e's outer loop, 0x43688b's line branch and 0x436b01's answer branch.
// The caller displays a line, then calls line_shown(); it presents an answer
// list, then calls choose(); a scene-object click ends the conversation with
// that object id (click_object()).
class ConversationSession {
public:
    ConversationSession(ConversationProgram program, SceneState& state);
    // `memory` is the persistent DAT_00482ad0 slice for this record. When it is
    // null the session behaves like the old one-shot runner.
    ConversationSession(ConversationProgram program, SceneState& state,
                        ConversationMemory* memory);

    void start();
    ConversationEvent next();
    void line_shown();
    void choose(std::size_t choice);
    void click_object(std::uint16_t object_id);
    // 0x436b01's ESC/right-click path: result 0xfffe, the script stops and the
    // scene stays. The state is snapshotted but remains "not done", so a later
    // 0x70 keeps the asked options and defaults.
    ConversationEvent cancel();

    const ConversationProgram& program() const { return program_; }
    std::size_t executed_instructions() const { return executed_; }
    std::uint16_t current_group() const { return group_; }
    bool done() const { return done_; }

private:
    enum class Phase { prelude, display, line_done, answers, answer_wait, finished, failed };

    struct RuntimeAnswerGroup {
        std::uint8_t flag = 0;
        std::optional<std::size_t> entry_group;  // groups2 index
        std::optional<std::size_t> entry_hop;
    };

    ConversationEvent fail(std::string message);
    ConversationEvent finish(std::uint16_t result);
    void run_instruction_list(const std::vector<ConversationInstruction>& list);
    void run_entry_list(const ConversationEntry& entry);
    ConversationArgument argument(const ConversationInstruction& instruction,
                                  std::size_t index) const;
    std::uint16_t literal(const ConversationInstruction& instruction, std::size_t index) const;
    std::uint16_t resolved(const ConversationInstruction& instruction, std::size_t index) const;
    std::optional<std::uint16_t> variable(std::uint16_t local) const;
    void store_variable(std::uint16_t local, std::uint16_t value);
    ConversationEntry* entry_at(ConversationGroups& groups, std::uint16_t index,
                                std::uint16_t hops, const char* what);
    ConversationEntry* entry_by_key(const ConversationEntryKey& key);
    bool entry_available(const ConversationEntry& entry) const;
    bool group_has_answers(std::size_t group) const;
    void apply_skips(std::uint8_t byte_slot, std::uint32_t pointer_slot);
    void advance_to_event();
    std::vector<ConversationAnswer> collect_answers(std::size_t group);
    ConversationLine make_line(std::uint16_t index) const;
    std::size_t current_line_entry(std::uint16_t group) const;
    std::uint16_t default_group(std::uint8_t value) const;
    void apply_memory();
    void save_memory(bool mark_done);

    ConversationProgram program_;
    SceneState& state_;
    Phase phase_ = Phase::prelude;
    std::uint16_t group_ = 0;
    std::uint16_t pending_group_ = 0;
    std::size_t pending_answer_ = 0;
    bool mode_ = false;          // state+0x0a: 0 line, 1 reply
    bool done_ = false;          // state+0x0e
    bool branch_ = false;        // DAT_0047a8d8
    bool cancelled_ = false;     // 0xfffe from the answer list
    bool failed_ = false;
    std::string error_;
    std::uint16_t result_ = 0xffff;
    std::size_t executed_ = 0;
    // state+0x0b / state+0x0d: the default reply/line group 0x43302b uses when
    // the same conversation state is entered again after it finished.
    std::uint8_t default_reply_ = 0xff;
    std::uint8_t default_line_ = 0xff;
    // The 50-slot arrays of 0x43340a's state. They start at the values
    // 0x410669 writes: 0xff for the byte arrays, null for the pointer arrays.
    std::vector<std::uint8_t> skip_a_ = std::vector<std::uint8_t>(50, 0xff);
    std::vector<std::uint8_t> skip_b_ = std::vector<std::uint8_t>(50, 0xff);
    std::vector<std::uint8_t> skip_c_ = std::vector<std::uint8_t>(50, 0xff);
    std::vector<std::optional<RuntimeAnswerGroup>> answer_a_;
    std::vector<std::optional<RuntimeAnswerGroup>> answer_b_;
    std::vector<std::optional<RuntimeAnswerGroup>> answer_c_;
    // state+0xf2 (50 entries) and state+0x490 (50 bytes) overrides.
    std::vector<std::optional<ConversationEntry*>> line_override_;
    std::vector<std::uint8_t> line_override_flag_ = std::vector<std::uint8_t>(50, 0);
    // state+0x2a/0x5c/0x8e: the 0x45/0x46 {byte, entry} slot tables. They are
    // part of the conversation state and persist across 0x70 calls.
    std::vector<std::optional<ConversationSlotValue>> slot_2a_;
    std::vector<std::optional<ConversationSlotValue>> slot_5c_;
    std::vector<std::optional<ConversationSlotValue>> slot_8e_;
    // state+0xc0: the answer entry chosen for each group.
    std::vector<std::optional<ConversationEntry*>> chosen_;
    std::vector<std::optional<ConversationEntryKey>> line_override_keys_ =
        std::vector<std::optional<ConversationEntryKey>>(50);
    const ConversationEntry* pending_line_ = nullptr;
    ConversationMemory* memory_ = nullptr;
};

}  // namespace byzantine
