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

## Player presentation and click architecture (read before touching the player)

The engine modules (`cc.cpp`, `compose.cpp`, `records.cpp`, `save.cpp`,
`panorama*.cpp`, `conversation.cpp`) are data-driven: script labels, records,
sounds, backgrounds and panorama nodes come from the shipped tables, and there
are **no scene-name branches**. The only scene names in code are recovered data
tables, each tied to an original address: `inventory.cpp`'s examine mapping
(`0x438afa`/`0x438173`), the notebook prefixes (`0x4216fe`), `Hints`
(`0x41e002`), the cursor tables (`0x45dbe8`/`0x45e090`) and `video.cpp`'s
`mcaft1a1` (`0x40d031`). Do not add a per-scene fix; fix the shared rule.

`port/src/play_main.cpp` is the host (window, menus, clips, diagnostics) and is
where duplication used to cause regressions. Keep these three single points:

1. **One presentation decision.** `Display::present_image` is the only
   `SDL_RenderPresent`. `ScreenContentScope` sets what is on screen
   (`scene`, `conversation`, `clip`, `menu`, `transition`) and `Display::overlay_allowed()`
   decides the hotspot-outline diagnostic from it plus `--hotspots`/`H`. It is
   **off by default**: the map's own `*Hi` art is the visible highlight
   (`hover_select`, see below). `--hotspots-hover` outlines the pointer's
   hotspot, `--hotspots`/`--hotspots-all` every active one. A clip (including
   its audio drain, which runs inside `play_decoded_clip`), a menu and a
   **transition frame** are never outlined (that was the reported bug). Do not
   add an `overlay` parameter to a new presenter; wrap the content instead.
   `ScreenContentScope` writes `screen_content now=… previous=…` to the detail
   trace, and every present line carries `overlay=0|1`, so a leak is
   attributable.
2. **One click commit.** `commit_click` (next to `describe_trace`) is the only
   way to finish a click: object/message windows, panorama events, the
   presented result, `click_result`, `follow_switches` and the refused-switch
   fallback. The window path, `--click`, `--click-at` and `--run-label` pass
   `ClickCommit` flags for their observable differences, never their own copy of
   the sequence.
3. **A `0x73` never presents the departing scene.** `action_switches_scene` /
   `present_action_result` skip the frame; `travel_to` presents the target, and
   `present_refused_switch` restores the click's frame only when the switch is
   refused. This is what stopped the map flashing between a site video and its
   destination.

Other rules learned here:

- The hotspot outline is a **port diagnostic**, not original behavior; the
  shipped build only changes the cursor (and on the map that cursor is `map`
  everywhere). The port's hover aid is the game's own select labels:
  `hover_select` runs the active kind-0x17 destination's label once (433/450…)
  when the pointer enters it; that draws its `*Hi` highlight and enables its
  travel object, so **one click then travels**. Only destination rectangles are
  considered — never the full-playfield "click anywhere" handlers (24/32) — and
  a hover is refused (logged `skipped reason=not_a_highlight`) when the label
  would travel, talk, play a clip or sound, or hit an unsupported command.
  Do not add a fallback/retry over full-playfield objects: that ran 8 labels in
  one sweep and churned the map.
- The hover preview is **transactional**: `hover_select` snapshots the whole
  `SceneState` + `SceneTrace` before it commits the select label, and
  `restore_hover_preview` puts that snapshot back exactly when the pointer
  leaves or changes destination (also on window leave/focus loss). Without the
  rewind the `*Hi` art accumulates and different destinations stack their
  colours. The pick must read the **snapshot**, never the live trace (the label
  disables the destination 41 and enables travel 7 + cancel 24), and staying on
  the same destination must not re-run the label (that flashed the plain map on
  every motion event). The log names it: `hover_highlight slot=N label=… at=x,y`
  and `hover_highlight slot=-1 restored`. Capture real-window evidence with
  `xwd -root`; ffmpeg `x11grab` recentred the pointer here and silently
  invalidated a hover test.
- A clip's last audio buffer can be torn data (e.g. `NBEST1.avi`'s last 67 ms).
  `trim_torn_audio_tail` handles only that shape and logs
  `tail_guard …`; `--no-audio-tail-guard` keeps the raw stream.
- **One transition presenter.** `Display::present_transition` is the only way a
  scene change is shown: `travel_to`/`restart_scene`/the close-up return call
  `present_arrival`, and a click whose label ran a `0xb9` calls it from
  `present_action_result`. The code comes from the script (`0xb9` operand 3 via
  `0x432133`, or the `0xbd`-set `DAT_00460664`, which crosses a `0x73`); a
  travel that carries none logs `source=travel_default` and uses
  `--transition`'s code. Do not add a second fade path: a new presentation
  point wraps the content, and `last_playfield` is kept by `present_image`.
  `--no-transitions` restores the cut; `--transition-shot <dir>` writes the
  steps. Report: `analysis/rea/TRANSITIONS.md`.
- The `--shot`/headless paths must keep `scene=… slots=…` as the **last stdout
  line** (the verification scripts parse it). `Display::log_present_summary()`
  is called before it.
- Report evidence in `port/output/…`, and update `CONTINUE.md` plus a focused
  `analysis/rea/*.md`. `PRESENTATION_ARCHITECTURE.md` describes this audit.

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
