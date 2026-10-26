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
