# Panorama mapping entries

25 October 2026. Static reconstruction; the original engine was not executed.
No REA session was opened and no decompilation was repeated. Before deriving
behavior, `verify_panorama_projection.py` verified target/profile identity,
20 saved export hashes against `pseudocode/decompile-index.json`, 16 retained
assembly listings/target byte ranges and seven data ranges. The existing
`panorama-projection-evidence.json` binds all sources used here, including
`0x423a37`, `0x423310`, `0x4236b5` and `0x4242bd`. Camera verifier also passes.

## Observed

`0x4236b5` stores unpadded source width at +0x24, source height at +0x20,
S=binary64(width/6.28318530718) at +0x28, padding P at +0x38 and padded
stride T=width+2P at +0x1c. Thus mapping uses padded stride, while angular
scale uses the unpadded width. The table pointer is +0x3c. Viewport dimensions
are +4/+8, pitch is +0x58 and zoom is +0x60.

In `0x423a37`, set F=binary64(zoom*S), c=fcos(pitch), s=fsin(pitch).
Coordinates are x=column-floor(viewport_width/2) and
y=row-floor(viewport_height/2), including odd dimensions. For each row:

```
N = (s*F + c*y)*S
R = 1/(c*F - s*y)
```

The instruction order matters: `0x423b7e..0x423b9b` multiplies each term
separately, adds or subtracts, then scales or reciprocates. For each pixel,
with Q the bias-based coordinate conversion described in PANORAMA_TABLES.md:

```
a  = arctangent[Q((R*x)*1000)]  # initialized binary32 table entry
hx = Q(a*S)
v  = Q((cosine[Q(a*1000)]*N)*R)
entry = (v + floor(source_height/2))*T + hx + floor(T/2)
```

`0x423bc0..0x423bea` establishes the horizontal and vertical multiply order.
`0x423bef..0x423c00` stores one DWORD entry; it is an RGB16 **word offset**,
not a byte offset. The inner loop increments x by one; rows increment y by
one. No yaw load occurs in this generator. The normal branch of `0x4242bd`
reads source words at `base + 2*entry + 2*yaw_offset`; the existing assembly
at `0x42459f` recovers yaw_offset=trunc(S*yaw). The effect branch has distinct
random/mask behavior and is not implemented by this helper.

## Portable reconstruction and inferences

`panorama_mapping` in `port/src/panorama.cpp` returns a row-major vector of
signed 32-bit offsets for explicitly supplied source dimensions, padding,
viewport dimensions, pitch and zoom. It uses the initialized-only lookup
accessors and nearest/even Q helper. Each arithmetic instruction has a
separate volatile binary64 result store to prevent fused multiply-add or a
host long-double expression from silently replacing PC53 operation rounding.
It requires an IEC559 binary64 host in FE_TONEAREST and leaves that mode
unchanged. This is a formula model with host sin/cos and table values.

Within normal binary64 arithmetic, the separate basic operations model
53-bit precision. This does not reproduce the x87 exponent range, underflow
behavior, hardware transcendental results retained in extended registers, or
the original CRT table values. In particular, mapping uses hardware fsin/fcos,
whereas this model obtains rounded binary64 host-libm results. Effective
original runtime RC is still unknown. These are not original pixel captures
or evidence of bit equality.

Port safeguards reject invalid dimensions/state, negative or nonfinite depth,
nonfinite coordinate results, uninitialized lookup access, offsets outside
the padded source and unsupported 32-bit allocation/offset arithmetic.
The original routine has no visible per-access guards; rejection is deliberate
port behavior rather than a recovered original error path. No coordinates
are clamped and no missing state/table cells are invented. Output source bounds
are checked **before yaw**; this is not a guarantee that a later yaw addition
is safe. A renderer must validate its final addresses separately.

## Validation

`make -C port -j2 all test` passes on Debian. Mapping regressions include:

- A manually checkable 64x16 source, padding=2, viewport=5x3 at zero pitch;
  exact word offsets distinguish stride, row ordering and byte/word mistakes.
- Odd source/viewport integer halves; zoom=2 contraction; positive/negative
  pitch sign; rejected depth, NaN, invalid zoom/dimensions and integer overflow.
- An arctangent lookup reaching the unwritten +2000 edge and an out-of-source
  row both fail explicitly.
- Non-nearest host rounding is rejected without changing the caller's mode.
- Explicit synthetic 2496x768 source / 640x480 viewport / P=304: all 307,200
  offsets fit the padded source at zero pitch and both recovered unit-zoom
  pitch limits (921,600 entries total); center and horizontal symmetry checked.

These are formula/structural tests, not an independent original-engine oracle.
Windows/macOS and CMake remain untested. `0xb9`/`0xbb` are still unsupported;
VM/SDL integration, reset/transition/resource effects and RGB16 decoding are
not supplied by this mapping helper.

## Next bounded step

Recover the normal RGB16 sampling path in `0x4242bd` with focused retained
assembly of the inner loop: final yaw address bounds, destination pitch and
rectangle offsets, plus map regeneration conditions. Keep its nonzero effect
parameter branch explicit and unsupported until its random/mask arithmetic is
resolved. Separately compare tables and full mapping against controlled x87
and ultimately original-engine execution before claiming pixel exactness.
