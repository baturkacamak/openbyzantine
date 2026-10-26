# Ordered conditional branches and calls

25 October 2026 continuation. Eight additional opcodes are implemented in
`port/src/cc.cpp`; the SDL2 player uses the same interpreter.

## Saved binary evidence

Target: `BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Reused the saved REA/Ghidra export under `pseudocode/functions/`, profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.
This is static evidence, not a new execution of the original game.

`0x431a36.c` resolves operand 0 into the first output (`local_28` at the
caller), and operand 1 into the second (`local_14`). `0x4319b3.c` reads type
2 operands from the scene variable mapping and other operands literally.
The following cases of `0x42db22.c` compare the low unsigned 16 bits:

| Jump | Call | Operand 0 compared with operand 1 |
| --- | --- | --- |
| `0x96` | `0x9f` | greater than |
| `0x97` | `0xa0` | greater than or equal |
| `0x98` | `0xa1` | less than |
| `0x99` | `0xa2` | less than or equal |

The destination is the literal label in operand 2. Call variants invoke
`FUN_0043194f` before changing the program pointer, preserving the following
instruction for return. The ordinary execution mode (`param_2 == 0`) is the
mode modeled by this interpreter. Preview/alternate execution is not modeled.
Unknown variables still stop evaluation, with the operand ID and instruction
index reported; values are never synthesized.

## Build and tests

`make -C port -j2 all test` passes with the Debian C++17 toolchain. Both the
CLI and SDL2 player were rebuilt with warning flags enabled.

The added truth-table checks cover all eight commands, six operand pairs
(including 0, 65535, 32767 and 32768), and three operand-source combinations:
144 branch evaluations plus 96 missing-variable checks. They verify equality
boundaries, unsigned ordering, taken/untaken branches, call continuation,
jump non-return, and unresolved operands on either side. Existing equality
calls, arithmetic, hotspot transitions and original EA/STAR checks still pass.

Original EA script integration checks explicitly seed variable 26:

- Label 428, value 8: `0x98` jumps to label 433 and returns at instruction 475.
- Label 428, value 9: falls through and reports unsupported `0xb1` at 470.
- Label 464, value 1: `0xa1` calls label 422 and reports `0x6c` at 462.
- Label 464, value 2: skips the call and reports `0xc1` at 520.

These seeded tests verify the recovered commands against original script
bytes. They do not establish how the game initializes variable 26.

## Coverage measurement

Before the change, all 7,693 label entries were traced with fresh state:
2,092 reached a supported return; 163 stopped first on one of the eight
ordered comparisons. Retesting those 163 entries after the change reports
an unresolved variable in every case. None now stops on an unsupported
ordered comparison, but no additional complete fresh-state path is claimed.
The comparison implementation exposes the next actual dependency: earlier
state initialization. Full per-entry results, original baseline totals and
source/data hashes are in `ordered-comparisons-coverage.json`.

## Panorama investigation and remaining work

The saved `0xb9` case calls `FUN_00432295`, which passes a node name to
`FUN_0040186e`. That function opens a `nodes/...jpg` asset and performs camera,
render and transition setup. `0xba` calls `FUN_00401c21`, which clamps its raw
operand to 0..2495 and stores it in `DAT_0045c064`. `FUN_00401ddf` later
consumes this pending position, computes a 0..360 angle using the node width,
and resets the pending position to -1. This path needs node rendering and
camera state, so it remains unsupported rather than being treated as a no-op.

EA variable 5 should not be equated with that native pending-position global:
the script explicitly assigns 0, 1, 2 and 3 to it in different cleanup paths
(instructions 508, 955, 2222 and 2010). Label 301 compares that variable to
select return/setup calls before `0xb9`. Its precise game-level name remains
unknown; it acts as a script return-context selector in these observed paths.
EA 475 → 595 is still supported, and its panorama return still reports missing
variable 5 when entered without the earlier initialization path.

Original assets and the saved executable were not modified. The next step is
to recover the required entry-state setup or implement the next required
media/resource command with its observable effects.
