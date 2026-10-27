# Panorama scene sprites: 0x40241c's projection branch

Date: 27 October 2026. Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`, profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

## Reported symptom (pinned user run)

`run-20261027T132308.815408Z-eox_w1hb`: EA 602 / `26.btb` -> MAP (index 25) ->
SM (index 30, `entry scene=SM steps=230 stopped_at=148 hotspots=5 records=2`).
The frame is the `nodes/sm/SM00.JPG` panorama. Clicking the character hotspot
(`click at 1329,416 (panorama) slot 7 action 375`, screen `280,256`) starts the
`IImSM1a` conversation, but the character is **never drawn**:

```
composed panorama=nodes/sm/SM00.JPG ... drawn=0 projected=0 ... pixels=0
record id=3 kind=0 flags=0x0 x=1302 y=353 frames=22 asset=smim00b
record id=4 kind=0 flags=0x0 x=1241 y=456 frames=1  asset=SMEmSh1
```

Both records are at x = 1241/1302 on a 640-wide surface. The same scene's 0x72
object 7 (the character's click rect) is `x=1302 y=335 w=65 h=139`, i.e. the
sprite and the hotspot use the **same panorama source coordinate space**. The
port treated the record position as a screen position, `0x40241c` rejected the
top-left as off-window, and nothing was composited. Every panorama compose in
the saved logs showed `drawn=0`; no panorama scene had ever drawn a sprite.

## Evidence

### `0x40241c` (objdump, not the Ghidra text)

`FUN_0040241c` has two branches. With no panorama it is the familiar clip test
(`0 <= x <= 0x27f`, `0 <= y-0x14 <= 399`, clip w/h, `SetRect`). With
`DAT_0045c060 != 0 && DAT_00464880 != NULL` it takes the panorama branch:

* it reads the record rect from `param_1`/`param_2` (+`0x10`/`0x14` = w/h) and
  builds five anchors in this order: **centre**, **top-left**, **bottom-right**,
  **top-right**, **bottom-left** (`0x40248d`..`0x4024ac`);
* each anchor is passed to `0x423e03` (source -> viewport). The first anchor
  that returns 1 gives the sprite's top-left, e.g. centre success writes
  `x = projected.x - w/2`, `y = projected.y - h/2` (`0x4024c7`..`0x4024e2`);
  bottom-right writes `x = projected.x - w + 1` (`0x402531`..`);
* if all five fail (`0x4025b6`) it returns 0.

The write-back at `0x402626`/`0x402632` (`*param_1 = x`, `*param_2 = y+0x14`)
belongs to the shared non-panorama path; the panorama branch returns at
`0x4025bd` (`mov $1,%eax; jmp 0x402716`) without storing. Ghidra's pseudocode
drops the projection values entirely, which is why `DRAW_PATHS.md` recorded
"test only; the draw position is not camera-projected". The assembly shows the
projected anchor being computed in the two slots the draw loop reads
(`&local_28`/`&local_30`), so the placement is what the code computes; the port
applies it. This is an **inference** from the shipped data plus the dead
computation, not a recovered store.

### `0x423e03` (the projection)

* input `/` output bounds: input `0 <= x < this+0x24`, `0 <= y < this+0x20`
  (source width/height), output checked against `this+0x04`/`this+0x08`
  (viewport 640x400). `0x4236b5` stores source width/height at `+0x24/+0x20`,
  `W/2pi` at `+0x28`, padding/stride at `+0x38/+0x1c`; `0x4241e2` resets
  yaw = +pi (`+0x50`), pitch = 0 (`+0x58`), zoom = 1.0 (`+0x60`) and
  `0x4242bd` adds the deltas.
* angle: `(x - W/2 - yaw*scale) / scale`, wrapped by two x87 loops into
  `[-pi, pi]`, then rejected outside `+/-1.0471975511966667` (60 degrees,
  constants `0x45b168`/`0x45b170`; `2pi` is `0x45b140`).
* the trigonometric helpers are MSVC dispatch stubs: `0x44bd80` -> table
  `0x463452` = "sin", `0x44bd8a` -> `0x463472` = "cos" (the tables carry their
  own names), and `0x44bc38` is `__ftol`.
* The export's multiply/divide chain is self-contradictory (it scales several
  terms by the depth the sampling map divides by), so the port does not copy it
  literally; it uses the **exact inverse of `panorama_mapping`** (the model the
  port already renders and validates), with
  `S = W/2pi`, `F = zoom*S`, `dy = y - H/2`:
  `depth = S*cos(pitch)*cos(angle) + dy*sin(pitch)`,
  `dx_screen = F*S*sin(angle)/depth`,
  `dy_screen = F*(dy*cos(pitch) - S*cos(angle)*sin(pitch))/depth`.

### Shipped data is source-space

| scene | layer | record | position | asset | matching object |
| --- | --- | --- | --- | --- | --- |
| SM | `nodes/sm/SM00.JPG` 2496x768 | 3 | 1302,353 | `Smim00b` 150x150 | slot 7 `x=1302 y=335 65x139` |
| SM | same | 4 | 1241,456 | `SMEmSh1` 23x13 | (shadow) |
| VA | `nodes/va/va04.jpg` 2496x768 | 7..10, 21, 22 | 1283, 1593, 349, 65, 1868, 2250 | `va04fl0x`/`va05fl0x` | object slots 19..39 |

`Smim00b` frame 1 renders two standing figures (Ima and Emre) inside one 150x150
frame, so the sprite is the character pair; the panorama itself has no people at
that spot (verified by cropping `SM00.JPG` at 1250..1510 x 320..580).

## Port change

* `byzantine::SceneProjector` (`compose.hpp`) — the 0x423e03 direction as an
  interface, so `compose_scene` stays independent of the renderer.
* `PanoramaView::project_source` (`panorama_view.cpp`) — the formula above, the
  source/viewport bounds, the +/-60 degree window and `__ftol` truncation.
* `project_record_position` (`cc.cpp`) — 0x40241c's five-anchor order.
* `compose_scene(state, surface, projector)` — a panorama layer passes the
  live `PanoramaView`, a background/close-up passes `nullptr` and keeps the
  record x/y as screen coordinates. `SceneCompose::projected` counts placements
  and the panorama log line reports `projected=`.
* `scene_animates(view)` uses the same anchor test (with the current frame's
  geometry) so a projected actor keeps repainting while it animates.
* `byzantine-cc --panorama --compose-out` passes its view as the projector and
  prints `projected=` too.
* New diagnostic input `--panorama-yaw [SCENE:]degrees` (absolute yaw, applied
  after the trace's own 0xb9/0xcb camera work; a port input, not game state).
  `--staged-x` is consumed by the first panorama scene, so a journey test needs
  this to look at a known part of a later node.

## Verification (real data)

Full journey, the user's own click chain (EA 602 / `26.btb`, MAP, SM) plus the
documented camera input:

```
./port/scene.sh ".." EA 602 26.btb --staged-x 1347 \
  --click-at 158,236 --click-at 604,220 --click-at 157,200 --click-at 150,203 \
  --panorama-yaw SM:17.5865 --shot /tmp/sm.ppm
```

* `click at 1191,437 slot 7 action 464` (EA), `click at 996,420 slot 154 action
  1790 scene_switch=25` -> MAP, `slot 41 action 433`, `slot 7 action 444
  scene_switch=30` -> SM (`travel index=30 scene=sm from=MAP`).
* SM: `composed panorama=nodes/sm/SM00.JPG ... yaw=17.5865 pitch=0 drawn=2
  projected=2 ... pixels=27465 drawn_ids=4,3`. The screenshot shows Ima and
  Emre standing in front of the railing, at the position of the click rect
  (`port/output/panorama-sprites/sm-entry-projected.png`).
* The next click in the same camera state, `280,256` -> `1329,460 slot 7 action
  375 conversation record=0`, still starts `IImSM1a`, so the drawn sprite and
  the hotspot agree (fonts were not supplied in the headless run, so it stopped
  with the documented "answers cannot be drawn" error).
* VA `--entry` 64.btb: was `drawn=0`, now `drawn=2 projected=2 pixels=195000
  drawn_ids=22,21` (the va05 brazier records now land on their authored source
  columns; `port/output/panorama-sprites/va-entry-projected.png`).
* Unchanged (no projector): MAP `drawn=6 pixels=33668`; CLOCK `drawn=3
  pixels=71239`; HSTMED `drawn=10 pixels=14237`; IU `drawn=3 pixels=15316`;
  EA/NB/AM/GC/VH panorama entries stay `drawn=0` (their records are flag 0x20 or
  have no frames).

Tests: `panorama-view-test` gains `check_projection_matches_render` — the
rendered synthetic source encodes its own coordinates, so every painted source
pixel must project back to the viewport pixel it was painted at (within two
pixels; the sampler's x87 tables and the analytic inverse are not bit-identical)
across reset/yaw/pitch/staged/24-rotation states, plus the source-bound,
behind-camera and edge rejects. `scene-state-test` composes a source-space
record through a fixed projector (centre anchor placement) and a rejected one.
`make -C port -j2 all test` passes; `verify_panorama_bindings/camera/projection`,
`verify_ea_layer`, `verify_ea_photo`, `verify_ea_inline_voice`,
`verify_ea_single_execution`, `verify_examine_controls`, `verify_tk_animation`,
`verify_tk_hotspots` and `verify_tk_media` still pass.

## Limits / open

* The anchor is axis-aligned; the original also blits a rectangular sprite, so
  a large sprite near the viewport edge is not sheared by the cylinder.
* The port's analytic projection differs from the sampler's table arithmetic by
  up to two pixels (reported, not hidden).
* `0x40241c`'s panorama branch never stores its result in the shipped binary.
  The port draws at the computed anchor because the shipped SM/VA record
  positions are otherwise unreachable; if a future runtime trace shows the
  original drawing those records somewhere else, this is the place to correct.
* Not modelled (unchanged): the `+0x09 & 0x80` blend, the `+0x3c` trail draw,
  the `+0x09 & 0x10` offset pair, and the panorama transition effects.
