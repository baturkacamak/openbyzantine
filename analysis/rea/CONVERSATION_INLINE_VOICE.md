# Inline conversation VOICE — 27 October 2026

## Observed failure

Pinned user run `run-20261027T104224.941655Z-_wyjx80u` was launched normally
with EA 602 / 26.btb. The user reached rug slot34/action1307, drawer slot81/
action1316, then slot83/action1325. That script reaches instruction2018,
`0x70 record2`, MCaEA3b. Both attempts failed on conversation opcode0x7c;
the click was rolled back instead of completing. This is an explicitly
unsupported command, distinct from the earlier exit/probe-state bug.

## Verified behavior and implementation

Saved `0x43340a` dispatches 0x7c/0x92/0x93/0x94 to `0x4096f9` with different
debug selectors. In normal runtime (`DAT_00485c6c == 0`), that helper reads
literal operand2 and calls the ordinary line player `0x424780`. Operands0/1
and the selector only alter the original debugging interface in this helper.
Thus none of these opcodes is safely skippable.

The conversation interpreter now exposes the requested inline line using its
existing line event. A pending-work cursor retains the rest of the current
instruction list, sub-list and phase continuation. `line_shown()` releases
the inline wait, then the next call resumes after that opcode. Later writes
or branch decisions therefore cannot occur before media/text playback ends.
This works in the prelude, a line's sub/own list, and a selected answer list.
Starting a session clears pending work. Existing debug-header no-ops remain
unchanged; unknown commands still fail explicitly.

No new media library or codec was written. Inline events reuse the current
subtitle, AVI/FFmpeg and SDL audio path and its existing speaker/media rules.
For this real conversation, inline index2 is Mctthu00 (text-only under those
rules); the following normal line Mctcar02 has the playable AVI. A line with
no media is not replaced with an invented clip.

## Validation and practical bounds

- Unit coverage checks all four opcodes, consecutive inline lines, writes
  before/after playback completion, no repeated writes, and prelude/answer
  continuations. Existing branching, memory, cancellation and rejection tests
  still pass.
- `make -C port -j2 all test` passes all 14 native suites plus logging tests.
- `verify_ea_inline_voice.py /tmp/byz-scene` uses diagnostic label1599 to
  enter the same failing conversation. It checks index2 before index3,
  completion0xffff, three lines, six conversation instructions, local0=1,
  script resume at2019 and a screen-coordinate exit via160/action1611.
  The exit returns to EA03 with active slots157/158. This shortcut is not
  full natural drawer-traversal proof.
- Real-media run: Mctcar02 decodes146 frames at15fps, 320x200, PCM audio
  9733ms; 858452 converted bytes queued, media=1, audio_failures=0. Dummy SDL
  video/audio devices were used; physical speaker output was not checked.
- Real-data TK conversation, EA desk single-execution/exit and inventory
  examine acceptance checks still pass. `git diff --check` passes.

Target SHA-256 was checked against the saved decompile index:
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Selected export hashes also match:

| Export | SHA-256 |
| --- | --- |
| 0x43340a | 3a9555ec83f82adcb4886ad5784a6c64b3bccd5d723091966c58ae1dbe5666a5 |
| 0x4096f9 | d7a86a3df0cc56bc07a31dfec2dbbecc50561200a616cb1e153609a446929222 |

No binary session or decompilation was started. General CF scene videos and
clock callback dispatch remain separate outstanding subsystems; this change
implements conversation VOICE commands only.
