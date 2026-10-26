# One AM conversation end to end: Nuray (`Inuam3a`, record 0)

Date: 26 October 2026.

This step replaces the four-record AM interpreter sweep with **one** real
interaction. It starts from the shipped AM save `62.btb`, reaches the
conversation with normal panorama input (no `--run-label`), plays the shipped
AVI/PCM media through the existing FFmpeg cache + SDL2 audio path, and runs the
script continuation after the `0x70`.

## Start state and normal click path

Save `62.btb` is AM, at the museum courtyard (`AM03`, entry yaw 180). The
entry enables eight hotspots (82..88, 90). The conversation object is slot 92
(`action 397`, source rect `50,70 550x250`, mode 10); it is not visible from the
initial camera.

The natural path found by the common action graph (not a label shortcut):

1. Rotate the panorama toward source x1470. Slot 88 (`action 324`, source
   `1470,398 62x88`) becomes visible (the test clicked its screen centre
   `55,248` at yaw ~50). Clicking it runs action 324 and switches to the next
   AM view; the trace reports `click_result steps=21 … active_slots=2`
   (slots 91 and 92).
2. Rotate back toward yaw ~150. Slot 92 becomes visible at screen
   `621,0 19x96` (centre `630,47`). Clicking it runs action 397 and reaches the
   conversation request at instruction 466:
   `conversation record=0 instruction=466`.

Both clicks and the camera turns are the normal player controls (`Left`/
`Right` and a left click); no `--run-label`, `--click`, or hidden state write is
used in the end-to-end run. The test's X11 pointer events are XTEST on an
isolated server (the same caveat as `MOUSE_INPUT.md`).

## The conversation

Record 0 is `Inuam3a` (`stem=INuAM3a`, speaker `nur`, 11 clips/texts, 22
instructions in the run):

```
conversation record=0 name=Inuam3a stem=INuAM3a clips=11 texts=11
conversation_line record=0 index=1 clip=NA1nur01 speaker=nur text=no media=no
conversation_answers record=0 group=1 choices=3
  choice=0 line=8 clip=NA1pro01 text=yes media=no
  choice=1 line=9 clip=NA1pro02 text=yes media=no
  choice=2 line=10 clip=NA1pro03 text=yes media=no
conversation_pick record=0 choice=0
conversation_line record=0 index=4 clip=NA1nur04 speaker=nur text=yes media=yes
conversation_media clip=NA1nur04 source=na1nur04.avi frames=666 fps=15 size=320x200 audio=pcm
conversation_audio clip=NA1nur04 wav=audio.wav duration_ms=44849 queued_bytes=3955720 device=SDL2 22050Hz 2ch
conversation_end record=0 name=Inuam3a result=65535 lines=2 answer_lists=1 media=1 audio_failures=0 instructions=8
```

The answer row was a real click at logical `(304,310)` in the 640x480
conversation screen. The selected line's media is the shipped
`dialog/inuam3a/na1nur04.avi` (Indeo 4 + PCM16 22050 Hz stereo, 320x200); the
existing cache decodes it with FFmpeg, the port draws the 320x200 frame at
source*2 and queues the PCM through SDL2. No media was invented or substituted.

## Script continuation

`0x42db22` case 0x70 returns `0xffff`; the port resumes at instruction 467 and
runs the original continuation:

```
conversation record=0 name=Inuam3a result=65535 … resume_instruction=467
conversation_branch none (0xffff: script continues after the 0x70)
conversation_resume_probe steps=2 stopped_at=468 switch=none sounds=0 blocker=
click_result steps=2 dialogue_visible … active_slots=0
```

The two resumed instructions are `0xb7` (restore the suspended object bits) and
`0x6f` (return). There is no random call and no scene switch.

**Honest boundary:** the action's own pre-conversation code (`0x6e` and its
called labels) runs before the `0xb8` at 465; by then the state has
`enabled=0/0` and `suspended=0/0`, so the `0xb7` at 467 has an empty snapshot
and the post-conversation scene has 0 active hotspots. That is the shipped
script's outcome, not a skipped continuation. The original's out-of-script
close-up/cursor-zone/map UI after this conversation is not modelled; the port
does not invent hotspots to hide it. The event log records
`conversation_state … enabled=0/0 suspended=0/0`.

## Recording

`port/output/recordings/am_nuray_end_to_end.mp4` — 60.73 s, 1280x960 H.264
30 fps + AAC 48 kHz stereo (mean -20.3 dB, max -0.7 dB):

- normal AM camera turns and the two clicks (slot 88 → slot 92);
- the answer list over the live conversation screen;
- the real Nuray AVI and its voice, full playfield;
- the post-conversation continuation.

The full player stdout is `port/output/recordings/am_nuray_end_to_end.log`
from the recording run.

## Map destinations

The black TK window was reproduced with `travel index=33 scene=tk` from MAP:
the entry ran, two hotspots were active, but `composed background=none` because
the partial test data dir had no `backs/tk/TKXA.jpg`. After extracting the
shipped `backs/tk/*` (and `nodes/am/*` etc.), the same travel reports
`composed background=TKXA.jpg` and the scene is the real TK door with hotspots
1 and 2. The travel code is generic: it resolves the target's sibling
`cc/cb/co/ci/cy/cf/cs/ca/ce` files and runs the target entry.

Two common safeguards were added:

- `port/scene.sh` writes a `.media_v2` marker after extracting all
  `backs/*`, `nodes/*`, `anims/*`, `sounds/*` from every CD; an older data dir
  without that marker is backfilled once on the next launch, so a map
  destination is not black because its media was never extracted. The dialog
  folders are already extracted per scene's `cd` records on each launch.
- `refresh_background` logs
  `background_missing scene=… name=… root=… (extract/backfill backs/ …)` when
  the script names a background that is not on disk, instead of leaving the
  window silently black. A missing panorama is already reported by the
  `panorama ... missing under ...` path.
- `0x72` now creates the `DAT_00464300[slot]` object with the rect (y =
  operand 2 + 0x14), kind = operand 6, label = operand 5 and, for kinds
  0x13/0x14, name id = operand 7; other kinds need `FUN_00408bb2`'s string-table
  resolution, which is not modelled, so they keep the original `0xffffffff`
  unset marker. `0x6d`/`0x6e` keep the object's `+0x08` active byte in sync. A
  save object or an earlier creation is not overwritten.

The 17-destination MAP census (`MAP` action → `SHOW.LCX` index) resolves every
target and runs every entry to a normal return (VB used to stop on
`missing_object=4`), composing each background or panorama:

```
223 ft  240 bc  257 mr  263 tk  291 hslower  308 am  325 as  342 hl
359 bb  376 gt  393 gc  410 nb  427 iu  444 sm  461 ea  478 sp  497 vb
```

The AM destination works with the same generic travel: MAP action 308
(`0x73 [4]` → LCX index 2 = AM) loads AM and its entry. The Nuray interaction
above was reached from the AM save directly; a map→AM→conversation run uses the
same entry state.

A forward map selection was also driven with real X11 clicks (not
`--run-label`): on `39.btb`, clicking logical `(289,63)` selects the GT location
(slot 37/action 365) and a second click at the same point runs slot 2/action 376,
i.e. `0x73 [15]` → `travel index=15 scene=gt`, `entry scene=GT steps=58 …
hotspots=1`, `composed panorama=nodes/gt/GT00.jpg`. Recording:
`port/output/recordings/map_click_to_gt.mp4`; log and frames alongside it. The
coordinate is example-specific; select-then-go and the travel are common.

## General support vs example-only

General:

- the scene travel/load path, sibling table resolution, media path lookup and
  entry execution;
- the conversation VM/media/audio/answer/continuation path;
- the `0x72` object declaration and `0x6d`/`0x6e` active byte;
- the missing-background diagnostic and the launcher media backfill.

Only this AM example:

- the two specific hotspot actions 324 and 397 and their camera positions;
- the `Inuam3a` record 0 text/choices/media;
- the post-conversation empty hotspot set (the script disables its own view
  before the 0xb8).

The previous four-record AM sweep is retained only as an interpreter
regression (`conversation-test`/the 0x45/0x46 commit); it is not the end-to-end
claim of this step.
