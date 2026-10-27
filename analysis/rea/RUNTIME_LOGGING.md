# Per-launch runtime diagnostics — 27 October 2026

The user requested durable logs for each development attempt, including input
and engine activity, to avoid repeatedly recreating a bug from descriptions.
This change adds host diagnostics; it derives no new original-game behavior.

## Capture and investigation

`port/scene.sh` now enters `port/run_logged.py` before any extraction or player
startup. A recursion marker lets the runner execute the original launcher.
Every invocation gets a unique UTC timestamp/random-suffix bundle under
`port/output/logs` (or `BYZ_LOG_DIR`):

| File | Contents |
| --- | --- |
| `.log` | Elapsed-time-prefixed stdout and stderr, including startup failures and final exit/signal result. The terminal still receives the original child output. |
| `.trace.log` | Sequence/elapsed time for delivered SDL events, script instructions and arguments, variable writes, conversation commands, state adoption and ignored sound-wait clicks. Flushed after each record. |
| `.summary.json` | Original command/cwd, Git HEAD/dirty paths, player SHA-256, selected device environment, elapsed time, raw exit code/signals, category counts, last 100 important and last 60 output lines. |
| `LATEST` | Atomic pointer to the most recently started run's main log. |

Open LATEST, then that basename's summary. Search the main log around the
reported interaction; inspect only the needed trace slice. If the summary
says `running`, use the live log tail. The summary is an index, not a verdict
that the game works. Warning/error counts are substring occurrence counts,
not classified bug counts. No logs are automatically discarded.

### When the user says "stuck/cannot exit"

1. Resolve LATEST once and record the user's basename before running anything
   else. Regression runs create their own logs and move the pointer.
2. Read `.summary.json`: `last_actions` retains the last 20 action lines;
   `possible_stalls` retains the last 20 zero-hotspot results or explicit
   unsupported-feature diagnostics. These are diagnostic hints, not proof of a bug.
3. Inspect the main log around the last successful click and first repeated
   miss. Search for `active_slots=0`, `hit no hotspot`, `stopped:`, `unsupported`,
   media errors, the composed background/panorama and scene transitions.
4. Inspect only that action's `click_plan` / VM evaluation / `state_adopted`
   region in the trace. Check whether the click was rejected, rolled back or
   actually applied; do not mistake its probe for committed state.
5. Reproduce with the same save/data root, then verify an exit and preservation
   of working navigation. Mark forced labels, dummy devices and request-only
   features accurately. Never invent an exit hotspot or silently bypass an
   unsupported command merely to make the test pass.

Example: `run-20261027T102315.015177Z-m1mx8hhh` enters the EA desk via slot 28,
then slot 41/action 1377 leaves no active hotspots. Comparing the probe with
the executed result identifies a state-replay bug: the probe sets local 43
and opens exit 37, but execution starts from that modified state and branches
past the exit setup. Execute from `entry_before`, not probe output. See
`EA_SINGLE_EXECUTION_FIX.md`. The separate request-only CF video is a genuine
remaining gap, but it is not evidence that a missing codec caused this lockup.

Direct binaries can be wrapped with `python3 port/run_logged.py -- <command>`.
Without that wrapper they do not create a run bundle. For native detail alone,
`BYZ_TRACE_LOG=<file>` enables the shared C++ diagnostic sink. The launcher
sets both flags automatically for ordinary use.

## Recorded engine activity

All four player SDL polling loops share `poll_logged_event`: scene input,
sound waits, conversation video and answer selection. Motion includes x/y,
relative deltas and pressed buttons; clicks include button/coordinates/count;
keys include code/name/modifiers/repeat; wheel and window geometry/focus events
are recorded. These are events delivered by SDL, not raw physical input.
Coordinates are the values delivered after SDL's existing transformations.

The shared VM records an evaluation id, starting instruction, budget, every
opcode/encoded operand, variable writes, stop/blocker and active hotspot ids.
Conversation command lists record opcode/arguments, current group/mode and
command count. `set_trace` records adopted state: active ids, actor positions/
flags, held/examined item, local variables, inventory states and object
rectangles/active flags/action labels. Existing media, render, travel, item and
conversation results remain in the main log.

**Important:** the VM also runs probes and sound-budget re-evaluations. These
are marked `semantics=evaluation_not_commit`. A recorded variable write can be
discarded. Interpret it using action results, failures/rollback and state
adoption. A delivered click can similarly be ignored. The log is not a save
file, deterministic replay system, physical speaker recording or per-frame
pixel dump. No screenshots/videos are automatically captured.

### Animation rate (`BYZ_ANIM_TRACE=1`)

The interactive loop is paced on the wall clock (16 ms per record pass, see
`ANIMATION_FRAME_PACING.md`). Setting `BYZ_ANIM_TRACE=1` makes the player print
`anim_trace ms=<SDL ms> id=<record> frame=<index> count=<+0x06> delay=<+0x04>
passes=<clock> applied=<passes>` for every loaded record with a frame list, at
most once per 100 ms. Use it for "the character moves too slowly/fast"
reports: `passes` should grow by `--anim-pass-hz` per second (default 250, so
~28 per sample) and the index should change every `delay` passes (delay 20 at
250 Hz = 80 ms). `applied` trails `passes` by the passes still due in the
current frame. It is a sampling diagnostic, not proof of what the original
executable played.

## Durability and checks

Unique files were verified with simultaneous processes. Native records and
main-log lines flush promptly, and the player stdout is unbuffered when the
runner is active. SIGINT/SIGTERM are forwarded to the child process group on
POSIX, then recorded. Child launch failure produces exit 127; signal exit is
retained as a negative raw code in the summary and mapped to shell 128+signal.
If the runner itself receives SIGKILL or power is lost, already written lines
remain but the summary can stay `running`. File-open/write failures are explicit.

Validation:

- `make -C port -j2 all test`: existing 14 native suites plus Python logging
  acceptance checks pass.
- `port/tests/logging.py`: concurrent isolation, stderr, partial lines,
  nonzero exit, missing command and SIGTERM forwarding/final summary.
- EA 602 / 26.btb real-data headless startup created all files with VM steps;
  `--selftest-events` logged 16 delivered events and exited normally.
- An isolated Xephyr display with XTEST injection logged mouse motion,
  down/up, wheel and keyboard input (20 events), then Escape exited normally.
  This is automated window-event validation, not physical mouse/speaker proof.
- Real `verify_examine_controls.py` and `verify_tk_media.py` still pass. The TK
  test now checks the final stdout line independently of launcher path notices
  on stderr; the game-state assertions are unchanged.

These files reduce repeated investigation only if agents read summaries and
targeted slices. Reading an entire detailed log would instead increase context
use. Logging adds synchronous file I/O per instruction/event; no new per-frame
pixel writes or scene-specific game rules were introduced.
