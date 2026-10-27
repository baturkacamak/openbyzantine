# EA navigation stuck after a close-up — 27 October 2026

## Evidence

The user's actual command was `./port/scene.sh . EA 602 26.btb`.
Preserved run bundle: `port/output/logs/run-20261027T101756.588677Z-_4qu0xq7.*`.
At 4.852 seconds, a panorama click selected slot 3/action 544. It completed
11 steps and enabled only flat close-up slots 18/19. Trace evaluations 3/4
show instruction 632, opcode `0x69`, background record 5 (`EAPupAll`).
Later user clicks continued to report `(panorama) hit no hotspot`.

The renderer's `update_layer` adopted a background only on a fresh scene or
when no layer existed. When a panorama was already active, a new flat close-up
therefore retained both the panorama image and its source-coordinate hit test.
The two active rectangles belonged to a different view and were unreachable.
This condition exists in HEAD b01cd37; the recent logging wrapper did not
introduce it. Logging made the exact failed transition identifiable.

## Correction

An explicit background load now selects the background layer even if a
panorama was previously active. Traces without a new background or node still
retain the current layer. The existing handling when both load types appear
in one trace remains unchanged. The obsolete fresh-scene argument was removed.

Both composition and hit testing already use `panorama_layer`, so the shared
correction synchronizes display, drag behavior and click coordinates without
adding a scene-specific exception. No game state or unsupported opcode was
invented or skipped. No binary analysis was needed for this source-level bug.

## Validation

Before rebuilding, reproduced using real EA data: `--click 3 --click-at 589,349`
kept drawing the EA00 panorama, mapped the exit point to `(236,527)`, missed,
and ended with slots 18/19 despite the EAPupAll background request.

After correction, `verify_ea_layer.py /tmp/byz-scene` checks:

- EA slot 3/action 544 shows `EAPupAll.jpg`, with slots 18/19.
- Playfield `(589,349)` maps to flat `(589,369)` and selects slot 18/action 551.
- That action returns to the EA00 panorama and restores slots 1..7.

The interactive branch with audio enabled and dummy SDL video/audio devices
also passed the same CLI-injected click sequence with `--selftest-events`.
This validates the interactive dispatch path, not physical mouse/speaker use.
Existing real-data examine controls and EA -> MAP -> TK conversation checks
pass. `make -C port -j2 all test` passes all 14 native suites plus logging
acceptance checks; `git diff --check` passes.

The user can restart with the same original command. This correction addresses
the demonstrated panorama/flat-view lockup, not every remaining game subsystem.
