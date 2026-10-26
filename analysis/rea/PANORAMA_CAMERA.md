# Panorama camera consumption and seam buffer

25 October 2026. Static recovery only; no original game execution and no new
decompilation session. `panorama-camera-evidence.json` records target/profile
identity and 17 selected exports. `verify_panorama_camera.py` verifies these
against `pseudocode/decompile-index.json` and the actual target/export bytes.

## Observed in saved REA/Ghidra exports

- `0x401ddf` clears both camera delta globals on entry. Only when staged x at
  0x45c064 differs from -1 does it request projection reset (`0x4241e2`, if a
  renderer exists), set vertical delta to 0 or -6 according to 0x464878, compute
  `clamp(floor(x / panorama_width * 360 + 0.5), 0, 360)` and clear staged x to
  -1. The division uses width global 0x46487c. Thus EA x=747 at width=2496
  produces 108 degrees; x=2495 produces 360, not 359.
- `0x4241e2` sets renderer +0x50 to pi, +0x58 to zero, +0x60 to one and +0x68
  to zero, then rebuilds its mapping. This separates a camera reset from the
  delta values passed to rendering.
- `0x4242bd`, through wrapper `0x42423d`, adds horizontal degrees in radians to
  renderer +0x50 and wraps to [-pi, pi]. It SUBTRACTS vertical degrees in radians
  from +0x58. The -6 setup value is therefore a delta, not an absolute -6-degree
  renderer pitch. Zoom multiplies +0x60 and has a lower bound of one. Pitch
  limits are computed through `0x423c42` and mapping rebuilt when needed.
- `0x4236b5` stores source width W at +0x24, height H at +0x20, scale W/(2*pi)
  at +0x28, padding P at +0x38, and row stride W+2P at +0x1c. P comes from
  `0x423cdf` plus one. Source rows are copied into the middle of a 16-bit-word
  buffer at offset P. `0x42382b` copies the last P source pixels into the left
  pad and first P source pixels into the right pad, independently per row.
- `0x401256` establishes the renderer, loads `nodes/sm/sm00.jpg`, and gives
  its decoded dimensions/pixels to `0x4236b5`. `0x401d29` reuses the established
  image dimensions/buffer for later JPEG decoding and refreshes seam copies.
- `0x40186e` calls camera setup after image loading. Transitions other than
  100/106 prepare a surface first; 101..106 select distinct transition calls.
  The branch for transition !=100 clears camera deltas before its final
  `0x401ee6` call. Rendering also invokes `0x43be09` resource composition;
  camera-only setup does not cover this loader's behavior.

## Inference and unresolved details

The renderer uses a cylindrical panorama mapping, rather than a flat scaled
JPEG: width/(2*pi), horizontal angular addressing and seam duplication support
that interpretation. `0x423a37` generates per-pixel offsets with pitch sin/cos,
perspective division, and two lookup tables constructed by `0x423310`.

Pixel-exact projection remains unresolved. The saved pseudocode loses x87
return expressions in `0x402e00`/`0x402e70` and the expression feeding `__ftol`
for yaw offset in `0x4242bd`. Its mapping conversion uses the low word of a
double plus 6.755399441056e15, which must not be replaced with an assumed
rounding rule. Trigonometric wrappers `0x44bd8a`, `0x44bd80`, `0x44d431`, the
padding calculation and assembly-level conversion behavior still need focused
inspection. No visual or runtime equality is claimed.

## Portable implementation and validation

`port/src/panorama.cpp` implements two bounded, standalone helpers:
`consume_panorama_position` and `pad_panorama_rows`. Explicit known x=-1 means
no staged request; the VM's unknown optional camera state is not passed to
this helper or silently converted into that sentinel. Width and alternate-mode
flag are explicit inputs. The row helper takes caller-supplied padding; it
does not guess the projection-dependent P. Input validation and size overflow
checks are portability safeguards, not claimed original-game error behavior.

Neither helper is connected to `0xb9` yet. The strict VM and SDL action commit
behavior are unchanged; unsupported loaders still stop execution.

`make -C port -j2 all test` passes, including new tests for EA x=747, endpoint
and half-degree rounding, one-time consumption, both mode branches, rejection
without consumption, distinct multirow seam words, full 16-bit preservation,
zero/single-pixel padding and invalid/overflowing dimensions. Existing scene
and variable-metadata tests pass. Make was tested on Debian; CMake wiring is
updated but not exercised, and Windows/macOS builds remain unverified.

Next: inspect only focused x87 disassembly around these projection functions
and their trig wrappers; reconstruct table/conversion and padding behavior
before enabling a renderer. Keep `0xb9` variable-reset/resource/transition
effects and `0xbb` as separate required blockers.

## Follow-up: projection arithmetic

The same-day continuation resolved the missing angle returns, yaw __ftol input,
trig descriptor identities and padding formula in PANORAMA_PROJECTION.md.
The paragraph above describing those expressions as unresolved is historical.
Table quantization precision and pixel-exact mapping remain unresolved.
`panorama_geometry` now computes padding from explicit dimensions; callers may
still provide known padding directly. No VM loader has been enabled.
