# In-scene text windows (fonts, `0x417a17`, `0x4177c7`) and the dialogue data

Status 26 October 2026. Scope: the game's own text renderer and the two text
windows, wired to the shipped per-scene text table. Target `BETRAYAL.EXE`,
SHA-256 `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
All hashes are the first 16 hex digits of the indexed export SHA-256.

## 1. Where the text lives

`0x412f77` loads one archive table per suffix built by `0x412d35`:

| suffix | table | used by |
| --- | --- | --- |
| `ca` | `DAT_0047dd70` | scene animation assets (record creation) |
| `cd` | `DAT_0046e870` | **conversation records** (name + `.ccd`) |
| `cs` | `DAT_0047a8e0` | sound names (`0xc4`, `0xd1`) |
| `cf` | `DAT_00484330` | video names (`0xbd`) |
| `co` | `DAT_00483d90` | panorama/node names (`0xb9`) |
| `ci` | `DAT_00485400` | dialogue/item name ids (`0x408bb2`) |
| `cy` | `DAT_0047a2b0` | channel name ids (`0x408c81`) |
| `ce` | `DAT_00480320` | **the text table** (`0x412206` → `DAT_00483640`) |
| `ccs`/`ccz` | … | caption tables |

`0x412206` (`3a35b26c1b54a0e2`) reads `engine/<scene>ce`, splits it into entries
at a line starting with `$`, drops CR and joins the lines with `\n`
(`DAT_00483640[i]`). The shipped `CE.ALL` has 16 records: CLOCK 4, EXAMVB 10,
EXAMVC 10, EXAMVH 8, EXAMVT 13, HSTED 4, LINTEL 3, MORRIS 3, MZ2 2, STAR 3,
VA 28, VB 30, VC 35, VH 32, VOICE 11, VT 62 entries, all in the game's Turkish
single-byte font encoding (see §3; not CP1254).

## 2. The two windows

- `0x417a17` (`9da3162b0c890a17`): the **message** block. It stores the text and
  calls `0x4063e0(text, 0x26e, 0x2d, font 3, surface, 0, 3, 4)` — right-aligned
  to x=0x26e, top y=0x2d, at most 3 lines, font slot 3. `0xcd` calls it with
  `DAT_00483640[raw(0)]` and mode `raw(1)` only when `DAT_00464878 == 0`.
- `0x4177c7` (`014ee44e6a6f988c`): the **examine scroller**. With `NULL` it
  clears `DAT_004603d4` (window off). With text it draws `0x4063e0(line, 5,
  0x14a + …, font 1, surface, 0, 3, 3)` — left-aligned at x=5, three lines, and
  advances one line per second (`DAT_0045f22c`/`DAT_0045f230`, scroll offset
  `DAT_0045f23c` starts at `2*line_height + 0x14a` and decreases).
- `0x40c329` (`5a9e9344f5661475`) is the click dispatcher: it clears the window
  (`0x4177c7(0)`) when a text is current, copies the clicked object's `+0x14`
  into `DAT_004603d4`, and `0x40df05(kind)` lets the object's action label run.
  An object with `+0x14 == 0xffffffff` therefore blanks the window.
- `0x4176d8` calls `0x4177c7(DAT_00483640[DAT_004603d4])` every frame, which is
  the reveal/scroll tick.

## 3. The font pipeline

- `0x406cfe` (`df4c09b307ceca77`) / `0x406ef5` fill seven 0x48-byte slots:
  0 dialog, 1 klio, 2 setup, 3 klio (message), 4 captions, 5 white, 6 yellow.
  Slot 1 gets `0x405e7e(klio, 1, 0x1b<<green_shift, 0xffff)`, slot 3 gets
  `0x405e7e(klio, 3, (0xb<<red_shift)|(0xe<<green_shift)|2, 0xffff)`. The shifts
  come from `0x4162a0`/`0x416375`: red 11, green 5 (6 for the 565 screen), blue 0,
  so the fills are 0x0360 (examine) and 0x5b82 (message) in RGB565.
- `0x4063e0` (`a321c3e91c9b99e1`) is the drawer: alignment 1 centres, 2/4
  right-align, 3 left-aligns; `\n` starts a line at `+line_height`; `param_8`
  bounds the line count. The glyph index is `byte - 0x20` for 0x20..0x7f; for
  bytes above 0x7f `0x4061e7`/`0x4063e0` call `FUN_00405bf4`, which (in the
  non-English branch) returns `byte - 0x21`, so the drawer uses `byte - 0x41`.
  That is the shipped Turkish font encoding: e.g. 0xa7 -> glyph 0x66 (ç),
  0xa8 -> 0x67 (ş), 0xaa -> 0x69 (ı), 0xab -> 0x6a (ğ), 0xac -> 0x6b (ü),
  0xad -> 0x6c (ö), with the capitals at 0xa1..0xa6 (Ç Ş İ Ğ Ü Ö). `0x4061e7`
  width and `0x4062bc` line width use the same mapping. `DAT_0045d0ac` is set
  by `0x406cbf` only when the language string contains "English"; in that
  branch the high byte keeps its direct `byte - 0x20` index. The port models the
  shipped Turkish path.
- `0x4067e2` (`99488c31c78b9447`) blends each glyph pixel: with the `bk` mask
  black (`0x4066c6(0, dest, a)`), then with the `gr` mask the fill colour
  (`0x4066c6(fill, dest, a)`), where `0x4066c6` (`fbcc5640b962cc1c`) is
  `(fore*(a+1) + back*(255-a)) >> 8` per channel (the 0x405b90 table is the plain
  channel product). `0x406041` (`f0955510ba79a92d`) reads the 18-byte TGA
  header, flips bottom-up rows and converts 16-bit 555 at `0x405c19`.
- **The `.SFN` atlas is not the glyph shape for the shipped text path**: the
  glyphs come from the `gr`/`bk` 8-bit TGA companions (e.g. `klio.SFN` +
  `GR_KLIO.TGA` + `BK_KLIO.TGA`, 375×93). The metrics are 223 entries of
  (x, y, advance, height); entries past the defined glyphs are `0xcdcd` filler.

## 4. Implementation

`port/include/byzantine/text.hpp` + `src/text.cpp`:

- `load_sfn_font(sfn, gr_tga, bk_tga, fill)`: strict SFN header/metrics/bounds
  checks (version 16, metrics and atlas inside the file, glyphs inside the
  atlas, `0xcdcd` entries as undefined), 8-bit greyscale TGA of exactly the
  atlas size with row flipping, line height = first glyph's height.
- `draw_text(surface, font, text, x, y, max_lines, alignment)`: the `0x4063e0`
  rules above and the exact `0x4067e2` blend. Bytes outside the font's glyph
  range are skipped, never substituted; the shipped Turkish bytes use the
  `FUN_00405bf4` high-byte index (`byte - 0x41`), so ç/ş/ı/ğ/ü/ö and their
  capitals reach the font's own glyphs without a re-encoding step.
- `parse_scene_text(bytes)`: `0x412206`'s `$`-separated entries; an unterminated
  entry is rejected.
- Player: `--fonts <dir>` loads klio slot 1 and 3; `--ce <file>` (a per-scene
  option, so a `0x73` travel reloads the target's record) loads the table.
  `scene.sh` extracts `copy2hd/fonts` and splits `CE.ALL` into `<data>/ce` (also
  when the data dir was prepared by an older revision).
  After a committed click the clicked object's `+0x14` selects
  `examine_text`; a `0xcd` request sets `message_text`; `Display::compose`
  draws both with `text_draw window=…` diagnostics. A scene load clears both.

Not modelled: the message panel that `0x417a17` blits behind the block (the text
draws directly on the scene), the one-line-per-second reveal/scroll of
`0x4177c7` (the block is drawn at its final position), the 0x4176d8 tick, and
the `0x4069a4` path that blits the `.SFN` atlas itself.

## 5. The conversations have text (correction, 26 October 2026)

The earlier claim that conversations are voice-only was **wrong**. `0x410669`
(`1a4831f4aa67ebf9`) loads three packed records for a cd entry:
`engine/<stem>.ccb` (bytecode), `.ccf` (voice-clip names) and `.byz` (the
dialogue lines, e.g. `MMhmr1a`, `IMhMR2a` inside `Byz.all`). `0x424780` draws the
BYZ line and `0x40d2a1` plays the paired `dialog/<name>/<clip>.avi` unless the
speaker id from `0x40ced0` is `-1` (`pro`, the player's own lines) or the text is
`null`; `0x40ced0` compares the clip's speaker code with `__strcmpi`, so the
shipped mixed-case codes (`Ima`, `Nur`, `Coi`, ...) count too. `0x424780` selects
the line; `0x424981`
clears rows 420..479 and draws it at (55,420) with font slot 4, green `0x03eb`,
wrapped at 530 px for at most 3 lines. The shipped Turkish `CAPTIONS.SFN/TGA`
has blanks where most Turkish glyphs belong, and its ASCII glyphs are
pixel-identical to `DIALOG`'s, so the port uses `DIALOG`'s shapes for the
subtitle and reports the missing CAPTIONS glyphs. A "conversation with text"
therefore uses the shipped text; see `analysis/rea/CONVERSATION.md` for the
format, the MR example (IMhMR2a, real subtitles and AVIs), and the implemented
VM slice.

The text-bearing examine/message window described above remains the other real
text interaction; it is not a substitute for the conversation subtitles.


## 6. Real results

```
$ ./port/scene.sh . CLOCK --entry 70.btb            # entry = instruction 0
fonts klio glyphs=223 atlas=375x93 line=22
scene=CLOCK label=0 state_origin=save objects=33 … records=3
composed background=vtclock.jpg drawn=3 … pixels=9949

click_at 330,250 -> 330,270 slot 31 action 576 click_result steps=1 …
text object=31 index=0 lines=4
text_draw window=examine x=5 y=330 width=308
```

The frame was written as PPM and the crop inspected: "ARAŞTIRMACIYA UYARI:
YERLEŞİK VERİ / MEKANİK SAAT / AHŞAP VE ALTIN, 1520" (the 4th line, the year, is
the one the scroll would reveal). VT 497 (72.btb) additionally shows the
**message** window: the startup trace requests text 1 and `0xcd`/`0x417a17`
draws `CE.ALL` VT entry 1 right-aligned at (378, 45) ("HÜNKAR SOFASI (TAHT
ODASI)", width 244, 1384 changed pixels); `--click 19` adds the examine entry 3
("SAZ HEYETİ BALKONU").

## 7. Tests

`text-test`: synthetic SFN + GR/BK TGAs, glyph metrics/lookup, the `0x4067e2`
blend values, left/centre/right alignment, `\n` and `max_lines`, undefined codes
skipped, seven rejections (version, truncation, glyph outside the atlas, missing
file, non-8-bit TGA, size mismatch, unterminated text entry). `make -C port -j2
all test` passes all ten suites.

## 8. Verified exports

| Address | SHA-256 (16) | Role |
| --- | --- | --- |
| 0x412206 | 3a35b26c1b54a0e2 | `engine/<scene>ce` → `DAT_00483640` |
| 0x417a17 | 9da3162b0c890a17 | message block (0x26e,0x2d,font 3) |
| 0x4177c7 | 014ee44e6a6f988c | examine scroller (5,0x14a,font 1) |
| 0x40c329 | 5a9e9344f5661475 | click → `DAT_004603d4` = object +0x14 |
| 0x406cfe | df4c09b307ceca77 | font registry + fills |
| 0x406ef5 | 7411bee3de54d03a | second registry pass |
| 0x405e7e | 74e5b2941030af57 | slot setup (metrics, two colours) |
| 0x406041 | f0955510ba79a92d | TGA reader (18-byte header, 16-bit 555) |
| 0x4061e7 | 53d847dfc7e31e96 | text width |
| 0x405bf4 | 898ae1b9123f325e | high-byte font index (`byte - 0x41`) |
| 0x406cbf | 62ebc4ffc90d31f4 | language gate (`DAT_0045d0ac`) |
| 0x4062bc | 6b7e8f71d8177b7b | line width |
| 0x406358 | c9a49e33b97cea8d | per-glyph advance |
| 0x4063e0 | a321c3e91c9b99e1 | glyph drawer (align/newline/max lines) |
| 0x4066c6 | fbcc5640b962cc1c | alpha blend |
| 0x4067e2 | 99488c31c78b9447 | gr over bk over destination |
| 0x4069a4 | ab6b7bd02064ab5d | `.SFN` atlas blit path (not used) |
| 0x410669 | 1a4831f4aa67ebf9 | conversation loader (.ccb/.ccf) |
| 0x4119e3 | beac71ca47ce006c | conversation bytecode parser |
| 0x43340a | 3a9555ec83f82adc | conversation VM (no text drawing) |
| 0x42db22 | 8df50fc5339a585b | case 0xcd |
