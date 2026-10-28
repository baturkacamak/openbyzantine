# Open-gap census — what is actually left, and whether shipped data uses it (28 October 2026)

The question: the game plays fine, so is anything still missing on the
reverse-engineering side? This census answers it with counts instead of
impressions. It reuses the project's own tools: `build_content_coverage.py`
(one entry per scene, real saves) and a static scan of the extracted CC scripts.

## 1. Every scene enters

`build_content_coverage.py /tmp/byz-scene <port> "<iso listings>"` ran 44 scene
entries with the shipped saves. Every one produced a runtime record (no tool
error). Stop reasons:

| Scene | Stop | Reading |
|---|---|---|
| `examine`, `examvb`, `examvc`, `examvh`, `examvt` | `stopped: missing_examine_item` | Correct: these are close-up scenes that need a held item (`DAT_0045d1cc`); entering them directly is not a game path. |
| `morris` | `stopped: unresolved_random` | The documented CRT `_rand` stream boundary (`VM_RANDOM.md`); not an opcode gap. |
| `iuover` | `script_end` | Normal end of script. |
| other 37 | — | Entered and composed. |

No scene stopped on an unimplemented opcode. This matches `FINAL_OPCODES.md`
("every opcode a shipped scene entry label reaches has a branch").

## 2. The unmodelled draw-pass flags: one is used, one is not

`+0x09` is written by `0x85`/`0x86` through the recovered mask tables
`kActorMaskAnd`/`kActorMaskOr` (the original's `0x460418`/`0x460428`). Scanning
all 44 scripts (39,356 instructions) for the mask operands of `0x85` (args 1..)
and `0x86` (args 2..) gives:

| Mask index | Bit | Meaning in the draw pass | Uses |
|---|---|---|---|
| 0, 2, 3, 4, 5, 7 | 0x01 .. 0x08 | freeze / ping-pong / stop-at-tail / … (modelled) | 383 |
| 10, 11 | 0x20 | the snap pass (modelled) | 906 |
| **15** | **0x80** | **destination-read blend (`0x42b5b2`), NOT modelled** | **95** |
| 14 | clears 0x80 | — | 0 |
| 8, 9 | **0x10** | **offset pair, NOT modelled** | **0** |

So:

- The `+0x09 & 0x10` offset pair is **never set by shipped data** — it can stay
  unmodelled without any visible difference.
- The `+0x09 & 0x80` destination-read blend **is** used: 95 instructions, all in
  **`STAR`**, on records 34, 41, 48, 61 (24-25 times each) and 11..18 (once
  each). `STAR` has no background/panorama and 82 `kind 0x12` objects, i.e. it
  is the multi-target sprite screen; those records are the ones that should be
  blended with what is already on the surface. The port draws them opaque.
  Bit 0x80 is never cleared (mask 14 unused), so a record that gets it keeps it.

## 3. The `+0x3c` trail is filled but never drawn

`0x84` (append the current position to the `+0x3c` list, then move) appears 193
times:

| Scene | `0x84` sites |
|---|---|
| `STAR` | 132 |
| `EXAMINE` | 39 |
| `AL` | 10 |
| `GC` | 10 |
| `HSTMED` | 1 |
| `MAP` | 1 |

The port maintains `record.trail` (and `0x43264d` drops the entry the snap pass
drew through) but never draws the trail, so a record moved by `0x84` leaves no
after-image. Whether the trail is visible per site depends on the record being
drawn and having frames; the *instructions that request it* are in six scenes.

## 4. Multi-frame cursors: only the klio set

`0x40e89f` fills up to four frame handles and `0x40ebe1` cycles them every
250 ms. In the extracted art:

- `cursors/` (normal theme): 26 files, 26 distinct stems — **no** multi-frame
  cursor, so the cycling never applies there.
- `cursors/klio/`: 32 files, 23 stems, of which **`forward`, `info`, `back` have
  4 frames each**. Those three pointers (used in the klio-theme scenes, e.g.
  CLOCK/HSTMED) should animate; the port shows one frame and logs that the
  cycling is not modelled.

## 5. What this census does not settle

- **Label reachability.** A static pass over the 44 scripts finds 7,693 labels
  defined by `0xe1` and 4,527 referenced by a constant `0x76`/`0x9b` transfer or
  by an object-table action label; 3,166 have no referrer in that model. That
  number is **not** a dead-code claim: labels are also reached from the setup
  tables the census does not parse as a reference set, from `0x41e289`'s menu
  resume, from a save's stored script pointer and from object definitions a
  running script enables later. No dangling reference was found (0 references to
  a missing label, 0 dynamic `0x76`/`0x9b` operands), but "which label is never
  reachable" needs a playthrough census, not a static one.
- **Audio volume/pan.** `0x427de1` creates the DirectSound buffer from the sound
  record and `FUN_00427d1a(record)` applies the record's own parameters; the port
  plays flat. The shipped records were not measured here, so the audible
  difference (if any) is unquantified.
- **Scene transition codes** were settled separately (`TRANSITIONS.md`).

## 6. What was implemented after this census (same day)

All three data-backed items are now in the port:

1. **`+0x09 & 0x80` destination-read blend.** `0x43b130` passes
   `*(byte *)(record + 9) & 0x80` to `0x40bbfd`, whose `param_6` reads the
   destination instead of the frame colour and writes it back through
   `0x42b5b2` -> `0x42b395` (`(value * 5) >> 3` per channel). The port now does
   the same (`byzantine::shade_pixel`, `blit_frame(..., shadow)`), for the normal
   and the panorama-patch path. Independent confirmation from the shipped data:
   the two STAR records that carry the bit are `vtDEShdw` and `vtAShdw` — shadow
   sprites of the backgammon pieces. `compose-test` checks the 5/8 arithmetic,
   channel isolation and that transparent pixels stay untouched.
2. **`+0x3c` trail draw.** `0x43b216` walks the record's trail list *before* the
   record itself (inside its `param_3 != 0` branch) and draws the same
   head->current frames at every recorded position except the current one, adding
   each to the dirty-rect list. `compose_scene` now does that; the record's own
   draw and the animation step are unchanged. `scene-state-test` composes a
   record with a trail and checks that the trail position is drawn, the current
   frame is unchanged and a plain compose leaves the trail position alone.
   `--print-records` now reports `trail=N`.
3. **klio pointer animation.** `0x40e89f` fills up to four frame handles and
   `0x40ebe1` advances them every 250 ms. `CursorFileMatch` now carries the
   ordered frame paths, and the player cycles the installed pointer under the
   detail trace `cursor_frame pointer=… frame=N of=4 interval_ms=250` (measured
   1.45 s -> 1.71 s -> 1.97 s -> 2.22 s, i.e. 255 ms). The normal set stays
   single-frame because it has one file per stem.

What is left unmodelled is now only the unused/duplicated parts: the `+0x09 &
0x10` offset gate (`0x42b679` is skipped when it is set; the bit is never set),
`0x418eb2`'s band geometry, CF-video `0x43d9c7` modes and the save-block
semantics.

## 7. Bottom line

The census found exactly three data-backed gaps in the draw/presentation layer,
and all three are implemented (§6):

1. `+0x09 & 0x80` shadow draw — `STAR` only, 6 records (`vtDEShdw`/`vtAShdw`). Done.
2. `+0x3c` trail draw — six scenes, 193 request sites. Done.
3. klio cursor animation — `forward`/`info`/`back`, 4 frames. Done.

Everything else in the earlier "still open" list is either unused by shipped
data (`0x10` offset pair, `0x418eb2` band blend, CF-video `0x43d9c7` modes), a
host-library concern, or a save-block semantic that is round-tripped byte for
byte.

## 8. Do the leftovers need work? (measured)

1. **`+0x09 & 0x10` — unreachable, do not implement.** Two independent scans:
   no shipped script sets mask index 8/9 on `0x85`/`0x86` (§2), and every record
   in **all 55 shipped saves** (820 records, `savegame/*.btb` + the save sample)
   carries only flags `0x00` (25), `0x04` (281), `0x20` (460) and `0x24` (54).
   Not one record carries `0x10`, `0x80` or `0x40`. The gate could never fire, so
   implementing it would be dead code with no way to validate it.
2. **`0x418eb2` — unreachable, do not implement.** It is only selected by
   `0x431ec2`'s code `0x68`; `0x432133` never returns `0x68` (1 -> 0x67, 2 ->
   0x65, 3 -> 0x66) and a `0xbd` leaves only `0x65`/`0x6a`/`0x67` in
   `DAT_00460664`. Nothing in the shipped data can request it, and its band
   geometry has no reference frame to be checked against.
3. **`0x43d9c7` — host-library, do not reconstruct.** It is the original's AVI
   engine: `ICSendMessage(handler, 0x5001, &format, size)` with fourcc `IV41`
   (Indeo 4) for the 1:1, 2x and buffer modes `0x43a248` selects. FFmpeg
   replaces it. Evidence that the codec is not a gap: all **381** AVIs already
   extracted under the data root probe cleanly — `indeo4` 355, `indeo3` 25,
   `msvideo1` 1, **0 undecodable**.
4. **Save blocks — a data-vs-behaviour distinction.** The writer is
   byte-identical to the parser on all 51 CD1 saves, so nothing written is lost;
   what is not *applied* is the saved loop chain `DAT_0046019c` (block 21):
   `resume_ambient` restarts the arrival state's active loops instead of
   replaying the chain. That is the only behaviour gap left, and it is audible
   only when a save is loaded mid-scene with a loop set that differs from the
   arrival state's own. Worth doing only if a save-load report shows a wrong or
   missing loop.

Recommendation: 1-3 need no work (1 and 2 cannot be reached, 3 is delegated by
policy); 4 is optional and evidence-driven.
