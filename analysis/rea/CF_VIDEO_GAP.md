# CF videos (`0xbd`) are never played — the NB stairs action is a silent no-op

27 October 2026. **Detection only; no port code was changed.** This report
separates what the log proves, what the saved binary exports explain, and what
is still unproven.

> **Fixed 27 October 2026.** The playback implementation, logs and validation
> are in `analysis/rea/CF_VIDEO_PLAYBACK.md`. This file stays as the detection
> evidence for the pinned user run.

## Pinned user run

`port/output/logs/run-20261027T153102.008778Z-wo0x6pwm` —
`port/scene.sh . MR 0 30.btb --entry`, normal windowed launch (`DISPLAY=:1`),
108.69 s, exit 0, no signals. Git HEAD `85df6cd3ded51e76a8d52ff050dd4c162b3411f8`
with a clean tree; player SHA-256 `019f1a11…be88`.

Journey actually played: MR entry → MR door (slot 42/165) → `MR02` panorama →
MAP (index 25) → MR (27) → MAP (25) → NB (29) → bartender conversation
`Ibrnb2a` (record 2, 3 lines, 1 decoded AVI, `audio_failures=0`) → `NBOKEY` key
close-up → back → `NBSTAIRS` (slot 5/233) → `NBDOWN` (slot 12/337).

## Symptom in the log

Four consecutive clicks on slot 12 produced the same committed result and left
the VM state bit-identical:

| t | click | result |
| --- | --- | --- |
| 96.171 s | `526,558` slot 5 action 233 (before) | `steps=23 sounds=0 fingerprint=0xda96707b0139e3e9 active_slots=2` |
| 98.259 s | `300,338` slot 12 action 337 | `steps=15 sounds=6 fingerprint=0xda96707b0139e3e9 active_slots=2` |
| 99.482 s | `324,336` slot 12 action 337 | same |
| 101.722 s | `343,317` slot 12 action 337 | same |
| 103.978 s | `330,329` slot 12 action 337 | same |

Only the picture changed, once: `composed background=NBSTAIRS.jpg` →
`composed background=NBDOWN.jpg` (`drawn=0 pixels=0`, the scene's only record
belongs to the panorama). After the first click nothing else happens on screen
or in the state, so the last three clicks are pure no-ops. The run then ends
without further action (the user quit).

Not a hit-test problem: `click_plan scene=NB slot=12 active_slots=12,13,`
matches every time, `vm_end … steps=15 stopped_at=414 blocker=` completes, and
`state_adopted … steps=15 stopped_at=414` commits. The two `possible_stalls`
entries the summary lists (`active_slots=0`, 66.8 s and 72.9 s) are MR's own
`0x73` scene switches, not this issue.

## What label 337 actually contains

NB script (`port/output/cc_all/NB`), label 337 → first instruction 410
(`byzantine-cc --labels`, `--flow`), 15 instructions, exactly as traced:

| instr | opcode | operands (trace) | meaning |
| --- | --- | --- | --- |
| 410 | `0x69` | type 2 / 3 | load background → `NBDOWN.jpg` |
| 411 | `0x9b` | 133 | call block at 145 |
| 145–147 | `0xbc` | 0, 3, 1 | stop ambient voices 0, 3, 1 |
| 148 | `0x9b` | 128 | call block at 139 |
| 139 | `0x6c` | 0 | clear media record 0's active flag |
| 140–142 | `0xbc` | 2, 6, 7 | stop ambient voices 2, 6, 7 |
| 143 | `0x6f` | — | return (to 149) |
| 149 | `0x6f` | — | return (to the click dispatcher) |
| **412** | **`0xbd`** | **raw 1, raw 4** | **play CF record 1 in mode 4** |
| 413 | `0xd2` | — | autosave request |
| 414 | `0x79` | — | end of script |

So everything the action changes in the VM (background, six ambience stops, the
media flag) the port already does. The remaining payload is the video — the one
thing the player drops — and an autosave request that is deliberately
request-only.

### The clip

`cf/NB` record 1 = `MCaNB1a` (`source=NB.CCG`, type 255). Mode 4 selects
`video\char`. The file is shipped on every CD:

```
Byzantine_2.iso  video/char/MCANB1A.avi   1 209 950 bytes
ffprobe: Indeo 4 + PCM s16le, 320x200, 15 fps, 63 frames, 4.20 s
```

The save's disc is 2 (`animation_media disc=2 overlay=/tmp/byz-scene/disc-media/cd2-…`),
so CD2's copy is the authoritative one.

## Binary evidence (hashes re-checked against `decompile-index.json`)

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; all four
exports match their recorded `sha256` exactly.

* `0x42db22` (`8df50fc5339a585b…`) `case 0xbd`: `FUN_00436652(*param_1, 0)` and
  `(…, 1)` — operands 0 and 1 are used **raw**, not resolved through a scene
  variable (operands 2/3 only when their type byte is `0x40`). So record 1,
  mode 4 is not a variable read; the port's `encoded_word()` use is correct.
* `0x40d031` (`3c03f813d14edff7…`): builds `avi/%s.avi`, falls back through
  `0x40c624` to `video\<folder>\<name>.avi` for the mode, then calls
  `0x40d3af(path, mode, start, end, local_8, 1)`. For mode 4 it sets
  `DAT_00460664 = 0x67` and `local_8 = 2`; for any mode except 5 it also sets
  **`DAT_004859cd = 1`** (the flag the save loader and `0xd4` set, which turns
  off the `0x43b216` snap pass).
* `0x40d3af` (`0441419c9da98e3e…`) → `FUN_00413bdd`.
* `0x413bdd` (`e159355056f45abf…`) plays the clip in a blocking loop
  (`while (DAT_0045f060 == 0) { … }`), so the script governor waits for the
  whole 4.2 s before executing `0xd2`/`0x79`.

## Port state

* `port/src/cc.cpp:1861-1913` resolves the CF record, applies the gamepref gate
  for modes 2/3 and stores a `SceneVideoRequest` in
  `SceneTrace::video_requests` (plus `pending_panorama_transition`, which
  `0xb9` does consume — `port/src/cc.cpp:2316`).
* **Nothing in `byzantine-play` reads `video_requests`.** The only consumer is
  the CLI tracer, `port/src/cc_main.cpp:814`. There is no playback, no blocking
  wait and **no log line at all**, so a request disappears without a trace even
  in the detailed `.trace.log`.
* The `DAT_004859cd = 1` side effect of `0x40d031` is not applied (only `0xd4`/
  `0xd5`/the save loader touch that flag: `port/src/cc.cpp:1722`, `:2733`).
* `port/scene.sh` never extracts `video/`; the prepared data root has no `avi/`
  or `video/` tree, so even a playback implementation would find nothing to
  open. The `avi/<name>.avi` → `video\<mode>\<name>.avi` redirect (`0x40c624`)
  is not modelled either.

## Does it repeat elsewhere?

**In the same 108 s run, three `0xbd` commands in three different scenes were
dropped:**

| scene / label | instr | operands | CF record | clip | length |
| --- | --- | --- | --- | --- | --- |
| MAP 257 (slot 13, travel to MR) | 300 | 2, 2 | 2 = `MREst1` | `video/site/MREST1.AVI` | 10.67 s |
| MAP 410 (slot 6, travel to NB) | 480 | 11, 2 | 11 = `NBEst1` | `video/site/NBEST1.avi` | 12.07 s |
| NB 337 (slot 12, stairs down) | 412 | 1, 4 | 1 = `MCaNB1a` | `video/char/MCANB1A.avi` | 4.20 s |

The two MAP ones are *destination establishing shots*: `MAP`'s CF table is
16 `*Est1` records, one per travel destination (FT, BC, MR, TK, HS, AM, AS, HL,
BB, GT, GC, NB, IU, SM, EA, SP), and labels 257/410/… place the `0xbd`
immediately before the `0x73`. In this run the port cut straight from the map
click to the new scene, so 22.7 s of intended footage was skipped without an
error.

**Game-wide (documented, `analysis/rea/VIDEO_AND_AUDIO.md`):** 347 `0xbd`
commands across all 44 scenes — mode histogram `5×110, 3×89, 4×38, 0×30,
2×24, 1×2`, of which 113 are preference-gated and the shipped
`GAMEPREF.000` (`1 1 1 1 1 1 0`) enables every gate. CD1 alone ships 55
`video/` files (171 MB). Every one of them is request-only today.

*Census caveat:* the quoted mode histogram sums to 293, not 347; the report does
not say where the remaining 54 sit (the default/other-mode bucket). The order of
magnitude is not in doubt, but the exact total should be recounted before it is
used as a completion metric.

Already-documented instances of the same gap:
* `EXAMINE` label 288 (the examine close-up's camera click) → CF `exCam`
  mode 0; `EXAMINE_ACTION_REGRESSION.md` explicitly says the clip is not
  claimed to play.
* `EXAMINE` label 759 → `hsain1` mode 3.

**Boundaries of this detection.**
* No video-only label other than NB 337 was *proven* exhaustively; an
  instruction-tail dump over all 7 693 labels would be needed to enumerate every
  "the click only plays a clip" action. The static count above shows how much
  content the class covers.
* The missing clip is not why the scene stops: label 337 contains no `0x73`, so
  the original also ends on `NBDOWN.jpg` with slots 12/13 active. Slot 13
  (action 304) therefore remains the way on; the run is not a hard dead end,
  it is an unsatisfying "click did nothing" loop of 4 clicks.
* Two `hit no hotspot` clicks at 58.99 s and 60.48 s in the MR desk close-up
  (action 239) are **not** explained by this: that view draws records 6/7 but
  exposes only slot 8 (back), because the record-conditional `0xb1` calls at
  instructions 363 and 351 did not take the drawer branch from this save's
  state. Whether that matches the original needs its own check.
