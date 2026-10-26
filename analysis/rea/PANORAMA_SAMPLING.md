# Normal RGB16 panorama sampling

25 October 2026. Static recovery from saved REA/Ghidra exports and focused
GNU objdump; no original-engine execution or new REA session. Before deriving
behavior, `verify_panorama_projection.py` verified the target/profile and
selected export hashes against `pseudocode/decompile-index.json`. The expanded
`panorama-projection-evidence.json` retains two additional assembly listings
and their target byte ranges; verification now covers 20 exports, 18 assembly
ranges and seven data ranges. Sources: `0x4242bd`, `0x423310`, `0x423a37`,
`0x423c42`, and the previously retained yaw/CRT exports.

## Observed: destination and normal source addressing

`0x423310` copies a supplied four-coordinate rectangle into object +0xc
(left), +0x10 (top), +0x14 (right), +0x18 (bottom). Width +4 and height +8
are right-left and bottom-top. The null-rectangle constructor path starts at
(0,0) and uses surface dimensions. These facts establish pixel coordinates;
the DirectDraw surface description/Lock identification is an interface-layout
inference from structure size 0x6c and vtable slots +0x58/+0x64/+0x80.

`0x4242bd` fails with return 1 if source +0x34 is null or surface lock fails.
On successful lock, +0x24 in the local descriptor supplies the base pointer
and +0x10 supplies signed byte pitch B. `0x424381..0x42439e` computes:

```
first_destination_byte = surface_base + 2*(top*(B >> 1) + left)
row_tail_bytes = B - 2*viewport_width
```

The row tail is set at `0x4245c8..0x4245d2`. Thus for positive even B the
rectangle origin is exactly top*B+2*left and each subsequent row advances B.
For odd B the initial expression discards a pitch bit before multiplication;
negative pitch uses arithmetic shift. Those layouts are outside the helper's
supported domain, rather than silently normalized to a conventional surface.

After yaw update/wrap into the inclusive [-3.14159265359, +3.14159265359]
interval, `0x42459f..0x4245b6` computes signed Y=trunc(S*yaw) via __ftol.
The last argument (stack +0x28) selects the effect. Zero enters the normal
assembly loop at `0x4245df..0x424625`:

```
for row in 0..viewport_height-1:
    for column in 0..viewport_width-1:
        byte_offset = 2*mapping[next] + 2*Y
        destination_word = source_word_at(byte_offset)
    destination += row_tail_bytes
```

Mapping advances by four bytes per pixel, destination by two. There is no
mask, blend, source modulo, per-row source clamp, or bounds check in this
branch. The word is copied unchanged, including high bits. A final index can
cross a source row boundary because it is a flat offset. To be safe in the
port, each final mapping[i]+Y must independently satisfy
0 <= index < padded_source_word_count. Earlier pre-yaw map bounds alone do
not establish that condition. Signed 32-bit shifts/adds in the original can
wrap; the port rejects unsupported offset domains instead of using wrapping
addresses. `0x42471d` onward unlocks the surface, returning 0 on success, 1 on
failure; this standalone sampler does not emulate the lock lifecycle.

## Observed: mapping regeneration conditions

For finite normal inputs, `0x4243aa..0x4244ef` skips camera-pitch/zoom changes
and regeneration exactly when vertical_delta_degrees == 0 AND zoom_factor == 1.
Otherwise:

1. pitch = binary64(pitch - vertical_delta_degrees*pi/180).
2. zoom = max(binary64(zoom*zoom_factor), 1).
3. Compute L with `0x423c42` at the updated zoom. If pitch >= L, assign +L and
   set +0x68 to 1. Otherwise if pitch <= -L, assign -L and set +0x68 to 1.
   Strictly interior pitch sets +0x68 to 0. Equality at either limit is flagged.
4. Call `0x423a37` unconditionally on that branch, even when the zoom floor or
   pitch clamp leaves the eventual geometry unchanged.

Yaw is then updated on every successful-lock draw. Yaw alone never triggers
map regeneration in this routine. With neutral pitch/zoom inputs, +0x68 is
left untouched. The upper-bound comparison uses the x87 return value before
popping it, while the clamp assignment and negative-limit comparison use the
stored binary64 L; a future state helper must preserve or disclose that
precision distinction. Nonfinite input flag semantics and the adjusted-FDIV
path are not modeled here. The two intervening DWORD arguments are not read
in the retained renderer body; no semantic names are assigned to them.

## Portable helper and deliberate safeguards

`sample_panorama_rgb16` in `port/src/panorama.cpp` accepts explicit padded
RGB16 words, a pre-yaw map, known signed yaw word displacement, destination
words, a pixel rectangle with byte pitch, and an explicit effect argument.
It supports positive even pitch and positive viewport dimensions only.
All final source addresses and destination/map extents are validated before
any write. Invalid state, odd/negative pitch, aliased buffers, unsupported
signed 32-bit byte-offset/allocation domains, and any nonzero effect throw.
The rejection/atomic-preflight policy is deliberate portable behavior, not
an observed original error path. Numeric uint16_t words preserve the pixels
on any host; this helper does not parse serialized little-endian bytes or
infer RGB555/RGB565 channel masks.

No renderer state/cache update, surface acquisition, JPEG decoding, SDL or
VM loader integration is supplied by this helper. Callers remain responsible
for explicitly known camera/map state. `0xb9` and `0xbb` remain unsupported.
The effect branch's random/mask arithmetic remains outside the implementation.
Existing mapping/trig/table approximations still prevent pixel-exact claims.

## Validation

`make -C port -j2 all test` passes on Debian. New checks include manually
specified RGB16 outputs at yaw 0/+1/-1, a nonzero rectangle origin, padded
byte pitch and untouched destination guards; full-word values 0x8001/0xffff;
first/last source words and flat cross-row addressing; invalid late entries
that must leave the entire destination unchanged; nonzero effects, invalid
pitch/dimensions/map size, final negative/overflow offsets and alias rejection.

The existing synthetic 2496x768 padded source / 640x480 viewport is sampled
at zero pitch and both recovered unit-zoom pitch limits, each at yaw +/-pi:
1,843,200 copied pixels plus untouched rectangle borders checked. These are
formula/structural regressions, not independent original-runtime captures.
Windows/macOS and CMake were not tested in this step.

## Next bounded step

Recover the projection reset and load-time camera/map initialization callers
of `0x4242bd`/`0x423a37` from saved exports, then implement an explicit known
projection state/cache update helper with the regeneration rules above.
Verify target and selected export hashes first. Keep VM loader variable resets,
transitions/resource effects, unknown state and nonzero sampling effects
explicitly unsupported. Controlled x87 and original-runtime comparisons are
still needed before any claim of pixel equality.
