#pragma once

#include "byzantine/anims.hpp"
#include "byzantine/cb.hpp"
#include "byzantine/compose.hpp"
#include "byzantine/records.hpp"
#include "byzantine/save.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace byzantine {

struct CcArgument {
    std::uint8_t type = 0;
    std::vector<std::uint8_t> encoded_value;
};

struct CcInstruction {
    std::uint8_t opcode = 0;
    std::vector<CcArgument> arguments;
};

struct SceneHotspot {
    std::size_t instruction = 0;
    std::uint16_t slot = 0;
    std::int16_t x = 0;
    std::int16_t y = 0;
    std::int16_t width = 0;
    std::int16_t height = 0;
    std::uint16_t action = 0;
    std::uint16_t mode = 0;
};

struct SceneBackground {
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    BackgroundEntry entry;
};

struct ScenePanorama {
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    BackgroundEntry entry;
    std::string jpeg_path;
};

struct CcTransfer {
    std::size_t instruction = 0;
    std::uint8_t opcode = 0;
    std::uint16_t label = 0;
    std::size_t target_instruction = 0;
};

struct SceneTextRequest {
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    std::uint16_t mode = 0;
};

struct CcControlFlow {
    std::map<std::uint16_t, std::size_t> labels;
    std::vector<CcTransfer> transfers;
    std::size_t duplicate_labels = 0;
    std::size_t returns = 0;
};

// One 8-byte entry of a DAT_004649e8 record's +0x34 extra list.
struct SceneNodeExtra {
    std::uint8_t control = 0;
    std::uint32_t first = 0;
    std::uint32_t second = 0;
};

// One 0x40-byte record of DAT_004649e8. The original has ONE table for actors
// and animation nodes: 0x64 (`case 100:`), 0x84, 0x85, 0x86, 0x87..0x8b, 0x91
// and 0x95 all reach it through FUN_00431b0f(id, opcode), and the save block 17
// (0x426b9d) restores the same slots. Field names stay offset-based where the
// meaning is not established. The initial values are the ones 0x40ffee writes
// after memset(0): y=0x14, kind=0, flags=0x20, +0x04=10, +0x2c/+0x30=-1. The
// animation loader (0x410078/0x41cb13) then adds the first frame header's
// offsets to x/y and fills the frame list; that asset data is NOT modelled
// here, nor the frame count at +0x14.
struct SceneRecordState {
    std::int16_t x = 0;          // +0x00
    std::int16_t y = 0x14;       // +0x02
    std::int16_t field04 = 10;   // +0x04, written by 0x64 (operand 2) and 0x86
    std::int16_t field06 = 0;    // +0x06, cleared by 0x64
    std::uint8_t kind = 0;       // +0x08, written only by 0x95
    std::uint8_t flags = 0x20;   // +0x09, kind-bit set by 0x64/0x85/0x86
    // +0x14: the frame count 0x40ffee stores from 0x410078's header and the
    // save restores. 0 means "not known" in the port (no asset, no save).
    std::uint16_t frame_count = 0;
    // +0x28 low byte: the animation file's 2-byte signature (0x0000/0x0001).
    std::uint16_t anims_signature = 0;
    // Per-frame geometry from the asset (0x410078 fills node +0x0c/+0x10 and
    // the x/y pair). Empty when the port has no asset file; `frames_known`
    // distinguishes "no frames" from "geometry not loaded".
    std::vector<AnimsFrame> frames;
    bool frames_known = false;
    // +0x1c: the current frame node as a 0-based index (0x40cf73 walks the list
    // from the head `count` times). nullopt is the NULL node.
    std::optional<std::uint16_t> current_frame;
    // +0x3c trail list: pairs of {x, y}, appended by 0x84.
    std::vector<std::pair<std::int16_t, std::int16_t>> trail;
    // +0x18/+0x1c frame-list head/current. 0x64 restarts the list and 0x91
    // advances it; the node count comes from the loaded asset, so these are
    // recorded as requests instead of a fabricated frame index.
    std::size_t frame_restart_requests = 0;
    std::size_t frame_advance_requests = 0;
    // Save block 17 only: the presence marker byte (1 = 0x40fe70 on load).
    std::uint8_t marker = 0;
    std::uint32_t frame_index_1c = 0;  // +0x1c, 0xffff = none (0x40cf73)
    std::uint32_t frame_index_20 = 0;  // +0x20, 0xffff = none (0x426ed0)
    std::uint32_t field_2c = 0;        // +0x2c
    std::uint32_t field_30 = 0;        // +0x30
    // +0x34 extra list (save block 17; 0x42db22 0x8a appends to +0x2c).
    std::vector<SceneNodeExtra> extra;
};

// One DAT_00486220 entry defined by opcode 0x65. Fields keep their recovered
// offsets; +0x1c..+0x38 are constant writes and are not duplicated here.
struct SceneSoundState {
    bool defined = false;
    std::uint16_t enabled = 0;   // +0x04
    std::uint16_t field08 = 0;   // +0x08
    std::uint16_t field0c = 0;   // +0x0c
    std::uint16_t field10 = 0;   // +0x10
    std::uint16_t field14 = 0;   // +0x14
    std::uint16_t field18 = 0;   // +0x18
    std::uint16_t field1c = 0;   // +0x1c, 0xbf/0xc0/0xc2 audio parameter
    std::uint16_t field20 = 0;   // +0x20, 0xc0/0xc2 audio parameter
    std::uint16_t field24 = 0;   // +0x24, 0xc0/0xc2 audio parameter
    std::uint16_t field28 = 0;   // +0x28, 0xbf audio parameter
    std::uint16_t field2c = 0;   // +0x2c, 0xbf audio parameter
    std::uint16_t field30 = 0xffff;  // +0x30, -1 from 0x65
    bool active = false;         // +0x34
};

// One 0xbd video request. FUN_0040d031 builds "avi/<name>.avi" (falling back to
// "video/<mode>" + ".avi") and plays it through FUN_0040d3af; no AVI decoder is
// reconstructed here, so the resolved request is retained instead.
struct SceneVideoRequest {
    std::size_t instruction = 0;
    std::uint16_t record = 0;    // CF table index from operand 0
    std::string name;            // CF record name
    std::string path;            // primary "avi/<name>.avi" attempt
    std::uint16_t mode = 0;      // raw operand 1
    std::optional<std::uint16_t> start;  // operand 2, only when its type is 0x40
    std::optional<std::uint16_t> end;    // operand 3, only when its type is 0x40
    // DAT_00460664, handed to the next 0xb9 as its panorama transition code:
    // modes 0/2/4 write 0x65/0x6a/0x67 unconditionally. Mode 3 compares the
    // alternate-mode flag DAT_00464878 (save/UI state) first, so it stays unset
    // instead of being guessed. Modes 1, 5 and the default never write it.
    std::optional<std::uint16_t> transition;
};

// One 0xb9 panorama/node load (0x432295 -> 0x40186e). The JPEG, the projection
// and the transition work are not rebuilt here: the resolved node, the staged
// horizontal position and the transition code are retained for the panorama
// layer, which already models 0x401ddf's angle/geometry separately.
struct ScenePanoramaLoad {
    std::size_t instruction = 0;
    std::uint16_t record = 0;                 // raw operand 0 (CO record id)
    std::string name;                         // CO record name, for example "ea00"
    std::string jpeg_path;                    // nodes/<first 2 chars>/<name>.jpg
    std::optional<std::uint16_t> horizontal;  // DAT_0045c064 staged by 0xba
    std::uint16_t force = 0;                  // resolved operand 1
    std::uint16_t vertical = 0;               // resolved operand 2
    std::uint16_t transition = 100;           // operand 3 mapping or DAT_00460664
    std::optional<std::uint16_t> flag_clear;  // raw operand 1 when force != 0
};

// One DAT_00486f90 dialogue record. 0x408662 allocates one 0x18-byte record per
// name of the global dialogue list (engine/SHOW.LCI, 126 in the shipped game)
// and writes +0x00 = 0, +0x04 = 0x20 + 2*id, +0x08 = id and +0x0c/+0x10/+0x14 = 0.
// Only +0x00 is read or written by the dialogue opcodes; +0x0c/+0x14 hold the
// inventor TGAs that 0x407254 loads, represented here as `inventory_loaded`.
struct SceneDialogueRecord {
    std::uint8_t state = 0;         // +0x00, the byte 0xb0/0xb1 compare
    bool inventory_loaded = false;  // +0x0c after a successful 0x407254
};

// The four engine tables that turn a script name id into a dialogue/channel id.
// 0x412f77 loads the per-scene CI (DAT_00485400) and CY (DAT_0047a2b0) record
// tables from the ALL archives; 0x438173 loads the global SHOW.LCI list
// (DAT_00482540, 0x15e max) and SHOW.LCY list (DAT_0047a8b0, 10 max).
// 0x408bb2/0x408c81 compare a CI/CY record name case-insensitively against the
// matching list and return its index, so list order IS the dialogue record id.
struct SceneDialogueTables {
    std::vector<NamedRecord> names;           // global SHOW.LCI (record ids)
    std::vector<NamedRecord> scene_names;     // per-scene CI (script operand 0)
    bool scene_names_known = false;
    std::vector<NamedRecord> channels;        // global SHOW.LCY (channels 0..9)
    std::vector<NamedRecord> scene_channels;  // per-scene CY (0xab operand 2)
    bool scene_channels_known = false;
};

// One DAT_00464300 object record restored from a save (block 18). The layout
// comes from 0x42db22 case 0x72; only the fields an opcode reads are modelled.
// +0x0a is deliberately absent: it has no recovered writer and 0x72 allocates
// the 0x18 bytes with an un-zeroed malloc, so the saved bytes there are stale
// heap data (SaveObjectRecord::uninitialized_0a).
struct SceneObjectState {
    std::int16_t x = 0;          // +0x00
    std::int16_t y = 0;          // +0x02
    std::int16_t width = 0;      // +0x04
    std::int16_t height = 0;     // +0x06
    std::uint8_t active = 0;     // +0x08
    std::uint8_t kind = 0;       // +0x09
    std::uint32_t label = 0;     // +0x0c, CC label id (CONVERSE 0x70)
    std::uint32_t name_id = 0;   // +0x10
    std::uint32_t argument = 0;  // +0x14, written by 0xcc
};

// One DAT_00464f60 media record restored from a save (block 22). +0x12 has no
// recovered writer and is intentionally not carried (stale heap data).
struct SceneMediaState {
    std::uint16_t value = 0;      // +0x00
    std::uint32_t frame = 0;      // +0x04
    std::uint32_t timestamp = 0;  // +0x08
    std::uint32_t media_id = 0;   // +0x0c
    std::uint8_t active = 0;      // +0x10, 0x6a sets / 0x6c clears
    std::uint8_t type = 0;        // +0x11
};

// One 0xcc object-argument write (DAT_00464300[id]+0x14) retained as an
// observable result, so a trace can show that the real branch ran.
struct SceneObjectWrite {
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    std::uint32_t value = 0;
};

// One 0xd1 audio start (0x428cbd). The nine-argument call is
// (id, mode, text, 0, -1, 0, 0xffffffff, 0, 0xffffffff); the port retains the
// request and the evaluated gate instead of opening an audio device.
struct SceneSoundStart {
    std::size_t instruction = 0;
    // 1-based executed-instruction index at which the request was queued
    // (trace.steps). A caller that runs the label with a step budget can replay
    // to exactly this point, play the sound, and continue from there.
    std::size_t step = 0;
    std::uint16_t id = 0;              // raw operand 0
    std::uint16_t mode = 0;            // raw operand 1; 0 = interactive dialogue
    std::optional<std::uint16_t> text;  // operand 2 when its type is 2
    bool record_present = false;       // DAT_00486220[id] != NULL
    // 0x428cbd's +0x18/gamepref[2] gate; nullopt when gameprefs were not supplied.
    std::optional<bool> allowed;
};

// One 0x6a media definition (DAT_00464f60).
struct SceneMediaDefinition {
    std::size_t instruction = 0;
    std::uint16_t id = 0;        // raw operand 0
    std::uint16_t value = 0;     // resolved operand 1
    std::uint32_t media_id = 0;  // raw operand 2
    std::uint8_t type = 0;       // operand 3 low byte when its type is 2, else 0
};

// One DAT_00486a10 dialogue chain restored from a save (block 24) or created
// by 0xc6. +0x00..+0x63 are 25 u32 dialogue-record ids (0xffffffff = empty) and
// +0x64 is the count used by 0xc6/0xc7.
struct SceneDialogueChain {
    SceneDialogueChain() { ids.fill(0xffffffffu); }
    std::uint32_t count = 0;              // +0x64
    std::array<std::uint32_t, 25> ids{};  // +0x00
};

// One 0x80 media-id write (DAT_00464f60[id]+0x0c).
struct SceneMediaIdWrite {
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    std::uint32_t media_id = 0;
};

// One 0xcb panorama rotation request (0x402285).
struct ScenePanoramaRotation {
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    std::uint16_t step = 10;
};

// One 0x86 node update (DAT_004649e8[id] +0x04 from the raw operand and +0x09
// through the 0x460418/0x460428 mask pair), retained as an observable result so
// a trace can show the branch ran on a save-restored node.
struct SceneNodeWrite {
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    std::int16_t value04 = 0;
    std::uint8_t flags = 0;
};

// One 0xcf release request (0x427a01). The original only creates a playback
// session when 0x428143 reports "not already playing", DAT_00485c90[id] holds a
// DirectSound object and the sound record's +0x14 is nonzero. The two device
// gates need the audio layer and are not evaluated; the request and the
// record fields it would copy are retained instead.
struct SceneSoundRelease {
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    bool record_present = false;   // DAT_00486220[id] != NULL
    std::uint16_t field14 = 0;     // +0x14, the last 0x427a01 gate
};

// One 0xd3 timed sound session. 0x428cbd is called as
// (id, 0, -1, p4, p5, p6, p7, p8, p9): mode 0 runs the dialogue window with no
// text, p4/p6/p8 are millisecond timeouts and p5/p7/p9 are the follow-up node
// records 0x431ec2(record, 0x66, 0) switches to when their timeout elapses.
struct SceneSoundTimeout {
    bool defined = false;            // operand 3/5 was type 0x40
    std::uint16_t milliseconds = 0;  // p6/p8; 0 disables the slot
    std::uint16_t follow_up = 0;     // p7/p9, raw operand 4/6
};

struct SceneSoundSession {
    std::size_t instruction = 0;
    std::size_t step = 0;   // see SceneSoundStart::step
    std::uint16_t id = 0;                            // raw operand 0
    std::array<SceneSoundTimeout, 3> timeouts{};      // p4/p5, p6/p7, p8/p9
    bool record_present = false;
    // 0x428cbd's +0x18/gamepref[2] gate, shared with 0xd1; nullopt when the
    // gameprefs were not supplied.
    std::optional<bool> allowed;
};

// One 0xc8 system/input sub-command, retained as an observable request.
struct SceneSystemCommand {
    std::size_t instruction = 0;
    std::uint16_t sub = 0;         // resolved operand 0
    std::optional<std::uint16_t> argument;  // resolved operand 1 for 0x1f/0x20
};

// One 0x70 CONVERSE pc replacement through DAT_004654e0[object->+0x0c].
struct SceneConverseJump {
    std::size_t instruction = 0;
    std::uint16_t id = 0;      // operand-0 object slot
    std::uint32_t label = 0;   // object +0x0c
    std::size_t target = 0;    // resolved CC instruction
};

// Mutable state retained while one scene script is running. Keys are the
// script-local variable and hotspot identifiers; the original EXE maps them to
// process-global storage, which is intentionally outside this first VM layer.
struct SceneState {
    // 0xba stages a horizontal pixel position (DAT_0045c064, clamped to
    // 0..0x9bf). 0x401ddf converts it to degrees and clears it back to -1 when
    // the panorama load reaches it.
    std::optional<std::uint16_t> pending_panorama_x;
    std::map<std::uint16_t, std::uint16_t> variables;
    std::map<std::uint16_t, bool> hotspot_enabled;
    // 0xb8 replaces this snapshot; 0xb7 restores and consumes its set bits.
    std::map<std::uint16_t, bool> suspended_hotspots;
    // DAT_004649e8: the single record table behind 0x64/0x84/0x85/0x86/0x8a/
    // 0x8b/0x91/0x95. Created by 0x40fd7b (scene start, CA type 2/3) or
    // 0x431b0f -> 0x40fe70 (CA type 0/1), and restored from save block 17.
    std::map<std::uint16_t, SceneRecordState> records;
    // Extracted `anims` tree (the parent of the `anims/` directory). 0x40fe70
    // builds "anims\<name[0]><name[1]>\<name>.rle"; when the library is
    // supplied and the file is absent, 0x40fe70 fails and the record stays
    // NULL, so the dispatcher skips the command. Shared because copying a
    // SceneState must not re-walk the tree.
    std::shared_ptr<const AnimsLibrary> anims_library;
    // DAT_0047dd70 (engine/<scene>ca, suffix "ca" at 0x45eaf0): the per-scene
    // animation asset table. Its +0x08 type byte decides whether a record can
    // exist at all: 0x40fd7b pre-loads 2/3, 0x431b0f loads 0/1 on demand, and
    // every other value (0xff in the shipped data) leaves the record NULL, so
    // the command body is skipped. Without the table the port cannot decide.
    std::vector<NamedRecord> actor_assets;
    bool actor_assets_known = false;
    // Sound/object instances created by 0x65 (DAT_00486220).
    std::map<std::uint16_t, SceneSoundState> sounds;
    // Objects behind DAT_00464f60 written by 0x6a/0x6c (media table).
    std::map<std::uint16_t, SceneMediaState> media;
    // Objects behind DAT_00464300 (hotspot definitions 0x72, 0xcc, 0x70).
    std::map<std::uint16_t, SceneObjectState> objects;
    // Per-object dialogue chains behind DAT_00486a10 (block 24, 0x68 bytes).
    std::map<std::uint16_t, SceneDialogueChain> dialogue_chains;
    // DAT_00460668: display/menu selector written by 0xc8 sub 0x32/0x33 and
    // restored from the save's mid-block dword.
    std::uint32_t field_460668 = 0;
    // DAT_00464878: the alternate-mode selector. 0x42c8a0 initialises it to -1
    // before the scene loop; 0x68 (0x42dad9) writes 0 or -1. 0xbd mode 3 reads
    // it and 0xcd's guard tests it; both are still request-only in this port,
    // which reports the write instead of guessing an effect.
    std::int32_t alternate_mode = -1;
    // DAT_0047e2f0: the carried 3000-word variable pool. 0x42c8a0 clears it once
    // before the scene loop and every scene reaches it through its own
    // 0x46ee00 row, so it is the state that crosses a 0x73 switch. `variables`
    // above is the current scene's 300-local projection of this pool; a label
    // only writes its own locals. Empty until a save or an imported pool
    // supplies it (0x42c8a0's clear leaves zeros anyway).
    std::vector<std::uint16_t> variable_pool;
    // Local operand numbers the executed script wrote, in order. 0x46ee00 can
    // map two locals of one scene onto the same global (a declared entry and the
    // identity tail), so an export must replay the writes instead of guessing
    // which local is current.
    std::vector<std::uint16_t> variable_writes;
    // DAT_0045f490: 0xc8 sub 0x3a sets it to 2. Not part of the save format.
    std::uint32_t field_45f490 = 0;
    // DAT_004859cd: set by 0x425ab4 after a successful load, written 1/0 by
    // 0xd4/0xd5. Its exact consumer meaning is not recovered.
    bool flag_4859cd = false;
    // DAT_0045d1d0: input/dialogue-active flag touched by 0xc8 sub 0x1e/0x20 and
    // 0x428cbd. BSS zero at startup; not part of the save format.
    bool dialogue_input_active = false;
    // DAT_00460670: the "examine" scene index, or -1 when the chapter has no
    // examine scene. nullopt means the engine scene list was not supplied.
    std::optional<std::int32_t> examine_scene_index;
    // Per-scene sound-name table (CS entry, 0x47a8e0). Record order is the id.
    // The original 0x412f77 loads it from engine/<scene>cs; a caller may supply
    // it, otherwise 0xc4's "endgame" name comparison is not evaluable.
    std::vector<NamedRecord> sound_records;
    // Distinguishes "no CS entry supplied" from a legitimately empty entry:
    // zero-length CS records exist (EXAMVB/EXAMVC/EXAMVH/HINTS/VTDISC).
    bool sound_table_known = false;
    // 0xc4 sets DAT_0046066c when the referenced sound record is named
    // "endgame"; retained as an explicit observable request.
    bool endgame_requested = false;
    // MSVC CRT _rand state at 0x462ef8 (0x44d070: state*0x343fd+0x269ec3,
    // result = (state & 0x7fff0000) >> 16). The original seeds it at
    // 0x415a4e with the literal 0xbeef and at 0x42a6aa with the local time,
    // so neither value may be assumed here. Unset means "seed unknown" and
    // opcode 0x7d reports unresolved_random instead of inventing a stream.
    std::optional<std::uint32_t> random_state;
    // Dialogue tables and per-id records behind DAT_00486f90 (see cc.cpp).
    SceneDialogueTables dialogue_tables;
    std::map<std::uint16_t, SceneDialogueRecord> dialogue_records;
    // Per-channel active-line stacks: DAT_00468cc0 holds the count and
    // DAT_00468d78 the entries with a 0x578-byte stride (0x15e per channel).
    std::array<std::vector<std::uint16_t>, 10> dialogue_channels;
    // DAT_0045d1c4 / DAT_0045d1c8 are zero-initialised BSS. 0x408adc restores
    // both from a save; that restore is outside this scene layer.
    std::uint16_t current_dialogue_channel = 0;
    std::optional<std::uint16_t> current_dialogue_record;
    // DAT_0045d1bc: message-window scroll offset that 0x407933 decrements.
    std::int32_t dialogue_scroll = 0;
    // DAT_0045d1c0: message-window visibility. BSS zero at startup; 0xb5 sets
    // it, 0xb4 clears it, and 0x408adc restores it from a save.
    bool dialogue_visible = false;
    // DAT_00482460, filled from engine/<chapter>.lcx by 0x438173: 0x73 indexes
    // it with its raw operand and writes the result to DAT_00482428. The
    // original array is 50 entries initialised to 0xffff, so an index between
    // the file count and 49 is a valid "no entry".
    std::vector<std::uint16_t> scene_switch_targets;
    bool scene_switch_table_known = false;
    // 0x407254 loads inventor/<name>.tga and inventor/hotitems/<h-name>.tga and
    // 0x4077b2 pushes the id on its channel stack only when both loads succeed.
    // The sets hold lowercase 8-character media names taken from the game CDs;
    // nothing is assumed while `inventory_assets_known` is false.
    std::set<std::string> inventor_assets;
    std::set<std::string> hotitem_assets;
    bool inventory_assets_known = false;
    // engine/<scene>cf records (DAT_00484330) named by 0xbd.
    std::vector<NamedRecord> video_records;
    bool video_table_known = false;
    // savegame/gamepref_%03d: seven dwords read by 0x41f753 and read back through
    // FUN_0041d440. 0xbd modes 2 and 3 only run when gamepref[5] / gamepref[4]
    // are nonzero, so the array is explicit state instead of an assumption.
    std::optional<std::array<std::uint32_t, 7>> gameprefs;
    // CO.ALL records behind DAT_00483d90. 0xb9 indexes them with its raw
    // operand 0 and uses the record name as the nodes/ JPEG stem.
    std::vector<BackgroundEntry> panorama_records;
    bool panorama_table_known = false;
    // DAT_00460664: panorama transition code that 0xbd (and 0x43340a/0x43c8d2)
    // hand to the next 0xb9, which consumes it.
    std::optional<std::uint16_t> pending_panorama_transition;
};

struct SceneTrace {
    std::optional<SceneBackground> background;
    std::vector<SceneHotspot> active_hotspots;
    std::size_t steps = 0;
    std::optional<std::uint8_t> unsupported_opcode;
    // A variable reference whose value predates this trace. The original engine
    // carries state between scenes; a standalone trace deliberately does not
    // invent that state.
    std::optional<std::uint16_t> unresolved_variable;
    // 0x7d RANDOM needs the CRT _rand stream (0x462ef8). When the caller did not
    // supply a known seed, the command is not executed and this flag is set.
    bool unresolved_random = false;
    // 0xc4 must compare the sound name with "endgame"; without the per-scene
    // sound table the comparison cannot be evaluated.
    std::optional<std::uint16_t> unresolved_sound_name;
    // 0x6c on an object that the trace never created: the original logs through
    // 0x40abb4 instead of writing. Retained as an observable request.
    std::size_t missing_media_requests = 0;
    // 0xcc/0x70 on an absent DAT_00464300 slot. The original has no NULL check
    // (0xcc dereferences the entry directly), so this is an explicit error
    // outcome; the port never allocates the object.
    std::optional<std::uint16_t> missing_object;
    // 0xcc wrote objects[id].argument (+0x14).
    std::vector<SceneObjectWrite> object_writes;
    // 0x70 replaced pc through the object's +0x0c CC label.
    std::vector<SceneConverseJump> converse_jumps;
    // 0x70 found the object but its +0x0c is not a label of this script.
    std::optional<std::uint32_t> unresolved_object_label;
    // 0x75's unsigned-underflow branch calls 0x40ac43/0x40ac38 (the engine's
    // error log) and skips the write; it does not stop execution. Counted so
    // the condition stays visible in a trace.
    std::size_t subtraction_underflows = 0;
    // The trace reached the 10,000-instruction safety bound. 0x70's modelled
    // conversation jump can return to the same label, so this is an explicit
    // outcome (the original waits on dialogue state this layer does not carry).
    bool step_limit_exceeded = false;
    // The caller asked for a step budget and the script is not finished yet:
    // the state is exactly "after `steps` instructions", so a caller can replay
    // the label with a larger budget and resume at the same point. Used by the
    // scene player to run a script step by step while a sound is playing.
    bool paused_by_budget = false;
    // Opcode 0x90 asks the original engine to close/reconcile scene resources.
    // The portable renderer is not present yet, so retain this observable
    // request rather than falsely treating it as a no-op.
    std::size_t resource_refresh_requests = 0;
    bool close_existing_resources = false;
    // 0xcd asks the original renderer to display DAT_00483640[id] with a mode.
    // The surface/text layer is not implemented, so the request is retained
    // instead of treated as a no-op. The original guard on DAT_00464878
    // (alternate mode) is not part of SceneState and is not modelled.
    std::vector<SceneTextRequest> text_requests;
    // 0xab/0xb0/0xb1/0xb6 resolve operand 0 through 0x408bb2 (and their channel
    // through 0x408c81). Without the per-scene CI/CY record table the id cannot
    // be resolved; the raw script operand is reported here instead of guessed.
    std::optional<std::uint16_t> unresolved_dialogue;
    std::optional<std::uint16_t> unresolved_dialogue_channel;
    // 0x407254's media load decides whether 0x4077b2 pushes the started id on
    // its channel stack; without the media name sets the trace stops here.
    bool unresolved_dialogue_inventory = false;
    // 0x407420 / 0x407b9c / 0x407ad5 redraw the message window. Counted as
    // requests; the renderer is not reconstructed.
    std::size_t dialogue_refresh_requests = 0;
    std::size_t dialogue_display_requests = 0;
    // 0x408bb2/0x408c81 log through 0x40ac43 and return id 0 when a name is
    // absent from the active list; counted so the fallback stays visible.
    std::size_t dialogue_name_fallbacks = 0;
    // 0xbd resolves its CF record name (DAT_00484330) and checks the gamepref
    // audio gate for modes 2/3. Neither can be rebuilt from the script alone.
    std::optional<std::uint16_t> unresolved_video_name;
    bool unresolved_video_setting = false;
    // 0xbf/0xc0/0xc2 rewrite audio parameters and call 0x427d1a, which pushes
    // them to the DirectSound objects. Audio is not reconstructed; counted.
    std::size_t sound_refresh_requests = 0;
    // 0xd1 starts playback through 0x428cbd; retained as a request.
    std::vector<SceneSoundStart> sound_starts;
    // 0x6a media definitions. timeGetTime() is not reproducible, so the saved
    // timestamp stays 0 and this counter records the unavailable field.
    std::vector<SceneMediaDefinition> media_definitions;
    std::size_t media_timestamps_unavailable = 0;
    // 0x80 rewrites an existing media record's +0x0c.
    std::vector<SceneMediaIdWrite> media_id_writes;
    // 0xcb asks 0x402285 to rotate/scroll the panorama.
    std::vector<ScenePanoramaRotation> panorama_rotations;
    // 0xc8 system/input sub-commands and the 0x36..0x3a status requests.
    std::vector<SceneSystemCommand> system_commands;
    std::optional<std::uint16_t> status_write_requested;
    // A record command whose DAT_004649e8 slot could not be resolved because
    // the per-scene DAT_0047dd70 (engine/<scene>ca) asset table was not
    // supplied. The original would try 0x40fe70, so the trace stops instead of
    // inventing the animation.
    std::optional<std::uint16_t> unresolved_node_asset;
    // The CA table exists but has no entry for the id. 0x431b0f reads
    // DAT_0047dd70[id]+0x08 without a NULL check, i.e. the original faults;
    // this is an explicit stop, never a skipped write.
    std::optional<std::uint16_t> missing_node_asset;
    // 0x431b0f found the CA entry but its type byte is neither 0/1 (lazy
    // 0x40fe70) nor 2/3 (0x40fd7b scene start), so the original logs through
    // 0x40ac43/0x40ac38, leaves the record NULL and the dispatcher skips the
    // command body. Counted so the skip stays visible.
    std::size_t skipped_node_commands = 0;
    // Records this trace created from the CA asset table (0x40fd7b/0x40fe70).
    std::size_t records_created = 0;
    // 0x40fe70 failed: the CA type allows a record but no `anims` file was
    // found, so 0x431b0f returns 1 and the command body is skipped.
    std::size_t anims_load_failures = 0;
    // 0x40cf73 walked past the last frame of a known frame count and logged
    // through 0x40ac43/0x40ac38; the current frame stays at the last node.
    std::size_t frame_overflow_logs = 0;
    // 0x86 updated a save-restored or asset-created record.
    std::vector<SceneNodeWrite> node_writes;
    // 0xcf releases and 0xd0's stop-all; the DirectSound layer is not rebuilt.
    std::vector<SceneSoundRelease> sound_releases;
    std::size_t sound_stop_all_requests = 0;
    // 0xd3's timed sound sessions (0x428cbd's nine-argument form).
    std::vector<SceneSoundSession> sound_sessions;
    // 0x79 ends the current script: the dispatcher returns 1, which 0x42d594
    // turns into "store 0x484e28 and stop". DAT_0045f490 is the scene-exit
    // reason it sets from 0 to 1 first.
    bool script_end_requested = false;
    // 0x8d asks 0x431a69/0x40c7b4 to make CD `disc` available. The check
    // enumerates CD-ROM drives, so the requested disc is recorded instead of
    // inventing a host answer. Disc 7 means "1, else 2" inside 0x431a69.
    std::optional<std::uint16_t> disc_request;
    // 0x7f reads DAT_00464f60[id] BEFORE checking it for NULL (the shipped code
    // faults on an absent record), so an absent entry is an explicit stop and
    // never a skipped write.
    std::optional<std::uint16_t> missing_media_record;
    // 0xc7 on an absent DAT_00486a10 chain (the original dereferences NULL).
    std::optional<std::uint16_t> missing_dialogue_chain;
    // 0xc8 sub 0x1f needs the input query 0x42a579.
    std::optional<std::uint16_t> unresolved_system_input;
    // 0xbe/0xd2 need DAT_00460670 and gamepref[0]; nullopt state means the
    // caller did not supply the engine scene list.
    bool unresolved_save_setting = false;
    std::size_t tempsave_requests = 0;
    std::size_t autosave_requests = 0;
    // 0x73 changes DAT_00482428 through DAT_00482460 (engine/<chapter>.lcx).
    // The trace stops instead of continuing with the old scene's variable row.
    std::optional<std::uint16_t> scene_switch_requested;
    // 0x73 without the LCX table supplied.
    std::optional<std::uint16_t> unresolved_scene_table;
    // 0x68 wrote DAT_00464878 (0x42dad9).
    std::size_t alternate_mode_changes = 0;
    std::vector<SceneVideoRequest> video_requests;
    // Modes 2/3 are skipped when their gamepref entry is zero (FUN_0041d440).
    std::size_t video_skipped = 0;
    // 0xb9 resolves its CO record through DAT_00483d90; without that table the
    // node name and JPEG path cannot be built.
    std::optional<std::uint16_t> unresolved_panorama_node;
    std::vector<ScenePanoramaLoad> panorama_loads;
    // 0xb9 with a nonzero resolved operand 1 clears DAT_0047e2f0 through the
    // 0x46ee00 per-scene index table. The table's meaning is not recovered.
    std::size_t panorama_flag_clears = 0;
    std::size_t stopped_at = 0;
};

// A trace that stopped because the port lacks data or settings the original
// would have (or because it hit a safety bound) must not be treated as a
// successful action: the original would either fault or do something this port
// cannot reproduce. Returns an empty string for a real end (0x6f return, 0x79
// script end) and for a 0x73 scene switch, otherwise a short "reason=detail".
std::string trace_blocker(const SceneTrace& trace);

// One record the scene composer drew.
struct SceneDraw {
    std::uint16_t id = 0;
    std::uint8_t kind = 0;
    FrameBlit blit;
};

// Result of composing the record table into a surface.
struct SceneCompose {
    std::size_t drawn = 0;            // records that wrote at least one pixel
    std::size_t skipped_no_frame = 0; // kind 0..4 but no decodable current frame
    std::size_t skipped_other_kind = 0;  // kind outside 0..4 (0x43b216's loop bound)
    // +0x09 & 0x20: handled by 0x43b216's snap pass, which this port does not
    // model (it needs the record's +0x20 word, left 0 by 0x40ffee).
    std::size_t skipped_snap = 0;
    // +0x09 & 0x40: skipped by the normal pass as well.
    std::size_t skipped_hidden = 0;
    std::size_t pixels_written = 0;
    std::vector<SceneDraw> records;
};

// 0x40fd7b: the scene loader frees the DAT_004649e8 table (0x4102ae) and then
// pre-loads every engine/<scene>ca entry whose +0x08 type is 2 or 3 through
// 0x40fe70 (which loads the `anims` asset). 0x42d52f runs first and rewrites the
// file's 0xff "load lazily" marker to type 0, so those entries are NOT preloaded.
// A failed asset load leaves the slot NULL in the original and makes 0x40fd7b
// return 1, which aborts the whole scene load in 0x42c8a0; the port reports the
// count instead of silently continuing. The save loader frees this table again,
// so a save-supplied record table must not be preloaded.
struct SceneRecordPreload {
    std::size_t created = 0;
    std::size_t failed = 0;  // 0x40fe70 failed: the original aborts the scene load
};
SceneRecordPreload preload_scene_records(SceneState& state);

// 0x43b216's record pass, reduced to what the port can draw: for kind 0..4 it
// walks the DAT_004649e8 table from the highest id down, and draws each
// record's current frame (+0x1c) at the record's x/y through 0x40bbfd, clipped
// to the surface. The +0x09 & 0x80 destination-read blend, the `+0x09 & 0x20`
// "snap" pass and the trail list are NOT modelled (see ANIMATION_ASSET.md).
// Records whose current frame is unknown are counted, never invented.
SceneCompose compose_scene(SceneState& state, Rgb16Surface& target);

// Parses a compiled .CC scene script. Opcode meanings are not yet reconstructed.
// Throws std::runtime_error on invalid input.
std::vector<CcInstruction> parse_cc(const std::vector<std::uint8_t>& file);

// Extracts opcode 0x72 hotspot declarations whose first seven operands are
// constants. Dynamic operands require the future scene interpreter.
std::vector<SceneHotspot> static_hotspots(const std::vector<CcInstruction>& script);

// Joins constant opcode 0x69 scene IDs with the matching CB.ALL record.
std::vector<SceneBackground> static_backgrounds(
    const std::vector<CcInstruction>& script,
    const std::vector<BackgroundEntry>& entries);

// CO.ALL uses the same string-pair/0xff records as CB.ALL in this campaign.
// This binds static 0xb9 IDs to assets; it does not execute the loader.
std::vector<ScenePanorama> static_panoramas(
    const std::vector<CcInstruction>& script,
    const std::vector<BackgroundEntry>& entries);

// Resolves 0xe1 labels and constant 0x76 jumps / 0x9b calls.
CcControlFlow resolve_control_flow(const std::vector<CcInstruction>& script);

// Restores the state a parsed savegame/NN.BTB carries into a fresh SceneState:// the 0x464300 object records (including +0x0c/+0x14), the 0x464f60 media
// records, the 0x486220 sound records, the 0x4649e8 node records and the
// dialogue window state that 0x408adc reads (channel selector, scroll, the ten
// channel stacks, the current record and the 0x4089b0 per-record state bytes).
// The original 0x425ab4 frees those tables before filling them, so this
// function replaces their contents. It does NOT restore the carried variable
// pool (DAT_0047e2f0); use load_save_variables for that.
void load_save(SceneState& state, const SaveGame& save);

// Projects the save's DAT_0047e2f0 (SaveGame::node_values, 3000 words) through
// the 0x46ee00 mapping exactly as 0x42c8a0/0x438173 build it: entry `local` of
// the current scene is identity (local -> local) unless an LCV entry overwrote
// it, so global = local < declared.size() ? declared[local] : local for the 300
// per-scene entries. The result is stored in SceneState::variables under the
// same local key the VM already uses for type-2 operands. Returns 300.
std::size_t load_save_variables(SceneState& state, const SaveGame& save,
                                const std::vector<std::uint16_t>& declared_global_ids);

// Executes a verified instruction subset from one label until return/unsupported.
// Hotspot definitions are preloaded from static 0x72 declarations.
SceneTrace trace_scene_label(const std::vector<CcInstruction>& script,
                             const std::vector<BackgroundEntry>& entries,
                             std::uint16_t label);

// As above, but preserves values and enabled hotspots for a sequence of labels
// in the same scene script.
// `step_budget` > 0 stops after that many executed instructions and reports
// paused_by_budget instead of a stop reason. The state is the same as a run
// interrupted at that instruction, so replaying the label from an identical
// starting state with a larger budget is exact (no time- or RNG-dependent state
// is advanced by the replay itself).
SceneTrace trace_scene_label(const std::vector<CcInstruction>& script,
                             const std::vector<BackgroundEntry>& entries,
                             SceneState& state,
                             std::uint16_t label, std::size_t step_budget = 0);

// Runs a scene script from its first instruction, which is what the engine does
// after 0x73 writes DAT_00482428: 0x42c8a0 reloads that scene and 0x42d594
// starts at DAT_0047a238, the instruction-list head (NOT at a label). The first
// instructions are 0x68 and the 0x72/0x7a hotspot declarations.
SceneTrace trace_scene_entry(const std::vector<CcInstruction>& script,
                            const std::vector<BackgroundEntry>& entries,
                            SceneState& state, std::size_t step_budget = 0);

}  // namespace byzantine
