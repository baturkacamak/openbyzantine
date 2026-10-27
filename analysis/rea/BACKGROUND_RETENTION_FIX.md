# Background lost after an action without 0x69 — 27 October 2026

## User report

The scene jumped to a different place while the player was looking at a photo:
`./port/scene.sh . EA 602 26.btb`. The reported run bundle is
`port/output/logs/run-20261027T115813.469736Z-x3d7vvnq.*`.

## Observation (user runs)

`run-20261027T115813.469736Z-x3d7vvnq`, interactive, 5 clicks:

| Time | Action | Rendered background |
| --- | --- | --- |
| 7.39 s | slot 28 / action 1356 | `EADESK.JPG` |
| 8.16 s | slot 38 / action 1401 | `EABlotMs.jpg` |
| 8.75 s | slot 64 / action 1415 | `EAHKEY.JPG` (`drawn_ids=15`) |
| 9.30 s | slot 68 / action 1442 | **`EAEmShMs.jpg`** (`drawn_ids=16`) |

`EAHKEY.JPG` is the desk close-up covered with polaroid prints; `EAEmShMs.jpg`
is a shelf with framed photographs, a different picture of the room. The player
clicked the photo, and the picture under it changed.

The second run `run-20261027T114033.089234Z-6r6h69iv` shows the same shape at
11.2 s: action 1325 ran on `EARugDr.jpg` and the composed frame became
`EAEmShMs.jpg` even though nothing in the action loads a background. That case
was not replayed end to end, because its click reached the action through a
panorama coordinate that depends on the run's camera state (see Unknowns).

## Cause (static trace evidence, no binary analysis needed)

Action 1442 starts at instruction 1810 of `cc_all/EA` and executes six
instructions:

```
1810 0x6e slot 68    disable the clicked photo hotspot
1811 0x85 record 15  flag masks
1812 0x64 record 16  create the flipped polaroid (asset EATexflp)
1813 0x6e slot 65
1814 0x6d slot 66    enable the next hotspot
1815 0x6f            return
```

There is no `0x69` (background load), and
`byzantine-cc cc_all/EA --trace 1442 cb_all/EA ...` prints
`steps=6 stopped_at=1815 records_created=2` with **no** `background_id=` line.
The original keeps the loaded 0x69 picture until another 0x69 replaces it.

The port lost it: each action adopts its own `SceneTrace`, and
`pick_background()` returned `static_backgrounds(...).front()` whenever the
trace had no background. In EA the first static 0x69 entry is instruction 532,
`EAEMShMs` — so the frame fell back to a picture the action never selected.
That is the wrong-scene jump.

## Correction

`update_layer` already receives every committed trace. It now records the 0x69
entry name of that trace (`SceneView::committed_background`) and
`resolve_frame_background()` (new, `src/cc.cpp`) resolves the frame in this
order:

1. the trace's own 0x69,
2. the previously committed 0x69,
3. the scene's first static 0x69 entry (startup only: a scene that never
   loaded one).

A fresh `SceneView` (scene travel, scene reload, examine return) starts with an
empty memory, so no name leaks between scenes. The layer selection is
unchanged: a 0xb9 after a 0x69 still wins, and a trace with neither load keeps
the current layer. No background, record or hotspot was invented, and the
action still runs exactly the six instructions the script contains.

## Validation

```sh
make -C port -j2 all test                     # 14 suites + per-run logging checks
python3 analysis/rea/verify_ea_photo.py /tmp/byz-scene
```

`tests/scene_state.cpp` gained a regression case for the three resolution
steps (a 0x69 trace, a later trace without one, and a scene with no memory).

`verify_ea_photo.py` drives the real chain with the reported coordinates
(`--run-label 653 --click 28 --click-at 295,261 --click-at 314,248
--click-at 354,236`) and asserts that the post-action frame keeps
`EAHKEY.JPG`, draws record 16 (`EATexflp`), and never composes `EAEmShMs`.
The interactive (`--selftest-events`) path gives the same result.

Rendered frame for visual inspection (dummy SDL video, not a physical display
check): `port/output/ea-photo-fix/after-1442.png`. The photo pile stays on the
desk and the flipped polaroid appears on it.

Re-run without regressions: `verify_ea_layer.py`, `verify_ea_single_execution.py`,
`verify_ea_inline_voice.py`, `verify_examine_controls.py`.

## Unknowns

- The rug instance (`EARugDr` -> `EAEmShMs`, action 1325) has the same trace
  shape (no 0x69, an evaluation that stops in a conversation), but its
  interactive state depends on variables written by the preceding panorama
  clicks; only the photo chain was replayed end to end.
- Whether some other scene intentionally relies on "first static entry" as a
  per-action default is not proven; the port now uses it only before the first
  0x69 of a scene, which matches the startup path that produced the same name
  in both reported runs.
