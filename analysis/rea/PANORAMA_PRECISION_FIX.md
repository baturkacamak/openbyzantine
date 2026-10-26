# Panorama precision fix: PC53 yaw offset and wrap

25 October 2026. Static evidence reuse plus port code/test correction. No new
binary session, no new Ghidra import and no original execution.

## Symptom

`make -C port -j2 all test` failed in `panorama-test` at
`projection reset camera`. The port's `panorama_yaw_offset` widened the
operands to `long double` before multiplying, while the recovered instruction
sequence is a plain x87 `fmul` of two binary64 values under the CRT-installed
control word. For the reset case (`source_width=64`, `yaw=pi`) the two models
disagree by exactly one unit in the last place:

| Model | `64/(2*pi)*pi` | `__ftol` result |
| --- | --- | ---: |
| PC53 (binary64 per operation) | 32.0 | 32 |
| PC64 (extended intermediate) | 31.9999999999999992 | 31 |

The reported defect was therefore in the port, not in the expectation.

## Evidence reused

- `projection-disassembly/0x4244f4.asm`:
  `fld QWORD [...]` / `fmul QWORD ds:0x45b160` / `fdiv QWORD ds:0x45b180`
  converts the horizontal delta. The alternative branch calls `0x44b298`
  instead of `fdiv`; every division in these exports carries the same
  `ds:0x462ae8` guard, matching a compiler-generated no-FPU emulation
  fallback (`inference`, not separately recovered). The arithmetic is the
  same division.
- `projection-disassembly/0x4244f4.asm` at `0x42459f`:
  `fld QWORD [renderer+0x28]` / `fmul QWORD [renderer+0x50]` / `call 0x44bc38`
  (`__ftol`), exactly two binary64 operands and no widening operation.
- `panorama-projection-evidence.json` data range `0x45b140` (verified against
  the target by `verify_panorama_projection.py`) decodes to
  `0x45b160 = 3.14159265359`, `0x45b180 = 180.0`,
  `0x45b140 = 6.28318530718`, `0x45b158 = -3.14159265359`.
- `PANORAMA_TABLES.md`: `0x44e870` -> `__controlfp(0x10000,0x30000)` installs
  hardware PC=53 at startup. Both operations above therefore round each step
  to binary64.

## Consequence for the wrap endpoint

`advance()` in `port/src/panorama.cpp` already used `rounded_mul`/`rounded_div`
(binary64 per step), which is correct. The test then exposed a second issue:
`-180` degrees is **not** exactly `-pi` under PC53. `round(round(-180*pi)/180)`
is one ulp below `-pi` (`-0x1.921fb54442eeap+1` -> `-0x1.921fb54442ee9p+1`),
so the strict `< -pi` comparison at `0x424568` immediately adds `2*pi`.

Observed port values after the fix, all exact binary64:

| Step from reset (`yaw=+pi`) | yaw |
| --- | --- |
| `+180` deg | `0` |
| `-180` deg | `0x1.921fb54442ee9p+1` |
| `-360` deg | `0x1.921fb54442ee7p+1` |

Exact `-pi` is unreachable through the public degree-delta path under PC53; a
PC64 model would produce exactly `-pi` and keep it negative. The rewritten test
assertion is therefore a PC53/PC64 discriminator, comparable to the recovered
`0.5001` table-bias case in `PANORAMA_TABLES.md`.

The command only ever stages `floor(x/panorama_width*360+0.5)` in 0..360, so
the negative endpoints are synthetic coverage, not observed game input.

## Change

- `port/src/panorama.cpp`: `panorama_yaw_offset` now uses `rounded_mul`
  (binary64, `volatile` store) before truncation, with the disassembly
  addresses and the 32/31 example recorded in the comment.
- `port/tests/panorama.cpp`: the negative-endpoint assertion now states the
  PC53 result bits and documents why `-pi` is not produced.

## Validation

- `make -C port -j2 all test`: scene-state, variable-metadata and all panorama
  tests pass.
- `verify_panorama_bindings.py` (44 scenes / 182 commands / 17 unmatched),
  `verify_panorama_camera.py` (target/profile, 17 exports) and
  `verify_panorama_projection.py` (target/profile, 28 exports, 18 assembly
  ranges, 7 data ranges) all pass.

## Not claimed

- No original execution, runtime capture or control-word trace. Evidence for
  PC53 is the startup `__controlfp` call; a later change to the control word by
  the game or a DLL is not excluded.
- Host libm and x87 return-value semantics still differ from the CRT; this fix
  only removes a precision widening the binary does not perform.
- `0xb9`/`0xbb` loader behavior remains unsupported.
