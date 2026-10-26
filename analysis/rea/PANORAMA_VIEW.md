# Panorama view: 0x4240ae inverse mapping and the playable node scene

26 October 2026. Static recovery plus port implementation. No original runtime
execution and no live binary session. Everything below is either **observed** in
the saved exports (address + hash listed), a **port decision** marked as such, or
listed as **unknown**.

## Verified evidence

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`. Every
function below was re-hashed against `pseudocode/decompile-index.json` and
matched (20/20):

| Address | Range | Role | SHA-256 (first 16) |
| --- | --- | --- | --- |
| `0x42db22` | `0x42db22-0x43194e` | dispatcher case `0xb9`/`0xba`/`0xcb` | `8df50fc5339a585b` |
| `0x432295` | `0x432295-0x432302` | node setup, `vertical` -> `DAT_0047a8dc` | `ebc09d92f42be6b1` |
| `0x40186e` | `0x40186e-0x401b4b` | `nodes\<2>\<name>.jpg`, load, transitions | `54ebff7cca860ec4` |
| `0x401d29` | `0x401d29-0x401dde` | JPEG load against the renderer's size | `4ce100ddf9de5812` |
| `0x401256` | `0x401256-0x401387` | renderer created; size from `nodes/sm/sm00.jpg` | `14244ab4d11bb8b5` |
| `0x40f135` | `0x40f135-0x40f341` | strict JPEG load (rejects a size mismatch) | `29c043e4379149ef` |
| `0x401ddf` | `0x401ddf-0x401ee5` | staged pixel position -> camera deltas | `5ef900b27f456012` |
| `0x401ee6` | `0x401ee6-0x402073` | camera advance + draw | `b9dbc196cbe49ddf` |
| `0x4236b5` | `0x4236b5-0x42382a` | seam-padded rows, geometry, camera reset | `468a314e335ed2f9` |
| `0x42382b` | `0x42382b-0x423976` | seam columns | `43de272114fabc47` |
| `0x4241e2` | `0x4241e2-0x42423c` | camera + map reset | `371e58cfb0d6be8c` |
| `0x42423d` | `0x42423d-0x42427a` | delta wrapper | `107803a432a66409` |
| `0x4242bd` | `0x4242bd-0x42475e` | advance + per-viewport-pixel sampling | `257fcd5a5d11d854` |
| `0x4240ae` | `0x4240ae-0x4241e1` | **inverse** mapping (screen -> source) | `b7440cc98f16b49f` |
| `0x401ca6` | `0x401ca6-0x401d28` | click point -> playfield/inverse -> bias back | `6626eb354a5894b9` |
| `0x40c02c` | `0x40c02c-0x40c328` | object rect hit test on that point | `ef06dfae27973bb1` |
| `0x402285` | `0x402285-0x40241b` | `0xcb`: rotate toward a source column | `07b43532650219e5` |
| `0x40241c` | `0x40241c-0x4025c6` | clip window used by the draw pass | `d70c4342c1903b9d` |
| `0x401c21` | `0x401c21-0x401c62` | `0xba` stages `DAT_0045c064` (0..0x9bf) | `4d9a78bcd8ff0f42` |
| `0x4069a4` | `0x4069a4-0x406cbe` | font glyph blit (not used in this step) | `ab6b7bd02064ab5d` |

## Observed behaviour

### The renderer's node size is fixed at startup

`0x401256` creates the renderer, then loads `nodes/sm/sm00.jpg` with
`0x40f135(path, &DAT_0046487c, &DAT_004654d8, NULL, 0)`. Because the buffer
argument is NULL, this call **writes** the decoded size into those two globals.
`0x401d29` (every later node) passes the same globals as the *expected* size, and
`0x40f135` returns NULL when the decoded width/height differ. So a node whose
size differs from `sm00.jpg` is a failed load, not a rescale. The shipped
`nodes/` census is 68/68 at **2496x768**, so the port takes the size from the
first successfully loaded node and rejects a later mismatch with an explicit
message (a stand-in for the startup file it does not read).

### The inverse mapping (0x4240ae)

Given the renderer object and a viewport point `(x, y)`:

```
linear = map[y * viewport_width + x] + yaw_word_offset
column = linear % stride_words - padding        // C truncating %
while (column >= source_width) column -= source_width
while (column < 0)              column += source_width
row    = linear / stride_words                  // C truncating /
```

`stride_words = source_width + 2*padding` (`+0x1c`), `padding` is `+0x38`, the
unpadded width is `+0x24`. `map` and `yaw_word_offset` are exactly the values
`0x4242bd` adds inside its sampling loop (`*puVar7 = *(ushort *)(source +
map[i]*2 + yaw*2)`), which is why the round-trip in `panorama-view-test`
compares the two directions instead of trusting either alone. The `__ftol`
Ghidra lost in `0x4240ae` is the same float yaw displacement
`panorama_yaw_offset` already models (port inference; the sampling loop makes it
the only consistent value).

### The click path (0x401ca6 -> 0x40c02c)

```
if (no renderer) || (y < 0x14) || (0x1a3 < y):  point = screen point
else:                                           y -= 0x14;
                                                0x4240ae(renderer, point)
                                                y += 0x14
```

`0x40c02c` then walks `DAT_00464300` **ascending** and takes the first object
that is active (`+0x08 != 0`) and satisfies
`x0 <= x <= x0 + w` / `y0 <= y <= y0 + h` (**inclusive** on both edges). A
kind-2 object first asks `0x40d84d`; that branch is not modelled.

Consequence, and the reason this step matters: for a panorama scene the object
rects are in **source** coordinates. The shipped EA table has rects up to
`x = 2422 + 74 = 2496` and `y = 665 + 99 = 764`; those hotspots can never be hit
by a screen-space test.

### Camera rotation from the script (0x402285, opcode 0xcb)

```
0x4240ae(renderer, {0x140, 0xf0})         // viewport centre -> source column
distance = abs(operand0 - centre_column)
forward  = centre_column < operand0
if (source_width/2 < distance) { forward = !forward; distance = source_width - distance; }
for (offset = 0; offset < distance; offset += step)
    degrees = min(step, distance-offset) / source_width * 360
    0x401ee6(forward ? +degrees : -degrees, 0, ...)
```

`step` is 10 unless operand 1's type byte is `0x40`. Each iteration also draws
(0x401ee6), i.e. this is a visible scroll animation; the port applies the same
slices in the same order without presenting every intermediate frame.

### `vertical` is not a camera parameter

`0x432295(node, force, vertical, transition)` only reacts to `vertical > 0` by
writing `_DAT_0047a8dc = (float)0x401c63()`. `0x417491` subtracts it from a
coordinate (`local_18 = param_1 - _DAT_0047a8dc`) while building a text request
— observed arithmetic; that it is the **message text** row is an inference from
the caller. It is not a panorama parameter either way, so the port reports the
operand (`panorama_text_offset=`) and does not move the camera with it.

`0x401ddf` sets the *pending* camera deltas: horizontal = the staged pixel
position converted to degrees, vertical = 0 or `-6` when `DAT_00464878 != 0`
(the transition alternate mode), then `0x42423d`/`0x4242bd` subtracts the
vertical delta from the stored pitch. It also calls `0x4241e2` (camera + map
reset to yaw = +pi) **before** exposing the delta, so a staged position is an
**absolute** view, not an increment. The port passes `alternate_mode = false`
outside a transition; the transition path (`0x418bcf`, `0x42bce1`, `0x42be07`,
`0x419cf6`, `0x418eb2`, `0x419a51`) remains unimplemented.

## Port changes

New: `port/include/byzantine/panorama_view.hpp`, `port/src/panorama_view.cpp`
(`PanoramaView`), `port/tests/panorama_view.cpp` (`panorama-view-test`).

- `PanoramaView(path, w, h, pixels, viewport_w, viewport_h)` builds the padded
  rows through `pad_panorama_rows`, then the projection through
  `panorama_geometry`. `padding()`/`stride_words()` come from the projection, so
  there is one source of truth for `+0x38`/`+0x1c`.
- `apply_staged_x()` wraps `consume_panorama_position`, performs the `0x4241e2`
  reset when the position is absolute, and advances the camera, i.e. `0x401ddf`
  + `0x42423d`.
- `advance()`, `render()` and `canvas_point()` are `0x4242bd`'s camera phase,
  its sampling phase (effect 0) and `0x4240ae`.
- `byzantine-play` applies the trace's `panorama_loads`/`panorama_rotations`
  once per committed label run (`apply_panorama_events`), so a gated (audio)
  replay cannot apply a delta twice. The scene layer is whichever of `0x69`
  /`0xb9` ran last in that trace; a node from an earlier label stays current.
- Clicks run through `hotspot_at`, which uses the inverse mapping for panorama
  scenes and the plain playfield point otherwise, then the inclusive ascending
  rect test.
- New CLI: `--staged-x <0..2495>` (headless camera position) and
  `--click-at <x>,<y>` (the real click path in `--shot` mode).
- `byzantine-cc --panorama` now uses the same `PanoramaView`; its output line is
  byte-identical to the previous step.

## Real results

Data: CD1 `copy2hd/engine/*`, all `savegame/*.btb`, `goldsave/GAMEPREF.000`,
`backs|nodes|anims|sounds` for EA/NB/VT/MAP; CD2+CD3 `sounds/vt`, `backs/vt`.

1. `EA` 602 with save `26.btb`:
   `panorama_load instruction=317 name=ea00 record=0 staged_x=none yaw=180
   loaded` then `composed panorama=nodes/ea/EA00.jpg size=2496x768 padding=313
   yaw=180 pitch=0 drawn=1 skipped_no_frame=0 pixels=13570` (349x310 px of the
   room, the record sprite over it). `staged_x=700 yaw_degrees=101 yaw=-79`
   shows a different part of the flat; `--staged-x 700 --shot` renders it.
2. `EA` click 246,294 at yaw 180: `click at 2423,496 (panorama) slot 4 action
   586 click_result steps=11` (the rect is `x=2422 y=496 74x52`, i.e. **x >
   640**). The same pixel at yaw -79 maps to `627,306` -> `slot 2 action 653
   stopped: unresolved random seed at instruction 328` (an honest stop, not a
   fabricated success); the same point at yaw 180 maps to `2422,306` and hits
   nothing.
3. `NB` 90 with save `57.btb`: `panorama_load instruction=98 name=NB00
   staged_x=none yaw=180 loaded`, `--staged-x 600 -> yaw_degrees=87 yaw=-93`,
   and screen 192,272 -> `480,473 slot 5 action 233 click_result steps=23`.
   Both frames were exported as PPM and inspected: no seam column, ceiling and
   wall lines stay continuous.
4. `byzantine-cc output/cc_all/EA --trace 602 ... --compose-out X.ppm
   --panorama nodes/ea/EA00.jpg --panorama-x 700` still prints
   `panorama=... size=2496x768 padding=313 yaw_degrees=101 pitch_degrees=0
   sampled=256000`.

## Validation

- `panorama-view-test`: a synthetic 256x96 source encodes `(row, column)` in
  every pixel. For every one of the 64x40 viewport pixels, the rendered value
  must decode to exactly the coordinate `canvas_point` returns. Checked after
  reset (yaw = +180), a yaw-only advance (`0x4242bd` does not rebuild the map),
  a pitch advance (map rebuilt), a staged position (absolute, 180 degrees) and 24
  further rotations (wrapping). Also: a staged position must land in the same
  view from any previous camera state, out-of-viewport points are reported
  unmapped, a mismatched destination is rejected, a short source is rejected and
  a staged position outside 0..2495 is rejected.
- `make -C port -j2 all test` passes all nine suites (new `panorama-view-test`
  included); the three panorama verifiers
  (`verify_panorama_bindings.py` 182 load commands / 17 missing,
  `verify_panorama_camera.py` 17 exports, `verify_panorama_projection.py`
  28 exports) pass.
- Non-panorama acceptance is unchanged: `MAP` 87 -> `composed
  background=MAPFINM1.jpg drawn=27 pixels=163079`, `--click 34` -> `action 269`
  `steps=6`; `VT` 72.btb 497 `--selftest-events --click 14` ->
  `audio step=4 id=35 name=voTins1 duration_ms=4709 ... (script continues)`,
  `gate stopped_after_step=1 of=12`, `click_result steps=12 dialogue_visible
  text=1 sounds=1 fingerprint=0x849a49bff1a28319`, `wait_selftest
  clicks_ignored=1 overlay_toggled=true`.

## Unknown / not modelled

- **Unknown:** whether `0x4240ae`'s lost `__ftol` is the same float yaw
  displacement as the sampler's (port inference above).
- **Unknown:** `0x40d84d` / the kind-2 object branch of `0x40c02c`, and
  `0x4242bd`'s `param_6 != 0` branch (`_rand`-driven noise rows using `+0x30`).
- Not modelled: the transition effects (`0x418bcf`, `0x42bce1`, `0x42be07`,
  `0x419cf6`, `0x418eb2`, `0x419a51`) and the transition codes `0x65..0x6a`;
  `_DAT_0047a8dc` (text offset); the `+0x09 & 0x20` snap pass, the
  `+0x09 & 0x80` destination-read blend (`0x42b5b2`) and the `+0x3c` trail list
  of the record pass.
- Not modelled as *input*: the original's cursor zones `0x0..0x28`
  (`DAT_0045f548`, `0x41e289`) — the on-screen look/turn buttons. The port uses
  arrow keys and left-drag; the mapping from those to a host input is a port
  decision, not original behaviour.
- The hotspot outline overlay is not drawn for panorama scenes: the rects are in
  source space and cannot be drawn as screen rectangles through the cylindrical
  projection.
