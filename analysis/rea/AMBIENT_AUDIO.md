# Ambient sound records through the SDL2 mixer

Date: 26 October 2026.

This step connects the script's sound records to a common SDL2 audio mixer. It
implements the original's start/stop/loop rules and scene transitions, and it
stops the script from waiting on every sound: only a `0xd1` interactive
dialogue (mode 0) and a `0xd3` timed session block; `0xc4` ambience starts and
continues.

## Evidence

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

| address | digest (first 16) | role |
| --- | --- | --- |
| `0x427de1` | `551daf477744ef8c` | play a `DAT_00486220` record |
| `0x4283a6` | `35847809915d4f5d` | open the WAV and create the DirectSound buffer |
| `0x428f8c` | `2c598bc126f22f54` | build the `sounds\...` path |
| `0x428cbd` | `8048e3e4dffeab96` | nine-argument start/session |
| `0x428143` | `d3d67f849fbfa5ab` | "is it still playing" poll |
| `0x427a01` | `ee7f1168e6b33c1c` | release; append a loop to `DAT_0046019c` |
| `0x4278ab` | `0131d714134b6a0d` | stop every playback session |
| `0x4281b1` / `0x4277ed` | `93197d2b54fd4dce` / `e59db1bb2f9978ab` | `0xbc` stop branches |
| `0x427135` / `0x42722b` | `2d6cdf1713162bd7` / `826b9560033bfc38` | sound table save writer/reader |

## Original rules

* `0x65` defines `DAT_00486220[id]`; `+0x14` is the **loop** flag and `+0x18`
  the device/gamepref gate.
* `0x427de1(id, param2)` calls `Play(..., 1)` when `+0x14 != 0` and
  `Play(..., 0)` otherwise. The call itself does not block.
* `0xc4` calls `0x427de1(id, 1)` for every type-2 operand and sets `+0x34`
  when the record loops. It is an **ambient start**, not a wait. (`endgame` is
  only flagged by name.)
* `0x428cbd` (`0xd1`/`0xd3`) starts the record and then, while
  `param_2 == 0` (interactive dialogue mode), runs the `0x43c8d2` window and
  polls `0x428143` until playback ends. `0xd1` mode != 0 and `0xd3`'s timed
  session are different paths; the port blocks only the interactive `0xd1`
  mode 0 and a session, with the session's first timeout as the cap.
* `0xbc`, `0xcf` (`0x427a01`) and `0xd0` (`0x4278ab`) stop a record, release a
  loop (the original queues it in `DAT_0046019c`) and stop everything.
* A scene load frees the per-scene sound table/handles (`0x40f3f0`), so a scene's
  ambience does not cross a `0x73`. The global loop chain `DAT_0046019c` is save
  block 21; it is not reconstructed here.

## Port implementation

`port/include/byzantine/ambient_audio.hpp` + `src/ambient_audio.cpp`:

* `AudioMixer` opens one S16 stereo SDL2 device with a **callback mixer** (the
  ready-made library part; no codec or DSP is reconstructed). Each tagged voice
  is an interleaved PCM buffer at the device rate from `SDL_AudioStream`.
* `start(id, clip, loop)` replaces the same-id voice; `stop(id)`, `stop_all()`,
  `playing(id)`, `active()`, `ambient()`. A loop wraps at the end of the buffer,
  a one-shot marks itself finished. The callback mixes every voice and clamps.
* `queue(clip)`/`queued()` keep the conversation AVI audio contract (an
  untagged one-shot voice and its remaining bytes), so the existing drain loop
  is unchanged.
* `parse_wav` moved into the same module from `play_main.cpp`.

`play_main.cpp`:

* `0xc4` now produces a `SceneSoundStart` (`opcode=0xc4`, `loop` from `+0x14`,
  the same `+0x18`/gamepref gate) instead of only setting the flag; `0xbc`
  produces `SceneSoundStop`, `0xcf`/`0xd0` keep per-instruction release/stop-all
  records.
* `sound_events` orders starts, stops, releases and stop-alls by executed step.
  `play_events` starts a voice for every event; it waits only when
  `blocking` (0xd1 mode 0 or a session). A `0xc4` prints
  `(ambient started)` and the script continues.
* `resume_ambient` starts every active `+0x14` loop of the arrival state (save
  sound table or the restored close-up parent). `travel_to` stops all voices
  before a scene load (`ambient_scene_stop`) and the target entry starts its
  own; the close-up return re-arms the parent's loops.
* `--print-audio` lists `kind`, `opcode`, `loop` and `blocking` per event.
* `port/scene.sh` extracts `copy2hd/sounds/*` (endgame/Emre2/Pxstart) and the
  player's sound lookup also accepts `copy2hd/sounds` if a data dir keeps the
  original layout.

## Verification

Headless, AM `62.btb` entry:

```
audio step=259 opcode=0xc4 id=10 name=amotambr duration_ms=4795 loop=1 blocking=0 (ambient started) voices=1 ambient=1
audio step=260 opcode=0xc4 id=9  name=amotambl duration_ms=5132 loop=1 blocking=0 (ambient started) voices=2 ambient=2
entry scene=AM steps=268 … hotspots=9
ambient_resume loops=2 started=0
```

The entry finishes without waiting for either 4.8 s/5.1 s loop.

Real window (Xephyr `:8`, 1280x960) with `SDL_AUDIODRIVER=disk` for a
content check: `ambient.raw` is 10.40 s, 22,050 Hz stereo, RMS 1427 / peak
7351, non-silent from t=0 through t=10 (past both clip lengths), i.e. the
loops repeated. Screenshot `am_ambient_window.png`; log `ambient_loop.log`.
The default SDL audio device also opens on the host
(`audio device freq=22050 … mixer=callback`), so this is the real device path,
but the audible check is the PCM capture, not a speaker measurement.

Stop and scene change: AM click object 85 (label 339) runs `0xbc 11/10/9`
then `0x73 [1]` -> MAP:

```
ambient_stop step=30 id=11 …
ambient_stop step=32 id=10 voices=1
ambient_stop step=33 id=9  voices=0
travel index=25 scene=map from=AM …
```

`ambient_stop.raw` has sound in the first second and silence afterwards.
`ambient_stop.log` has the full chain.

Examine round trip: AM ambient loops -> examine `ibeering`
(`EXAMINE`) -> close-up exit:

```
ambient_scene_stop from=AM voices=2
closeup_return scene=AM from=EXAMINE … ambient=2
ambient_resume id=9 name=amotambl …
ambient_resume id=10 name=amotambr …
```

Blocking dialogue: VT `72.btb` label 497 reaches `0xd1 35` (`voTins1`):

```
audio step=4 opcode=0xd1 id=35 name=voTins1 duration_ms=4709 loop=0 blocking=1
 waited_ms=4706 frames=323 events=17 (script continues)
```

The window stayed live during the wait and the script continued. VT's entry
also has `0xc4 0` (`tkamb`) as a loop; it does not block.
`--print-audio` reports the same kind/opcode/loop/blocking fields.

Tests: `audio-test` (SDL dummy driver) covers strict WAV parsing and five
rejections, loop start/restart, two independent ambient voices, stop of one
voice, `stop_all`, and the conversation `queue`/`queued` drain.
`scene-state-test` covers the `0xc4` start trace (`opcode=0xc4`, `loop`), the
`0xbc` stop, the `0xcf` release step and the `0xd0` stop-all step.
`make -C port -j2 all test` passes all suites.

## Unsupported / boundaries

* `DAT_0046019c` (the saved loop chain, block 21) is not restored as a chain.
  The port starts the arrival state's active `+0x14` records; a loop saved in a
  previous scene is not resumed from that chain.
* Volume/pan fields `+0x1c/+0x20/+0x24/+0x28` and the `0xbf`/`0xc0`/`0xc2`
  parameter controls are still not applied (full volume). The `0xd3` follow-up
  records (`0x431ec2`) are not executed; the session waits its first timeout
  and reports the request.
* The `endgame` record remains a flag (`DAT_0046066c`) and is not played as a
  normal clip.
* The real-device check proves the device opens and the mixer feeds it; it is
  not an acoustic measurement. The disk-driver PCM is the content evidence.
