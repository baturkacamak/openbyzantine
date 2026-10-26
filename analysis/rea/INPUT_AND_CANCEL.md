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

Verification (MR `30.btb`, X11 pointer events on an isolated server
(`xdotool`/XTEST, the same SDL event path as a physical pointer; see
`MOUSE_INPUT.md` for why the host pointer was isolated); log
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
  load from the scene's **arrival entry_state**: the save state for the start
  scene, the carried pool/state for a 0x73 target (`scene_restart
  reason=conversation_session_exit_user_choice`). It does not reopen the global
  save into a traveled scene. This is the port's substitute for the original's
  return to the main menu; no progress is rolled back silently.
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

## 3. Same interpreter, different character: one end-to-end AM conversation

The end-to-end target is **one** conversation, not the four-record sweep:
`AM_NURAY.md` documents record 0 `Inuam3a` (speaker `nur`), started from save
`62.btb` with normal panorama clicks and played with the shipped
`dialog/inuam3a/na1nur04.avi` + PCM. The previous four-record run
(`icuam1a/ICuAm3a/icuam5a/INuTK3a`) is retained only as an interpreter
regression for the shared 0x45/0x46 gap, not as the end-to-end claim.

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

The AM record 0 continuation has no random call and completes at 467
(`0xb7`/return, steps=2). The startup RNG seed (`0xbeef`) and the
save-time-stream limitation are in `VM_RANDOM.md`; the map destinations now
run their entries with the original startup seed instead of stopping on
`unresolved_random`. The exact old sequence from a save is not claimed.

## 4. General support vs example-only observations

General (no scene/character/label condition in the code):

- SDL logical mouse mapping and `input-test` (`MOUSE_INPUT.md`).
- ESC dialog with real `MESSAGES.TXT` id 9/labels, return-to-conversation vs
  terminate, preserved choices/memory/variables.
- `0xfffe` scene-session end, explicit `R` reload, no silent rollback.
- Conversation opcodes 0x45/0x46 and the persistent slot tables.
- Scene travel/load, `copy2hd/nodes` flattening, the `.media_v2` media
  backfill, `background_missing`, the startup RNG seed/carry, the `0x72` object
  declaration with `0x6d`/`0x6e`, and the conversation media/answer/continuation
  path.
- The 640x480 surface, the save-carried inventory strip, item selection/examine,
  the recovered examine scene-group table and the close-up snapshot return;
  `INVENTORY_AND_CLOSEUP.md`.
- Mouse vertical look: left-drag sends yaw and pitch, wheel sends pitch; the
  same `PanoramaView::advance` as the arrow keys. The exact original cursor-edge
  bands (`0x4013ae`) are not claimed.

Only verified in the MR example:

- MR's specific `0xb8`/no-`0xb7` hotspot state and the first-list choices.
- The MR recording/screenshots and the exact `(217,260)` answer-row click.

Only verified in the AM example:

- The two action/camera positions to reach `Inuam3a` record 0, its
  text/choices/media, and its post-conversation empty hotspot set.
- The four-record sweep is an interpreter regression only; it is not the
  end-to-end claim.
- Item 11 (`ibeering`) -> `EXAMINE`/`EBEERING.jpg` and the close-up exit
  coordinates in the AM example.
- VT `73.btb` item 11 -> `EXAMVT`/`EBEERING.jpg` and exit `589,349` in the
  second headless check (same common path, not a second physical-mouse run).

## 5. Map selection (real clicks in both directions)

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

A forward selection was also verified with real X11 clicks on save `39.btb`:
the first click at logical `(289,63)` selects the GT map location (slot
37/action 365, `click_result steps=6`); the second click at the same point hits
the revealed slot 2/action 376, which runs `0x73 [15]`:
`travel index=15 scene=gt`, `entry scene=GT steps=58 stopped_at=36 hotspots=1`,
`composed panorama=nodes/gt/GT00.jpg`. The 12 s recording is
`port/output/recordings/map_click_to_gt.mp4` (map frames then the GT
panorama), the log `map_click_to_gt.log`, and the frames `map_click_map.png` /
`map_click_gt.png`. The location coordinates are only this example; the
select-then-go hotspot and travel handling are the common script/VM path.

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
| `0x415a4e` | `2eb658d9c5d4dc9f` | game/render init; calls `_srand(0xbeef)` |
| `0x44d060` | `a2ad7fec38ea0c58` | `_srand` writes `DAT_00462ef8` |
| `0x44d070` | `0f3e322e81915145` | `_rand` LCG |
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

# AM Nuray end to end (normal controls; record 0 Inuam3a)
BYZ_DATA=/tmp/byz-conv ./port/scene.sh . AM 0 62.btb --entry
# rotate Left/Right, click slot 88 then slot 92, click the first answer
# (full media/audio; the recording script is /tmp/opencode/record_nuray.py)

# inventory strip -> examine close-up -> parent return (AM 62.btb)
./port/byzantine-play ... --entry --bar-select 11 --bar-use --click-at 589,370
# same common path on VT 73.btb -> EXAMVT, exit at 589,349
./port/byzantine-play ... --entry --bar-select 11 --bar-use --click-at 589,349

# MAP destination census (headless, generic travel)
python3 /tmp/opencode/map_census.py
```

## 8. Remaining boundaries

- The original main menu is not implemented; `R` is the port's explicit
  substitute for "load/start again". The dialog's vertical button layout is the
  port's; the button ids/actions are the original's.
- The menus/dialog do not implement Save/Load actions for buttons 1/2/7; the
  original caller also only resumes the answer loop for those results.
- `Inuam3a`'s `dialog/inuam3a/na1nur04.avi` was extracted from CD5 and played;
  the other AM records' media folders are not claimed.
- The player applies the original startup seed `0xbeef`; the save does not
  carry `DAT_00462ef8`, and the per-instruction `_rand()` draws are not in the
  static trace, so the exact old stream from a mid-game save is not
  reproducible. The `0x42a6aa` local-time reseed remains an explicit stop.
- The 17-destination MAP census composes every background/panorama and runs
  every entry to a normal return; the generic `0x72` object creation removed
  VB's `missing_object=4` (entry returns at instruction 217 with 4 hotspots).
- The top 20-px menu bar is not drawn/implemented; top-band clicks are consumed.
  The strip's item-on-object compatibility (`0x407c5a`)/consume path
  (`0x407bd7`) and the map/other top-menu screens remain open.
- The close-up snapshot is a whole-`SceneView` port representation of
  `DAT_004603d0`, not the original's individual subsystem tables. `va`'s
  original out-of-range group 5 is left unresolved.
