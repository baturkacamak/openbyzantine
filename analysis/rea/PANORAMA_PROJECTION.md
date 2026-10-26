# Panorama projection arithmetic recovery

25 October 2026. Static evidence only; no original game execution, new Ghidra
import or whole-program decompilation. Reuses the saved REA profile identified
in `CONTINUE.md`. `panorama-projection-evidence.json` now binds 20 selected exports,
16 focused GNU objdump assembly listings and seven data ranges to the target.
`verify_panorama_projection.py` validates export hashes against
`pseudocode/decompile-index.json`, the target hash, retained files, and original
byte ranges via an independent PE section-table address mapping.

## Observed

- `0x402e00` loads renderer +0x58 (pitch); `0x402e70` loads +0x50 (yaw).
  Both multiply by binary64 180 at `0x45b080`, divide by binary64
  3.14159265359 at `0x45b088`, and return ST0. Neither converts to an integer,
  negates pitch, nor normalizes yaw. Saved pseudocode omitted these returns.
- `0x42459f..0x4245b5` loads scale +0x28, multiplies yaw +0x50 and calls
  `0x44bc38` (`__ftol`). That helper saves the control word, sets rounding
  control to truncate, stores a signed 64-bit integer, restores the old control
  word, and returns EDX:EAX. The renderer uses EAX as a signed word offset,
  multiplied by two for RGB16 addressing. This conversion differs from the
  table generator's floating-point bias conversion.
- `0x44bd80` selects the CRT descriptor at `0x463452`, containing `sin`;
  `0x44bd8a` selects `0x463472`, containing `cos`; `0x44d431` selects
  `0x463810`, containing `atan`. Their jump chain reaches the common CRT
  dispatch at `0x450df5`. Descriptor identification is observed; full CRT
  numerical/error behavior has not been reconstructed.
- In `0x423c42`, let S be renderer +0x28, A=floor(source_height/2),
  B=floor(viewport_height/2), and F=binary64(S*zoom). The returned expression is
  `atan(binary64((A*F - S*B) / (S*F + A*B)))`. Intermediate operations before
  that store use x87 precision. The final ST0 atan result remains live after
  a non-popping binary64 store, so not every caller sees a rounded result.
- `0x423cdf` explicitly stores the pitch limit at zoom=1 to binary64 L,
  computes and stores binary64 C=cos(L)*S, then forms
  `R=binary64(floor(viewport_width/2) / (C - sin(L)*B))`.
  Crucially, `0x423d86` multiplies atan(R) by S before `__ftol`.
  `0x4236b5` adds one to that truncated extent for the seam padding P.
  Source scale is binary64(source_width / 6.28318530718). No current zoom
  enters this padding calculation.
- `0x423a49..0x423a56` constructs the table conversion bias with exact bits
  `0x4338000000000100` (decimal 6755399441056000). After adding this bias in
  x87 and storing binary64, the assembly reads the low DWORD and subtracts
  `0x100`. This pattern appears at each table-coordinate conversion.
  The saved decimal pseudocode alone obscures why 256 is subtracted.

## Inference and remaining unknowns

Follow-up: [`PANORAMA_TABLES.md`](PANORAMA_TABLES.md) recovers the CRT startup
PC53 selection and lookup initialization bounds. RC is preserved by that call,
so nearest/even remains conditional on effective rounding state. A controlled
x87 probe demonstrates PC64 double rounding at 0.5001; host `long double`
plus a binary64 store is not a universal replacement. The bounded PC53/nearest
model and approximate host-libm tables are now implemented as standalone
helpers. Full mapping intermediate arithmetic, effective runtime RC, original
CRT binary32 values and original-engine comparison remain unresolved.
Hardware fsin/fcos in mapping and CRT calls in setup remain distinct.

## Portable code and validation

`port/src/panorama.cpp` adds standalone geometry, pitch-limit, signed yaw-offset
and angle-conversion helpers. Geometry uses explicitly supplied dimensions,
integer half dimensions, recovered binary64 constants/stores, and host libm.
The padding can now be supplied to the existing seam-copy helper without
inventing a viewport. Finite/range/dimension checks and rejection of unsupported
seam geometry are portability safeguards; these are not original error paths.
The yaw helper rejects results outside signed int rather than reproducing the
original low-DWORD wrapping or invalid x87 conversions.

This is formula reconstruction, not a pixel-exact projection implementation.
Host libm, long-double precision (especially on Windows), and evaluation rules
can differ from the original x87/CRT. The pitch helper returns binary64 and
therefore does not reproduce a caller comparing against a still-extended ST0
pitch-limit result. No helper is wired into the VM/SDL panorama loader.

`make -C port -j2 all test` passes. Added checks cover explicit synthetic
2496x768/640x480 geometry (P=304), odd half-dimension truncation, equal-height
zero pitch limit (P=270), a smaller viewport (P=198), zoom dependence, literal
angle conversion, positive/negative yaw truncation, and invalid/overflow inputs.
These are formula/regression checks, not original-engine comparison captures.
The evidence verifier passes for all selected sources and byte ranges.
Debian Make tested; Windows/macOS and CMake remain untested this pass.

Next bounded step: reconstruct and validate mapping offsets in `0x423a37`,
using the recovered initialized table bounds and PC53 model documented in
`PANORAMA_TABLES.md`. Keep `0xb9` variable-reset, transitions, resource
composition and `0xbb` as distinct loader blockers. Do not enable a flat JPEG
background as a substitute for projection.

Follow-up: [`PANORAMA_MAPPING.md`](PANORAMA_MAPPING.md) now recovers mapping
entries with explicit state and checked bounds. The normal RGB16 sampling
path, original numeric comparison and VM loader integration remain pending.
