# Panorama record patches are blitted in screen space, not warped with the panorama

Date: 27 October 2026. Diagnosis and fix (the player now draws the patch through
the sampler inverse).

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; analysis
profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

## Reported symptom

User screenshot `Byzantine - NB entry` (the nargile bar). The bartender record
`nbBr00a` "does not sit on the background unless the camera reaches a certain
angle". The screenshot matches the NB entry frame at yaw -49 exactly. Pinned
user run `run-20261027T160640.291181Z-yhxqcbai` (the last interactive run; the
same NB entry is in `run-20261027T153102.008778Z-wo0x6pwm` and
`run-20261027T154739.489445Z-e38pem8c`):

```
[+16.348s] composed panorama=nodes/nb/NB00.jpg ... yaw=-49 pitch=0
           drawn=1 projected=1 pixels=12960 drawn_ids=0
[+24.100s] composed ... yaw=-28.375 pitch=4.35 drawn=1 projected=1 pixels=22400
[+37.037s] composed ... yaw=-69.7692 pitch=14.55 drawn=0 projected=0
[+40.354s] composed ... yaw=92.6346 pitch=17.1549 drawn=0 projected=0
```

`--print-records` for the same scene:

```
record id=0 kind=0 flags=0x0 x=1124 y=370 snap_x=1124 snap_y=370
       frame=1 frames=31 delay=1 asset=nbBr00a type=0
object slot=0 x=1189 y=402 w=68 h=93 active=1 kind=0x13 label=250
```

## Evidence 1 — the record frame is a panorama SOURCE-space patch

`anims/nb/Nbbr00a.rle` frame 0 is not a cut-out character. It is a 140x160
rectangle of the bar background with the bartender baked into it. The SM record
`Smim00b` frame 0 is the same thing for the courtyard (150x150 with both
people). Cropping `nodes/nb/NB00.jpg` at source `(1124,350)` 140x160 (record
y 370 minus the `0x14` surface bias) reproduces the same tiles, tap, counter
and green object, with only the bartender added:

* `port/output/nb-alignment/nbcompare.png` — panorama crop (left) vs
  `Nbbr00a` frame 0 (right).
* `port/output/nb-alignment/nb-frames.png` — frames 0/8/16/24; the later frames
  are transparent deltas on the same patch.

The record's pixels therefore live in the **same coordinate space as the
panorama image**. The background is sampled through the panorama mapping
(`PanoramaView::render`), so the record patch must be resampled with that same
mapping. A 1:1 screen blit is correct only where one source pixel maps to one
viewport pixel.

## Evidence 2 — the port's own hit test shows the source rect warping

The clickable object for the same character (`slot 0`, source 68x93) is
hit-tested by inverse-mapping every screen pixel (`hotspot_at` ->
`PanoramaView::canvas_point`). The measured screen footprint is:

| camera yaw | object source 68x93 -> screen | scale (x/y) |
| ---: | ---: | ---: |
| 0 | 69x95 | 1.01 / 1.02 |
| -30 | 87x110 | 1.28 / 1.18 |
| +20 | 82x107 | 1.21 / 1.15 |

This is the expected local panorama scale: horizontal `1/cos^2(angle)`, vertical
`1/cos(angle)` at pitch 0. The **background uses the same mapping**, but the
140x160 record patch is drawn at a fixed 140x160 at every yaw. Projecting the
same source rectangle pixel-by-pixel gives the footprint the patch would have
if it were warped like the background:

| camera yaw | port placement / size | projected source-rect bbox | result |
| ---: | ---: | ---: | --- |
| 0 | 196,166 140x160 | 192,165 145x168 | +4,+1; 5 px narrower, 8 px shorter |
| -28.375 | 399,169 140x160 | 394,161 162x186 | 22 px narrower, 26 px shorter; feet 18 px high |
| -30 | 412,169 140x160 | 406,160 166x190 | feet 20 px high |
| +20 | 41,171 140x160 | 11,157 183x203 | 43 px narrower/shorter; feet 28 px high |
| -49 | 559,161 140x160 | 559,157 80x205 (clipped) | top matches; bottom 40 px early |

(Coordinates are the 640x400 playfield. The bbox is the set of screen pixels
whose inverse map lands in the source rect, i.e. exactly `hotspot_screen`'s
path; it agrees with the measured object footprint within 1-2 px.)

The patch is at source x 1124..1264, so it is centred 54 source pixels left of
the panorama centre (about -7.8 degrees). Near that camera yaw the local scale
is ~1 and the patch happens to align (the yaw-0 crop is only 1-3% off). As the
camera turns, the surrounding background is stretched by up to 1.3x at yaw
+20 and 2x at yaw -49, while the patch stays 140x160. That is the reported
"only at a certain angle".

## Evidence 3 — the visible seam

* `port/output/nb-alignment/nb-y0-crop.png` — at yaw 0 the patch blends into
  the bar; the character looks placed.
* `port/output/nb-alignment/nb-y20-crop.png` — at yaw +20 a rectangular seam is
  visible around the bartender (left and bottom patch edges): the baked
  background inside the patch is not stretched like the panorama around it.
* Full frames: `nb-y-49.png`, `nb-y0.png`, `nb-y-20.png`, `nb-y20.png`.

This seam is not a sprite-size detail: it is the patch boundary.

## Evidence 4 — anchor switching and the edge reject

`project_record_position` (`port/src/cc.cpp`) tries five anchors in the order
centre, top-left, bottom-right, top-right, bottom-left and takes the **first
success**, subtracting half the *unscaled* size. The result changes
discontinuously when the centre stops being projectable: for NB between yaw -50
and -45 the chosen top-left jumps `(569,160)` -> `(551,177)`, a 18 px left /
17 px down pop.

After a successful projection, `compose_scene` rejects the record when its
top-left is outside the surface (`target.inside(x, y)`). For NB at yaw +25 the
projected top-left is `(-5,174)` and at +30 it is `(-58,178)`, so `drawn=0`
even though the projected source footprint is `x=1..154` and `x=3..112`
respectively — a large part of the character should still be visible. The log
therefore shows `projected=1 drawn=0` (our yaw 30/40 renders): the character
pops out of existence instead of sliding off the edge. The `projected` counter
counts the anchor success, not the draw, so this is easy to miss.

The shipped `0x40241c` panorama branch, after computing its anchor, returns at
`0x4025bd` **before** the non-panorama clip test at `0x4025db`; the port's
post-projection `target.inside` rejection is not what that branch does. The
branch also returns without storing the projected anchor (the non-panorama path
writes `*param_1`/`*param_2` at `0x402626`/`0x402632` and calls `SetRect` at
`0x402702`), so the port's centre-minus-half placement is an inference. That
does not change the patch evidence above: the port's own source-rect hit test
and the baked patch both say the record must be warped.

## Scope

This is shared code, not an NB-only bug:

* `project_record_position` + `compose_scene` are used by every panorama scene
  (NB, SM, HL, VA, AM, VH, GC, ...), and by the snap/trail passes.
* The earlier SM/VA verification was taken at yaw 17.5865 / entry, where the
  local scale is 1.01-1.03, so the fixed blit looked correct. The
  `PANORAMA_SPRITES.md` "Limits / open" already listed "a large sprite near
  the viewport edge is not sheared by the cylinder" as not modelled; this
  report shows it is the user-visible defect.
* The background and the click rectangles are already correctly warped; only
  the record draw path is wrong, so the fix is local to the record composition
  (not the camera math, which `panorama-view-test` validates to <=2 px).

## Fix

* `SceneProjector` gained `map_viewport` (0x4240ae's inverse) and
  `source_rect_viewport_bbox`. `PanoramaView` implements them with
  `canvas_point` (the sampler's own map) and a conservative analytic bbox,
  expanded by 8 px for the x87/table difference; a patch crossing the camera
  plane falls back to the whole viewport.
* `compose_scene` now takes a source-space patch path whenever the projector
  reports a bbox: it accumulates frames 0..current into a source patch with
  their coverage masks, scans the clamped viewport bbox, inverse-maps each
  viewport pixel and writes the covered patch pixel. Partially visible patches
  are clipped, not dropped. The old first-success-anchor placement remains only
  as a fallback for projectors without an inverse (synthetic tests) and for
  background/close-up scenes.
* `scene_animates` uses the same bbox, so an edge-clipped animated record keeps
  the window repainting.
* `compose_scene(..., advance)` is part of the same change; see
  `ANIMATION_RATE_GLOBAL.md`.

### Verification (real data)

The user's MR -> MAP -> NB chain on `/tmp/byz-scene`:

```
BYZ_DATA=/tmp/byz-scene ./port/scene.sh ".." MR 0 30.btb --entry \
  --no-audio --no-video --click-at 573,347 --click-at 118,117 --click-at 118,117 \
  --panorama-yaw NB:<yaw> --shot /tmp/nb.ppm
```

| yaw | before | after |
| ---: | --- | --- |
| -49 | drawn=1 pixels=14633 | drawn=1 pixels=15850 |
| -30 | drawn=1 pixels=27549 | drawn=1 pixels=28977 |
| -28.375 | 140x160 at 399,169 | sampled patch, 27938 unique pixels |
| 0 | drawn=1 pixels=27549 | drawn=1 pixels=23384 |
| +20 | drawn=1 pixels=27549 | drawn=1 pixels=33375 |
| +30 | **drawn=0** (top-left -58,178) | drawn=1 pixels=21388 |
| +40 | **drawn=0** | drawn=1 pixels=2651 |

(`pixels` is now unique viewport pixels; the old head-walk summed every frame,
so the raw counts are not directly comparable at the same yaw.) The +20 patch
seam is gone and the character now fills the projected source rect; screenshots
`port/output/nb-alignment/after/` (`nbwarp-*.png`, `before-after-*.png`,
`nbwarp-20-crop.png`). SM at yaw 17.5865 still `drawn=2 projected=2`
(`sm-warp.ppm`), VA entry `drawn=2 projected=4` (two bboxes are off-window), and the edge shots show the clipped patch instead of a gap.

`make -C port -j2 all test` passes; new `panorama-view-test` checks
`map_viewport` against `canvas_point` and the bbox superset at the viewport
edges/centre; new `scene-state-test` checks a 2x source-space patch (one source
pixel covers four viewport pixels) and the `advance=false` compose path.
All `analysis/rea/verify_*.py` checks still pass (`verify_ea_layer`,
`verify_ea_photo`, `verify_ea_single_execution`, `verify_examine_controls`,
`verify_mr_arrival`, `verify_tk_animation`, `verify_tk_hotspots`,
`verify_tk_media`, and the panorama bindings/camera/projection exports).

## Regression / next check

The unit tests now cover the mapping and the warp, not the real-data sweep. For
NB rec 0, SM rec 3 and HL rec 0, a scripted sweep of yaw
`-49, -28.375, 0, +20, +30` and pitch `0, +-10` should still assert that the
drawn patch footprint matches the projected source-rect bbox and that a
partially visible patch is drawn. `--panorama-yaw` covers yaw today; pitch
currently only comes from the trace (the user's drag), so a diagnostic pitch
option would help. A single-yaw screenshot is no longer sufficient evidence
for this path.

## Not claimed / open

* The exact original record draw is not recovered: the shipped panorama branch
  computes and discards its anchor, and no original execution is available.
  The observation is the inconsistency between the port's own source-space hit
  rectangle, the baked background patch and the screen-space blit.
* The patch's baked background can still differ slightly in colour (RLE
  RGB565/555 vs JPEG panorama); the geometry is now exact but a faint colour
  step at the patch edge is possible.
* The snap pass goes through the same `compose_scene` warp path. The separate
  `+0x3c` trail draw is still not modelled (DRAW_PATHS.md), so a panorama
  record that uses trails has no visual check here.
