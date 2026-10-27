# 0x70 CONVERSE: the real MR conversation (IMhMR2a), its text and media

Status 26 October 2026. Scope: replace the old `0x70` shortcut (operand 0 used
as an object id) with the shipped conversation VM for one real example: MR label
413, conversation record 2 = `IMhMR2a`. Target `BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`. All hashes
are the first 16 hex digits of the indexed export SHA-256.

## 1. What 0x70 actually does

`0x42db22 case 0x70` (`0x42dc…`):

```
operand0 = *(u16*)(instruction + 0x0a)          // NOT an object slot
index    = FUN_00410f09(operand0,
                        DAT_0046e870[operand0]->name)
result   = FUN_00432d6e(index, DAT_00482ad0[index],
                        *(u16*)(instruction + 0x0e))
if (result == 0xfffe) dispatcher result = 1      // ESC: stop
else if (result != 0xffff) {
    pc = DAT_004654e0[object[result]->+0x0c]     // real object click
}
```

- Operand 0 is an index into the **scene's `engine/<scene>cd` table**
  `DAT_0046e870` (0x411dfc), not an object id. `0x410f09` maps that record's
  `name1` into the scene's `ldm` name list, and the resulting index selects the
  per-conversation state block `DAT_00482ad0[index]` (allocated by `0x410669`).
- Operand 1 is the object id the conversation starts with (`0x432d6e`'s third
  argument); it is compared with the object clicked during an answer list.
- `0xffff` means "no branch": `pc` stays after the `0x70`.
- Any other value is an object id the player clicked while the answer list was
  up; the scene continues at that object's `+0x0c` CC label.
- `0xfffe` is the input layer's ESC result (stop the dispatcher).

The port's previous behaviour (`operand 0` as an object slot) was therefore a
guess: for MR label 413 it produced `missing_object=2` / `object 2 -> label
265`. Object 2 is the shop **door** hotspot (action 265, the exit), not the
conversation. The new VM pauses the trace at the `0x70` and resumes with the real
result.

## 2. The shipped conversation data (a correction)

`0x410669` (`1a4831f4aa67ebf9`) reads the cd record:

| file | source | content |
| --- | --- | --- |
| `engine/<stem>.ccb` | `CCB.ALL` entry `<stem>` | two instruction lists + two group tables |
| `engine/<stem>.ccf` | `CCF.ALL` entry `<stem>` | voice-clip names, one per line |
| `engine/<stem>.byz` | `Byz.all` entry `<stem>` | **dialogue lines**, one per line |

`stem` is the cd record's second string with `.ccd` changed to `.ccb`
(`0x4119e3` then parses it, `local_22c` changes the last character to `f` for the
`.ccf`, and the first string `sprintf`ed as `engine\%s` then the 3-char extension
is overwritten with `byz`). In packed mode `0x411b4b` opens
`engine\<ext>.all` and matches the entry name.

This corrects `TEXT_WINDOW.md` §5: the conversations are **not** voice-only. The
subtitle asset exists, packed in `Byz.all`; it was not found earlier because it is
not a loose file and the loader was outside the text-window path. MR's cd table
(`CD.ALL` entry `MR`) has three records:

| index | name1 (dialog folder) | name2 (file stem) |
| --- | --- | --- |
| 0 | `MMhmr1a` | `MMhMr1a.ccd` |
| 1 | `IMhMR1a` | `IMhMr1a.ccd` |
| 2 | `IMhMR2a` | `IMhMr2a.ccd` |

MR's conversation folders are `dialog/mmhmr1a`, `dialog/imhmr1a`,
`dialog/imhmr2a`. The files are **Indeo 4 video + PCM16 stereo 22050 Hz** AVIs,
15 fps, 320x200 (verified with `ffprobe`):

`mr2meh02..09`, `Mr2meh11..14` exist; **`mr2meh00`, `mr2meh01` and `mr2meh10` are
not on any CD** (the CD1 listing has no `mr2meh00/01/10`, and no `mr2pro*` at
all). The `pro` clips in the CCF are the player's own lines: `0x40ced0` returns
`-1` for `"pro"` and `0x424780` skips `0x40d2a1` only for that id, so the
original never plays them. Lines whose clip has no file still show their text
(0x40d2a1 returns 0). See "Correction: `0x40ced0` is case-insensitive" below.

## 3. CCB format (FUN_00412762 / FUN_00412aea / FUN_004119e3)

- `u16` count, then that many instruction nodes. A node is
  `opcode (0x32..0xe1)`, one type byte per argument (count from `0x460466 +
  opcode`), then the argument data:
  | type | bytes | stored value |
  | --- | --- | --- |
  | 0 | 0 | 0xffff |
  | 1, 4 | 1 | byte, high half 0x00ff |
  | 2, 8, 0x10, 0x18, 0x40 | 2 | little-endian u16 |
  | 0x20 | 4 | first u16; second u16 read and discarded |

  `FUN_00436652` combines the two slot halves as `high*0x100 + low`, which is the
  little-endian value for 2-byte types and `0xff00|byte` for 1-byte types.
- list 1 and list 2 (both `u16` counted), then a **group array**: one byte N =
  number of **non-empty** groups; each group record is one byte entry count
  followed by entries. A group with count 0 still consumes an array slot and does
  not increment N, so reading continues until N non-empty groups were seen. An
  entry is `u16 value, u16 link, instruction list, u8 sub-count, sub-entries`
  (sub-entries chain through `+0x0c`, entries through `+0x08`). The arrays have
  50 slots; `FUN_00412cc4` allocates 0x14-byte entries initialised
  `+0x10 enabled = 1`, `+0x11 unspeak = 0`.
- The first group array is `state+0x131` (lines), the second `state+0x163`
  (answers). `0x4367eb` uses an operand's **low** byte as the array index and its
  **high** byte as the number of `next` hops.
- `0x4119e3` stops after the two lists and two group arrays; trailing bytes are
  not instructions. The shipped `IMHMR2A` consumes 538/538 bytes; `IMHMR1A`
  leaves 314 trailing bytes (retained as a count by the port, not executed).

## 4. The conversation VM (0x43340a) and its loop

- `0x432d6e` (`3967dc020f1d5bf1`) runs list1 and list2 once, then repeats
  `0x4330bf`/`0x4331d2` (skip tables) and `0x43688b` until `state+0x0e` (done).
- `0x43688b` (`19b2b583e3a1cea3`):
  - mode `state+0x0a == 1` → `0x436b01` (answer input).
  - otherwise display the current group's line entry (or the `state+0xf2`
    override), run each sub-entry's list then the entry's list, set mode 1.
- `0x436b01` (`5c305d3e5540154f`) builds the answer list from `groups2[group]`
  plus runtime answer groups, draws it, waits for a mouse click, plays the chosen
  line (`0x424780`), marks it unspeak, runs its list, and advances the group by
  one unless the list set `DAT_0047a8d8`. A click on a scene object returns that
  object id and sets done (`0x0e`); ESC returns `0xfffe`. The port displays the
  selected answer's text; the shipped IMhMR2a answers all use the `pro` speaker,
  which the original itself does not play (no media). Answer-line media for a
  known speaker is outside this slice.
- `0x424780` (`c31b11acff556e9d`) displays/plays a line: `state+0x720` is the
  BYZ text, `state+0x12dc` the CCF clip; `null` (case-insensitive) means no text
  and no media, `#` means media only. The speaker is clip characters 3..5 and
  `0x40ced0` (`dabc11839d698c3d`) compares it with **`__strcmpi`** against
  `aka nur sha esk meh car emr jer ima prf cur bar bas coi boo mer`, then against
  `hnt` (`-2`) and `pro` (`-1`), else `-3`. `0x424780` calls `0x40d2a1`
  (`7378a7da0065e607`) when the id is not `-1` and the text is not `null`, so it
  tries `avi\<clip>.avi` then `dialog\<name1>\<clip>.avi` for every speaker
  except the player's own `pro` lines.

### Opcodes IMhMR2a uses

| opcode | action |
| --- | --- |
| 0x3a | no-op (aligned header; no `0x43340a` case) |
| 0x3e | debug-only `0x40a7e5` when `DAT_00485c6c != 0` |
| 0x32 | `state.group = operand0`, mode 0, stop list |
| 0x33 | `state.group = operand0`, mode 1, stop list |
| 0x36/0x37 | enable/disable an answer entry (`groups2` unless type bit 0x10) |
| 0x38 | select an entry as `state[low+0xf2]` (line override) |
| 0x39 | done |
| 0x40 | `state+0xd` default line group |
| 0x42/0x43 | if variable == value, enable/disable an answer entry |
| 0x44 | if variable == value, select a line entry |
| 0x48 | IFREPLY: if variable == value, select operand 2 as the reply target (`state+2/state+0xa`; the port treats it as the reply group) |
| 0x49 | if variable == value, switch line group (a group index, not a CC label) |
| 0x4a/0x4b | disable/enable an answer entry plus redraw record |
| 0x67 | store literal/variable into a scene local |
| 0x74 | add literal/variable to a scene local |

`0x48` is the one opcode whose original cursor (`state+2`) has no direct
counterpart in the port's group machine: the implementation selects operand 2
as the reply group when the variable matches. The real Hints1 conversation
(`0x41e002`) uses it and completes (24 lines, 15 answer lists, 1,627
instructions), which is the observed acceptance case.

The port rejects every other conversation opcode with an explicit error (it does
not skip it). `0x34/0x35`, `0x45/0x46`, the inventory/cursor branches and the
media opcodes are outside this one-conversation slice.

## 5. Implementation

- `port/include/byzantine/conversation.hpp` + `src/conversation.cpp`:
  `parse_conversation_lines`, `parse_conversation_records`,
  `parse_conversation_program`, `ConversationSession`.
- `SceneState` gained `conversation_records` / `conversation_table_known`
  (`--cd`), reloaded per scene.
- `0x70` now stops with `SceneTrace::conversation_request {instruction, record,
  object_context}` instead of guessing. `trace_scene_from_instruction` resumes at
  an instruction, and `apply_conversation_result` performs the 0x70 branch
  (`0xffff` -> next instruction, an object -> its `+0x0c` label; absent
  object/label is an error). `byzantine-cc --trace` prints the request.
- Player: `--cd`, `--ccb`, `--ccf`, `--byz`, `--dialog`, `--dialog-cache`,
  `--run-label`, `--conversation-choices`, `--conversation-object`,
  `--conversation-shot`, `--conversation-video-shot`.
  `execute_plan` runs the pre-conversation segment (with its sounds), the real
  conversation, and the resumed segment; a conversation that does not finish is
  **not committed** (the pre-click state is restored).
- UI (the original 640x480 surface): rows `0x14..0x413` are the playfield, rows
  `0x1a4..0x1df` the 60-px dialogue band. The port composes its 640x400 scene at
  row `0x14` and the conversation screen is 640x480; the shop JPEG is **not**
  the conversation background. When a character video starts, `0x40d5ff`
  clears all `0x4b000` words of the 640x480 surface (the mode-0 branch); a mode-2
  640x400 frame covers the whole playfield. The port clears the playfield in
  mode 0 and lets the last decoded frame stay under the answers, like the
  original's DirectDraw target; the scene JPEG never shows through a mode-2
  clip.
- Subtitle (`0x424981`): it clears `0x12c00` bytes at offset `0x83400` (rows
  420..479, 640x60) on every draw, then draws `DAT_0045fdb4` at
  **x=0x37 (55), y=0x1a4 (420)** with font slot 4, left alignment, at most
  `DAT_0045fdb8` = 3 lines. `0x424780` wraps the line at `0x212` = 530 px and
  `0x424b15` keeps a 3-line buffer. The slot-4 fill is green:
  `(0x1f << 5) | 0xb = 0x03eb` (the same `0x03eb` the port passes to
  `draw_text`). The port reproduces the x/y/wrap/max-lines/colour and keeps the
  current subtitle visible while the answers are open.
- **Subtitle font data blocker:** the shipped Turkish `fonts/CAPTIONS.SFN` +
  `CAPTIONS.TGA` does not carry the full Turkish set. Metrics `0x62..0x66`
  (İ Ğ Ü Ö ç) and `0x69..0x6c` (ı ğ ü ö) are 2-pixel blanks, and the remaining
  accents sit at non-contiguous indices (`0x61` is ü, `0x67` is ç, `0x74` is ö,
  `0x7c` is ş, `0x8b` is ğ, `0x8c` is ı). Rendering the `.byz` bytes with it drops
  letters. Its ASCII glyphs are pixel-identical to `GR_DLG.TGA`'s (62 of 63
  compared glyphs), so the port uses `DIALOG.SFN`/`GR_DLG`/`BK_DLG` with the green
  `0x03eb` fill for the subtitle; the position, width, line count, shadow blend
  and colour still come from `0x424981`/`0x406cfe`. This is reported at startup
  (`fonts captions: N Turkish glyph(s) are blank in CAPTIONS.SFN … using
  DIALOG.SFN's identical glyph shapes`), not hidden. If a release has a complete
  CAPTIONS font, the port uses it directly.
- Answers (`0x436b01`): x=100, wrap width `0x1b8` = 440, font slot 0 (DIALOG,
  normal `0xe2c0`, selected/hovered `0xffff` white), line step = slot 0's glyph
  height (20 in the shipped font), and the block bottom is
  `0x19f - 0x2d = 370` (45 rows are left for the subtitle band). Each answer is
  prefixed with byte `0x7f` + space; `0x7f` selects DIALOG glyph `0x5f`, the
  star-shaped bullet. The hit rectangle is exactly the drawn block:
  `x=100..100+max(line widths)`, `y=answer top..top+lines*line_step`. The list
  is redrawn on pointer motion, so the option under the pointer uses the white
  colour. The port draws the same star, colours, x/wrap/bottom and click rects.
- Text lifetime: a new conversation starts with no caption (`0x432d6e` calls
  `0x424c4e(1)`); each NPC line with text replaces it; opening the answer list
  does **not** clear it (the reference frame shows both); the selected answer's
  line becomes the subtitle (`0x436b01` calls `0x424780` before `0x43340a`);
  the conversation end clears it (`0x424c4e`). The port logs
  `conversation_caption source=start|npc|answer|cleared` so the changes are
  observable. The caption is local to the conversation screen; the resumed
  scene composes from the scene surface, so it cannot carry into the next scene.
- Media: Indeo 4 is a standard codec the port must not reimplement. The player
  asks the available FFmpeg tools (`ffprobe` for the stream fps, `ffmpeg` for
  `f%04d.jpg` + PCM `audio.wav`) to materialise a per-clip cache once; playback
  uses the port's existing JPEG decoder and WAV/SDL2 audio path. If FFmpeg or the
  AVI is absent the line's text still shows and the failure is reported
  (`conversation_media … missing/…`), never hidden. The talking-head video is
  **not** a centred 320x200 rectangle: `0x40d3af` type 4 passes
  `FUN_0041d440(1)` = gamepref[1] = 1 (shipped `GAMEPREF.000`), so mode 2 is
  kept, `0x43a248` calls `0x43d9c7(codec, 2)` and sets the destination to
  **source*2 = 640x400** at x=0, y=0x14 in the 640x480 surface, i.e. the whole
  ported playfield. The port doubles the 320x200 frame exactly (nearest 2x,
  `blit_image_2x`); the original's exact stretch filter is a
  DirectDraw/driver detail that is not recovered. A decoded frame is opaque, so
  there is no key/mask or background blend in this path. If gamepref[1] is 0,
  `0x40d3af` takes the other branch: mode 0, 320x200 at x=160, y=0x14+100=120
  in the 640x480 surface; the port implements both branches from `--gameprefs`
  and reports when that file is absent.
- Audio: the AVI carries PCM16 stereo 22050 Hz (verified with `ffprobe` for all
  IMhMR2a `meh` clips); there is no separate voice file. `ffmpeg -vn` writes
  `audio.wav` into the clip cache, `parse_wav` reads it and `SoundPlayer::queue`
  converts it through `SDL_AudioStream` to the device format. The clip is queued
  before the frame loop and drained after it, so audio and video start and end
  together. The port prints a per-clip error if the device is not open,
  `--no-audio` is set, the WAV is malformed or the queue fails, and a summary
  `conversation_audio ERROR …`; it never silently plays nothing.
- Conversation end: the 0xffff branch resumes at instruction 524 and executes
  the travel `0x73` to scene index 25 = **MAP** (`travel index=25 scene=map
  from=MR`). The object-click branch (door slot 2 -> label 265 -> 324) ends in
  the same travel. The shop frame in the user's third reference image is the
  scene where the conversation happens (the pre-conversation layer), not the
  post-conversation scene. The port now renders the original 640x480 surface:
  playfield at row 0x14 and the inventory strip at 0x1a4
  (`INVENTORY_AND_CLOSEUP.md`). The strip's top menu and item-on-object use are
  still outside this layer.

## 6. Real results

MR save `30.btb`, scene entry + `--run-label 413`:

```
conversation record=2 name=IMhMR2a stem=IMhMr2a clips=30 texts=30 trailing_bytes=0 object_context=0
conversation_line ... index=1 clip=Mr2meh01 speaker=meh text=no media=no group=0
conversation_line ... index=3 clip=Mr2meh03 speaker=meh text=yes media=yes group=2
conversation_line ... index=4 clip=Mr2meh04 speaker=meh text=yes media=yes group=2
conversation_answers ... group=2 choices=3
  choice=0 line=18 clip=Mr2pro03 text=yes media=no
  choice=1 line=19 clip=Mr2pro04 text=yes media=no
  choice=2 line=20 clip=Mr2proXA text=yes media=no
...
conversation_end record=2 name=IMhMR2a result=65535 lines=8 answer_lists=4 media=7 instructions=43
conversation record=2 name=IMhMR2a result=65535 lines=8 answer_lists=4 media=7 resume_instruction=524
conversation_branch none (0xffff: script continues after the 0x70)
run_label 413 click_result steps=3 ... fingerprint=0x1e6d3b1d25532648
conversation_variable local=14 before=0 after=1
conversation_variable local=15 before=1 after=2
conversation_variable local=16 before=0 after=1
conversation_variable local=17 before=0 after=1
conversation_state locals=300 changed_locals=4 objects=44/44 hotspots=2/2 records=0/0
```

The conversation changed only four of the save's 300 scene locals; objects,
hotspots and records are untouched. With choices `1,0,0,0,0,0` the resumed
segment starts at instruction 524 and stops after 3 instructions (the
`0x71`/`0x9d`/`0x76`/`0x6f` tail of label 413) with fingerprint
`0x1e6d3b1d25532648`.

A different real answer path (`0,0,0,0`, with the same `--inventor/--hotitems`
sets scene.sh supplies) continues from 524 through `0x9d` to the script's `0x73`
at instruction 547 and **switches to MAP**:

```
conversation_resume_probe steps=18 stopped_at=547 switch=25
run_label 413 click_result steps=18 ... fingerprint=0xc8470f2394d7e98a
travel index=25 scene=map from=MR pool=3000 locals=300
  pool_changed=7 (8,79,80,480,713,714,715)
  from_locals_digest=0xa07b393d3bdfad23 pool_digest=0xf6e4b1d4351ee40d
  locals_digest=0xee703ffe4f8a3ec1 record_preload=17
entry scene=MAP steps=147 stopped_at=133 hotspots=8 records=17
  locals_digest=0x75aea108d0e56efa
```

The conversation's four writes are therefore part of the state MAP receives.
(Without `--inventor/--hotitems` the resumed `0xab` at instruction 550 stops
with `unresolved_dialogue_inventory` and the click is not committed; that is the
honest data blocker, not a conversation failure.)

Object-click path (`--conversation-object 2`, a click on the shop door during the
answer list):

```
conversation_object record=2 slot=2
conversation_end record=2 name=IMhMR2a result=2 lines=3 answer_lists=1 media=2 instructions=17
conversation_branch object=2 label=265 target=324
run_label 413 click_result steps=20 ... fingerprint=0xefcc0d48e65d85ef
```

This is the real branch to label 265 (the door action), now caused by an actual
scene click during the conversation, not by operand 0.

Interactive verification with SDL mouse events (`--selftest-events`,
SDL dummy video/audio): four `conversation_pick` events through the real answer
rect hit test, the same `result=65535` and `changed_locals=4`. With the same
choice sequence (`0,0,0,0`) and the same inventor/hotitems tables, the
interactive run and a headless `--shot` run both resume with `steps=18`,
`switch=25`, the same `pool_changed=7` and fingerprint
`0xc8470f2394d7e98a`; the `1,0,0,0,0,0` headless run above instead takes the
3-step tail.

Answer geometry from `--conversation-shot` (the same rects the hit test uses):

```
conversation_answer_rect choice=0 x=100 y=250 w=234 h=20
conversation_answer_rect choice=1 x=100 y=270 w=440 h=40
conversation_answer_rect choice=2 x=100 y=310 w=440 h=60
```

The 6 wrapped Turkish lines (1 + 2 + 3) end at y=370 and the green subtitle
starts at y=420, exactly the `0x19f - 0x2d` and `0x1a4` anchors. Hovering the
third option logs `conversation_hover choice=2 logical=300,330` and redraws it
white while the other two stay orange.

Text lifetime observed in the same selftest run:

```
conversation_caption source=cleared reason=start
conversation_caption source=npc index=3 visual_lines=1
conversation_caption source=npc index=4 visual_lines=2
conversation_pick record=2 choice=0
conversation_caption source=answer line=18 visual_lines=1
conversation_caption source=npc index=9 visual_lines=1
...
conversation_caption source=cleared reason=end result=65535
```

So opening the answer list keeps the NPC line, the selected answer becomes the
next subtitle, the following NPC line replaces it, and the caption is cleared at
the conversation end. The scene shot after the run is MAP with no caption.

Three 640x480 images for the same conversation (`0,0,0,0`; the first two were
captured during that run, the third is the final `--shot` padded into the
original 640x480 geometry):

- `port/output/recordings/shot_talk.png` — a 640x480 frame of the real-device
  recording: `Mr2meh03` video at (0,20) 640x400, black top/bottom bands, green
  "Ah, gazeteci. Gelin, girin.".
- `port/output/recordings/shot_answers.png` — the headless first answer list over
  the last `Mr2meh04` frame (three orange rows, star bullets, x=100); the
  subtitle stays green at the bottom.
- `port/output/recordings/shot_answers_hover.png` — the same list in the real
  window with the pointer on the third option: it is white while the other two
  stay orange (log: `conversation_hover choice=2 logical=300,330`).
- `port/output/recordings/shot_after_480.png` — MAP after the 0x73 travel, no
  conversation text.

Real-window/real-device recording (not a dummy audio claim):
`port/output/recordings/mr_new_layout.mp4`, 9 s, 1280x960 H.264 + AAC 48 kHz
stereo, mean -21.8 dB, max -0.7 dB. It starts 1.8 s into `Mr2meh03`, shows the
full-playfield video and its green Turkish subtitle, then cuts to the next
`Mr2meh04` subtitle. The recording's audio envelope against the cached
`Mr2meh03` `audio.wav` has correlation **0.999** at offset **-1.825 s** (the
recording began 1.825 s into the clip), so it is the correct clip's PCM, time
aligned with the frame loop; the AVI is the only audio source. The
`--no-audio` and unavailable-device runs print `conversation_audio … ERROR …`
per clip and a `conversation_audio ERROR N clip(s) …` summary (tested with
`SDL_AUDIODRIVER=invalid`).

## 7. Playable continuation (natural click path, persistence, cancel)

### Natural start from the save

Save `30.btb` starts at MR label 0 with hotspots 42 (door, action 165) and 43.
The real click chain is:

1. click the door at logical `(352, 201)` (hotspot 42) -> label 165 loads
   `nodes/mr/MR02.jpg` at staged_x=1815 (`yaw=82`); hotspots 1,2,4,5 are active.
2. click logical `(300, 300)` -> inverse panorama mapping hits source
   `(1796, 504)` = hotspot 1, action 261. Label 261 loads `nodes/mr/MR01.jpg`
   (staged_x=824, yaw=-61), rotates to column 2300 (yaw=151.596) and reaches the
   conversation `0x70` at instruction 523 (`record=2`, object context 0).

This is the same `hotspot_at` / `execute_plan` path the mouse uses; no label
413 is forced. The first list is the shipped `IMhMR2a` group 2.

### Same-scene state

The original allocates `DAT_00482ad0[index]` once in `0x410669` and
`0x432d6e` resets only the per-call cursor (`state+2`), mode (`+0xa`) and the
chosen array (`+0xc0`). The entry flags (`enabled`, `unspeak`), line overrides
(`+0xf2`/`+0x490`), skip arrays (`+0xf/+0x41/+0x73`) and the default bytes
(`+0xb`/`+0xd`) stay in the state. `0x42c8a0`'s scene load calls
`0x41316f -> 0x40fbf6 -> 0x40f98c`, which frees every conversation state; a
`0x73` scene switch therefore resets it. The port stores this slice in
`SceneState::conversation_memory[record_index]`; `travel_to` builds a fresh
`SceneState`, so the map does not cross a scene switch.

Measured on the real `IMhMR2a` with choices `1,1,1,1`:

```
conversation_memory record=2 asked=0 disabled=0 done=no default_line=255 ...
conversation_end ... result=65535 lines=7 answer_lists=3
...
conversation_memory record=2 asked=3 disabled=4 done=yes default_line=5 ...
conversation_end ... result=65535 lines=1 answer_lists=0
```

After the first conversation the script resumes at 524 and returns to **the same
MR scene** (`hotspots=17,18,19,20,21`, panorama yaw=180 when started from label
413) instead of travelling. Clicking active hotspot 17 (action 396) re-enters
record 2; the log proves the asked/disabled flags and `state+0xe`/default line
survived. This re-entry is a real hotspot action, not a forced label.

The post-conversation camera in the natural door/NPC chain is yaw=151.596 and
the active hotspots 17..21 are outside the 640-wide viewport; the player must
rotate the panorama (arrow keys in the port, on-screen arrows in the original)
before the re-talk click. `--print-hotspots` reports the actual logical rectangle
and centre for every active hotspot under the current camera, and logs
`not_visible` when none exists.

### Cancel

The original ESC/right-click is not an immediate cancel. `0x414cd2` sets
`DAT_0045f058`; `0x436b01` opens `FUN_0041d6df(9)` (`0x41d6df`, proc
`0x41d523` with buttons 1/2/6/7). `MESSAGES.TXT` is read 0-based by `0x41d45a`:
id 9 is the “Kaydetmeden oyundan çıkmak istiyor musunuz?” message and labels
0x4c..0x4f are EVET/HAYIR/TAMAM/İPTAL. Only result 6 (EVET) returns `0xfffe`
and sets `DAT_0045f48c`; results 1/2/7 return to the same answer list.

The port draws that dialog with the real strings (`--messages`); ESC,
HAYIR/TAMAM/İPTAL resume without recreating the session, so the current
choices, `ConversationMemory` and variables are preserved. EVET/6 calls
`ConversationSession::cancel()` (0xfffe, `state+0xe` stays clear). The full
evidence is in `INPUT_AND_CANCEL.md` §1.

After 0xfffe the original ends the scene session: `0x42d594` stores the script
pointer, `0x42c8a0` calls `0x41e289(0x41de4b())`, `DAT_0045f48c` makes
`0x41e289(6)` return 1, and `0x41535a` returns. It does not restore the
`0xb8` snapshot. The port leaves the scene committed (`hotspots=0`), shows the
explicit `R`/ESC notice and reloads only when the user presses R
(`scene_restart reason=conversation_session_exit_user_choice`). Silent reloads
were removed. `--conversation-cancel N` is the deterministic terminate path.

### Scene exit / return

The re-entry above ends with one line and resumes at 524, which travels
(`0x73`) to scene 25 = **MAP**. The travel log now shows
`conversation_memory reset=scene_load previous_entries=1`; the fresh MAP state
has no conversation memory, matching `0x40f98c`.

MAP's active hotspots 34,36,39,40,41,46,47,49 are the region/info toggles. The
return leg **is implemented and verified** (see
`analysis/rea/INPUT_AND_CANCEL.md` §5): slot 47/action 246 opens the region
overlay (record 27, Istanbul map; hotspot 13 enabled), and a second click at the
same logical point runs action 257 → `0x73 [31]` → `SHOW.LCX[31] = 27` → MR.
The earlier "no MAP click travelled back" observation came from testing only a
single click.

### Host input note (superseded)

The earlier note about a 2x-scaled X11 display came from
`Display::window_to_logical` multiplying window points by
`renderer-output/window` before `SDL_RenderWindowToLogical`, which applies the
same DPI factor again. SDL's `SDL_RendererEventWatch` already delivers mouse
event coordinates in renderer-logical space; the latest step uses them directly
and verifies real X11 pointer events at seven window sizes (report §1).

## 8. Tests and census

- New `conversation-test`: CCF/BYZ CRLF records, cd name pairs and the 0xff
  separator, a synthetic CCB with both group arrays and an empty slot, line ->
  answer -> branch/end, 0x39, 0x67/0x74 (including the variable-write log),
  same-scene memory (the chosen answer is hidden on re-entry), cancel returning
  0xfffe without setting `state+0xe`, and a cancelled list restarting unchanged,
  0x37/0x42 conditions, object-click result, unsupported opcode error, trailing
  bytes, truncated operand/missing group/group slot 50 rejections, and the real
  `IMHMR2A` BYZ record count/line (from `analysis/rea/engine_sample`).
- `save-test` now asserts the new 0x70 request + `apply_conversation_result` for
  the synthetic script and the real EA 1779 (record 3, object 3 -> label 544).
- `make -C port -j2 all test` passes all eleven suites.
- The VM census now treats the 0x70 pause as an **interaction** outcome:
  `blocking_total=0`; 80 of 7,693 labels require the interactive conversation
  (previously 36 of them were silently "completed" through the operand guess).
  Completed labels are 5,899 (5,877 return + 22 script_end); unresolved variable
  state remains 1,296; `records_created` 733 -> 730 and
  `skipped_node_commands` 1,925 -> 1,924 because those labels now stop earlier.

## 8. Verified exports

| Address | SHA-256 (16) | Role |
| --- | --- | --- |
| 0x410669 | 1a4831f4aa67ebf9 | conversation loader (.ccb/.ccf/.byz) |
| 0x4119e3 | beac71ca47ce006c | bytecode parser |
| 0x412762 | 96dc07ef7781917e | instruction-list parser |
| 0x412aea | 170b271ce08e8f98 | group-array parser |
| 0x412cc4 | 9b915451430256fb | 0x14-byte entry allocation |
| 0x43340a | 3a9555ec83f82adc | conversation VM |
| 0x432d6e | 3967dc020f1d5bf1 | outer conversation loop |
| 0x43688b | 19b2b583e3a1cea3 | line branch |
| 0x436b01 | 5c305d3e5540154f | answer list/input |
| 0x436652 | 3d298553bbe3de0e | operand word |
| 0x4367eb | 79b12278b4dd0b96 | entry selection (index + hops) |
| 0x4364ef | 1b28d5423b803207 | answer redraw record |
| 0x4365b5 | d918cf5058fe359a | redraw flag update |
| 0x43302b | 8d18e3e276603198 | default group on re-entry |
| 0x4330bf | 4bc9805263bf29c7 | byte skip table |
| 0x4331d2 | 013febececb95dc8 | pointer skip table |
| 0x424780 | c31b11acff556e9d | display/play one line |
| 0x40d2a1 | 7378a7da0065e607 | AVI lookup/start |
| 0x40ced0 | dabc11839d698c3d | speaker list |
| 0x406cfe | df4c09b307ceca77 | font registry (slot 0 fill) |
| 0x405bf4 | 898ae1b9123f325e | high-byte font index (`byte - 0x41`) |
| 0x406cbf | 62ebc4ffc90d31f4 | language gate (`DAT_0045d0ac`) |
| 0x40d2a1 | 7378a7da0065e607 | character-video lookup/start |
| 0x40d3af | 0441419c9da98e3e | AVI type/mode/position |
| 0x40d5ff | 0caca3a1f27c63f2 | character-video start |
| 0x424780 | c31b11acff556e9d | reply -> subtitle/video/animation dispatch |
| 0x424981 | a1357d81bcd27a50 | subtitle band clear + draw (55,420, slot 4) |
| 0x424b15 | 8e44bbff50c7ed4a | subtitle wrap into the 3-line buffer |
| 0x424c4e | 29da2f297277e3ea | subtitle buffer clear |
| 0x436b01 | 5c305d3e5540154f | answer list (x=100, wrap 440, 0x7f star) |
| 0x413bdd | e159355056f45abf | AVI player entry |
| 0x43a248 | 7d898e9bd016ed27 | IV41 mode/destination size |
| 0x43d643 | 8b65b199f1b577f7 | DirectDraw frame blit |
| 0x43d9c7 | b0fcc1a4cf07c9ff | Indeo decompress format |
| 0x41d45a | a44998bd0603574a | language string query |
| 0x411dfc | 554520e53cef4b24 | engine/<scene>cd reader |
| 0x4123f9 | e8d9635a75c74eae | line reader (.ccf/.byz) |
| 0x40f350 | 76ac6f66af4ead0c | fopen helper |
| 0x410f09 | d93bf9dd2f48a5e7 | name -> ldm/state index |
| 0x42db22 | 8df50fc5339a585b | main dispatcher (case 0x70) |

## 9. Try it

```
# interactive: entry -> the real MR conversation record 2 (label 413)
./port/scene.sh . MR 0 30.btb --entry --run-label 413
```

When the answer list appears, click any of the three answer rows at the bottom
(around logical y=274/302/350 in the 640x400 playfield; the window is 2x). The
first row is "Nasıl yani, `Afacan' mı?". The conversation ends at line 12; the
resumed tail is answer-dependent: some sequences stop after the 0x70, the
`0,0,0,0` sequence continues to the label's `0x73` and travels to MAP. During an
answer list, clicking the scene door returns object 2 and runs label 265; that
path is verified headlessly with `--conversation-object 2`.

Headless reproduction (about 60 s because the video plays in real time):

```
./port/scene.sh . MR 0 30.btb --entry --run-label 413 \
    --conversation-choices 1,0,0,0,0,0 --shot /tmp/mr-final.ppm
```

With `--conversation-choices 0,0,0,0` the same command resumes to the `0x73`
travel (MAP entry) instead. `scene.sh` prepares `CD.ALL`, `CCB.ALL`, `CCF.ALL`
and `Byz.all`, extracts only this scene's `dialog/` folders, and passes
`--cd/--ccb/--ccf/--byz/--dialog` (plus the inventor/hotitems listings the
resumed `0xab` needs).
