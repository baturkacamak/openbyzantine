# Load-time projection state (0x4236b5)

25 October 2026. Static recovery from saved REA/Ghidra exports plus focused
GNU objdump on the preserved target. No original execution and no new
decompilation session.

Evidence identity: target `analysis/rea/BETRAYAL.EXE` SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`,
profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.
`0x4236b5`, `0x423cdf`, `0x423a37`, `0x4241e2`, `0x401256`, `0x401d29`,
`0x40186e`, `0x401ee6` and `0x401ddf` are among the 28 exports already bound by
`panorama-projection-evidence.json`; `verify_panorama_projection.py` re-verified
the target, 28 export hashes, 18 assembly ranges and 7 data ranges.

## Observed

`0x4236b5(this, pixels, width, height)`:

1. `pixels == null` -> return 1 without touching any field.
2. If `+0x24 != width` or `+0x20 != height` (`0x4236db..0x4236ed`):
   - free `+0x34` if non-null (`0x423713` -> `0x44a820`);
   - store `+0x24 = width`, `+0x20 = height` (`0x423721..0x42372a`);
   - `fild width` / `fdivl two_pi` / `fstpl +0x28` (`0x423733..0x42375b`):
     scale is a binary64 store of `width / 6.28318530718`;
   - `call 0x423cdf` then `inc`, stored at `+0x38`: padding = truncated extent + 1;
   - `+0x1c = padding*2 + width` (`0x42376d..0x42377b`): stride in 16-bit WORDS;
   - allocate `stride*height*2` bytes; `+0x34` = buffer. If null, return 1
     **without** copying rows, seam refresh or reset (`0x4237a3..0x4237ae`).
3. Copy every row: `memcpy(buffer + padding*2 + stride*row*2, pixels + row*width*2, width*2)`
   (`0x4237d1..0x423805`).
4. `call 0x42382b` (seam refresh), **then `call 0x4241e2` (camera reset +
   mapping rebuild)** at `0x423815..0x423818` — both outside the
   dimension-change branch.
5. return 0.

`0x423cdf` uses renderer `+0x4`/`+0x8` as viewport width/height, `+0x28` as
scale and `+0x20` as source height, i.e. the newly stored values from step 2.
So padding and stride depend only on the decoded size and the renderer's
viewport, never on the current camera.

`0x4241e2` (already modelled by `PanoramaProjection::reset`) sets
`+0x50` yaw = pi, `+0x58` pitch = 0, `+0x60` zoom = 1, `+0x68` limit flag = 0,
then calls `0x423a37` to rebuild the mapping.

Consequences that the port previously did not represent:

- A successful image load **always** resets the camera and rebuilds the mapping,
  even when the decoded size is unchanged. Only the geometry is reused.
- Scale, padding and stride are recomputed **only** on a size change. If the size
  is unchanged, the caller-supplied (already recovered) padding stands.
- Stride is `width + 2*padding` words, which is exactly what
  `panorama_mapping` already uses internally.

## Portable implementation

- `panorama_geometry` now also returns `stride_words = width + 2*padding`
  (0x4236b5 `+0x1c`), alongside the existing scale/pitch-limit/padding fields.
- `PanoramaProjection::set_source(tables, width, height)` models the successful
  load path above: returns `true` iff the dimensions changed and geometry was
  recomputed, and always resets the camera and rebuilds the mapping. It computes
  the new geometry and map before committing, so a rejected seam/map cannot
  leave half-updated dimensions. `padding()` and `stride_words()` expose the
  recovered `+0x38`/`+0x1c` fields.
- The row copy itself remains `pad_panorama_rows`; `set_source` does not allocate
  or own the padded buffer, and the allocation-failure branch is not modelled.

## Validation

`make -C port -j2 all test` passes. New assertions:

- EA geometry `2496x768` in a `640x480` viewport -> padding 304, stride 3104.
- A same-size `set_source` keeps 304/3104, rebuilds the identical map, and
  restores a moved camera to yaw=pi, pitch=0, zoom=1, flag=0.
- A `2496x480` load recomputes padding 270 / stride 3036 and rebuilds the map
  accordingly.
- An invalid `width=0` load is rejected and leaves dimensions, geometry, camera
  and mapping unchanged.
- A later same-size load still resets the camera.

These are formula/regression checks, not original-engine captures.

## Not claimed

- No original execution, runtime trace or control-word trace; host libm still
  approximates the CRT, and PC53 is established from the startup `__controlfp`
  call, not from a measured control word at this function.
- Allocation failure, buffer ownership and row-copy byte traffic are not
  modelled; only the geometry/camera state transition is.
- `0xb9`/`0xbb` VM loaders remain unsupported, so `set_source` is a standalone
  helper and is not wired into the scene player.
