# Project continuation

- Start by reading `CONTINUE.md`; use it as the compact investigation index.
- The deliverable is a portable C++17 reconstruction of Byzantine: The Betrayal.
- Use saved REA/Ghidra exports for binary behavior. Do not repeat whole-program
  decompilation when the needed function exists in `analysis/rea/pseudocode/functions/`.
- Verify target and selected export hashes against `decompile-index.json` when
  deriving new behavior. Keep observations, inferences and unknowns distinct.
- Never invent missing game state or silently skip unsupported VM commands.
- Preserve original ISO/game files; work on extracted analysis copies.
- After a substantive step update `CONTINUE.md` and a focused evidence report.
- Build and validate behavior with `make -C port -j2 all test`.

## Runtime diagnostics (user request, 27 October 2026)

- Normal `port/scene.sh` launches automatically retain a separate run bundle in
  `port/output/logs/` (`BYZ_LOG_DIR` overrides the directory). Preserve the run
  associated with a reported bug; do not overwrite it with the next attempt.
- Start an investigation from `port/output/logs/LATEST`, then read that run's
  `.summary.json`. Search the `.log` around the relevant action/error; only
  read the needed slice of `.trace.log`. Do not paste entire traces into model
  context. The summary contains paths, revision/dirty state, command, exit
  status and selected diagnostics; it is not a game-correctness verdict.
- Resolve and record the user's run basename BEFORE launching tests, because
  tests also replace LATEST. If the summary is still `running`, inspect that
  run's live log tail. Keep investigation output separate from the user's run.
- For "stuck/cannot exit", check `last_actions` and `possible_stalls`, then
  search the main log for the last successful action, `active_slots=0`,
  `hit no hotspot`, `stopped:`, `unsupported`, media errors and transitions.
  Repeated misses alone do not establish a coordinate bug. Check which image,
  layer and active object set the action actually left behind; distinguish
  wrong input mapping from missing runtime video/timer/continuation behavior.
- Compare the probe's `vm_end` with the executed result. A divergent branch
  can mean the action ran from its already-mutated probe output. Execute from
  `entry_before`; probes must not become an action's initial game state.
- Reproduce the minimal action chain using the same save and actual asset
  root. Verify state before/after and a usable exit after the correction.
  Label shortcuts and dummy-device checks must be reported as such. If a
  capability remains missing, report that explicitly; avoiding a softlock is
  not completion of the underlying interaction. Detailed workflow/evidence:
  `analysis/rea/RUNTIME_LOGGING.md`.
- VM traces include probes and budgeted re-evaluations. They are explicitly
  evaluations, not proof an action committed. Use action results, rollback
  diagnostics and state-adoption records to interpret them.
- Direct player/CLI debugging can use `python3 port/run_logged.py -- <command>`.
  SDL input logs report delivered events, including mouse motion during waits;
  no physical-input, pixel-perfect rendering or speaker validation is implied.

## Library strategy and playable milestones (user decision, 26 October 2026)

- Prioritize a playable scene and then connected scenes. Report what the player
  can actually do, not just opcode counts or request-only implementations.
- Use existing libraries for standard image/audio/video decoding, audio output,
  windowing and input. Do not reconstruct standard library internals from the
  executable or write new codecs when an appropriate existing library suffices.
- Keep SDL2 for the current player, including audio output/conversion and input.
  Do not write a replacement audio library or migrate SDL versions/engines merely
  to adopt another dependency. The existing PCM WAV reader can stay; use a proven
  decoder for additional formats rather than expanding it into a codec library.
- Preserve working code unless replacing it has a concrete benefit. In particular,
  keep the current JPEG decoder for now; do not expand its format support merely
  for completeness. If it becomes a blocker, evaluate libjpeg-turbo behind the
  existing image API. For AVI, evaluate FFmpeg against actual game codecs/files
  rather than implementing Indeo or assuming all files are supported.
- Keep game-specific formats and rules in the reconstruction: script execution,
  variables, saves, dialogue/inventory, special RLE/animation data, panorama
  coordinates, draw order and timing. Libraries perform media operations; the
  engine decides when they start, block, finish and resume the script.
- Integrate one change at a time using real extracted assets. Preserve dimensions,
  RGB565 conversion, coverage masks, coordinates and script sequencing; document
  tolerable visual differences instead of claiming original-runtime equivalence.
  Validate wait/completion and input responsiveness; distinguish dummy-device
  tests from actual window/speaker checks. Never hide missing media or unknown
  behavior by silently skipping it.
- Do not search for original engine sources unless the user requests it again.
  See `analysis/rea/LIBRARY_STRATEGY.md` for the audit and implementation context;
  this section records the user's current direction, not a claim of completion.
