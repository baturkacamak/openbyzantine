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

## 1. Two dispatchers, one family (recovered)

**`0x40186e`** installs a panorama node and then:

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

**`0x431ec2(image_id, code, progress)`** is the *background* path, called from
the VM's `0x69` case (`0x42db22`), from `0x426935` (save restore), `0x428cbd`,
`0x43340a` and `0x42db22`. It loads the image into the new surface with
`0x41045b`, draws the record pass (`0x43b216`), and blends the departing surface
`DAT_0047a8a4` into it:

```
switch (code) {
case 100: case 0x69: 0x41842b(new, 0x3e800); break;      // draw at once
case 0x65: 0x418bcf(old); 0x42be07(old, new, …, 2); 0x41045b(old, id, code);
case 0x66: 0x418bcf(scratch); 0x42bce1(scratch, new);
case 0x67: 0x419cf6(0, new, …);
case 0x68: 0x418bcf(old); 0x418eb2(old, new, …); 0x41045b(old, id, code);
case 0x6a: 0x419a51(new, …);
}
```

So a `0x69` background load is a transition too — the earlier note that "the
blend lives only in the node loader" was wrong. `0x42db22`'s `0x69` case resolves
the code with `0x432133(operand 1, 0)` when operand 1 is a constant
(0/100 -> 100, 1 -> 0x67, 2 -> 0x65, 3 -> 0x66, **4 -> 4** = the examine
close-up), otherwise from `DAT_00460664` (consumed), otherwise 100.

`0xb9` resolves with `0x432133(id, 1)`: 1 -> 0x67, 2 -> 0x65, 3 -> 0x66, else
100, or takes `DAT_00460664`. `DAT_00460664` is a **process-global**, set by a
`0xbd` (mode 0 -> 0x65, mode 2 -> 0x6a, mode 4 -> 0x67; modes 1/5 leave it), so a
site video's code reaches the destination scene's first `0x69`/`0xb9` across the
`0x73`.

`0x431ec2` maps `0x68` to `0x418eb2` (a four-step band blend with the same 60 ms
gate) rather than to `0x41920f`. In practice `0x68` cannot arrive from a script
operand (`0x432133` never produces it) and `0xbd` never sets it, so that variant
is not reachable through shipped data.

Every routine blends the **playfield rectangle** `(0,0x14)-(0x280,0x1a4)`; the
inventory strip and the top bar are outside it and are not mixed.
`0x41842b(surface, 0x3e800)` = 640*400 pixels is the present used between steps,
and `0x41842b(surface, 0x4b000)` (640*480) is the full-screen variant.

## 2. The shapes (read from the routines)

| Code | Routine | Shape | Steps | Pacing |
|---|---|---|---|---|
| 0x65 | `0x42be07` -> `0x42c1b8` | linear cross-fade, `delta = (new-old)>>3` re-added per step, 2x2 block processing | 8 | present per step |
| 0x66 | `0x42bce1` (+`0x42bc8b`) | interleaved reveal: each phase copies every **27th** 16-bit word (`param_1 + param_3`, `+= param_4`), so the picture fills in a 27-step pattern | 27 | present per step |
| 0x67 | `0x419cf6` -> `0x4196a4` + `0x419a51` | **blink**: the departing picture dims to black (brightness `0xe1,0xc1,0x81,0x00`), then the new picture reveals from black | 4 + 7 | present per step |
| 0x68 | `0x41917b` + `0x41920f` | cross-fade with the RGB555 multiply table `DAT_00487710`; `local_14 = 2, 8, 32, 128` | 4 | **60 ms** per step (`do { now = timeGetTime(); } while (now < start + 0x3c)`) |
| 0x69 | — | none | 0 | — |
| 0x6a | `0x419a51` | the reveal half of 0x67 only: the new picture appears **from black** | 7 | present per step |

`0x42c57e` is the same 8-step fade as `0x42c1b8` but row-sequential instead of
2x2-block; `0x42be07` picks it with mode 0 and either of them with modes 1/2.

## 3. What the port does

New module `port/{include/byzantine,src}/transition.*`:

- `transition_kind(code)` — the switch above; every other code is
  `TransitionKind::none`. Kinds: `fade8`, `interleave27`, `blink`, `fade4`,
  `reveal`.
- `TransitionPlayer(kind, from, to)` — produces the steps in the original's
  order (`next()` then `surface()`), so the accumulating ones (`0x419a51`'s
  phases, `0x42bc8b`'s stride) keep their state. `done()` then the caller
  presents the real new picture.
- `transition_steps` / `transition_step_ms` / `transition_weight` — the counts,
  the 60 ms gate and the brightness sequences.
- `blend_playfield(out, from, to, weight)` — per-channel RGB565 lerp, weight
  0..256 (`256` is exactly `to`).
- `dim_playfield(out, from, weight)` — `0x4196a4`'s uniform dim toward black.
- `reveal_phase(out, to, phase, weight)` — `0x419a51`'s 2x2 phase: bit 0 of the
  phase is the column parity, bit 1 the row parity; the pixels of that phase are
  written from `to` at `weight`, everything else keeps its previous value.
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
- The code is the recovered one when the scene carries it (`0x69` operand 1
  through `0x432133`, `0xb9` operand 3, or the `0xbd`-set `DAT_00460664`).
  Shipped entries usually load with **100** (MR, MAP and EA all do), i.e. the
  original drew those pictures at once. The port therefore uses its own code
  when the data says 100 and logs `source=default`; a code that came from the
  script logs `source=background`/`source=panorama`. `--transition <code>`
  overrides the default and logs `source=forced` — `--transition 0x64` is the
  data-faithful "draw at once" everywhere, `--transition 0x66` the 27-phase
  reveal everywhere.
  `DAT_00460664` is a process-global in the original, so `travel_to` carries
  `pending_panorama_transition` across the `0x73` like the RNG state.
- Options: `--no-transitions` (cut), `--transition <code>` (code used where the
  data says 100), `--transition-shot <dir>` (writes every step as a PPM). Each
  run prints `transition code=0x65 kind=fade8 source=default steps=8
  step_ms=16 elapsed_ms=139`; the steps appear in the detail trace as
  `present … tag=transition detail=code=0x65 step=3/8`, and every applied
  `0x69` prints `background_load instruction=… id=… transition=0x…`.

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
| default (no flag) | `fade8` (`source=default`) | 8 | `elapsed_ms=139` |
| `--transition 0x64` | none (`source=forced`) | 0 | `kind=none (the picture appears at once)` |
| `--transition 0x65` | `fade8` | 8 | `elapsed_ms=132` (8 x 16 ms) |
| `--transition 0x66` | `interleave27` | 27 | `elapsed_ms=437` |
| `--transition 0x67` | `blink` | 11 | `elapsed_ms=209` |
| `--transition 0x68` | `fade4` | 4 | `elapsed_ms=243` (4 x 60 ms) |
| `--transition 0x6a` | `reveal` | 7 | `elapsed_ms=149` |
| `--no-transitions` | — | 0 | `skipped reason=--no-transitions` |

Frame means (0..255 over the whole screen) from `--transition-shot`, which show
the shapes without relying on a viewer:

- `0x6a` reveal: 9.8, 24.1, 41.0, 58.9, 67.7, 72.2, 74.5 — the picture grows in
  a 2x2 dither and brightens (non-black coverage 19% -> 79%).
- `0x67` blink: 74.6, 64.1, 43.7, **0.0**, then 9.8, 24.1, 41.0, 58.9, 67.7,
  72.2, 74.5 — fade out to black, then the same reveal.

What the shipped data says (the `background_load`/`panorama_load` lines):

- `MAP` 87 entry: `background_load instruction=112 id=0 transition=0x64`.
- `MR` arrival and `MR` -> `MAP` exit: both load with `transition=0x64`.
- `EA` 602 entry: `panorama_load … transition=0x64`.

Frames: `port/output/transition-demo/port-0x65.gif`, `-0x66`, `-0x67`, `-0x68`
and their `-sheet.png` contact sheets — these are the pixels the port presented,
written by `--transition-shot`, not a synthetic reconstruction.

The first travel of a headless `--click-at` run reports
`skipped reason=no_departing_frame`: those runs present nothing before the first
click, so there is no outgoing picture to blend from. A windowed session always
has one. Headless `--shot` click paths also set `present_frame=false`, so an
in-scene `0x69` fade is only visible in the window path (or in an arrival).

## 5. Boundaries (not claimed)

- **Fading where the scripts say 100 is a port decision.** Most shipped entries
  load with `transition=0x64` (100) and the original drew those pictures at
  once. The port's default fades them with `--transition`'s code and says
  `source=default`; `--transition 0x64` restores the shipped behaviour. The
  travel-side snapshot `DAT_004603d0`/`0x439349` is still unmodelled, but it no
  longer decides the code: the destination's own `0x69`/`0xb9` provides it.
- **The `0x419a51`/`0x4196a4` shape is now read from the instruction stream**
  (`objdump -D -Mintel --start-address=0x419a51`), not inferred: the scratch is
  cleared with `0x42b280` (memset 0) before the reveal, the seven steps halve
  `[ebp-0x10]` (0x7f -> 1), the phase offsets are `+0`, `+1`, `+0x280`, `+0x281`
  words with a two-pixel step and a two-row stride, and `0x41842b(0x3e800)`
  presents the playfield each step. What remains approximate is the *colour*
  arithmetic (below), not the shape.
- **`0x418eb2`** (a 4-step band blend with 60 ms gates) is implemented as the
  plain `fade4`; its band/scroll geometry is not modelled. It is only reachable
  through `0x431ec2`'s `0x68` case, which no shipped operand produces.
- **Colour precision.** The original blends RGB555 through the multiply table
  `DAT_00487710`; the port blends RGB565 channels arithmetically. The visible
  difference is at most one least-significant bit per channel.
- **Pacing of the non-waiting routines** (16 ms per step) is a host assumption:
  those routines only present, so the original's step rate was its blit speed.
  `0x41920f`'s 60 ms is recovered and exact.
- No original-runtime capture is compared against; "matches the original" here
  means "implements the recovered routines with the recovered step counts and
  the one recovered timing".
