# EA desk exit lost through probe replay — 27 October 2026

## User evidence and cause

Pinned user run: `port/output/logs/run-20261027T102315.015177Z-m1mx8hhh.*`.
Command: `./port/scene.sh . EA 602 26.btb`.
The user successfully returned from the earlier close-up and entered EA01.
At 13.051 s slot 28/action 1356 opened EADESK with four active hotspots.
At 13.909 s slot 41/action 1377 opened EASHOMER and left zero active hotspots;
all later clicks missed.

The first hypothesis was an unsupported CF-video/timer continuation. Those
features are indeed missing, but comparing the probe with execution identified
the direct cause of this exit loss: `plan_hotspot` copies the initial state to
`plan.entry_before` and `plan.entry`, then mutates `plan.entry` while probing.
`execute_plan` incorrectly passed that post-probe state into the real runner.

For this action, the probe starts with local 43 = 0. It sets local 51, reaches
label 1386, enables exit slot 37, and calls label 727 (which sets local 43 = 1
and starts Emre2 audio). The real execution then starts with local 43 already
equal to 1 and skips label 1386, leaving no exit. This is a common executor
bug: non-idempotent writes/branches can run against their own probe output.

## Correction and bounds

Non-conversation actions now call the runner with `plan.entry_before` rather
than `plan.entry`. The conversation path already uses the pre-probe state.
The interactive/audio-gated and screenshot paths share this correction, as do
diagnostic `--run-label` actions. The probe still computes its lookahead, but
does not supply mutated initial state for execution. Budgeted sound replays
likewise receive the same original entry, preserving existing gate behavior.

The script now really enables slot 37; no exit rectangle, variable value,
forced continuation or scene-specific bypass was invented. An experimental
unsupported-video guard was not retained because it addressed a different
limitation and would not fix the double application of state.

CF `homer1` (mode 5) remains request-only; the general CF-video and clock-driven
callback player are still outstanding. This fix restores the interaction's
actual script/exit and existing Emre2 sound request, not that movie subsystem.

## Validation

- `verify_ea_single_execution.py /tmp/byz-scene`: diagnostic label 653 enters
  EA01, actual slot 28 opens the desk with local43=0; actual screen click
  (133,245) selects slot41/action1377 and leaves local43=1, local51=1, exit37.
  Screen (589,349) selects slot37/action1392 and returns to EA01 with slots
  27..35. Label 653 is an explicit room-entry shortcut, not a physical mouse
  traversal claim.
- The interactive branch with dummy SDL devices and audio enabled passes
  the same sequence using `--selftest-events`: 22-step object script, an
  Emre2 ambient request at step20, one exit, then 30-step return with nine
  active room hotspots. Speaker output was not physically checked.
- Existing EA layer, inventory examine, TK hotspot and EA -> MAP -> TK
  conversation acceptance scripts pass.
- `make -C port -j2 all test`: all 14 native suites plus logging tests pass.
  `git diff --check` passes.

The logging summary now keeps `last_actions` and `possible_stalls` separately
from the output tail. AGENTS.md and RUNTIME_LOGGING.md document pinning the
user's basename before tests overwrite LATEST, comparing probe versus executed
branches, and checking an actual exit after a correction.

No new decompilation or binary session. During investigation, saved exports
were checked against the decompile index (and the target hash matched):

| Export reviewed | SHA-256 |
| --- | --- |
| 0x43c566 (clock callback polling) | 60b3e465cd48d3a8fc16eafd9ca43e9fec09e4dea51caa826e56e309fcd4bbee |
| 0x40d031 (CF video dispatch) | 3c03f813d14edff74503d097495239a4513dffcabe932da14a11998dc8a68a28 |
| 0x40da1f (movie subsystem cleanup) | 1248af271804eb53a21b40cbd5b4d58a161fee22dd0a814cb3a4b88bc6dd0540 |

These confirm separate outstanding runtime responsibilities; the executor
correction itself follows directly from the C++ source and reproduced state.
