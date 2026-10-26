# Animation assets (`anims/…/*.RLE`) — container, runs, decoding

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Analysis profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
(Ghidra 12.1.4). Evidence is the saved exports, focused `objdump -d`, the 922
shipped `.RLE` assets and the 77 shipped CD1 saves. No original execution.

This step turns `0x64`/`0x91` from request-only into real frame state: a record
created from the CA table now gets its frame count, per-frame geometry and the
first frame's x/y offset, exactly as `0x40fe70 -> 0x410078` does.

## 1. Observation — path resolution

`0x40fe70` builds the file name from the CA record (`DAT_0047dd70[id]->name`):

```
sprintf(buf, "%c%c\\%s", name[0], name[1], name)      ; 0x45e800 "%c%c\%s"
0x40c624(out, "anims", buf, ".rle")                   ; 0x45e810 "anims", 0x45e808 ".rle"
if (0x410078(out, record) == 0) {
    strip ".rle"; append ".avi"                       ; 0x45e818 ".rle", 0x45e820 ".avi"
    0x41cb13(out, record);                            ; AVIFile-based fallback
}
```

So the asset is **`anims/<name[0]><name[1]>/<name>.RLE`**, with `.AVI` as the
fallback. The shipped CD names are upper case (`anims/vb/VBMEDTS.RLE`) while the
CA tables and scripts use mixed case, so a case-sensitive filesystem needs an
index.

## 2. Observation — the `.RLE` container (0x40cb26 + 0x410078)

`0x40cb26(file, signature)` reads, in this order (objdump `0x40cb26`–`0x40cbc2`):
`fread(sig, 2)`, four `fgetc`, `fread(u16)` (discarded), `fread(u16)` and
**returns the last one** = frame count. `0x41199e` only returns the file length.

| Offset | Size | Meaning |
| ---: | ---: | --- |
| `+0x00` | 2 | signature, copied to record `+0x28`; `0x0000` ×875, `0x0001` ×47 |
| `+0x02` | 4 | unused (zero in all 922 files) |
| `+0x06` | 2 | unused (zero in all 922 files) |
| `+0x08` | 2 | **frame count** |

Then, per frame, a 26-byte (`0x1a`) header followed by `words*2` bytes:

| Header offset | Size | Meaning (0x410078) |
| ---: | ---: | --- |
| `+0x02` | s16 | width -> frame node `+0x0c` |
| `+0x04` | s16 | height -> frame node `+0x10` |
| `+0x06` | s16 | **x offset**: `record->x += this` on frame 0 |
| `+0x08` | s16 | **y offset**: `record->y += this` on frame 0 |
| `+0x16` | u32 | `words`: `malloc(words*2)` then `fread(buf, 2, words)` |

* **Observation** `0x410078` calls `0x40ffee(record, frame_count)` once, which
  stores the count at `+0x14` and initialises the frame list; the first frame
  also adds its offsets to the record's position (`0x410151`–`0x41017b`).
* **Observation** each later frame allocates a `0x18`-byte node, chains it
  through `+4`, and stores width/height/x/y at `+0xc`/`+0x10`/…; `0x40cf73`
  then walks that list for `0x91`.
* **Observation (pixel stream)** the frame payload is a run stream, decoded by
  `0x40bbfd` (called from the renderer `0x43b130`, from the UI loaders
  `0x417054`/`0x417314`/`0x417491`/`0x417e9c`/`0x41f80c`/`0x4090c4` and from
  `0x4200bb`). See §4 below.
* **Observation** `0x40be80` is the **load-time colour pass**, not the decoder:
  it walks the same run stream and applies `0x4164a0` to the pixel words of
  commands 0 and 2 only, for `height` rows (`[ebp-0x4]` is the row counter that
  `inc`s on each command 1). `0x4164a0` is
  `(p & 0x1f) | ((p & 0xffe0) << 1)`, i.e. **RGB555 -> RGB565** (bit 15 shifts
  out), skipped only when `DAT_00485c78 == 10`.
* **Unknown** the meaning of the 2-byte signature, of the skipped u16s, and of
  `DAT_00485c78 == 10` (some display mode).

## 3a. Observation — the `.AVI` fallback (0x41cb13)

When the `.RLE` is absent, `0x40fe70` rewrites the suffix to `.avi` and
`0x41cb13` builds an AVIFile object:

* frame count = the video stream length (loop bound, and `0x40ffee`'s argument);
* every frame node gets the stream's width and height;
* **no** x/y offset is added to the record;
* the decoder object is stored at record `+0x38`.

The 40 AVIs under `anims/` are real RIFF AVIs (`vids`/`DIB ` streams, 15 or 30
fps). 8 of the 10 CA entries without a `.RLE` have one; `BC`'s `bckill1` and
`bckill2` have neither file.

## 3b. Observation — the run stream (`0x40bbfd`)

State set by `0x40bbb0(x, y, height, stride)`: `stride` at `0x46c42c`,
`row_base = (y-1)*stride + x`, `x = row_base`, `source = 0`, `height` at
`0x46c434`, `row = -1`. The decoder consumes 16-bit words:

| Command | Word | Meaning |
| --- | --- | --- |
| `0` | `0x0000｜n` | **literal run**: the next `n` words are pixels |
| `1` | `0x4000｜x` | **row start**: `row++`, `x = row_base + n` |
| `2` | `0x8000｜n` | **colour run**: one pixel word follows, repeated `n` times |
| `3` | `0xC000｜n` | **transparent gap**: `x += n`, nothing is written |

`n` is the low 14 bits, read back as a signed 16-bit value (`movsx`), so
`0x2000`–`0x3fff` would be negative; no shipped frame contains one. The loop
stops when `row >= height` (a `0x4000` closing marker after the last row does
exactly that) or when `y + row` passes the clip bottom. Pixels outside the clip
window are skipped, not clamped. `param_6` (record `+0x09 & 0x80`) switches to a
"read the destination pixel" path (`0x42b5b2`), a draw-time blend that is not
part of the frame data.

## 3c. Validation of the run stream

* **All 2,846 frames of the 922 shipped `.RLE` files decode cleanly**: the word
  stream is consumed exactly, no negative count, the final `row` equals `height`
  and no run exceeds the header's width. 64 frames are legitimately blank
  (e.g. `BBBM00A.RLE` frame 20: 248 empty rows, `0x4094` = row at x = width).
* Decoding `MAPHL.RLE` frame 0 (`99×58`, offset `143,43`) yields 3,373 covered
  pixels of 5,742 and 2 transparent pixels; the luminance map shows a coherent
  icon, not noise.
* `VT` `73.btb` label 1638 selects `vtssdr` frame 5 and `vtsslt` frame 4, which
  decode to `640×400` with 16,023 and 24,733 covered pixels — the VM's
  `current_frame` and the decoded pixels agree end to end.

## 4. Validation against the shipped data

* **All 922 `.RLE` files parse exactly**: every frame header inside the file,
  every `words*2` payload inside the file, and the last frame ending on the last
  byte. (40 further files under `anims/` are RIFF AVIs, skipped by extension.)
* 2,846 frames across those files; sizes `640×400` (729), `100×100` (273),
  `75×100` (104), `50×50` (78), `69×99` (46), …; **179 frames have a non-zero
  x/y offset** (`200,0` ×36, `172,14` ×7, …), so the offset path is not
  hypothetical.
* **Frame count**: 448 saved node slots (across the 77 CD1 saves) belong to CA
  entries of type ≤3 whose asset exists. **447 have `save.+0x14 == asset frame
  count`.** The single outlier is `28.btb` scene `hl`, slot 4, `HLDc00b`: the
  save says 21, the asset has 39. Unexplained; the likely cause is a
  transition-installed record (`0x439349`) whose frame count belongs to a
  different scene's asset. Recorded as an open question, not smoothed over.
* **First-frame offsets**: 429 of those slots have a non-zero asset offset, and
  **429/429 satisfy `save.x == asset.x_offset` and `save.y == 0x14 +
  asset.y_offset`** exactly (e.g. `map` slot 0 `maphl`: save `143,63`, asset
  `143,43`; `vh` slot 43 `vrFiles`: save `164,91`, asset `164,71`). This is the
  strongest available confirmation of the header layout and of the `+0x14` y
  bias from `0x40ffee`.
* `HSTMED` save `55.btb` slot 48 `vbmedts`: save `+0x14 = 8`, asset `8` frames,
  asset offsets `0,0`, save `x=0, y=20` — the worked example.

## 5. Port model

* `byzantine::parse_anims` (new `port/src/anims.cpp`): strict `.RLE` container
  parse; a truncated header, a truncated payload, a frame that overruns the file
  or trailing bytes are errors.
* `byzantine::parse_avi_anims`: RIFF walk for the first `vids` `strh`
  (`dwLength`) plus `strf` (`biWidth`/`biHeight`); rejects non-AVI, missing
  stream, zero dimensions and a length outside 1..0xffff. No frame is decoded.
* `byzantine::AnimsLibrary`: case-insensitive index of an extracted tree, tries
  `<name>.RLE` then `<name>.AVI`, exactly like `0x40fe70`.
* `SceneRecordState` gained `frame_count` (`+0x14`), `anims_signature`
  (`+0x28`), `frames` (geometry), `frames_known` and `current_frame` (`+0x1c`).
* Record creation (`resolve_record`, `0x431b0f`) now:
  1. reads the CA type; 0..3 may create;
  2. with `--anims`, loads the asset — success applies the frame count, the
     first-frame offsets, `+0x28` and `current_frame = 0`; failure (no `.RLE`,
     no `.AVI`) is `0x40fe70` failing, so the command body is **skipped**
     (`anims_load_failures`);
  3. without `--anims` it keeps the previous initialiser-only record.
* `0x91` -> `0x40cf73(record, count)`: `0xffff` clears `current_frame`;
  otherwise `current_frame = count` and a `count >= frame_count` walk logs
  through `0x40ac43`/`0x40ac38` and stays on the last frame
  (`frame_overflow_logs`). `frame_count == 0` (save without asset, or no
  `--anims`) keeps the old request counters.
* `load_save` restores `+0x14` from `raw[0x14..0x15]` and `current_frame` from
  the save's frame index (`0x426b9d` feeds it to `0x40cf73`).
* `byzantine::anims_frame_words(data, index)` re-walks the container for one
  frame and `byzantine::decode_anims_frame(frame, words, rgb565 = true)` decodes
  the run stream into a `DecodedFrame` (`width×height` RGB16 pixels + a coverage
  mask, because command 3 writes nothing). Clipping matches `0x40bbfd`; a
  negative count, a short literal/colour run, a row count that does not match
  the height and a word count that does not match the header are errors.
  `rgb565` applies `0x4164a0` (the load-time `0x40be80` pass).
  `AnimsLibrary::decode(name, index)` ties the two together for `.RLE` assets.
* `--trace` prints one `record id=… frames=… current=…` line per record with
  frame data and, for a created record, a `record_frame id=… index=… size=…
  covered=…` line from decoding the VM's current frame; `--census` aggregates
  `records_created`, `anims_load_failures` and `frame_overflow_logs`.

## 6. Real traces

| Command target | Result |
| --- | --- |
| `VOICE` 531 `--ca --anims` (no save) | `record id=3 frames=5 current=4 x=0 y=20 frames_known=true` — `vtroxeUp` is `640×400`, 5 frames, `0x91` selected frame 4 |
| `VT` 73.btb 1638 | `records_created=2 frame_overflow_logs=1`; `record id=24 frames=6 current=5` (`vtssdr`, `0x91` count 6 → clamped + logged), `record id=26 frames=5 current=4` |
| `NB` 57.btb 556 | `record id=10 frames=3 current=0 … frames_known=true` (`NBBdSld`) |
| `HSTMED` 55.btb 259 | records come from the save: `frames=1/8` and `current=0/4/2` restored, `frames_known=false` (no asset reload for marker 0) |
| `VT` 73.btb 1638 (with decode) | `record id=24 frames=6 current=5` then `record_frame id=24 index=5 size=640x400 covered=16023/256000`; `id=26 index=4 covered=24733/256000` |
| `anims-test <tree>` | `parsed 922 real animation assets, decoded 2846 frames (64 blank)` |
| `anims-test <tree> --render maphl 0` | `99x58 covered=3373/5742 runs=3373 skipped=2` plus a coherent luminance map |

## 7. Census (44 scenes, CA.ALL + anims tree supplied)

`python3 analysis/rea/build_vm_census.py` now extracts `anims/*` from **all six**
ISOs (a scene's asset can live on another CD) and passes `--anims`.

| Outcome | Before | After |
| --- | ---: | ---: |
| `return` / `script_end` / completed | 5,913 / 22 / 5,935 | 5,913 / 22 / **5,935** |
| unsupported opcode | 0 | 0 |
| `unresolved_variable` | 1,296 | 1,296 |
| everything else | unchanged | unchanged |

Diagnostics: **733** records created with real frame data (738 before the asset
load existed), **8** record commands skipped because `0x40fe70` could not load
any file (both `BC` `bckill1`/`bckill2`), **1** `0x91` walk past the last frame,
1,925 skips for CA types outside 0..3.

The outcomes do not move because frame state only affects geometry and the
`+0x1c` index, not which instructions run. The value of the step is fidelity:
`0x64`'s x/y and `0x91`'s frame index are now real instead of requests.

## 8. Not claimed

* Drawing: `0x43b130`'s clip rectangle, the `+0x09 & 0x80` destination-read
  blend (`0x42b5b2`), the GDI blits (`0x42b679`) and the trail list. The frame is
  decoded into a standalone buffer, not drawn.
* `DAT_00485c78 == 10` (the mode in which the 555 -> 565 pass is skipped); the
  port always converts.
* `0x40fe70`'s AVI *decoder* (`0x41c2bb`/`0x41c704`): only the header is read.
* The 2-byte signature semantics and the two skipped header u16s.
* The `hl`/`HLDc00b` 21-vs-39 outlier.
* Marker-1 save slots: `0x426b9d` calls `0x40fe70` and the asset load would
  replace the saved record; the port keeps the saved bytes and documents it.

## 9. Verified inputs

Target/profile as above. Saved exports re-hashed against
`analysis/rea/pseudocode/decompile-index.json`:

| Address | SHA-256 (first 16) | What it settles |
| --- | --- | --- |
| `0x40fe70` | `4ca6defafd38ee24` | `anims` path, `.rle` -> `.avi` fallback |
| `0x40cb26` | `084bbb4b1017cbcc` | 10-byte header, frame count |
| `0x410078` | `1abee4b493e60ea7` | frame loop, offsets, `+0x28` |
| `0x41199e` | `a2633178d9778ffb` | file length only |
| `0x41cb13` | `78f23728b638cbae` | AVI fallback (frame count, w/h) |
| `0x40ffee` | `16955dd55a53620b` | `+0x14` count, initialiser |
| `0x40cf73` | `a63b8280c8f6c629` | `+0x1c` frame walk |
| `0x40be80` | `a635c53194a9fd58` | frame-buffer pass (unknown pixel role) |
| `0x4164a0` | `b9ee16635e1c9e82` | RGB555 -> RGB565 remap |
| `0x426b9d` | `5094e97ef5708b02` | save restore of `+0x14` and the index |
| `0x40bbfd` | `e24b78d2eae4297c` | frame run stream decoder |
| `0x40bbb0` | `c1c7404f9f9136d9` | decoder state: stride, row base, height |
| `0x43b130` | `529ef379c272785e` | renderer entry: current frame + call |
| `0x43c114` | `a77c14f8911db67d` | frame rect (width/height at `+0xc`/`+0x10`) |
| `0x4200bb` | `8bcea0c63f209e48` | second loader that runs `0x40be80` |

Assembly ranges read from the target (`objdump -d -M intel`): `0x410078`–
`0x4102ac` (frame loop, offsets, `0x28` + `fread`), `0x40cb26`–`0x40cbc2`
(header), `0x40be80`–`0x40bf84` (frame-buffer pass), and the `.data` strings at
`0x45e800` (`"%c%c\%s"`), `0x45e808` (`.rle`), `0x45e810` (`anims`), `0x45e820`
(`.avi`).

## 10. Commands

```sh
make -C port -j2 all test
./port/anims-test /tmp/<extracted-tree>                       # 922 assets, 2846 frames
./port/anims-test /tmp/<extracted-tree> --render maphl 0      # 99x58, covered=3373/5742
./port/anims-test /tmp/<extracted-tree> --render vtroxeup 4   # 640x400, a real frame
python3 analysis/rea/build_vm_census.py            # extracts anims/* from all six ISOs

E=<extracted copy2hd tree>; S=$E/copy2hd/savegame
port/byzantine-cc port/output/cc_all/VT --trace-save $S/73.btb 1638 port/output/cb_all/VT \
  --engine $E/copy2hd/engine --lcx $E/copy2hd/engine/SHOW.LCX \
  --lci $E/copy2hd/engine/SHOW.LCI --lcy $E/copy2hd/engine/SHOW.LCY \
  --inventor $E/inventor.txt --hotitems $E/hotitems.txt \
  --gameprefs $E/copy2hd/goldsave/GAMEPREF.000 --ca $E/ca/VT --anims $E \
  --co port/output/co_all/VT --ci $E/ci/VT --cy $E/cy/VT --cf $E/cf/VT
# records_created=2 frame_overflow_logs=1
# record id=24 frames=6 current=5 x=0 y=20 frames_known=true
# record_frame id=24 index=5 size=640x400 covered=16023/256000
# record id=26 frames=5 current=4 x=0 y=20 frames_known=true
```
