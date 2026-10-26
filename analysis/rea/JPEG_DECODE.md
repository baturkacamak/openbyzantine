# JPEG decode and the background/panorama layers

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
(Ghidra 12.1.4). Saved exports + `analysis/rea/jpeg_sample/`; no original
execution. The shipped fixtures and the synthetic validation streams were
cross-decoded with an independent decoder (`ffmpeg`) only as a *validation*
oracle, never as a source of behaviour.

This step closes the last missing image layer: `backs/<2ch>/<name>.jpg` behind
`0x69` and `nodes/<2ch>/<name>.jpg` behind `0xb9` are now decoded and drawn into
the composed surface before the record pass. While validating that composition
against a real background, the playfield origin of the previous step turned out
to be wrong; §6 corrects it.

## 1. Observation — the loader chain

| Address | Bytes | Role |
| --- | --- | --- |
| `0x401d29` | `0x401d29`-`0x401dde` | panorama load: `0x40f135(path, &DAT_0046487c, &DAT_004654d8, DAT_0045c040, DAT_00468ca0)`, then `0x42382b(DAT_00464880)` rebuilds the seam columns |
| `0x41045b` | `0x41045b`-`0x410606` | background load: builds `"backs\\"` + `"%c%c\\%s"` (first two characters of the `engine/<scene>cb` entry name) or `"inventor\\examine\\"` when the third argument is 4, then `0x40f135(path, [0x280,400], &height, surface+0x6400, 0x280)` |
| `0x40f135` | `0x40f135`-`0x40f341` | one image: libjpeg decompress + `0x40f080` per row |
| `0x40f080` | `0x40f080`-`0x40f134` | 3-byte RGB row -> RGB16 |

`0x40f135` passes `param_4`/`param_5` as an existing buffer and its **word**
stride, and fails (returns `NULL`) when the decoded width/height differ from the
`*param_2`/`*param_3` the caller supplied. Both callers therefore hard-code their
expected size: the background path requires exactly 640x400, the panorama path
requires the size the renderer already holds (`DAT_0046487c` x `DAT_004654d8`).

`0x40f080` switches on `_DAT_00485c80`, which `0x4162a0` sets from the DirectDraw
green-channel mask (`0x7e0` -> 6). Mode 6 is:

```
pixel = ((r << 8) & 0xf800) | ((p >> 5) & 0xffe0) | ((p >> 19) & 0x1f)     r>>3 g>>2 b>>3
```

i.e. plain RGB565 from libjpeg's `JCS_RGB` byte order. Mode 5 (5-bit green) is a
separate branch that is not used by the shipped data.

## 2. Observation — the linked library is IJG libjpeg 6a

The decompressor is statically linked, so no import table names it. The call
sequence in `0x40f135` and the state values inside the exports identify it:

| Export | libjpeg entry | Evidence |
| --- | --- | --- |
| `0x43e900` | `jpeg_std_error` | installs the five error-manager methods and returns its argument |
| `0x43de70` | `jpeg_CreateDecompress` | rejects any version other than `0x3d` (61 = libjpeg 6a) and any struct size other than `0x1a8` (424) |
| `0x43e7b0` | `jpeg_stdio_src` | allocates the 0x28-byte source manager |
| `0x43dfc0` | `jpeg_read_header` | requires global state `200`/`201` |
| `0x43e3d0` | `jpeg_start_decompress` | requires `202` |
| `0x43e560` | `jpeg_read_scanlines` | requires `205` |
| `0x43e320` | `jpeg_finish_decompress` | accepts `205`/`206` |
| `0x43df40` | `jpeg_destroy_decompress` | tail call to `0x441a40` |

The binary also still contains the 6a diagnostics (`"Wrong JPEG library
version"`, `"Sorry, there are legal restrictions on arithmetic coding"`,
`"Invalid SOS parameters for sequential JPEG"`). The decompressor keeps
libjpeg's defaults, which matters for pixels: `JDCT_ISLOW` (jidctint), the
"fancy" upsampler for 2:1 chroma, and `out_color_space = JCS_RGB` (`jdcolor`'s
16.16 fixed point).

## 3. Observation — the shipped JPEGs

All six ISOs were extracted (`backs/`, `nodes/`) and every file scanned:

| Property | Result |
| --- | --- |
| Files | **283** (215 `backs/` 640x400, 68 `nodes/` 2496x768) |
| Process | SOF0 baseline, 8-bit, 3 components — **all 283** |
| Sampling | `2x2 / 1x1 / 1x1` (4:2:0) — **all 283** |
| Scans | one interleaved scan per image; no DRI, no restart markers |
| Quantisation | `DQT` table 0 (luma) and 1 (chroma); the game files carry them as two separate segments |
| APPn | APP0 "JFIF" only; no Adobe APP14, so no colour-transform signalling |

Because the shipped corpus is uniformly baseline 4:2:0, progressive and
4:2:2/4:4:4 support (§5) is verified with synthetic streams only.

## 4. Observation vs inference — the pixel model

Observation (from the exports/strings above):

* The image is decoded by libjpeg 6a with default parameters, to RGB888 rows.
* `0x40f080` narrows each row to RGB565 as in §1.
* Nothing in the game touches the DCT, quantisation or upsampling: they are
  libjpeg internals, so their *algorithms* are the library's documented ones and
  cannot be read out of the game binary.

Inference (implemented, explicitly not claimed bit-exact against the original
build):

* **IDCT**: libjpeg 6a's `jidctint` integer IDCT (`CONST_BITS 13`,
  `PASS1_BITS 2`, the `FIX_*` constant set). The port reproduces it, including
  the AC-zero shortcuts (pass 1 rounds the DC to `left_shift(dc, 2)`; pass 2
  truncates with `right_shift(dc, 5)`).
* **Chroma upsampling**: a separable 3:1 triangle filter — the filter family
  libjpeg's "fancy" upsampler uses for 2:1 factors — applied vertically then
  horizontally with per-pass rounding (`+2 >> 2`). libjpeg performs both
  directions in one pass over a 4-bit fixed-point sum, so its edge rounding
  differs by at most one unit; the port does not reproduce that combination.
  Non-2:1 factors are nearest-neighbour replicated (never exercised by the
  shipped data).
* **Colour**: `jdcolor`'s constants (`FIX(1.40200)=91881`, `FIX(1.77200)=116130`,
  `FIX(0.34414)=22554`, `FIX(0.71414)=46802`, `ONE_HALF`) and its `RIGHT_SHIFT`
  rounding, then clamping.
* **Strictness**: unsupported processes (SOF3, SOF5..SOF15, DAC/arithmetic),
  12-bit precision, 2- or 4-component colour, a second SOF, a sampling factor
  that does not divide the maximum, a truncated segment, missing EOI, a marker
  inside entropy data and running out of scan data are **errors**, not guesses.

## 5. Port

New `port/include/byzantine/jpeg.hpp` + `port/src/jpeg.cpp`:

* `read_jpeg_header(bytes)` — marker walk to the first SOF.
* `decode_jpeg(bytes)` / `decode_jpeg_file(path)` — SOF0/SOF1/SOF2, 1..3
  components, Huffman (canonical, byte-stuffed reader), DQT/DHT/DRI, spectral
  selection and successive-approximation refinement for progressive streams,
  jidctint IDCT, upsampling, `jdcolor` conversion -> `JpegImage{width,height,rgb}`.
* `rgb_to_rgb565(image)` — `0x40f080` mode 6.
* `port/src/compose.cpp` gained `blit_image(target, w, h, pixels, x, y)`, the
  unconditional clipped row copy the background/panorama stage needs.
* CLI: `--background <file.jpg>` (must be 640x400, as `0x41045b` requires) and
  `--panorama <file.jpg> [--panorama-x <0..w-1>]`, both applied before
  `compose_scene`. The panorama path reuses the recovered helpers: decode ->
  `rgb_to_rgb565` -> `panorama_geometry` -> `pad_panorama_rows` ->
  `PanoramaProjection` + `consume_panorama_position` -> `sample_panorama_rgb16`,
  i.e. the same 2496x768 source, seam padding, per-pixel mapping and yaw offset
  documented in `PANORAMA_MAPPING.md`/`PANORAMA_SAMPLING.md`.

## 6. Correction — the playfield origin of the record pass

`SCENE_COMPOSE.md` §2 claimed `0x40241c`'s `-0x14` y bias "cancels in the
RECT". It cancels between the clip test and the returned rectangle, but **not**
between the record and the destination surface:

* `0x42b280(dst, 0x4b000)` clears **0x4b000 words = 307200 px = 640x480** before
  the background is drawn, so the scene surface is 640x480, not 640x400.
* `0x41045b` writes the 640x400 background at `surface + 0x6400` bytes, i.e.
  exactly **20 rows** of 640 words into that surface.
* `0x40241c` accepts a record only when `0 <= y - 0x14 <= 399` and the draw loop
  copies with a 640-word stride.

So the 640x400 playfield is surface rows `0x14..0x413`, and a record's stored
`y` (record creation sets `y = 0x14`, and the save/asset cross-check in
`ANIMATION_ASSET.md` §4 confirmed `save.y == 0x14 + asset.y_offset`) is in that
480-row space.

The port composes the **band** (a 640x400 surface), so `compose_scene` now draws
at `y - kPlayfieldTop` and rejects a record whose top-left falls outside the
window, exactly as `0x40241c` does. Measured effect on `MAP`/`tempsave.btb` 87:

| | before | after |
| --- | --- | --- |
| pixels written | 156022 | **163079** |
| records with `clipped != 0` | 4 (ids 30, 21, 11, 10) | **0** |
| `draw id=33` | `x=284 y=178` | `x=284 y=158` |

No record in this scene is clipped any more, which is what data authored for the
band should produce, and the annotation sprites now sit on the map they annotate
instead of 20 rows below it. `VT 73.btb` 1638's two full-screen 640x400 frames
move from `y=20` (bottom 20 rows clipped) to `y=0`.

## 7. Acceptance (shipped data)

```sh
E=<extracted copy2hd + anims tree>; S=$E/copy2hd/savegame
port/byzantine-cc port/output/cc_all/MAP --trace-save $S/tempsave.btb 87 port/output/cb_all/MAP \
  --engine ... --lcx ... --lci ... --lcy ... --co ... --ci ... --cy ... --cf ... \
  --ca $E/ca/MAP --anims $E --gameprefs ... --inventor ... --hotitems ... \
  --compose-out /tmp/map87.ppm --background $E/backs/ma/MAPFINM1.jpg
# background=... size=640x400 written=256000 clipped=0
# composed size=640x400 drawn=27 ... pixels=163079
# draw id=33 kind=0 x=284 y=158 written=7272 clipped=0
# draw id=0  kind=0 x=143 y=43  written=3373 clipped=0
```

The written PPM shows the real paper map behind 27 annotation records (the blue
hand-written labels and the pictograms). The JPEGs used as validation fixtures
are in `analysis/rea/jpeg_sample/`.

Panorama variant (a 2496x768 `nodes/` image, staged `0xba` position 700):

```sh
port/byzantine-cc ... --compose-out /tmp/pan.ppm --panorama <nodes/vc/vc05.jpg> --panorama-x 700
# panorama=... size=2496x768 padding=313 yaw_degrees=101 pitch_degrees=0 sampled=256000
```

`NB 57.btb` 556 with `backs/nb/NBDESK.jpg`: `written=256000`, one record
(`id=10 kind=1 x=0 y=0 written=20299`), the blue journal drawn over the desk.

Corpus-wide decode check (all six ISOs):

| Check | Result |
| --- | --- |
| Files decoded without error | **283 / 283** |
| Dimensions | 215 x `640x400`, 68 x `2496x768`, none other |
| Header kind/sampling | all SOF0, `2x2 1x1 1x1` |
| Wall time (port, `-O2`) | 11.3 s for all 283 (0.04 s average) |
| Sanitizers | the same 283 files also decode clean under `-fsanitize=address,undefined` (30 s); the IDCT's signed power-of-two scalings are written as multiplications, so no negative left-shift is left in the code |

Independent-decoder cross-check (`ffmpeg` as an oracle, RGB888):

| Stream | mean abs diff | max | note |
| --- | --- | --- | --- |
| synthetic 4:4:4 (no upsampling) | 0.009 | 3 | Huffman + dequant + IDCT agree |
| synthetic 4:2:2 | 1.15 | 114 | chroma edges only |
| synthetic 4:2:0 | 1.69 | 160 | chroma edges only |
| `BCLINTEL.jpg` (shipped 4:2:0) | 0.79 | 15 | 0.3 % of bytes differ by > 4 |
| luma of `BCLINTEL.jpg` | 0.20 | 5.08 | `0.299R+0.587G+0.114B` of the port's RGB vs the reference Y plane |
| luma of `vc05.jpg` (2496x768) | 0.17 | 4.67 | same method |

The residual is the chroma interpolation kernel (§4): the luma plane matches to
rounding, and the colour differences appear only on 4:2:0 chroma edges.

## 8. Tests

* `jpeg-test` builds its own baseline and progressive streams (a 16x16 image,
  one component, an all-8 quantisation table, four blocks with DC 0/10/20/5) and
  checks per-block placement (128/138/148/133), the negative DC difference,
  greyscale equality, progressive == sequential, `rgb_to_rgb565`
  (`0xf800`/`0x07e0`/`0x001f`/`0x8410`) and nine rejections (empty, no SOF, no
  SOI, truncated, short scan, lossless SOF3, DAC, 12-bit precision, zero
  sampling factor).
  With the two fixtures it also locks the shipped-plane hashes:
  `BCLINTEL.jpg` `0x2e281d7511acda46` (RGB) / `0x526bd1a48e740f63` (RGB565),
  `vc05.jpg` `0x4b5f10afeee6c24e` / `0xe6ae2e47246397ba`.
* `compose-test` covers `blit_image`: full placement, clip counting, an
  off-surface image writing nothing, a zero-size image.
* `scene-state` locks the playfield origin (`y=0x15` -> band row 1) and the
  `0x40241c` top-left rejection (`y=0x13` draws nothing, `clipped=1`).
* `make -C port -j2 all test` passes all eight suites; the three panorama
  verifiers pass unchanged; the VM census is unchanged (it does not render).

## 9. Not claimed

* Pixel-exact equality with the original build: the IDCT and upsampler are
  reproduced from libjpeg 6a's algorithms but were never compared against a
  capture of the original renderer (no original execution).
* libjpeg's own error recovery: the port rejects corrupt streams where libjpeg
  warns and substitutes grey.
* `0x41045b`'s `inventor\examine\` background variant (third argument 4), the
  `0x426...` transitions and blends (`0x418bcf`, `0x42bce7`, `0x42bce1`,
  `0x419cf6`, `0x418eb2`, `0x419a51`) and the `0x42b5b2` destination-read
  blend inside the record pass.
* The AVI/mode-5 path, the `+0x09 & 0x20` snap pass and the `+0x3c` trail.
* Whether the scene surface is exactly 640x480: the *clear size* (`0x4b000`
  words) and the *background offset* (20 rows) are both observed, the playfield
  start `0x14` is directly enforced by `0x40241c`.

## 10. Verified inputs

| Address | SHA-256 (first 16) | What it settles |
| --- | --- | --- |
| `0x401d29` | `4ce100ddf9de5812` | panorama load + seam rebuild |
| `0x41045b` | `bfd535306a102e94` | `backs\<2>\<name>.jpg`, 640x400, `+0x6400`, stride 0x280 |
| `0x40f135` | `29c043e4379149ef` | libjpeg call order, expected-size check, buffer/stride |
| `0x40f080` | `52a4a2b0270fbc4b` | RGB888 -> RGB565, display-mode branch |
| `0x42b280` | `4e6fd498011e7f63` | `memset(dst, 0, words*2)`; 0x4b000 words = 640x480 |
| `0x43e900` | `3fc06ce8a33a4307` | `jpeg_std_error` |
| `0x43de70` | `219ee3ad7b92ca14` | `jpeg_CreateDecompress`, version 61 / size 424 |
| `0x43e7b0` | `71be190c6e4fce2f` | `jpeg_stdio_src` |
| `0x43dfc0` | `827da35ddfe17595` | `jpeg_read_header` |
| `0x43e3d0` | `92abdae9414335f5` | `jpeg_start_decompress` |
| `0x43e560` | `ef853ba96dac7a4c` | `jpeg_read_scanlines` |
| `0x43e320` | `77c69b6683ae1f43` | `jpeg_finish_decompress` |
| `0x43df40` | `ab4423cbba8a2107` | `jpeg_destroy_decompress` |
| `0x42b679` | `bb16e443aa648373` | row copy used by the background stage |
| `0x4242bd` | `257fcd5a5d11d854` | panorama sampling (already modelled) |

All fifteen digests match `pseudocode/decompile-index.json`.

## 11. Commands

```sh
make -C port -j2 all test
python3 analysis/rea/build_vm_census.py      # 5,935 completed, blocking 0 (unchanged)
python3 analysis/rea/audit_port_coverage.py  # 121/840 functions, 75,257 B (30.2 %)

# header census over every extracted JPEG (the pass recorded in §3)
find <extracted> -iname '*.jpg' -print0 | xargs -0 port/jpeg-test --scan | tail -2

# real composition (see §7 for the full argument list)
port/byzantine-cc port/output/cc_all/MAP --trace-save <savegame/tempsave.btb> 87 \
  port/output/cb_all/MAP ... --compose-out /tmp/map87.ppm \
  --background <extracted>/backs/ma/MAPFINM1.jpg
```
