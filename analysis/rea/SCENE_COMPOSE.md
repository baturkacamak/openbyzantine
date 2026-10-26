# Scene composition — records onto an RGB16 surface

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
(Ghidra 12.1.4). Saved exports + focused `objdump`; no original execution.

This step closes the loop that the animation steps opened: a decoded frame is
now drawn at its record's position into an explicit surface, in the order the
renderer uses. Backgrounds/panoramas are still missing because they are JPEGs
(`0x401d29`, a separate step).

## 1. Observation — the record draw loop (`0x43b216`, 2401 B)

`FUN_0043b216(param_1, param_2, param_3)` scans `DAT_004649e8` for the lowest
(`local_4c`) and highest (`local_24`) occupied slot, then runs two passes:

1. **"snap" pass**, only when `DAT_004859cd == 0` (i.e. not after a load) and the
   table is non-empty: for ids from the highest down, records whose `+0x09` has
   bit `0x20` are drawn through `0x43c114(..., 1)` (the `+0x20` rect),
   `0x40241c`, `0x42b679`; afterwards `+0x20`/`+0x22` are zeroed and `+0x29`
   cleared.
2. **kind pass**: `for (local_3c = 0; local_3c < 5; ++local_3c)` groups records
   by the `+0x08` kind byte; within a group ids run from the highest down. Each
   record needs `DAT_004649e8[id] != 0`, `*(int *)(record + 0x20) != 0` and
   `((+0x09 & 0x20) != 0) || (CA type 1/2 && (+0x09 & 4 == 0))`; it is then
   drawn with `0x43c114(..., 1)`, clipped by `0x40241c` and copied by `0x42b679`.
   Records with `+0x09 & 0x20` are also "snapped" afterwards (`0x43264d`).

`0x43b130(record, surface, x, y, clip, ?, mode)` is the per-record draw: it picks
the frame (`+0x1c`, or the head `+0x18` when `mode == 1 && record+0x28 != 0`),
calls `0x40bbb0(x, y, frame->height, stride)` and `0x40bbfd(frame->buf, surface,
...)`, then follows `frame->next` until it reaches the current frame again (the
list is circular, so the normal case draws exactly the current frame).

## 2. Observation — rect, clip, copy

* `0x43c114(id, &x, &y, &w, &h, mode)`: `mode == 0` uses the record's `+0x00`/
  `+0x02` and the frame at `+0x1c`; `mode != 0` uses `+0x24`/`+0x26` and the
  frame at `+0x20`. `w`/`h` are the frame node's `+0x0c`/`+0x10`, i.e. the
  animation header's width/height.
* `0x40241c(&x, &y, &w, &h, &rect)`: with no panorama it tests the box against
  `0 <= x <= 0x27f` and `0 <= y-0x14 <= 399` (a `-0x14` bias that cancels in the
  returned `RECT` — but NOT between the record and the destination surface; see
  the correction in §9), clips `w`/`h` to the 640x400 window, rejects anything
  entirely outside, and otherwise returns `SetRect(x, y, x+w-1, y+h-1)`.
* `0x42b679(src, dst, srcX, srcY, srcStride, dstStride, dstX, dstY, width,
  height)` is a row `memcpy` — **no per-pixel transparency**. The sprite must
  therefore already be composed into the source surface by `0x40bbfd`, which
  writes only the pixels a run covers.

## 3. Port model

* `port/include/byzantine/compose.hpp` + `src/compose.cpp`:
  `Rgb16Surface` (explicit width/height, RGB565), `make_surface`,
  `blit_frame` (writes only covered pixels, clips to the surface, reports
  `covered`/`written`/`clipped`) and `write_ppm` (P6, 565 -> 888 with bit
  replication).
* `compose_scene(SceneState&, Rgb16Surface&)` reproduces pass 2: kind 0..4,
  ids descending; each record's current frame is decoded through the anims
  library and blitted at the record's `x`/`y`. Records with an unknown frame or
  no asset are counted (`skipped_no_frame`), kinds >= 5 in
  `skipped_other_kind` — never invented.
* CLI: `--compose-out <file.ppm>` (or `-` for none) with `--surface <WxH>`
  (default 640x400) composes the traced state and prints
  `composed size=… drawn=… skipped_no_frame=… skipped_other_kind=… pixels=…`,
  one `draw id=… kind=… x=… y=… written=… clipped=…` line per record, and a
  coarse luminance map.

## 4. Acceptance (shipped data)

| Command target | Result |
| --- | --- |
| `MAP` `tempsave.btb` 87, `--ca --anims --compose-out` | `composed size=640x400 drawn=27 skipped_no_frame=0 skipped_other_kind=0 pixels=156022`; draw order is the original's descending id (33, 32, 30, … 0); three icons at the bottom edge report `clipped` (id 30 `clipped=2900`, 21 `957`, 11 `1163`, 10 `2037`) |
| `VT` `73.btb` 1638 | `drawn=2 pixels=40756`: `id=26 x=0 y=20 written=24733`, `id=24 x=0 y=20 written=16023` |
| `anims-test <tree> --render maphl 0` | `99x58 covered=3373/5742 runs=3373 skipped=2` |

The MAP composition renders the map icons at their shipped positions (the same
`x/y` values that the save cross-check in `ANIMATION_ASSET.md` §4 validated
against each asset's first-frame offset), which is the strongest evidence that
rect, frame and offset handling agree.

## 5. Tests

* `compose-test`: surface allocation/bounds, in-bounds blit, a transparent
  column leaving the destination, clipping at the right/bottom, a fully
  offscreen blit, a negative origin, a short frame buffer rejection and the PPM
  header/values.
* `scene-state`: `compose_scene` orders two records from the highest id down,
  counts a kind >= 5 as `skipped_other_kind`, leaves an offscreen record
  undrawn, and writes the converted pixel (`0x1111` -> `0x2211`).
* `make -C port -j2 all test` passes all seven suites; the three panorama
  verifiers pass unchanged.

## 6. Not claimed

* The `+0x09 & 0x20` "snap" pass (draws through `+0x20` and then clears it),
  the `+0x09 & 0x80` destination-read blend (`0x42b5b2`), the mode-1
  head-to-current frame sequence and the `+0x3c` trail list.
* Background (`backs/*.jpg`) and panorama (`nodes/*.jpg`) layers: both need the
  JPEG decoder `0x401d29`. The composed surface starts black.
* The real double-buffered surfaces (`DAT_0046e82c`/`DAT_0047a8a4`) and the GDI
  `BitBlt`/`StretchBlt` path; the port composes into a plain buffer.
* Any claim about on-screen colours: RGB565 conversion is modelled, the final
  GDI/palette behaviour is not.

## 7. Verified inputs

| Address | SHA-256 (first 16) | What it settles |
| --- | --- | --- |
| `0x43b216` | `b2f51a16a7115afc` | draw loop, kind order, snap pass |
| `0x43b130` | `529ef379c272785e` | per-record frame selection |
| `0x43c114` | `a77c14f8911db67d` | rect from record + frame node |
| `0x40241c` | `d70c4342c1903b9d` | 640x400 clip and `-0x14` bias |
| `0x42b679` | `bb16e443aa648373` | row copy (no transparency) |

All five digests match `decompile-index.json`. Assembly ranges read:
`0x43b216`–`0x43bb76` (the whole draw loop), `0x43b130`–`0x43b215`,
`0x43c114`–`0x43c1a7`, `0x40241c`–`0x40271a` (clip tests) and
`0x42b679`–`0x42b736` (row loop).

## 8. Commands

```sh
make -C port -j2 all test
python3 analysis/rea/audit_port_coverage.py
# game functions 840 (249250 B); documented counterparts 109 (13.0%), 73034 B (29.3%)
# remaining 731: media/gui-api 74 (24312 B), other 657 (151904 B)

E=<extracted copy2hd tree>; S=$E/copy2hd/savegame
port/byzantine-cc port/output/cc_all/MAP --trace-save $S/tempsave.btb 87 port/output/cb_all/MAP \
  --engine $E/copy2hd/engine --lcx $E/copy2hd/engine/SHOW.LCX \
  --lci $E/copy2hd/engine/SHOW.LCI --lcy $E/copy2hd/engine/SHOW.LCY \
  --inventor $E/inventor.txt --hotitems $E/hotitems.txt \
  --gameprefs $E/copy2hd/goldsave/GAMEPREF.000 --ca $E/ca/MAP --anims $E \
  --co port/output/co_all/MAP --ci $E/ci/MAP --cy $E/cy/MAP --cf $E/cf/MAP \
  --compose-out /tmp/map87.ppm
# composed size=640x400 drawn=27 pixels=156022
```

## 9. Correction (26 October 2026) — the playfield origin

The record pass composes into the original's **640x480** scene surface, not into
the 640x400 playfield directly:

* `0x42b280(dst, 0x4b000)` clears `0x4b000` words = 307200 px = 640x480
  (`JPEG_DECODE.md` §6, hash `4e6fd498011e7f63`).
* `0x41045b` stores the decoded 640x400 background at `surface + 0x6400` bytes,
  i.e. exactly 20 rows of 640 words.
* `0x40241c` accepts a record only for `0 <= y - 0x14 <= 399`.

So the visible band is surface rows `0x14..0x413`, and a record's stored `y`
(creation writes `y = 0x14`; `ANIMATION_ASSET.md` §4 confirmed
`save.y == 0x14 + asset.y_offset`) carries that bias. §2's "cancels in the RECT"
is therefore only true inside `0x40241c`; the destination keeps the bias.

`port/src/cc.cpp compose_scene` now draws at `y - kPlayfieldTop` (20) into the
band surface and rejects a record whose top-left is outside the window (the
`0x40241c` top-left test, never a partially negative origin). `MAP`
`tempsave.btb` 87: pixels 156022 -> **163079**, clipped records 4 -> **0**,
`draw id=33 x=284 y=178 -> y=158`. `VT 73.btb` 1638's two 640x400 frames move
from `y=20` to `y=0`. `scene-state` locks both rules. The numbers in §4 are the
values *before* this correction; `JPEG_DECODE.md` §7 has the current ones.
