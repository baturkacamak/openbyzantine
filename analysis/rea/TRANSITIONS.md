# Scene transitions — 0x40186e's blend family (28 October 2026)

The player used to cut: the departing frame stayed on screen until the new one
was composed, then it was replaced in a single `SDL_RenderPresent`. The shipped
engine blends the two pictures. This report records what was recovered, what the
port now does, and what is still an inference.

Evidence: `analysis/rea/pseudocode/functions/0x40186e.c`, `0x418bcf.c`,
`0x41920f.c`, `0x4196a4.c`, `0x419a51.c`, `0x419cf6.c`, `0x42bce1.c`,
`0x42bc8b.c`, `0x42be07.c`, `0x42c1b8.c`, `0x42c57e.c`, `0x418eb2.c`,
`0x41842b.c` (Ghidra exports, `BETRAYAL.EXE` hash recorded in
`decompile-index.json`).

## 1. The dispatch (recovered)

`0x40186e` installs a node and then:

```
switch (param_3) {                 // the engine transition code
case 0x65: FUN_0042be07(old, new, progress, 2); break;   // -> 0x42c1b8
case 0x66: FUN_0042bce1(old, new); break;                // 27 phases
case 0x67: FUN_00419cf6(old, new, progress); break;      // 0x4196a4 + 0x419a51
case 0x68: FUN_0041917b(); FUN_0041920f(old, progress, new); break;
case 0x69: break;                                        // nothing
case 0x6a: FUN_00419a51(new, progress);                  // 0x419a51 only
}
```

`param_3` is the code the loader received: `0x432133` maps a `0xb9` operand-3 id
(1 -> 0x67, 2 -> 0x65, 3 -> 0x66, else 100) and `DAT_00460664` carries the code a
preceding `0xbd` left (mode 0 -> 0x65, mode 2 -> 0x6a, mode 4 -> 0x67; modes 1/5
leave it untouched). 100 is the "no code" default, i.e. no blend at all.

Every routine blends the **playfield rectangle** `(0,0x14)-(0x280,0x1a4)`; the
inventory strip and the top bar are outside it and are not mixed.
`0x41842b(surface, 0x3e800)` = 640*400 pixels is the present used between steps,
and `0x41842b(surface, 0x4b000)` (640*480) is the full-screen variant.

## 2. The shapes (read from the routines)

| Code | Routine | Shape | Steps | Pacing |
|---|---|---|---|---|
| 0x65 | `0x42be07` -> `0x42c1b8` | linear cross-fade, `delta = (new-old)>>3` re-added per step, 2x2 block processing | 8 | present per step |
| 0x66 | `0x42bce1` (+`0x42bc8b`) | interleaved reveal: each phase copies every **27th** 16-bit word (`param_1 + param_3`, `+= param_4`), so the picture fills in a 27-step pattern | 27 | present per step |
| 0x67 | `0x419cf6` -> `0x4196a4` + `0x419a51` | two-stage fade; `0x4196a4` runs 4 steps with alpha `0x1f,0x3f,0x7f,0xff`, `0x419a51` then halves its alpha from `0x7f` | 4 + 7 | present per step |
| 0x68 | `0x41917b` + `0x41920f` | cross-fade with the RGB555 multiply table `DAT_00487710`; `local_14 = 2, 8, 32, 128` | 4 | **60 ms** per step (`do { now = timeGetTime(); } while (now < start + 0x3c)`) |
| 0x69 | — | none | 0 | — |
| 0x6a | `0x419a51` | the second half of 0x67 only | 7 | present per step |

`0x42c57e` is the same 8-step fade as `0x42c1b8` but row-sequential instead of
2x2-block; `0x42be07` picks it with mode 0 and either of them with modes 1/2.

## 3. What the port does

New module `port/{include/byzantine,src}/transition.*`:

- `transition_kind(code)` — the switch above; every other code is
  `TransitionKind::none`.
- `transition_steps` / `transition_step_ms` / `transition_weight` — the counts,
  the 60 ms gate and the alpha sequences.
- `blend_playfield(out, from, to, weight)` — per-channel RGB565 lerp, weight
  0..256 (`256` is exactly `to`).
- `interleave_playfield(out, to, phase)` — the stride-27 phase of `0x42bc8b`.

Host wiring (`port/src/play_main.cpp`):

- `Display::present_image` keeps the playfield of the last presented full screen
  in `last_playfield` (a presentation that is not a 640x480 screen or a 640x400
  playfield invalidates it instead of guessing).
- `Display::present_transition(view, code, source)` presents the intermediate
  frames and then the arrival through the single `present_image` path. Its frames
  run under `ScreenContent::transition`, so the hotspot-outline diagnostic is off
  them and the detail trace records the owner.
- `present_arrival` is the one arrival presenter: `travel_to` (entry and save
  paths), the close-up return and `restart_scene` all use it.
  `present_action_result` uses `present_transition` when the clicked label ran a
  `0xb9` (a node change inside the scene); a label that only loads a `0x69`
  background keeps the cut, because the original's blend lives in the node
  loader.
- The code is the recovered one when the scene carries it (`0xb9` operand 3 or
  the `0xbd`-set `DAT_00460664`). A flat `0x69` travel has none: the port uses
  `--transition`'s default (0x65) and logs `source=travel_default`.
  `DAT_00460664` is a process-global in the original, so `travel_to` now carries
  `pending_panorama_transition` across the `0x73` like the RNG state.
- Options: `--no-transitions` (cut), `--transition <code>` (default code for
  travels that carry none), `--transition-shot <dir>` (writes every step as a
  PPM). Each run prints
  `transition code=0x65 kind=fade8 source=travel_default steps=8 step_ms=16
  elapsed_ms=132`; the steps appear in the detail trace as
  `present … tag=transition detail=code=0x65 step=3/8`.

`port/tests/transition.cpp` (`transition-test`, part of `make test`) covers the
code table, step counts, the 60 ms pacing, the alpha sequences, the blend
endpoints/midpoint, channel isolation, monotonicity, the size-mismatch guard and
the stride-27 phases (exactly `i % 27 == phase` changed, 27 phases reach the new
picture).

## 4. Verified on real data

Same click chain in every run (`MAP` 87 / `27.btb` -> MR arrival conversation ->
MR exit -> MAP), headless dummy devices and a real Xephyr `:9` window:

| Run | Effect | Steps | Measured |
|---|---|---|---|
| `--transition 0x65` | `fade8` | 8 | `elapsed_ms=132` (8 x 16 ms) |
| `--transition 0x66` | `interleave27` | 27 | `elapsed_ms=437` |
| `--transition 0x67` | `fade7` | 7 | `elapsed_ms=115` |
| `--transition 0x68` | `fade4` | 4 | `elapsed_ms=243` (4 x 60 ms) |
| `--no-transitions` | — | 0 | `skipped reason=--no-transitions` |

Frames: `port/output/transition-demo/port-0x65.gif`, `-0x66`, `-0x67`, `-0x68`
and their `-sheet.png` contact sheets — these are the pixels the port presented,
written by `--transition-shot`, not a synthetic reconstruction.

The first travel of a headless `--click-at` run reports
`skipped reason=no_departing_frame`: those runs present nothing before the first
click, so there is no outgoing picture to blend from. A windowed session always
has one.

## 5. Boundaries (not claimed)

- **The per-`0x73` code is not recovered.** The original reads it from the
  travel-side snapshot `DAT_004603d0`/`0x439349`, which is still unmodelled, so a
  travel without a `0xb9` code uses the port default (0x65) and says
  `source=travel_default`. That is a port decision, not a recovered value.
- **`0x419a51`'s pixel roles are an inference.** The step count, the halving
  alpha and the "blit param_2 to the screen" order are read from the routine,
  but which surface holds the old and the new picture during its write pass is
  not settled here. The port implements it as an old->new cross-fade with new
  weights `0x81,0xc1,0xe1,0xf1,0xf9,0xfd,0xff`; the visible result is a quick
  fade, and `fade7` is the only kind whose arithmetic is partly inferred.
- **`0x4196a4`'s four extra steps for `0x67` are not modelled** (the port runs
  the 7-step half only). `0x67` therefore looks close to `0x6a` in the port.
- **`0x418eb2`** (a 4-step band blend with 60 ms gates, called from `0x431ec2`,
  not from the node switch) is not implemented.
- **Colour precision.** The original blends RGB555 through the multiply table
  `DAT_00487710`; the port blends RGB565 channels arithmetically. The visible
  difference is at most one least-significant bit per channel.
- **Pacing of the non-waiting routines** (16 ms per step) is a host assumption:
  those routines only present, so the original's step rate was its blit speed.
  `0x41920f`'s 60 ms is recovered and exact.
- No original-runtime capture is compared against; "matches the original" here
  means "implements the recovered routines with the recovered step counts and
  the one recovered timing".
