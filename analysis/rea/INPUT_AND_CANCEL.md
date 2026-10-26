# Conversation exit dialog, session end, and the shared interpreter

Date: 26 October 2026.

Scope: common engine/player behaviour only. Nothing here branches on a scene
name, character name or hard-coded label. The mouse-coordinate fix is a
separate committed change: `analysis/rea/MOUSE_INPUT.md`.

## 1. The original ESC path is a dialog, not an immediate cancel

Evidence (saved REA/Ghidra exports, target
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`):

- `0x414cd2` (`9013436661652e9e`): WM_KEYDOWN `VK_ESCAPE` sets
  `DAT_0045f058 = 1`.
- `0x436b01` (`5c305d3e5540154f`) sees that flag in the answer loop and calls
  `FUN_0041d6df(9)`.
- `0x41d6df` (`c33db8179865f922`) is `DialogBoxParamA`; `0x41d523`
  (`227d3f8e8d80d6f8`) ends the dialog with button id 1, 2, 6 or 7.
  Only result 6 sets `0xfffe` and `DAT_0045f48c = 1`. Results 1/2/7 clear
  `DAT_0045f058` and the answer loop continues with the **same** list.
- `0x41d45a` (`a44998bd0603574a`) reads
  `copy2hd/setupart/MESSAGES.TXT` **0-based**: it reads line 0 and then
  `param_2` more lines. The dialog message id 9 is therefore line 9
  ("Kaydetmeden oyundan çıkmak istiyor musunuz?") and the labels for dialog
  items 6, 7, 1, 2 are lines 0x4c..0x4f
  (`EVET`, `HAYIR`, `TAMAM`, `İPTAL`).

Port mapping (common, no scene lookup):

- ESC opens a 4-button dialog drawn over the conversation screen: the real id-9
  message and the real id-0x4c..0x4f labels, loaded from `--messages`
  (`scene.sh` extracts `copy2hd/setupart/MESSAGES.TXT`). The vertical layout is
  the port's; the button ids/actions are the original's.
- ESC again (IDCANCEL, id 2), `HAYIR` (7), `TAMAM` (1) or `İPTAL` (2) closes
  the dialog and returns to the same `ConversationEvent::answers` list. The
  session object, per-call chosen slots, `ConversationMemory` and scene
  variables are not recreated, so the current choices, memory and variables are
  preserved.
- `EVET` (6) or key `6` returns `0xfffe` through `ConversationSession::cancel()`.
- The port logs `conversation_menu open`, `conversation_menu result=N action=…`
  and `conversation_menu_ready source=messages|fallback`.

Verification (MR `30.btb`, real X11 input; log
`port/output/recordings/menu_flow/menu_flow.log`):

```
conversation_menu_ready message_id=9 buttons=4 source=messages
conversation_menu open message_id=9 action=return_to_conversation_or_terminate
conversation_menu result=2 action=resume (same answer list, memory and variables)
conversation_hover choice=0 logical=217,260
conversation_pick record=2 choice=0
```

After the resume the same first list was still active and choice 0 was picked.
Screenshots: `menu_flow/menu_open.png`, `menu_flow/menu_resumed.png`.

## 2. Session end: no silent reload

The `0xfffe` chain is the same one already documented for the original:

- `0x42db22` case `0x70` (`8df50fc5339a585b`) maps `0xfffe` to `local_8 = 1`.
- `0x42d594` (`546a5c43ede36e64`) stores `DAT_00484e28`, clears the script
  pointer and returns 1.
- `0x42c8a0` (`6d09fd55ad59f5e1`) calls `FUN_0041e289(FUN_0041de4b())`;
  `0x41de4b` (`6a7d8fe78b3a8317`) returns 6, and `0x41e289`
  (`a89ac0f7d4449865`) returns 1 because `DAT_0045f48c` is set. The scene
  session ends; `0x41535a` (`902ca0771bb0bb4a`) returns to its caller.
- The original does not restore the `0xb8` snapshot in that scene. MR has
  `0xb8` (`0x438cf9`, `7014e4f6df47bf9e`) at instructions 166/278 and no `0xb7`
  (`0x438d78`, `b47bd5f7a4400eb1`), so the conversation exit really leaves every
  object disabled until a new scene load.

Port behaviour:

- `execute_plan` does **not** call `restart_scene`. It leaves the scene exactly
  as committed, sets `Session::reload_available`, and draws the explicit notice
  `OTURUM SONLANDI / YENİDEN YÜKLE: R / ÇIKIŞ: ESC` in the message window.
  It logs `scene_session_ended reason=conversation_exit hotspots=0` and
  `reload_choice available=yes key=R … (explicit user choice; not automatic)`.
- Only when the user presses `R` does `restart_scene` perform the 0x42c8a0-head
  fresh load from the supplied save/entry (`scene_restart
  reason=conversation_session_exit_user_choice`). This is the port's substitute
  for the original's return to the main menu; no progress is rolled back
  silently.
- `--conversation-cancel N` is the deterministic terminate path: it leaves
  `hotspots=0`, prints the explicit choice and does not reload. The interactive
  R path is what reloads.

Verification (same MR run):

```
conversation_menu result=6 action=terminate
scene_session_ended reason=conversation_exit hotspots=0
reload_choice available=yes key=R scene=MR (explicit user choice; not automatic)
...
scene_restart reason=conversation_session_exit_user_choice scene=MR point=entry hotspots=2
```

Screenshots: `menu_flow/menu_terminated.png` (closed scene + notice),
`menu_flow/menu_reloaded.png` (door hotspot 42 back). After R, real door and
seller clicks started the conversation again.

Recording (one file, mouse fix + dialog + explicit reload + re-talk):
`port/output/recordings/step_mouse_esc_menu_reload.mp4` — 22.3 s, 1280x960,
H.264 30 fps, silent (`--no-video --no-audio` so the dialog text/state are the
focus). The 1024x768 door click and 1280x960 seller click are both real X11
pointer events.

## 3. Same interpreter, different character (AM curator)

The common conversation VM was exercised on the shipped AM conversations using
save `62.btb` (AM) and the real script action labels, not a scene-specific
branch:

| action | record | name | lines | answer lists | instructions | resume |
| --- | --- | --- | --- | --- | --- | --- |
| 1512 | 6 | `icuam1a` | 8 | 7 | 44 | 1898 |
| 1530 | 7 | `ICuAm3a` | 8 | 6 | 18 | 1918 |
| 1546 | 8 | `icuam5a` | 11 | 10 | 30 | 1937 |
| 1633 | 10 | `INuTK3a` | 7 | 4 | 20 | 2051 |

All four reach `result=0xffff` with no unsupported opcode. `icuam1a` initially
stopped at `unsupported conversation opcode 0x46`; that was a real gap in the
shared interpreter, not an AM special case.

The same exit dialog was also exercised on the AM curator's first answer list:
`conversation_answers record=6 group=3` → ESC opens the menu → ESC resumes the
same list (`conversation_menu result=2 action=resume`). Log/screenshot:
`port/output/recordings/am_curator_menu.log`, `am_curator_menu.png`.

### 0x45 / 0x46

`0x43340a` (`3a9555ec83f82adc`) cases 0x45/0x46 write an 8-byte record:
operand 1's byte plus a pointer resolved by `FUN_004367eb`
(`79b12278b4dd0b96`). `FUN_004367eb` uses the operand's type bit 0x10 to choose
the line group array (`state+0x4c4`) or the answer group array (`state+0x58c`),
then follows the entry's `+8` chain operand-2-word-1 times. The records go to:

- 0x45: `state+0x2a` (50 pointers)
- 0x46: `state+0x5c` when operand 3 word 0 is 2, otherwise `state+0x8e`

The port stores `{byte, ConversationEntryKey}` in `ConversationSlotValue` arrays
on `ConversationMemory`; `apply_memory`/`save_memory` carry them across 0x70
calls. No decompiled reader consumes these records, so the port does not invent
one; it executes the writes instead of skipping the opcodes. Regression test:
`conversation-test`'s `test_slot_opcodes` (synthetic CCB, both selectors, both
10-slot tables, persistence on re-entry).

The AM **resumed** segments stop at `unresolved_random` (instruction 1777 for
records 6/7/8; 2005 for record 10) because the CRT `_rand` seed is not in the
save. That is the documented general data blocker, not a conversation opcode.
The AM start used `--run-label <action>` as a development entry: the save's
active hotspot set does not contain those slots. This is explicitly **not**
claimed as physical-mouse verification; the dialog/session-end/re-talk flow was
verified with real mouse input on MR.

Media note: the AM conversation AVI folders are not in the prepared test data
dir, so `conversation_media clip=… missing`; text/VM/choice behaviour is what
was verified for AM. The MR recording uses the real MR video where present.

## 4. General support vs example-only observations

General (no scene/character/label condition in the code):

- SDL logical mouse mapping and `input-test` (`MOUSE_INPUT.md`).
- ESC dialog with real `MESSAGES.TXT` id 9/labels, return-to-conversation vs
  terminate, preserved choices/memory/variables.
- `0xfffe` scene-session end, explicit `R` reload, no silent rollback.
- Conversation opcodes 0x45/0x46 and the persistent slot tables.

Only verified in the MR example:

- MR's specific `0xb8`/no-`0xb7` hotspot state and the first-list choices.
- The MR recording/screenshots and the exact `(217,260)` answer-row click.

Only verified in the AM example:

- The four AM records and their line/answer counts.
- The missing AM media folders (video not verified there).

## 5. Map return (prior verified result, unchanged)

The MAP return leg is independent of the exit dialog and was verified with real
mouse input from the conversation-returned MAP state:

- MAP slot 47/action 246 opens the region overlay (record 27, Istanbul map) and
  enables slot 13.
- A second click at the same logical point (192,300) hits slot 13/action 257,
  which runs `0x73 [31]`; `SHOW.LCX[31] = 27` = MR.
- The physical log is `port/output/recordings/map_return_physical.log`; the
  overlay shot is `port/output/recordings/map_region_overlay.png`.

The earlier “no MAP click travelled back” observation was a one-click test gap.
See the previous revision of this report and `CONTINUE.md` for the full
evidence.

## 6. Verified function digests

First 16 hex digits of each SHA-256 from `pseudocode/decompile-index.json`:

| address | digest | role |
| --- | --- | --- |
| `0x414cd2` | `9013436661652e9e` | VK_ESCAPE sets `DAT_0045f058` |
| `0x436b01` | `5c305d3e5540154f` | answer loop; dialog call; 0xfffe + `DAT_0045f48c` |
| `0x41d6df` | `c33db8179865f922` | `DialogBoxParamA` menu |
| `0x41d523` | `227d3f8e8d80d6f8` | dialog proc (buttons 1/2/6/7) |
| `0x41d45a` | `a44998bd0603574a` | 0-based MESSAGES.TXT reader |
| `0x42db22` | `8df50fc5339a585b` | case 0x70 maps 0xfffe to 1 |
| `0x42d594` | `546a5c43ede36e64` | result 1 stops the script |
| `0x42c8a0` | `6d09fd55ad59f5e1` | scene loop; result 1 → `0x41e289` |
| `0x41de4b` | `6a7d8fe78b3a8317` | returns 6 from `DAT_0045f058` |
| `0x41e289` | `a89ac0f7d4449865` | mode 6 returns 1 on `DAT_0045f48c` |
| `0x41535a` | `902ca0771bb0bb4a` | game session loop |
| `0x438cf9` | `7014e4f6df47bf9e` | 0xb8 snapshot + clear |
| `0x438d78` | `b47bd5f7a4400eb1` | 0xb7 restore |
| `0x4119e3` | `beac71ca47ce006c` | CCB loader: list0/list1 + two group arrays |
| `0x412aea` | `170b271ce08e8f98` | group/entry parser |
| `0x412762` | `96dc07ef7781917e` | instruction decoder |
| `0x43340a` | `3a9555ec83f82adc` | conversation opcode interpreter |
| `0x4367eb` | `79b12278b4dd0b96` | entry reference resolver |

## 7. Commands used

```sh
make -C port -j2 all test

# real-window dialog / session-end / reload / re-talk (isolated X11)
Xephyr :8 -screen 1280x960 -no-host-grab -ac -br &
python3 /tmp/opencode/record_menu.py

# AM curator conversations (development entry via real script labels)
./port/byzantine-play ... --entry --run-label 1512 --conversation-choices 0,... \
  --no-video --no-audio --shot /tmp/am_1512.ppm
```

## 8. Remaining boundaries

- The original main menu is not implemented; `R` is the port's explicit
  substitute for "load/start again". The dialog's vertical button layout is the
  port's; the button ids/actions are the original's.
- The menus/dialog do not implement Save/Load actions for buttons 1/2/7; the
  original caller also only resumes the answer loop for those results.
- AM dialog media is missing from the prepared data dir; the AM VM/text/choices
  are verified, not its video.
- The AM resumed segments need the CRT random seed (`unresolved_random`), which
  the save does not carry.
