# Animation frame pacing — 27 October 2026

Reported: in the TK entrance the character moves slower than in the rest of the
game. Pinned user run `run-20261027T121108.021855Z-_6z7t4zt`
(`EA 602 26.btb` -> EA02 -> MAP -> TK -> slot 2 / action 61), user screenshot
`Byzantine - TK entry` with the woman in front of the door.

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; analysis
profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.
No new binary session or decompilation; saved exports and a focused `objdump`
of the same target were used.

## Observation — the record that plays in TK

Action 61 runs TK instruction 87, `0x64(1, 1, 20, 244, 150)`:

| operand | value | effect (`0x4324b0`) |
| --- | --- | --- |
| 0 | record 1 | CA slot `TKNUXB` |
| 1 | raw 1 | `+0x09 = 0x01` (ping-pong) |
| 2 | 20 | `+0x04 = 20` (drawn passes per animation frame) |
| 3 | 244 | `+0x00 = 244` |
| 4 | 150 | `+0x02 = 170` (`+0x14` bias) |

`--click-at 390,269 --print-records` on the same data prints
`record id=1 kind=0 flags=0x1 x=244 y=170 frames=18 frames_known=1 delay=20`.
The CD2 asset is a 150x250, 18-frame delta animation whose frames 1..17 are a
palindrome, so one visual cycle is 18 drawn animation frames.

`0x43bb77` steps that list once per drawn record pass after `+0x04` countdown
passes: **the pass rate decides the animation speed**, and `+0x04` is a divisor.

## Observation — the original pass rate is uncapped

`0x43b216` (record pass) reads the clock, compares the elapsed milliseconds
with 16 and only then draws:

```
0x43b2dc  call ds:0x4a9444        ; GetTickCount (IAT slot verified in the PE)
0x43b2e4  sub  ecx,ds:0x46187c    ; ecx = now - _DAT_0046187c
0x43b2ea  mov  ds:0x47ae58,ecx    ; DAT_0047ae58 = elapsed
0x43b30c  cmp  ds:0x47ae58,0x11   ; 0x10 < elapsed
0x43b313  jae  ...
0x43b32f  mov  eax,ds:0x47ae58
0x43b334  mov  ds:0x46187c,eax    ; store the DELTA, not the timestamp
```

The stored value is the delta, so the next `now - 0x46187c` is a full
`GetTickCount` value and the gate stops throttling after the first passes. The
callers are uncapped (`0x42d594`: message pump, render, one VM command), so the
executable's animation rate is its raw pass rate, which is machine dependent.

## The two host models tried

**First (wrong).** Read the gate as a 16 ms cap and run one record pass per
16 ms. That gives `20 x 16 ms = 320 ms` per animation frame for TKNUXB, i.e.
~3 frames/s, and the delay table reads as 60/6/3/1.9/1 frames per second for
`1/10/20/32/60`. After the loop was paced to hold that tick (`FramePacer`, one
16 ms budget incl. the compose work) the measured step was 336 ms — still
visibly slow, and the user confirmed that the *unpaced* host loop (the
`--compose-frames` runs, 200-345 record passes per second on this host,
i.e. ~10-17 animation frames/s for delay 20) looked like the original. So the
60 Hz reading was a host invention, not the game's behaviour; the delays are
divisors of a pass rate in the hundreds of Hz.

**Current.** Two clocks:

| clock | rule |
| --- | --- |
| render | compose/present at most once per 16 ms (`FramePacer`, unchanged) |
| record pass | `PassClock` at `--anim-pass-hz` (default **250**) |

Every pass the clock owes beyond the one the compose already did is applied to
the records that compose actually drew (`advance_record_passes` ->
`0x43bb77`), so the animation speed is independent of the compose cost and of
the scene. `250 Hz / 20 = 12.5` animation frames per second for TKNUXB (80 ms
per frame), inside the range the user identified as correct. A pass backlog
larger than one second (scene load, conversation, media clip) is dropped
instead of replayed; a scene with no animated record keeps the event-driven
redraw and drains the clock.

`--anim-pass-hz` is a host-model value, not recovered game data: it is the only
free parameter of this model and is exposed so the speed can be matched against
the original. Nothing about `+0x04`, `0x43bb77` or the frame lists changed.

## Verification

* `make -C port -j2 all test` passes all 14 native suites plus the logging
  checks. New `scene-state-test` cases: `FramePacer` (tick, boundary,
  catch-up, stall), `PassClock` (250 Hz arithmetic, applied/skip accounting,
  zero-rate guard) and `advance_record_passes` (only listed records advance,
  five passes of delay 2 land on `current=2/count=1`, zero passes do nothing).
* Live TK state (`--click-at 390,269 --anims-overlay <cd2>`,
  `BYZ_ANIM_TRACE=1`): `passes` grows 28 per 112 ms sample = 250 Hz, and the
  record index advances every 80 ms (measured over 420 passes = 21 steps:
  10->12->13->...->4 on the ping-pong leg). Before this change the same state
  advanced every 336 ms.
* Rendering/decoding unchanged: `analysis/rea/verify_tk_animation.py` still
  reports `bad_pixels 0` for frames 1/3/9/17 (10/50/180/350 composes), and
  `verify_tk_hotspots.py` still passes TKXA/TKXB/TKXC switching.
* The user's pinned run and its logs were left untouched (all probes wrote to
  scratch `BYZ_LOG_DIR` directories).

## Not claimed

* No original execution: the original's absolute pass rate is uncapped and
  hardware dependent. 250 Hz is a host reference, chosen from the user's
  on-screen comparison and the host's own unpaced rate (200-345 Hz here), not
  a measured 1997 frame rate.
* The intermediate passes are stepped without being drawn, so a host frame can
  skip animation frames exactly like a slow display would; no interpolation.
* The pixel-accurate result, speakers and real input devices are not validated;
  the loop measurements use the `:1` X display with dummy audio.
* The doubled compose on a click (the handler composes, then the paced frame
  composes again) is unchanged and not modelled against the original.
