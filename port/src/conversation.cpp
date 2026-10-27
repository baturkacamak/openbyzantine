#include "byzantine/conversation.hpp"
#include "byzantine/diagnostics.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace byzantine {
namespace {

constexpr std::size_t kGroupSlots = 50;  // 0x43340a's 0x32-entry arrays
constexpr std::size_t kMaxLines = 0x2ee; // FUN_004123f9's 750-record array

struct Reader {
    const std::vector<std::uint8_t>& bytes;
    std::size_t at = 0;

    std::uint8_t u8() {
        if (at >= bytes.size()) throw std::runtime_error("truncated conversation bytecode");
        return bytes[at++];
    }
    std::uint16_t u16() {
        const auto low = u8();
        const auto high = u8();
        return static_cast<std::uint16_t>(low | (high << 8));
    }
    void skip(std::size_t count) {
        if (at > bytes.size() || bytes.size() - at < count) {
            throw std::runtime_error("truncated conversation bytecode");
        }
        at += count;
    }
};

ConversationArgument parse_argument(Reader& reader) {
    ConversationArgument argument;
    argument.type = reader.u8();
    switch (argument.type) {
    case 0:
        // FUN_00412762 leaves both halves at 0x00ff.
        argument.value = 0xffff;
        argument.high = 0xffff;
        break;
    case 1:
    case 4:
        argument.value = reader.u8();
        argument.high = 0x00ff;
        break;
    case 2:
    case 8:
    case 16:
    case 24:
    case 64:
        argument.value = reader.u8();
        argument.high = reader.u8();
        break;
    case 32:
        // Two little-endian words: the first becomes the operand, the second is
        // read and discarded (FUN_00412762 0x4129da reuses the stack buffer).
        argument.value = reader.u8();
        argument.high = reader.u8();
        reader.skip(2);
        break;
    default: {
        std::ostringstream text;
        text << "unknown conversation argument type 0x" << std::hex
             << static_cast<int>(argument.type);
        throw std::runtime_error(text.str());
    }
    }
    return argument;
}

std::vector<ConversationInstruction> parse_instruction_list(Reader& reader) {
    const auto count = reader.u16();
    std::vector<ConversationInstruction> instructions;
    instructions.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        ConversationInstruction instruction;
        instruction.opcode = reader.u8();
        const auto arguments = cc_argument_count(instruction.opcode);
        for (std::uint8_t j = 0; j < arguments; ++j) {
            instruction.arguments.push_back(parse_argument(reader));
        }
        instructions.push_back(std::move(instruction));
    }
    return instructions;
}

// FUN_00412aea: one group record is a byte entry count followed by that many
// 0x14-byte entries. The declared count is the number of NON-EMPTY groups; an
// empty group is still present in the file and consumes one array slot.
ConversationGroups parse_groups(Reader& reader) {
    const auto wanted = reader.u8();
    ConversationGroups groups(kGroupSlots);
    std::size_t slot = 0;
    std::size_t nonempty = 0;
    while (nonempty < wanted) {
        if (slot >= kGroupSlots) {
            throw std::runtime_error("conversation group slot beyond 49");
        }
        const auto count = reader.u8();
        std::vector<ConversationEntry> entries;
        entries.reserve(count);
        for (std::uint8_t e = 0; e < count; ++e) {
            ConversationEntry entry;
            entry.value = reader.u16();
            entry.link = reader.u16();
            entry.instructions = parse_instruction_list(reader);
            const auto subs = reader.u8();
            entry.subs.reserve(subs);
            for (std::uint8_t s = 0; s < subs; ++s) {
                ConversationEntry sub;
                sub.value = reader.u16();
                sub.link = reader.u16();
                sub.instructions = parse_instruction_list(reader);
                entry.subs.push_back(std::move(sub));
            }
            entries.push_back(std::move(entry));
        }
        if (!entries.empty()) ++nonempty;
        groups[slot] = std::move(entries);
        ++slot;
    }
    return groups;
}

std::string lower(std::string text) {
    for (char& ch : text) {
        if (ch >= 'A' && ch <= 'Z') ch = static_cast<char>(ch - 'A' + 'a');
    }
    return text;
}

bool same_text(const std::string& text, const char* against) {
    return lower(text) == lower(against);
}

// 0x40ced0: the speaker id of a line, taken from clip characters 3..5. The
// original compares with __strcmpi, i.e. case-insensitively: the shipped CCF
// mixes cases for the same actor (`Iiaima05`/`ima`, `Nt1nur02`/`Nur`, `Coi`,
// `Prf`, `Boo`, ...), and a case-sensitive test silently removed the media from
// roughly half of the dialogue lines. After the 16-entry table (PTR_DAT_0045f4a0)
// two extra names are tested: "hnt" -> -2 and "pro" -> -1; anything else -> -3.
int speaker_id(const std::string& speaker) {
    static const char* const names[16] = {
        "aka", "nur", "sha", "esk", "meh", "car", "emr", "jer",
        "ima", "prf", "cur", "bar", "bas", "coi", "boo", "mer"};
    for (int index = 0; index < 16; ++index) {
        if (same_text(speaker, names[index])) return index;
    }
    if (same_text(speaker, "hnt")) return -2;
    if (same_text(speaker, "pro")) return -1;
    return -3;
}

}  // namespace

std::vector<ConversationRecord> parse_conversation_records(
    const std::vector<std::uint8_t>& file) {
    std::vector<ConversationRecord> records;
    std::size_t at = 0;
    const auto cstring = [&file, &at]() {
        std::string text;
        while (at < file.size() && file[at] != 0) {
            text.push_back(static_cast<char>(file[at++]));
        }
        if (at >= file.size()) {
            throw std::runtime_error("truncated conversation record name");
        }
        ++at;  // the NUL
        return text;
    };
    while (at < file.size()) {
        ConversationRecord record;
        record.name = cstring();
        record.file = cstring();
        if (at >= file.size()) {
            throw std::runtime_error("conversation record is missing its separator");
        }
        if (file[at] != 0xff) {
            throw std::runtime_error("conversation record separator is not 0xff");
        }
        ++at;
        if (record.name.empty() || record.file.empty()) {
            throw std::runtime_error("conversation record has an empty name");
        }
        records.push_back(std::move(record));
    }
    return records;
}

std::vector<std::string> parse_conversation_lines(const std::vector<std::uint8_t>& file,
                                                  const char* what) {
    std::vector<std::string> lines;
    std::string current;
    for (const auto byte : file) {
        if (byte == '\n') {
            if (!current.empty() && current.back() == '\r') current.pop_back();
            lines.push_back(current);
            current.clear();
        } else {
            current.push_back(static_cast<char>(byte));
        }
    }
    if (!current.empty()) {
        if (current.back() == '\r') current.pop_back();
        lines.push_back(current);
    }
    if (lines.size() > kMaxLines) {
        throw std::runtime_error(std::string(what) + " has more than 750 records");
    }
    return lines;
}

ConversationProgram parse_conversation_program(const std::vector<std::uint8_t>& ccb,
                                               const std::vector<std::string>& clips,
                                               const std::vector<std::string>& texts,
                                               std::string name) {
    Reader reader{ccb};
    ConversationProgram program;
    program.name = std::move(name);
    program.clips = clips;
    program.texts = texts;
    program.lists[0] = parse_instruction_list(reader);
    program.lists[1] = parse_instruction_list(reader);
    program.lines = parse_groups(reader);
    program.answers = parse_groups(reader);
    program.trailing_bytes = ccb.size() - reader.at;
    return program;
}

// ---------------------------------------------------------------------------
// Runtime (0x432d6e / 0x43688b / 0x436b01)
// ---------------------------------------------------------------------------

ConversationSession::ConversationSession(ConversationProgram program, SceneState& state)
    : ConversationSession(std::move(program), state, nullptr) {}

ConversationSession::ConversationSession(ConversationProgram program, SceneState& state,
                                         ConversationMemory* memory)
    : program_(std::move(program)), state_(state), memory_(memory) {
    answer_a_.resize(kGroupSlots);
    answer_b_.resize(kGroupSlots);
    answer_c_.resize(kGroupSlots);
    line_override_.resize(kGroupSlots);
    slot_2a_.resize(kGroupSlots);
    slot_5c_.resize(kGroupSlots);
    slot_8e_.resize(kGroupSlots);
    chosen_.resize(kGroupSlots);
}

std::uint16_t ConversationSession::default_group(std::uint8_t value) const {
    // 0x43302b: 0xff means the first non-NULL line group, otherwise the byte.
    if (value != 0xff) return value;
    for (std::size_t index = 0; index < program_.lines.size(); ++index) {
        if (!program_.lines[index].empty()) {
            return static_cast<std::uint16_t>(index);
        }
    }
    return 0;
}

ConversationEntry* ConversationSession::entry_by_key(const ConversationEntryKey& key) {
    auto& groups = key.list != 0 ? program_.answers : program_.lines;
    if (key.group >= groups.size() || key.hop >= groups[key.group].size()) {
        return nullptr;
    }
    auto* entry = &groups[key.group][key.hop];
    if (key.sub != 0xffff) {
        if (key.sub >= entry->subs.size()) return nullptr;
        entry = &entry->subs[key.sub];
    }
    return entry;
}

void ConversationSession::apply_memory() {
    if (memory_ == nullptr) return;
    const auto apply = [this](ConversationGroups& groups, std::uint8_t list) {
        for (std::size_t group = 0; group < groups.size(); ++group) {
            for (std::size_t hop = 0; hop < groups[group].size(); ++hop) {
                auto& entry = groups[group][hop];
                const auto apply_key = [this](ConversationEntry& target,
                                              const ConversationEntryKey& key) {
                    target.enabled = memory_->disabled.count(key) == 0;
                    target.unspeak = memory_->unspeak.count(key) != 0;
                };
                apply_key(entry, ConversationEntryKey{
                                      list, static_cast<std::uint16_t>(group),
                                      static_cast<std::uint16_t>(hop), 0xffff});
                for (std::size_t sub = 0; sub < entry.subs.size(); ++sub) {
                    apply_key(entry.subs[sub],
                              ConversationEntryKey{list,
                                                   static_cast<std::uint16_t>(group),
                                                   static_cast<std::uint16_t>(hop),
                                                   static_cast<std::uint16_t>(sub)});
                }
            }
        }
    };
    apply(program_.lines, 0);
    apply(program_.answers, 1);
    skip_a_ = memory_->skip_a;
    skip_b_ = memory_->skip_b;
    skip_c_ = memory_->skip_c;
    default_reply_ = memory_->default_reply;
    default_line_ = memory_->default_line;
    line_override_flag_ = memory_->line_override_flag;
    for (std::size_t index = 0; index < line_override_.size(); ++index) {
        line_override_[index].reset();
        line_override_keys_[index].reset();
        if (memory_->line_override[index]) {
            if (auto* entry = entry_by_key(*memory_->line_override[index])) {
                line_override_[index] = entry;
                line_override_keys_[index] = *memory_->line_override[index];
            }
        }
    }
    slot_2a_ = memory_->slot_2a;
    slot_5c_ = memory_->slot_5c;
    slot_8e_ = memory_->slot_8e;
}

void ConversationSession::save_memory(bool mark_done) {
    if (memory_ == nullptr) return;
    memory_->unspeak.clear();
    memory_->disabled.clear();
    const auto save = [this](const ConversationGroups& groups, std::uint8_t list) {
        for (std::size_t group = 0; group < groups.size(); ++group) {
            for (std::size_t hop = 0; hop < groups[group].size(); ++hop) {
                const auto& entry = groups[group][hop];
                const auto save_key = [this](const ConversationEntry& target,
                                             const ConversationEntryKey& key) {
                    if (!target.enabled) memory_->disabled.insert(key);
                    if (target.unspeak) memory_->unspeak.insert(key);
                };
                save_key(entry, ConversationEntryKey{
                                    list, static_cast<std::uint16_t>(group),
                                    static_cast<std::uint16_t>(hop), 0xffff});
                for (std::size_t sub = 0; sub < entry.subs.size(); ++sub) {
                    save_key(entry.subs[sub],
                             ConversationEntryKey{list,
                                                  static_cast<std::uint16_t>(group),
                                                  static_cast<std::uint16_t>(hop),
                                                  static_cast<std::uint16_t>(sub)});
                }
            }
        }
    };
    save(program_.lines, 0);
    save(program_.answers, 1);
    memory_->skip_a = skip_a_;
    memory_->skip_b = skip_b_;
    memory_->skip_c = skip_c_;
    memory_->default_reply = default_reply_;
    memory_->default_line = default_line_;
    memory_->line_override = line_override_keys_;
    memory_->line_override_flag = line_override_flag_;
    memory_->slot_2a = slot_2a_;
    memory_->slot_5c = slot_5c_;
    memory_->slot_8e = slot_8e_;
    if (mark_done) memory_->done = done_;
}

void ConversationSession::start() {
    work_.clear();
    inline_line_.reset();
    apply_memory();
    phase_ = Phase::prelude;
    group_ = 0;
    mode_ = false;
    done_ = false;
    branch_ = false;
    failed_ = false;
    error_.clear();
    result_ = 0xffff;
    cancelled_ = false;
    executed_ = 0;
    pending_line_ = nullptr;
    chosen_.assign(kGroupSlots, std::nullopt);
    // 0x432d6e resets the per-call cursor, then uses the persistent default
    // groups when the previous run ended (state+0xe). The pending reply flag is
    // state+0xc; the shipped MR program does not set it but the state keeps it.
    if (memory_ != nullptr) {
        if (memory_->done) group_ = default_group(memory_->default_line);
        if (memory_->default_reply_pending) {
            group_ = default_group(memory_->default_reply);
            memory_->default_reply_pending = false;
        }
    }
}

ConversationEvent ConversationSession::fail(std::string message) {
    failed_ = true;
    phase_ = Phase::failed;
    error_ = std::move(message);
    ConversationEvent event;
    event.kind = ConversationEventKind::error;
    event.error = error_;
    return event;
}

ConversationEvent ConversationSession::finish(std::uint16_t result) {
    phase_ = Phase::finished;
    result_ = result;
    ConversationEvent event;
    event.kind = ConversationEventKind::finished;
    event.result = result;
    event.group = group_;
    if (cancelled_) return event;
    done_ = true;
    save_memory(true);
    return event;
}

ConversationEvent ConversationSession::cancel() {
    // 0x436b01: ESC/right-click sets local_8 = 0xfffe and DAT_0045f48c.
    // 0x42db22 case 0x70 then stops the script (local_8 = 1); it is not a
    // completed conversation, so state+0xe stays clear and the next 0x70 starts
    // at group 0 with the persisted entry flags.
    cancelled_ = true;
    result_ = 0xfffe;
    phase_ = Phase::finished;
    save_memory(false);
    ConversationEvent event;
    event.kind = ConversationEventKind::finished;
    event.result = 0xfffe;
    event.group = group_;
    return event;
}

ConversationArgument ConversationSession::argument(const ConversationInstruction& instruction,
                                                   std::size_t index) const {
    if (index >= instruction.arguments.size()) {
        throw std::runtime_error("conversation instruction is missing an operand");
    }
    return instruction.arguments[index];
}

std::uint16_t ConversationSession::literal(const ConversationInstruction& instruction,
                                           std::size_t index) const {
    return argument(instruction, index).word();  // FUN_00436652
}

std::optional<std::uint16_t> ConversationSession::variable(std::uint16_t local) const {
    return read_local_variable(state_, local);
}

void ConversationSession::store_variable(std::uint16_t local, std::uint16_t value) {
    write_local_variable(state_, local, value);
}

std::uint16_t ConversationSession::resolved(const ConversationInstruction& instruction,
                                            std::size_t index) const {
    const auto operand = argument(instruction, index);
    const auto word = operand.word();
    if (operand.type != 2) return word;
    const auto value = variable(word);
    if (!value) {
        throw std::runtime_error("conversation reads unknown scene local " +
                                 std::to_string(word));
    }
    return *value;
}

ConversationEntry* ConversationSession::entry_at(ConversationGroups& groups,
                                                 std::uint16_t index,
                                                 std::uint16_t hops,
                                                 const char* what) {
    if (index >= groups.size()) {
        throw std::runtime_error(std::string("conversation ") + what +
                                 " index outside the 50 group slots");
    }
    auto& entries = groups[index];
    if (entries.empty()) {
        throw std::runtime_error(std::string("conversation ") + what +
                                 " group " + std::to_string(index) + " is empty");
    }
    if (hops >= entries.size()) {
        throw std::runtime_error(std::string("conversation ") + what + " entry " +
                                 std::to_string(hops) + " outside group " +
                                 std::to_string(index));
    }
    return &entries[hops];
}

bool ConversationSession::entry_available(const ConversationEntry& entry) const {
    // FUN_00436b01: entry+0x10 must be 1; +0x11 hides it unless DAT_00483070 is
    // set (a debug/UI flag that is zero in the shipped game).
    return entry.enabled && !entry.unspeak;
}

bool ConversationSession::group_has_answers(std::size_t group) const {
    if (group >= program_.answers.size()) return false;
    for (const auto& entry : program_.answers[group]) {
        if (entry_available(entry)) return true;
    }
    if (group < answer_a_.size()) {
        const auto& runtime = answer_a_[group];
        if (runtime && runtime->entry_group && *runtime->entry_group < program_.answers.size()) {
            for (const auto& entry : program_.answers[*runtime->entry_group]) {
                if (entry_available(entry)) return true;
            }
        }
    }
    return false;
}

void ConversationSession::apply_skips(std::uint8_t byte_slot, std::uint32_t pointer_slot) {
    (void)pointer_slot;
    // 0x4330bf: state[byte_slot + group] is the alternate group when the
    // current group has no enabled answer; 0xff means "stay".
    if (byte_slot == 0) {
        if (group_ >= skip_a_.size() || skip_a_[group_] == 0xff) return;
        if (!group_has_answers(group_)) group_ = skip_a_[group_];
        return;
    }
    if (byte_slot == 1) {
        if (group_ >= skip_b_.size() || skip_b_[group_] == 0xff) return;
        if (!group_has_answers(group_)) group_ = skip_b_[group_];
        return;
    }
    if (group_ >= skip_c_.size() || skip_c_[group_] == 0xff) return;
    if (!group_has_answers(group_)) group_ = skip_c_[group_];
}

void ConversationSession::execute_instruction(const ConversationInstruction& instruction) {
        ++executed_;
        diagnostics::write("conversation_step", [&](auto& out) {
            out << "step=" << executed_ << " group=" << unsigned(group_)
                << " mode=" << mode_ << " opcode=" << unsigned(instruction.opcode);
            for (const auto& arg : instruction.arguments)
                out << " arg=" << unsigned(arg.type) << ':' << arg.value << ':' << arg.high;
        });
        switch (instruction.opcode) {
        case 0x7c:
        case 0x92:
        case 0x93:
        case 0x94:
            // 0x43340a -> 0x4096f9: in normal runtime all four VOICE
            // forms play literal operand 2 through the ordinary line player.
            // Operands 0/1 and the selector only affect the original debug UI.
            inline_line_ = make_line(literal(instruction, 2));
            break;
        case 0x3a:  // 0x43340a has no case: the list1 header is a no-op.
        case 0x3e:  // debug-only 0x40a7e5 call when DAT_00485c6c != 0
            break;
        case 0x32: {  // SAY: select a line group in line mode
            group_ = argument(instruction, 0).value;
            mode_ = false;
            branch_ = true;
            break;
        }
        case 0x33: {  // REPLY: select an answer group
            group_ = argument(instruction, 0).value;
            mode_ = true;
            branch_ = true;
            break;
        }
        case 0x36: {  // enable an answer entry
            const auto index = argument(instruction, 0);
            ConversationGroups& groups = (index.type & 0x10) ? program_.lines : program_.answers;
            entry_at(groups, index.value, index.high, "enable")->enabled = true;
            break;
        }
        case 0x37: {  // disable an answer entry
            const auto index = argument(instruction, 0);
            ConversationGroups& groups = (index.type & 0x10) ? program_.lines : program_.answers;
            entry_at(groups, index.value, index.high, "disable")->enabled = false;
            break;
        }
        case 0x38: {  // select a line entry for the group
            const auto index = argument(instruction, 0);
            const bool answers = (index.type & 0x10) != 0;
            ConversationGroups& groups = answers ? program_.lines : program_.answers;
            auto* entry = entry_at(groups, index.value, index.high, "select");
            if (index.value >= line_override_.size()) {
                throw std::runtime_error("conversation select index outside 50 slots");
            }
            line_override_[index.value] = entry;
            line_override_keys_[index.value] = ConversationEntryKey{
                static_cast<std::uint8_t>(answers ? 0 : 1), index.value, index.high,
                0xffff};
            line_override_flag_[index.value] = static_cast<std::uint8_t>(index.high);
            break;
        }
        case 0x3b:  // default reply group for a later re-entry
            default_reply_ = argument(instruction, 0).value;
            break;
        case 0x3c: {  // skip table used by 0x4330bf
            const auto index = argument(instruction, 0);
            const auto value = argument(instruction, 1);
            if (index.value >= skip_a_.size()) {
                throw std::runtime_error("conversation skip index outside 50 slots");
            }
            skip_a_[index.value] = static_cast<std::uint8_t>(value.value);
            break;
        }
        case 0x40:  // default line group for a later re-entry
            default_line_ = argument(instruction, 0).value;
            break;
        case 0x41: {  // skip tables used by 0x436b01
            const auto index = argument(instruction, 0);
            const auto value = argument(instruction, 1);
            const auto selector = argument(instruction, 2);
            if (index.value >= skip_b_.size()) {
                throw std::runtime_error("conversation cursor-skip index outside 50 slots");
            }
            if (selector.type == 2) {
                skip_b_[index.value] = static_cast<std::uint8_t>(value.value);
            } else {
                skip_c_[index.value] = static_cast<std::uint8_t>(value.value);
            }
            break;
        }
        case 0x39:
            done_ = true;
            break;
        case 0x42:    // if var0 == value: enable an answer entry
        case 0x43: {  // if var0 == value: disable an answer entry
            const auto left = resolved(instruction, 0);
            const auto right = literal(instruction, 1);
            if (left == right) {
                const auto index = argument(instruction, 2);
                entry_at(program_.answers, index.value, index.high, "condition")->enabled =
                    instruction.opcode == 0x42;
            }
            break;
        }
        case 0x44: {  // if var0 == value: select a line entry
            const auto left = resolved(instruction, 0);
            const auto right = literal(instruction, 1);
            if (left != right) break;
            const auto index = argument(instruction, 2);
            auto* entry = entry_at(program_.lines, index.value, index.high, "IFSET");
            if (index.value >= line_override_.size()) {
                throw std::runtime_error("conversation IFSET index outside 50 slots");
            }
            line_override_[index.value] = entry;
            line_override_keys_[index.value] = ConversationEntryKey{
                0, index.value, index.high, 0xffff};
            line_override_flag_[index.value] = static_cast<std::uint8_t>(index.high);
            break;
        }
        case 0x45:    // store {operand1 byte, resolved entry} in state+0x2a
        case 0x46: {  // store it in state+0x5c (operand3 == 2) or state+0x8e
            // 0x43340a: operand 0 is the 50-slot index, operand 1 the byte, and
            // operand 2 is an entry reference resolved by FUN_004367eb: its
            // type bit 0x10 selects the line list (state+0x4c4), otherwise the
            // answer list (state+0x58c); word 0 is the group and word 1 the
            // +8 hop. The game writes these 8-byte records but no decompiled
            // reader consumes them; the store is still part of the state.
            const auto slot = argument(instruction, 0);
            const auto byte = argument(instruction, 1);
            const auto index = argument(instruction, 2);
            if (slot.value >= kGroupSlots) {
                throw std::runtime_error("conversation 0x45/0x46 slot outside 50");
            }
            const bool line_list = (index.type & 0x10) != 0;
            auto& groups = line_list ? program_.lines : program_.answers;
            entry_at(groups, index.value, index.high, "0x45/0x46");
            const ConversationSlotValue value{
                static_cast<std::uint8_t>(byte.value),
                ConversationEntryKey{static_cast<std::uint8_t>(line_list ? 0 : 1),
                                     index.value, index.high, 0xffff}};
            if (instruction.opcode == 0x45) {
                slot_2a_[slot.value] = value;
            } else if (argument(instruction, 3).value == 2) {
                slot_5c_[slot.value] = value;
            } else {
                slot_8e_[slot.value] = value;
            }
            break;
        }
        case 0x49: {  // if var0 == value: switch line group (not a CC label)
            const auto left = resolved(instruction, 0);
            const auto right = literal(instruction, 1);
            if (left == right) {
                group_ = argument(instruction, 2).value;
                mode_ = false;
                branch_ = true;
            }
            break;
        }
        case 0x4a:    // enable/disable an answer entry and add its redraw record
        case 0x4b: {
            const auto index = argument(instruction, 0);
            auto& groups = (index.type & 0x10) ? program_.lines : program_.answers;
            entry_at(groups, index.value, index.high, "IFTO")->enabled =
                instruction.opcode == 0x4b;
            break;
        }
        case 0x4c:    // conditional TOFF/TON; 0x4367eb resolves operand 2
        case 0x4d: {
            if (resolved(instruction, 0) == literal(instruction, 1)) {
                const auto index = argument(instruction, 2);
                auto& groups = (index.type & 0x10) ? program_.lines : program_.answers;
                entry_at(groups, index.value, index.high, "IFTO")->enabled =
                    instruction.opcode == 0x4d;
            }
            break;
        }
        case 0x4f: {  // SPEAK: hide this answer even when enabled
            const auto index = argument(instruction, 0);
            entry_at(program_.answers, index.value, index.high, "SPEAK")->unspeak = true;
            break;
        }
        case 0x3f: {  // UNSPEAK
            const auto index = argument(instruction, 0);
            entry_at(program_.answers, index.value, index.high, "UNSPEAK")->unspeak = false;
            break;
        }
        case 0x67: {  // variable = literal/variable
            const auto operand = argument(instruction, 0);
            const auto value = (operand.type == 2) ? resolved(instruction, 0) : operand.word();
            store_variable(literal(instruction, 1), value);
            break;
        }
        case 0x74: {  // variable += literal/variable
            const auto operand = argument(instruction, 0);
            const auto value = (operand.type == 2) ? resolved(instruction, 0) : operand.word();
            const auto local = literal(instruction, 1);
            const auto current = variable(local);
            if (!current) {
                throw std::runtime_error("conversation adds to unknown scene local " +
                                         std::to_string(local));
            }
            store_variable(local, static_cast<std::uint16_t>(*current + value));
            break;
        }
        case 0x4e: {  // count the available answers of a group into a variable
            const auto group = argument(instruction, 0).value;
            if (group >= program_.answers.size()) {
                throw std::runtime_error("conversation count index outside 50 slots");
            }
            std::uint16_t count = 0;
            for (const auto& entry : program_.answers[group]) {
                if (entry_available(entry)) ++count;
            }
            if (group < answer_a_.size() && answer_a_[group] &&
                answer_a_[group]->entry_group) {
                const auto target = *answer_a_[group]->entry_group;
                for (const auto& entry : program_.answers[target]) {
                    if (entry_available(entry)) ++count;
                }
            }
            store_variable(literal(instruction, 1), count);
            break;
        }
        default: {
            std::ostringstream text;
            text << "unsupported conversation opcode 0x" << std::hex
                 << static_cast<int>(instruction.opcode) << std::dec;
            throw std::runtime_error(text.str());
        }
        }
        // When an instruction sets (or clears) an answer's visibility the
        // affected group is the one in its operand, not necessarily 0x43340a's
        // `param_2` current group: 0x43340a only dispatches the command.
}

void ConversationSession::run_instruction_list(
    const std::vector<ConversationInstruction>& list) {
    work_.push_back(Work{&list, 0, {}});
}

void ConversationSession::run_pending_work() {
    while (!work_.empty() && !inline_line_ && !failed_) {
        auto& task = work_.front();
        if (task.list) {
            if (branch_ || task.cursor >= task.list->size()) {
                work_.pop_front();
                continue;
            }
            const auto& instruction = task.list->at(task.cursor++);
            execute_instruction(instruction);
        } else {
            auto after = std::move(task.after);
            work_.pop_front();
            after();
        }
    }
}

void ConversationSession::run_entry_list(const ConversationEntry& entry) {
    for (const auto& sub : entry.subs) {
        work_.push_back(Work{nullptr, 0, [this] { branch_ = false; }});
        run_instruction_list(sub.instructions);
    }
    work_.push_back(Work{nullptr, 0, [this] { branch_ = false; }});
    run_instruction_list(entry.instructions);
}

ConversationLine ConversationSession::make_line(std::uint16_t index) const {
    ConversationLine line;
    line.index = index;
    if (index >= program_.texts.size()) {
        throw std::runtime_error("conversation line " + std::to_string(index) +
                                 " outside the " + std::to_string(program_.texts.size()) +
                                 " BYZ records");
    }
    if (index >= program_.clips.size()) {
        throw std::runtime_error("conversation line " + std::to_string(index) +
                                 " outside the " + std::to_string(program_.clips.size()) +
                                 " CCF clips");
    }
    line.text = program_.texts[index];
    line.clip = program_.clips[index];
    if (line.clip.size() >= 6) line.speaker = line.clip.substr(3, 3);
    line.show_text = !same_text(line.text, "null") && line.text != "#";
    // 0x424780: the clip plays unless the speaker is "pro" (-1, the player's own
    // lines) and the text is not "null" (case-insensitive). An "hnt" (-2) or
    // unknown (-3) speaker still reaches 0x40d2a1, which logs and returns 0 when
    // no AVI exists for the clip, so only "pro"/"null" may suppress the attempt.
    line.media_allowed =
        speaker_id(line.speaker) != -1 && !same_text(line.text, "null");
    return line;
}

std::vector<ConversationAnswer> ConversationSession::collect_answers(std::size_t group) {
    std::vector<ConversationAnswer> answers;
    const auto add = [this, &answers](ConversationEntry& entry) {
        if (!entry_available(entry)) return;
        ConversationAnswer answer;
        answer.choice = answers.size();
        answer.line = entry.value;
        const auto line = make_line(entry.value);
        answer.text = line.text;
        answer.clip = line.clip;
        answer.show_text = line.show_text;
        answer.media_allowed = line.media_allowed;
        answer.entry = &entry;
        answers.push_back(std::move(answer));
    };
    if (group < program_.answers.size()) {
        for (auto& entry : program_.answers[group]) add(entry);
    }
    if (group < answer_a_.size() && answer_a_[group] &&
        answer_a_[group]->entry_group) {
        const auto target = *answer_a_[group]->entry_group;
        if (target < program_.answers.size()) {
            for (auto& entry : program_.answers[target]) add(entry);
        }
    }
    return answers;
}

void ConversationSession::advance_to_event() {
    for (;;) {
        run_pending_work();
        if (inline_line_) return;
        switch (phase_) {
        case Phase::prelude: {
            // 0x432d6e runs list1 and list2 before the line/answer loop.
            run_instruction_list(program_.lists[0]);
            work_.push_back(Work{nullptr, 0, [this] {
                if (!branch_) run_instruction_list(program_.lists[1]);
                work_.push_back(Work{nullptr, 0, [this] { branch_ = false; }});
            }});
            phase_ = Phase::display;
            break;
        }
        case Phase::display: {
            if (done_) {
                phase_ = Phase::finished;
                break;
            }
            apply_skips(0, 0x2a);
            if (group_ >= program_.lines.size()) {
                done_ = true;
                phase_ = Phase::finished;
                break;
            }
            const ConversationEntry* line = nullptr;
            if (group_ < line_override_.size() && line_override_[group_]) {
                line = *line_override_[group_];
            } else if (!program_.lines[group_].empty()) {
                line = &program_.lines[group_].front();
            }
            if (line == nullptr) {
                // 0x43688b: a NULL group slot ends the conversation.
                done_ = true;
                phase_ = Phase::finished;
                break;
            }
            pending_line_ = line;
            phase_ = Phase::line_done;
            return;
        }
        case Phase::line_done: {
            // 0x43688b displays the line first, then runs its sub-entry lists
            // and its own list.
            if (pending_line_ == nullptr) {
                phase_ = Phase::display;
                break;
            }
            mode_ = true;
            run_entry_list(*pending_line_);
            work_.push_back(Work{nullptr, 0, [this] {
                pending_line_ = nullptr;
                phase_ = done_ ? Phase::finished : (mode_ ? Phase::answers : Phase::display);
            }});
            break;
        }
        case Phase::answers: {
            if (done_) {
                phase_ = Phase::finished;
                break;
            }
            apply_skips(1, 0x5c);
            std::size_t guard = 0;
            while (!group_has_answers(group_) && guard++ < kGroupSlots) {
                const auto before = group_;
                apply_skips(2, 0x8e);
                if (group_ == before) break;
            }
            const auto answers = collect_answers(group_);
            if (answers.empty()) {
                // 0x436b01 logs through 0x40ac43/0x40ac38 and sets done.
                done_ = true;
                phase_ = Phase::finished;
                break;
            }
            phase_ = Phase::answer_wait;
            return;
        }
        case Phase::answer_wait:
            // next() is only called after choose()/click_object(); a caller
            // that ignores the answer event is a programming error.
            throw std::runtime_error("conversation is waiting for an answer");
        case Phase::finished:
            return;
        case Phase::failed:
            return;
        }
    }
}

ConversationEvent ConversationSession::next() {
    try {
        if (phase_ == Phase::failed) {
            ConversationEvent event;
            event.kind = ConversationEventKind::error;
            event.error = error_;
            return event;
        }
        advance_to_event();
        if (inline_line_) {
            ConversationEvent event;
            event.kind = ConversationEventKind::line;
            event.line = *inline_line_;
            event.group = group_;
            return event;
        }
        if (phase_ == Phase::line_done) {
            ConversationEvent event;
            event.kind = ConversationEventKind::line;
            event.line = make_line(pending_line_->value);
            event.group = group_;
            return event;
        }
        if (phase_ == Phase::answer_wait) {
            ConversationEvent event;
            event.kind = ConversationEventKind::answers;
            event.answers = collect_answers(group_);
            event.group = group_;
            return event;
        }
        return finish(result_);
    } catch (const std::exception& error) {
        return fail(error.what());
    }
}

void ConversationSession::line_shown() {
    if (inline_line_) {
        inline_line_.reset();
        return;
    }
    if (phase_ != Phase::line_done) {
        throw std::runtime_error("conversation line completion without a displayed line");
    }
    // advance_to_event() performs the line's instruction list.
}

void ConversationSession::choose(std::size_t choice) {
    if (phase_ != Phase::answer_wait) {
        throw std::runtime_error("conversation answer chosen without an answer list");
    }
    const auto answers = collect_answers(group_);
    if (choice >= answers.size()) {
        throw std::runtime_error("conversation answer index outside the list");
    }
    // 0x436b01: mark the selected entry as spoken, remember it, run its list,
    // and advance the group only when the list did not branch.
    auto* selected = answers[choice].entry;
    if (selected == nullptr) {
        throw std::runtime_error("conversation answer entry disappeared");
    }
    selected->unspeak = true;
    chosen_[group_] = selected;
    branch_ = false;
    run_instruction_list(selected->instructions);
    work_.push_back(Work{nullptr, 0, [this] {
        if (done_) {
            phase_ = Phase::finished;
            return;
        }
        if (!branch_) group_ = static_cast<std::uint16_t>(group_ + 1);
        phase_ = mode_ ? Phase::answers : Phase::display;
    }});
    run_pending_work();
}

void ConversationSession::click_object(std::uint16_t object_id) {
    if (phase_ != Phase::answer_wait) {
        throw std::runtime_error("conversation object click without an answer list");
    }
    // 0x436b01's object-click path returns the object id and sets state+0xe.
    result_ = object_id;
    done_ = true;
    phase_ = Phase::finished;
}

}  // namespace byzantine
