# Animation pass rate is one 250 Hz host guess, calibrated on a single TK record

Date: 27 October 2026. Diagnosis and fix: one global 60 Hz reference, and the
animation is now stepped only by the pass clock.

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; analysis
profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

Reported: in NB the bartender moves too fast. The previous animation fix
(`ANIMATION_FRAME_PACING.md`) made one TK record look right; it is a global
250 Hz constant applied to every scene, so it is not a game-wide solution.

## Current model and what it actually does

* `Options::anim_pass_hz` defaults to **250** (`port/src/play_main.cpp:170`).
  `PassClock` turns elapsed time into record passes; the paced 16 ms compose
  counts as one pass and the rest are applied to the records that compose drew
  (`play_main.cpp:5137-5149, 5264-5290`).
* `advance_record_animation` (`port/src/cc.cpp:556-604`) implements `0x43bb77`:
  a pass with `+0x06 != 0` decrements it and returns; the next pass reloads
  `+0x06 = +0x04` and advances. Therefore the step rate is
  **`pass_hz / (delay + 1)`**, not `pass_hz / delay` (the report's "250/20 =
  12.5" is really 250/21 = 11.9).

Live NB measurement (same user chain, SDL dummy video/audio, `BYZ_ANIM_TRACE=1`):

```
anim_trace ms=759  id=0 frame=1  count=1 delay=1 passes=0   applied=0
anim_trace ms=871  id=0 frame=13 count=1 delay=1 passes=28  applied=24
anim_trace ms=984  id=0 frame=27 count=1 delay=1 passes=56  applied=52
anim_trace ms=1095 id=0 frame=10 count=1 delay=1 passes=84  applied=80
```

* pass clock: 28 passes / 112 ms = 250/s;
* NB record 0 (`nbBr00a`, 31 frames, delay 1): 14 frame steps / 112 ms =
  **125 steps/s**, i.e. a complete 31-frame cycle about **4 times per second**
  (0.25 s). The frames are a slow body/shoulder motion; at 125 steps/s it is a
  blur, not a walk.

## The shipped delay table reads as a ~60 Hz scale

`0x40ffee`'s record initialisation sets `+0x04 = 10` by default
(`0x410021: mov WORD PTR [eax+0x4],0xa`). The scripts and saves use 1, 2, 6, 10,
20, 32 and 60. The frame counts and delays of the records already exercised:

| scene / record | asset | frames | delay | steps/s @250 | cycle @250 | steps/s @60 | cycle @60 |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: |
| NB rec 0 | `nbBr00a` | 31 | 1 | 125 | 0.25 s | 30 | 1.03 s |
| SM rec 3 | `smim00b` | 22 | 1 | 125 | 0.18 s | 30 | 0.73 s |
| HL rec 0 | `hldc00a` | 93 | 1 | 125 | 0.74 s | 30 | 3.10 s |
| TK rec 1 | `TKNUXB` | 18 | 20 | 11.9 | 1.51 s | 2.86 | 6.30 s |

At 60 Hz (one pass per 16 ms) the table is a normal 60 Hz-tick scale: delay 1
gives 30 steps/s (the natural walk/idle range; the HL receptionist takes ~3 s
per 93-frame idle), the default 10 gives 5.5 steps/s, 20 gives 2.9 and 60 gives
1.0. At 250 Hz every record is about **4.2x faster** than that scale, and the
delay-1 actors (NB/SM/HL) are the most obvious. This is the reported NB speed.

## The executable itself is uncapped; the host constant is still a choice

`0x43b216`'s 16 ms gate stores the elapsed delta back into `_DAT_0046187c`
(`0x43b332 mov [0x46187c],eax`), so after the first passes the gate stops
throttling; `0x42d594` (message pump, render, one VM command) has no sleep or
frame limiter in the scene loop. The original animation rate is therefore
machine-dependent and a host reference is unavoidable. But there must be **one
reference for all `+0x04` values**, not a per-record calibration:

* the shipped 1..60 delays and the default 10 line up with a ~60 Hz update;
* 250 Hz was chosen so that TK's delay-20 record looks like an idle animation,
  after the host's own unpaced loop (200-345 passes/s) was compared with it.
  That comparison was relative to the port, not to the original machine.

The previous fix sped TK up 4x relative to the authored delay table while every
delay-1 actor became 4x too fast. A game-wide solution means selecting one
reference (60 Hz is the first candidate) and re-checking TK, NB, HL and SM
together.

`--anim-pass-hz` already exists, so a no-code A/B is possible, e.g.:

```
BYZ_DATA=/tmp/byz-scene ./port/scene.sh ".." MR 0 30.btb --entry \
  --click-at 573,347 --click-at 118,117 --click-at 118,117 --anim-pass-hz 60
```

Compare NB/SM/HL/TK against the original; then set the global default and
update the tests that currently encode 250.

## Secondary cause — input events advance the animation outside the clock

`turn()` composes the scene once per drag/key event (`play_main.cpp:5064-5073`)
and every compose advances every drawn record once (`compose_scene`). The paced
loop only counts its own compose (`pass_clock.applied(1)`), so event composes
are extra passes not covered by the clock.

In the pinned NB run the log has `panorama yaw` (one `turn()`/compose each) at:

```
second 17: 30 events    second 24: 24    second 25: 84
second 37: 17          second 38: 36    second 39: 55
```

At second 25 that is +84 steps/s on top of the 125 clock steps/s for NB delay 1
(about 209 steps/s), exactly while the user was rotating to look at the
character. Clicks also compose more than once. The animation speed is therefore
input-dependent, and the 250 Hz clock applies several steps per 16 ms compose
(4 for delay 1), so the host samples a fast animation in jumps rather than
drawing the intermediate frames.

## Fix

* `Options::anim_pass_hz` now defaults to **60** (one pass per 16 ms tick; the
  delay table's own scale). `--anim-pass-hz` remains the A/B knob.
* `compose_scene` takes `advance` (default true for `--compose-frames`, tests and
  `byzantine-cc`). The interactive paced loop calls
  `display.compose(view, false, false)`, draws the current frame, then applies
  `PassClock::remaining(now)` through `advance_record_passes` and accounts for
  those passes. It no longer counts one implicit step per compose
  (`applied(1)` removed), so the animation rate is the clock, not the host's
  compose count.
* `turn()` (drag/key camera) composes with `advance=false`; camera input redraws
  the current frame and no longer steps the animation. The 84 turn composes in
  one second of the pinned NB drag no longer add 84 steps.
* Effective step rate is `pass_hz / (delay + 1)`; the old report's `250/20`
  should be read as `250/21` (and now `60/(delay+1)`).

### Verification

NB live (`BYZ_ANIM_TRACE=1`, same click chain, SDL dummy):

```
ms=766  frame=1  passes=0  applied=0
ms=878  frame=3  passes=6  applied=5
ms=990  frame=7  passes=13 applied=12
ms=1102 frame=10 passes=20 applied=19
ms=1214 frame=13 passes=26 applied=25
```

3-4 frame steps per 112 ms = **30 steps/s**, 31-frame cycle ~1.03 s (the old
default was 14 steps per 112 ms = 125/s, 0.25 s). `--anim-pass-hz 250` still
reproduces the old rate for a direct A/B. `make -C port -j2 all test` passes;
the new `scene-state-test` checks `compose_scene(advance=false)` leaves the
record untouched, and the existing `PassClock`/`advance_record_passes` tests
pass. Real-data `verify_tk_animation.py` still reports `bad_pixels 0` for
frames 1/3/9/17 (it uses `--compose-frames`, which keeps `advance=true`).

## Tradeoff

At 60 Hz the delay-20 TK record becomes 2.86 steps/s (18-frame cycle ~6.3 s),
while the previous 250 Hz made it 11.9 steps/s (~1.5 s). The delay table and the
`+0x04 = 10` default say 60 Hz is the authored scale; the earlier 250 was chosen
from TK alone. If an original capture shows TK faster, the global value needs a
different single reference, not a TK-only special case.

## Scope

All animated records in all scenes go through the same `PassClock`, not only
panoramas. Conversation/video playback is blocking and unaffected. The delay-1
records were the most visible at 250, but the default delay 10 was also 4x fast.

## Suggested validation

* A/B the whole journey at 60 vs 250 (`--anim-pass-hz`) and compare NB, SM, HL
  and TK against the original. The 60 Hz live check above is a host trace, not
  an original comparison; the chosen value stays a host reference.
* Add a step-rate regression with a known delay over wall time (e.g. NB delay
  1 should advance `pass_hz/2` steps per second) and assert that turn/click
  composes do not change the rate.
* Re-check the delay-20 TK case after changing the global rate so the earlier
  "too slow" report is not reintroduced by accident.

## Not claimed / open

* No original execution or capture fixes the absolute rate; ~60 Hz is inferred
  from the shipped delay table and the `+0x04 = 10` default, not measured.
* No frame interpolation; a 60 Hz host still samples a 30 steps/s animation at
  two host frames per step.
* The `+0x04` countdown off-by-one should be stated as `delay + 1` in the
  previous report.
