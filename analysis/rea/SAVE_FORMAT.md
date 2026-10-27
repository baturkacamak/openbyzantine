# `savegame/NN.BTB` save format

> 27 October 2026: the writer `FUN_00424dc0` is now implemented
> (`byzantine::write_save`) and is byte-identical to `parse_save` on all 51
> extracted CD1 saves. The menu's save path, the live-state builder and the
> resume rules are in `analysis/rea/TOP_MENU.md` §5.

Bounded step of 25 October 2026. Scope: **read and document** the save image.
Restoring state, `0xcc`, `0x70` (CONVERSE) and the carried variable pool are
deliberately *not* part of this step.

Evidence base (saved exports only, no binary session opened):

- target `analysis/rea/BETRAYAL.EXE`, SHA-256
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
  (matches `pseudocode/decompile-index.json` `target_sha256`);
- analysis profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
  (matches every export header and `analysis_profile_digest`);
- reader `FUN_00425ab4` = `analysis/rea/pseudocode/functions/0x425ab4.c`;
- writer `FUN_00424dc0` = `0x424dc0.c` (the same blocks in the same order, which
  independently confirms the read order);
- one focused `objdump -d` of `0x426d40–0x426f00` to settle the node extra-list
  loop that the decompiler body loses.

All 102 function exports used below were re-hashed against
`decompile-index.json` (`sha256` per `relative_path`); every one matched — see
§9.

Terms used below: **observation** = read directly from the export or the shipped
bytes; **inference** = a conclusion that fits the code and the data but is not
stated by either; **unknown** = not recovered.

---

## 1. Entry point and file naming

```
0x425ab4(name):
    strcpy(buf, "savegame\")          ; 0x45ff00
    strcat(buf, name)                 ; name has no directory part
    f = fopen(buf, "rb")              ; 0x44bc20
    if (!f) { 0x40ac43/0x40ac38 log; return 1; }
    0x40df05(0x1f)
    ...
```

* **Observation** `0x425ab4` builds `"savegame\" + name` and opens it `"rb"`.
  Callers pass a bare file name:
  - `0x41efc1`: `sprintf("%02d.btb", index)` → `0x425ab4` (so `00.btb` … `73.btb`);
  - `0x41e289` case 3: the literal `"new.can"`;
  - `0x41e289` case 5: the literal `"qsave.000"`.
  The string table also holds `"savegame\%02d.btb"` and `"savegame\%s"`.
* **Observation** `0x40df05(0x1f)` runs before the first read (not modelled).
* Related files next to the saves (same string block, not parsed here):
  `savegame\btb.sav` (slot list, the CD1 ISO copy lists `00.btb` … `73.btb`),
  `savegame\jnk.sav`, `savegame\status.000`, `savegame\notebook.000`,
  `savegame\gamepref.%03d`, `savegame\autosave.btb`,
  `savegame\tempsave.btb`, `drives.btb`.

---

## 2. Block order

Writer `FUN_00424dc0` → reader `FUN_00425ab4`. Sizes are file bytes. The
example offsets are the real CD1 image `00.BTB` (85,068 bytes).

| # | offset (00.BTB) | size (00.BTB) | writer | reader | content |
|---|---|---|---|---|---|
| 1 | 0 | 4 | `0x424dc0` | `0x425ab4` | `u32` required-CD number (`DAT_0045da78`) |
| 2 | 4 | 5 | `_fputs` | `_fgetc` loop | NUL-terminated chapter name (`DAT_004859c0`) = `"show"` |
| 3 | 9 | 2 | `_fputs` | `_fgetc` loop | NUL-terminated save name / description (`DAT_00484e30`) = `"1"` |
| 4 | 11 | 56,448 | `0x420888` | `0x4209d2` (`fseek +0xdc80`) | 168×168 RGB16 thumbnail (`0xdc80` bytes) |
| 5 | 56,459 | 2 | `0x424dc0` | `0x425ab4` | `u16` scene index (`DAT_00482428`) |
| 6 | 56,461 | 3,502 | `0x425218` | `0x426261` ×0x15e | per-scene object records (`DAT_00482ad0`) |
| 7 | 59,963 | 6,000 | `0x424dc0` | `0x425ab4` | `u16[3000]` `DAT_0047e2f0` |
| 8 | 65,963 | 12 | `0x408a1e` | `0x408adc` | `u32` ×3: inventory strip visible (`DAT_0045d1c0`), page (`DAT_0045d1c4`), scroll (`DAT_0045d1bc`) |
| 9 | 65,975 | 40 | `0x408a1e` | `0x408adc` | `u32[10]` item count per inventory page (`DAT_00468cc0`) |
| 10 | 66,015 | 14,000 | `0x408a1e` | `0x408adc` | `u32[10][350]` item ids per inventory page (`DAT_00468d78`) |
| 11 | 80,015 | 4 | `0x408a1e` | `0x408adc` | `i32` selected item (`DAT_0045d1c8`), `-1` = none |
| 12 | 80,019 | 253 | `0x40892d` | `0x4089b0` | `(flag,value)` pairs, terminator `0xff`, max 0x15e |
| 13 | 80,272 | 12 | `0x424dc0` | `0x425ab4` | `u32` `DAT_0045f210`, `DAT_00460668`, `DAT_0046066c` |
| 14 | 80,284 | 1 | `0x424dc0` | `0x425ab4` | one byte; if non-zero a NUL-terminated string (`DAT_0045f214`), here `0x00` = empty |
| 15 | 80,285 | 4 | `0x424dc0` | `0x425ab4` | `u32` `DAT_0045c060` (display mode selector) |
| 16 | 80,289 | 2 | `0x424dc0` | `0x425ab4` | `u16` `DAT_0048253e` — **only when #15 == 0** |
| 16′ | — | 8 | `0x401000` | `0x40114a` | `u32` `DAT_00480330` (CO.ALL panorama index) + `u32` `DAT_0045c064` (panorama x) — **only when #15 != 0**; reader then sets `DAT_0048253e = 0xffff` |
| 17 | 80,291 | 2,051 | `0x4269ec` | `0x426b9d` ×0x15e | node/animation records (`DAT_004649e8`) |
| 18 | 82,342 | 1,574 | `0x424dc0` | `0x425ab4` ×0x15e | object records `DAT_00464300`, 0x18 bytes each |
| 19 | 83,916 | 4 | `0x427135` | `0x42722b` | `u32` `DAT_0045fd2c` |
| 20 | 83,920 | 350 | `0x427135` | `0x42722b` ×0x15e | sound records `DAT_00486220`, 0x3c bytes each |
| 21 | 84,270 | 1 | `0x427135` | `0x42722b` | sound chain entries `DAT_0046019c`, 0x5c bytes each, then a `0xff` terminator |
| 22 | 84,271 | 350 | `0x424dc0` | `0x425ab4` ×0x15e | media records `DAT_00464f60`, 0x14 bytes each |
| 23 | 84,621 | 4 | `0x424dc0` | `0x425ab4` | `u32` `DAT_00464878` (scene id the image was written in) |
| 24 | 84,625 | 350 | `0x408d3f` | `0x408dbd` ×0x15e | dialogue chains `DAT_00486a10`, 0x68 bytes each |
| 25 | 84,975 | 93 | `0x42118c` | `0x4210af` | notebook text, one line each until EOF |

Note for `00.BTB`: blocks 20 and 21 hold only absent markers here (350 slot
markers + the chain terminator = 351 bytes), and the media table is likewise
350 absent markers, so block 23 starts at 84,621.

**Observation** blocks are *not* delimited or length-prefixed; every record
class uses one of two marker conventions:

* `0x00` = present, `0xff` = absent (blocks 6, 18, 20, 22) — the loaders test
  `(char)c == 0`;
* anything but `0xff` = present (blocks 17, 24) — the loaders test
  `(char)c != -1`.

The three fixed *single-array* tables whose marker *must* be one of those two
values (18 objects, 20 sounds, 22 media) are validated strictly by the port
parser: `0x424dc0` only ever writes those two bytes, so anything else means the
stream is misaligned.

**Observation** the 77 CD1 images (74 numbered + `autosave` + `tempsave` +
`NEW.CAN`) range from 79,534 to 119,215 bytes; the whole spread comes from the
variable-length blocks 6, 17, 20, 21, 22, 24 and 25 (plus the variable-length
header strings), and 75 of the 77 sizes are distinct.

---

## 3. Block details

### 3.1 Thumbnail (block 4) — observation

`0x420888` builds `local_14 = {w=0xa8, h=0xa8}`, asks the renderer for image id
`0x6c`, then writes `0xa8` rows of `0xa8` 16-bit pixels:

```c
local_88[0] = 0x6c;                    // image id
for (i = 0; i < 0xa8; ++i) { fwrite(row, 2, 0xa8, file); row += 0x150; }
```

`0x150` is the source stride in bytes (336 bytes) and `0xa8 * 2 = 336`, so the
saved pixels are the 168×168 crop. The loader does not read the pixels back:
`0x4209d2` is `fseek(file, 0xdc80, SEEK_CUR)`. **Inference**: the thumbnail is
only consumed by the save/load UI (`0x4207ef` path when no video surface
exists); the loader that runs on load skips it, which is why a corrupt
thumbnail cannot break loading.

### 3.2 Chapter name and save name (blocks 2, 3)

`0x424dc0` copies `DAT_004859c0` and `DAT_00484e30`; `0x425ab4` reads them with
unbounded `_fgetc` loops.

* **Observation** `DAT_00484e30` is the text of the save-name edit box
  (`0x421d5e` uses `SetWindowTextA`/`GetWindowTextA` with limit `0x46`), i.e.
  the player-visible save name. Autosaves get `0x7f` + `" "` + a generated name
  (`0x42515f`: `DAT_00484e30 = 0x7f; DAT_00484e31 = 0x20;`), which is why the
  shipped `autosave.btb`/`tempsave.btb` descriptions start with `0x7f` and then
  Turkish CP1254 text (`Otomatik kayıt`).
* **Observation** `DAT_004859c0` is the chapter/`.lcc` base name. `0x425ab4`
  feeds it to `0x437cad`, which appends `"."` (no extension case, string
  `0x461524`) and calls `0x437f76`; `0x437f76` makes `DAT_0046e250 =
  name + "lcc"`, `DAT_00485980 = name + "lcv"`, `DAT_00484320 = name + "lvm"`,
  `DAT_00482430 = name + "lci"`, `DAT_00482ac0 = name + "lcy"`, then calls
  `0x438173`, which opens `"engine\%s"` and fills the **scene name table**
  `DAT_00483bc0` (9 bytes per entry, `DAT_00482528` = count) from the file's
  first lines.
* **Observation** `0x438173` reads lines with `fgets(buf, 0x50)`, replaces
  `'\n'` with `'\0'`, lowercases every remaining byte, rejects names longer than
  8 bytes (`", is too long; must be no more than 8 characters"`) and keeps the
  order. The file is opened in text mode (`"r"`), so on Windows CRLF arrives as
  LF; `SHOW.LCC` itself is CRLF on the CD.
* **Observation** all 77 shipped CD1 saves store the single chapter `"show"`;
  `analysis/rea/engine_sample/copy2hd/engine/SHOW.LCC` holds 44 names, which is
  exactly the 44 scene directories of `port/output/cc_all/`.

### 3.3 Scene index (block 5)

* **Observation** `0x425ab4` stores the `u16` in `DAT_00482428` and then loads
  the scene name with
  `strcpy(DAT_0046e260, &DAT_00483bc0 + DAT_00482428 * 9)` followed by
  `strcat(".cc")` (string `0x45ff48`), i.e. **the index selects the N-th line
  of the chapter's `.lcc` list**. There is no bounds check in the original.
* **Observation** the port validates the index against the supplied chapter
  list and returns "no name" past the end instead of inventing one.
* **Inference (validated, §5)** the stored index is the chapter-list position of
  the scene that was active when the save was written. This is the check that
  the acceptance criterion uses, because the scene name derived this way makes
  every object label resolve in that scene's CC script.

### 3.4 Per-scene object records (block 6, 0x15e slots)

`0x426261(slot, file)` reads one slot; `0x425218(slot, file)` writes it. A slot
is present when the marker byte is `0x00`.

```
u8    marker            0x00 present, 0xff absent
if present:
  cstr  name            copied to record +0x1e94 (record size 0x1ea4)
  u16   word08          -> +0x08
  u8[5]                 -> +0x0a .. +0x0e
  u8[0x32]              -> +0x0f
  u8[0x32]              -> +0x41
  u8[0x32]              -> +0x73
  50 × FUN_0042658f     list -> +0x4c4[i]   : int8 count, count × (u8 value, u8 code)
  50 × FUN_0042658f     list -> +0x58c[i]
  50 × FUN_00426662     ptr  -> +0xa8[i]    : 0xff absent, else u8 id, u8 list, u8 index
  50 × FUN_00426662     ptr  -> +0x170[i]
  50 × FUN_00426662     ptr  -> +0x238[i]
  50 × FUN_00426788     pair -> +0x3c8[i]   : always 2 bytes, 0xff first = absent
  u8[0x32]              -> +0x490
  50 × FUN_00426892     chain-> +0x654[i]   : u8 n, n × (u8 name, u8 table index)
```

* **Observation** `FUN_0042658f` compares the count as a *signed* `char`
  (`local_14 < (char)iVar1`), so a count of `0x80..0xfe` reads no entries even
  though it consumes its own byte. The port reproduces that. Across all 77
  shipped images the 17,149 link-list elements come from count bytes of
  `0x00..0x0b` — no count ever reaches the `0xff` marker value, so the
  signed/unsigned difference never shows.
* **Observation** `FUN_00426892` uses `byte` counters, so its count is unsigned
  (128..255 entries are possible in principle). In the shipped data the 122,350
  chain slots all hold `0x00` or `0x01`.
* **Observation** `FUN_00426662` stores the record pointer at list index
  `u8 #2` after stepping `u8 #3` times, i.e. these are (name id, list, rank)
  triples; `FUN_00426788` is the same idea with an explicit list selector.
* **Observation** the name is resolved by `0x410669(slot, &id, name)`: it copies
  the name, appends `".ccb"` (`0x45e8e0`) and opens `engine\<name>.ccb` (direct
  file when `DAT_0045e7a8 == 0`, otherwise `0x411b4b`'s archive lookup), then
  allocates the 0x1ea4 record in `DAT_00482ad0[id]`. CD1 has **no**
  `engine/*.ccb` and no `CCB.ALL`, so that asset lookup cannot be satisfied from
  the CD1 layout; the shipped names are real (`IAkEA1a`, `VOemphn2` …) and appear
  in `engine/Byz.all` and `sounds/`. The port does not model the lookup, because
  the block itself is **self-describing**: every list is length-prefixed, so a
  save can be walked without any engine table. Resolving what each name loads is
  restore-step work.
* **Unknown** the meaning of `+0x08`, `+0x0a..+0x0e`, the three `0x32`-byte
  arrays, `+0x490` and which engine table the ids index. `0x436b01` walks
  `+0x41`/`+0x73`/`+0x170`/`+0x238` as an animation sequencer but the field
  meanings were not recovered in this step. They are parsed (so the stream stays
  aligned) and exposed offset-named.

### 3.5 `DAT_0047e2f0` (block 7)

* **Observation** 3000 `u16` values, written as `fwrite(&DAT_0047e2f0, 2, 3000)`.
* **Observation** `0x42db22` case `0x67` writes one entry through the per-scene
  node map `DAT_0046ee00[...][DAT_00482428]`; `0x40a4bc` reads one entry for the
  same map. **Inference**: a per-node word (a node flag/value) addressed by the
  scene's node id, not by the save's node table slot.
* **Unknown** the exact semantics of the value; the port keeps the table.

### 3.6 Inventory strip state (blocks 8–12, function `0x408adc`)

The first analysis pass called this "dialogue state"; later work on the bottom
bar showed that `DAT_00468cc0`/`DAT_00468d78`/`DAT_00486f90` are the inventory
pages/items drawn by `0x407420` and hit-tested by `0x408076`/`0x4083d4`.
`0xbb` selects the page; `0xab` pushes an item; `0xb0`/`0xb1` compare item
state. This is the same data the conversation code never touches.

```c
0x408adc(file):
  DAT_0045d1c8 = 0;
  fread(&DAT_0045d1c0, 4, 1);   // strip visible; gates 0x4075fe/0x4083d4/0x40d750
  fread(&DAT_0045d1c4, 4, 1);   // inventory page, 0x407420 indexes it
  fread(&DAT_0045d1bc, 4, 1);   // first visible item of that page
  fread(&DAT_00468cc0, 0x28,1); // 10 × u32 item count per page
  fread(&DAT_00468d78, 14000);// 10 × 350 × u32 item ids
  fread(&i, 4, 1);              // selected item index
  if (i == -1) DAT_0045d1c8 = 0; else DAT_0045d1c8 = DAT_00486f90[i];
  0x4089b0(file);               // first byte (state) of each item record
  0x407420(0);                  // redraw the strip
```

* **Observation** the strides match exactly: `0x28 = 10 × 4` and
  `14000 = 10 × 0x578 = 10 × 350 × 4`. `0x4077b2(channel, id)` pushes on
  channel `ch` with `depth = DAT_00468cc0[ch]++` and writes
  `DAT_00468d78[ch * 0x578 + i * 4]` after shifting the older entries up;
  `0x407420` walks `DAT_0045d1c4`'s stack for at most 6 visible lines and reads
  `DAT_00468cc0[ch]`, `DAT_00468d78[ch*0x578 + i*4]` then `DAT_00486f90[id]`.
  So block 9 = item count per page, block 10 = the 350-entry item ids.
* **Observation** block 11 is written as `DAT_0045d1c8->+0x08` (or `-1`), i.e.
  the selected item's `+0x08` (its name id in `DAT_00485400`) is the index into
  `DAT_00486f90`; `0x408adc` restores the pointer from it.
* **Observation** `0x4089b0` loops `i < 0x15e` while `fgetc != EOF`, reading one
  value byte per record; `0x40892d` writes `(0x00, record[0])` per allocated
  record and terminates with `0xff`. So this block is “the state byte of each of
  the N inventory item records”.
* **Data check** all 77 images have exactly **126** entries here, and
  `analysis/rea/DIALOGUE_RECORDS.md` independently counts **126**
  `DAT_00486f90` item records built from `engine/SHOW.LCI` on CD1. Every item id
  stored in block 10 is `< 126` (checked for all 77 images), and the largest
  page count is 20 (limit 350).
* **Observation (corrected 26 October 2026)** `DAT_0045d1c0` is the bottom
  inventory strip's visibility: `0x4075fe`/`0x4083d4` draw the 60-px strip only
  when it is set, `0xb4`/`0xb5` clear/set it, and `0x407420`/`0x408076` use the
  pages. It is not a dialogue-window gate.

### 3.7 Mid-block globals (blocks 13–16)

| field | observation | meaning |
|---|---|---|
| `DAT_0045f210` | `0x417054`, `0x4177c7`, `0x417a17`, `0x417e9c` test it **together with** `DAT_0045c060`; `0x417a17` passes it to `0x417a17(…,0)` and `0x416ec0` passes it as a text buffer | **inference**: “a text/panel object is currently loaded”; **unknown** which |
| `DAT_00460668` | written as `1`/`2` in `0x42db22`; read by `0x401184`, `0x40d031`; reset to 0 by `0x41dd22` | **inference**: video/letterbox mode; **unknown** |
| `DAT_0046066c` | written `1`→`2` by `0x422770`; tested by `0x4013ae`, `0x42d594` | **inference**: a one-shot UI/animation latch; **unknown** |
| `DAT_0045f214` | optional string; freed by `0x417a17`; `0x4176d8` uses it only when `DAT_0045c060 == 0`; `0x416ec0` → `0x417a17(DAT_0045f214, 0)` | **inference**: text of the current text block; **unknown** |
| `DAT_0045c060` | `0x401074`, `0x4013ae`, `0x401ca6`, `0x401ee6`, `0x402285`, `0x417491` gate rendering on `DAT_0045c060 != 0`; `0x42db22` `0x69` (BACKGRND) sets it to 0 | **inference**: “a panorama/background is displayed” mode flag which selects block 16 or 16′ |
| `DAT_0048253e` | `0x41df43` → `FUN_0041045b(DAT_0046e82c, id, 100)`; `0x426935` uses `DAT_00484e80[id]`; `0x42d594` sets `0xffff` | **unknown** (a name/record id in another table) |
| `DAT_00480330` | `0x417c33` reads `DAT_00483d90[DAT_00480330]` = **CO.ALL panorama record**; `0x42db22` `0x69` sets `-1` | **observation**: panorama id |
| `DAT_0045c064` | `0x401ddf` divides it by `DAT_0046487c` to derive the panorama yaw and then resets it to `-1` | **observation**: staged panorama x position |

### 3.8 Node / animation table (block 17, 0x15e slots)

```
u8    marker        0xff absent; 0x00 record only; 0x01 record + animation load
if present:
  u8[0x40]  record   -> DAT_004649e8[slot] (allocation size 0x40)
  u32       idx      -> +0x1c frame index, 0xffff means "none" (0x40cf73)
  u32       idx      -> +0x20 frame index, 0xffff means "none" (0x426ed0)
  u32       field_2c -> +0x2c
  u32       field_30 -> +0x30
  repeat: u8 control; while 1..127: u32, u32  -> extra list at +0x34 (0xc per node)
```

* **Observation** the `0x40`-byte record is copied field by field in
  `0x426b9d` (`+0,+2,+4,+6,+8,+9,+0xa,+0xc,+0xe,+0x10,+0x12,+0x14,+0x24,+0x26,
  +0x29`); everything else keeps what `0x40ffee` wrote after `memset(rec,0,0x40)`:
  `+0x02 = 0x14` (y), `+0x04 = 10`, `+0x09 = 0x20` (flags), `+0x14 = kind`
  (its parameter), `+0x2c/+0x30 = 0xffffffff`, `+0x18` = a fresh 0x18-byte frame
  list head. `analysis/rea/ANIMATION_RECORD.md` documents the same initialiser.
* **Observation** marker `0x01` calls `0x40fe70(slot, &DAT_004649e8[slot])`: it
  builds `anims\<3 bytes of the anim name>` and loads it. The lookup needs
  `DAT_0047dd70[slot]` to exist — the record that `0x426261`'s per-scene block
  also indexes. CD1 `00.BTB` uses marker 1 four times.
* **Observation (writer/reader mismatch)** `0x4269ec` writes a **count** byte
  followed by `count × 8` bytes, but `0x426b9d` reads a **control** byte and
  then 8 bytes while that byte is `1..127` (assembly `0x426e28`:
  `movsx`… `test eax,eax; jle`). A count of 2 or more would therefore leave the
  stream one byte short per entry on reload. Every entry in all 77 shipped
  images has control `0x00` and an empty list, so the mismatch never fires in
  this data set. The port follows the **reader**, which is what a load does.
* **Unknown** the `0x40`-byte record's field meanings beyond the initialiser,
  and the extra-list `(u32,u32)` pairs.

### 3.9 Object table `DAT_00464300` (block 18, 0x15e slots)

0x18 bytes per present slot. Layout **observed** from `0x42db22` case `0x72`
(the hotspot definition that allocates the same 0x18 bytes):

| offset | type | written by | meaning |
|---|---|---|---|
| `+0x00` | `s16` | `0x72` operand 1 | x |
| `+0x02` | `s16` | `0x72` operand 2 **+ 0x14** | y |
| `+0x04` | `s16` | `0x72` operand 3 | width |
| `+0x06` | `s16` | `0x72` operand 4 | height |
| `+0x08` | `u8` | `0x72` writes 0; `0x6d` (ON) writes 1, `0x6e` (OFF) writes 0; `0x43340a`, `0x438cf9`, `0x438d78` too | active/visible |
| `+0x09` | `u8` | `0x72` operand 6 | kind (2, 8, 0x13, 0x14, 0x17 …) |
| `+0x0a` | `u16` | **no recovered writer** | stale heap bytes |
| `+0x0c` | `u32` | `0x72` operand 5 | **CC label id** |
| `+0x10` | `u32` | `0x72` operand 7 (resolved) or `0xffffffff` | name/target id |
| `+0x14` | `u32` | `0x72` = `0xffffffff`; overwritten by `0xcc` | argument used by `0xcc` |

* **Observation** `+0x0c` is the load-bearing field:
  `0x42db22` case `0x70` (CONVERSE) does
  `*param_1 = DAT_004654e0[ DAT_00464300[id]->+0x0c ]`, and `DAT_0042d383` builds
  `DAT_004654e0` from every script node whose opcode is `0xe1`, i.e.
  `label id → node`. `0x43c8d2` uses the same index. Hence **every `+0x0c` must
  be a CC label of the scene that is being loaded** — this is the
  self-validating property the port test checks.
* **Observation** `+0x0a` has no writer in the recovered set: `0x72` writes
  `+0x08`, `+0x09`, `+0x0c`, `+0x10`, `+0x14` and the four `s16` rect fields
  only, and the buffer comes from an un-zeroed `malloc(0x18)`. The shipped
  files contain non-zero values there (e.g. `0xE75F`), which is stale heap
  content and must not be interpreted. The port exposes it as
  `uninitialized_0a`.
* **Observation** `0x40c329` consumes `+0x14` (`DAT_004603d4`) and
  `+0x10` (`0x407c5a`), `+0x09`, `+0x08`, `+0x00..+0x07` for hit-testing
  (`0x40c02c`).

### 3.10 Sound table `DAT_00486220` (blocks 19–21, 0x15e slots)

* **Observation** `0x42722b` reads `u32 DAT_0045fd2c`, then 0x15e slots
  (`0x00` present + `0x3c` bytes, `0xff` absent) and then a linked list:
  a marker byte per record (`0xff` terminates) + `0x5c` bytes.
* **Observation** `0x427135` (writer) emits the same three parts, and drops
  records whose `+0x30 == 0` when `0x41d440(2)` (the “detail” gamepref) is off.
* **Observation** `+0x04/+0x08/+0x0c/+0x10/+0x14/+0x18/+0x30` are the fields the
  VM steps (`0x65`, `0xbc`, `0xbf`, `0xc0`, `0xc1`, `0xc2`) address; see
  `analysis/rea/SOUND_TABLE.md` and `SceneSoundState` in `port/include/byzantine/cc.hpp`.
* **Unknown** the remaining bytes of the 0x3c record and the 0x5c chain record
  (`DAT_0046019c`, touched by `0x42722b`/`0x422f37`/`0x439349`).

### 3.11 Media table `DAT_00464f60` (block 22, 0x15e slots)

0x14 bytes per present slot; layout **observed** from `0x42db22` case `0x6a`:

| offset | type | writer | meaning |
|---|---|---|---|
| `+0x00` | `u16` | operand 1 | value |
| `+0x04` | `u32` | `= 0` | frame/offset |
| `+0x08` | `u32` | `timeGetTime()` | wall-clock stamp |
| `+0x0c` | `u32` | operand 2 | media id |
| `+0x10` | `u8` | `= 1`; `0x6c` clears it | active |
| `+0x11` | `u8` | instruction type byte, or 0 | type |
| `+0x12` | `u16` | **no writer** | stale heap bytes |

* **Observation** shipped values confirm the stale tail: `12.BTB` media slot 0 is
  `88 4f 00 00 | ff 0f 00 00 | 98 ac 28 00 | de 00 00 00 | 01 00 | cd cd` — id
  `0xde`, active `1`, then `cd cd` in `+0x12`. `57.btb` has `a5 31` there.
* **Observation** `0x41dcc9` refreshes `+0x08` for every media record, so the
  saved timestamp is not meaningful after a reload (**inference**).

### 3.12 Dialogue chains `DAT_00486a10` (block 24, 0x15e slots)

* **Observation** `0x408dbd`/`0x408d3f`: `0xff` absent, otherwise 0x68 bytes.
* **Observation** `0x407c5a` (kind `0x12/0x13/0x14` hotspot visibility) reads
  `u32` at `+0x64` as a count and then `count` `u32` dialogue ids from `+0x00`;
  each id indexes `DAT_00486f90`.
* **Unknown** the rest of the 0x68 bytes.

### 3.13 Notebook tail (block 25)

* **Observation** `0x42118c` (writer) reads `savegame\notebook.000` line by line
  (`fgets(buf, 0x50)`) and appends `"%s\n"` for every line; `0x4210af` (reader)
  reads until EOF and appends the same text back to `savegame\notebook.000`
  (`fopen("w")`), one `fgetc` per byte until `'\n'`.
* **Observation** the region is therefore line text: no NUL, no control bytes
  other than `\t\r\n`, and when non-empty it ends with `\n`. All 77 shipped
  images satisfy this (sizes 0…3,218 bytes; `NEW.CAN` has 0). The content is
  debugging text such as `Scene: 0\tDialog: 0\n0: 1\n0: 2\n-1: 6\n`, not player
  state.
* **Inference** the parser's strictness here (reject NUL / a missing final
  newline) is what rejects “extra bytes appended to a save”.

---

## 4. `0x40c7b4`: the CD check and the `new.can` special case

```
0x425ab4:
    fread(&v, 4, 1, f);
    if (strcmp(name, "new.can") != 0) {
        if (0x40c7b4(v) != 0) { fclose(f); return 1; }   // abort the load
        DAT_0045da78 = v;
    }
```

* **Observation** the comparison is a plain case-sensitive `_strcmp` against the
  literal `"new.can"` (`0x45ff40`), *not* `_strcmpi`, and it is applied to the
  bare file name the caller passed (`0x41e289` case 3 passes exactly
  `"new.can"`). For that name the image's first `u32` is neither checked nor
  stored — `DAT_0045da78` keeps its previous value. That is the documented
  special case: **`new.can` can be loaded without the disc the save claims**.
* **Observation** `0x40c7b4(v)` is a *disc* check, not a format check:
  `0x40c9ce(v)` compares the wanted number with `DAT_00486a0e` (the current CD
  drive letter); on a miss `0x40c7b4` loops over `0x41d6df(v)`, a modal
  `DialogBoxParamA` (resource `0x72`/`0x73`/`0x71`), and prints
  `"Now accessing CD %d"` when the drive changes. Returning 1 aborts the load.
  `0x40c911` is what enumerates CD-ROM drives (`GetDriveTypeA() == 5`).
* **Observation** `0x424dc0` writes `if (DAT_0045da78 == 1) DAT_0045da78 = 2;`
  before storing it, so the value is never 1 on disk. **Inference**: the field
  is the game-disc number (CD1 is the install disc; the five play discs are
  2…6), which matches the shipped spread (22×`2`, 15×`3`, 13×`4`, 15×`5`,
  12×`6`) and the six-ISO Turkish release.

---

## 5. Validation data and results

Extraction (originals untouched):

```sh
mkdir -p /tmp/opencode/byz_saves && cd /tmp/opencode/byz_saves
7z x -y "Byzantine-The-Betrayal_Win_TR_CD1/Byzantine_1.iso" \
      "copy2hd/savegame/*" "copy2hd/goldsave/*"
```

* **Observation** the CD1 ISO `copy2hd/savegame/` holds 74 numbered saves
  (`00.BTB`–`24.BTB` upper-case, `25.btb`–`73.btb` lower-case), `autosave.btb`,
  `tempsave.btb`, `NEW.CAN`, `BTB.SAV` (592-byte slot list), `STATUS.000`
  (4 bytes), `notebook.000` (2,423 bytes), `GAMEPREF.000/001` (28 bytes each).
* `goldsave/GAMEPREF.000` is seven little-endian dwords `1 1 1 1 1 1 0`
  (already used by the `0xbd` video step).

Four sizes were copied into `analysis/rea/save_sample/` as fixtures:

| file | bytes | SHA-256 |
|---|---|---|
| `00.BTB` | 85,068 | `c86a9ff06c712239a020cce47f19bffa822a263055a8785616de622d8399321a` |
| `12.BTB` | 112,489 | `d59afbe8e3609b2dcf70e2138f4fb987845d1c913822876214713de88718e93a` |
| `26.btb` | 88,515 | `8e2a06ecb6653f363af33dcd9a4e5e7ea86b6df5b3db9d288f44005d976fb293` |
| `73.btb` | 103,333 | `d8cdda5ff99e2541d6c3586842dd947ca55d6f44fd5f3d85c2b847bdd6a3f583` |

`26.btb` (scene `ea`) was added in the restore step so the real-save `0x70`
acceptance can run from the fixture set.

Results with the port parser (`byzantine-cc --save-dump`, and `save-test`):

| check | result |
|---|---|
| all 77 images parse with every block inside its bounds and the remaining tail a valid notebook text region (no NUL, final `\n`) | 77 / 77 |
| distinct file sizes | 75 (79,534 … 119,215) |
| chapter field | `"show"` in 77 / 77 |
| scene index < 44 | 77 / 77 |
| object `+0x0c` in the scene's CC label set | 4,716 / 4,716 object records, 0 unmatched |
| dialogue flag block size | 126 in 77 / 77 (matches the 126 `DAT_00486f90` records) |
| dialogue ids < 126 | all stack entries, max channel depth 20 |
| truncation / appended NUL rejected | yes (`save-test`) |

Distribution across the 77 images: CD 2/3/4/5/6 = 22/15/13/15/12; scenes
`map` 43, `vh` 6, `va` 3, `vb` 3, `hstmed` 3, `am` 2, `clock` 2, `iu` 2,
`mr` 2, `vt` 2, `as`/`bc`/`ea`/`gc`/`hl`/`lintel`/`nb`/`start`/`vc` 1 each
(19 of the 44 chapter scenes appear); 4,716 objects, 51 media records,
808 sound records, 1,363 nodes, 2,447 per-scene records, 24 dialogue chains;
`DAT_00464878` is `0xffffffff` in 56 images and `0` in 21 (the loader compares
it with the value it just read and only calls `0x42dad9` on a difference).

Commands used:

```sh
port/byzantine-cc <save> --save-dump --lcc analysis/rea/engine_sample/copy2hd/engine/SHOW.LCC
port/byzantine-cc <save> --save-dump --lcc <…>/SHOW.LCC --cc port/output/cc_all/<SCENE>
make -C port -j2 all test          # includes ./save-test with the four fixtures
```

---

## 6. Port implementation

* `port/include/byzantine/save.hpp` — `SaveGame`, `SaveObjectRecord`,
  `SaveMediaRecord`, `SaveSoundRecord`, `SaveNodeSlot`, `SaveSceneRecord`,
  `SaveDialogueChain`; all offset comments above.
* `port/src/save.cpp` — `parse_save`, `parse_chapter_scene_list`,
  `save_scene_name`. Every read goes through one bounds-checked reader that
  reports the failing offset; a short read, an unterminated string, an invalid
  `0x15e` marker or a non-text notebook tail throws `std::runtime_error`.
* `byzantine-cc … --save-dump [--lcc <file>] [--cc <scene.cc>]` prints one line
  per block, per present slot and the table occupancy
  (`scene_records=5/350`, `objects=51/350`, …), plus
  `object_labels cc_labels=<n> matched=<m>/<k>` when `--cc` is given.
* `port/tests/save.cpp` (+ `save-test` target) always checks the chapter-list
  parser (lowercasing, 8-character limit, index bounds) and, when the fixture
  directory is supplied, every real image: block sizes, CD range, scene name,
  **object labels against `resolve_control_flow`**, truncation and trailing-byte
  rejection. Without fixtures it prints `real save fixtures skipped`.

Nothing in this step writes a save, reconstructs `0x408adc`'s state, allocates
`DAT_00464300`, or touches `0xcc`/`0x70`.

---

## 7. Observation / inference / unknown summary

**Observation (direct)**

- block order, sizes and marker conventions of §2 — confirmed by the
  reader/writer pair and by all 77 shipped images parsing with their tail a
  valid notebook text region;
- thumbnail = 168×168 RGB16, `0xdc80` bytes, skipped by the loader;
- object `+0x0c` = CC label, `+0x14` = `0xcc` argument, `+0x08` = active,
  `+0x09` = kind, rect at `+0x00..+0x07` (from `0x42db22` case `0x72`);
- media `+0x10` = active, `+0x0c` = id, `+0x08` = `timeGetTime()`
  (from `0x42db22` case `0x6a`, `0x6c`);
- dialogue channels = 10, stack depth = 350, 126 records;
- `+0x0a` (objects) and `+0x12` (media) have no writer;
- node extra list: writer emits a count, reader consumes control bytes.

**Inference (marked, not proven by the binary)**

- the first `u32` is the game-disc number (from `0x424dc0`'s `1 → 2` and the
  drive-letter check);
- the scene index is the chapter-list position of the active scene
  (validated indirectly by the label test);
- `DAT_0045c060` selects block 16 vs 16′ (panorama mode);
- the notebook tail is debug text and not game state;
- a media timestamp is meaningless after reload.

**Unknown (explicitly not recovered)**

- `DAT_0045f210`, `DAT_00460668`, `DAT_0046066c`, `DAT_0045f214`,
  `DAT_0048253e`, `DAT_0045fd2c`, `DAT_0047e2f0` value semantics;
- the `0x40`-byte node record fields beyond the `0x40ffee` initialiser;
- the bulk of the 0x3c sound record and the 0x5c sound-chain record;
- the `0x32`-byte arrays and list ids of the per-scene `0x1ea4` record;
- the body of the 0x68 dialogue-chain record beyond `+0x64`/ids;
- why `0x4269ec`/`0x426b9d` disagree on the node extra list, and whether any
  save ever contained a non-empty list;
- `0x40df05(0x1f)`, `0x40105d`, `0x41316f`/`0x4131a1`/`0x41393c`/`0x41399a`/
  `0x4139f8`/`0x413a59` (pre/post-load resets), which run around the file read
  but were not interpreted.

---

## 8. Deliberate anomalies recorded for the next step

1. `0x425ab4` frees and reallocates `DAT_00464300[i]`/`DAT_00486220[i]`/
   `DAT_00486a10[i]`/`DAT_00464f60[i]` only through `0x40f3f0`, called *before*
   the scene tables are read (`0x425ab4`: `0x40f3f0`, `0x40f4e7`, then the scene
   `.cc` open). The port keeps the raw records so the next step can populate
   `SceneState::objects` without re-reading the file.
2. `0x408adc` ends with `0x407420(0)`, a redraw that touches `DAT_0045d1b4`,
   `DAT_00486f90`, `DAT_00468cc0`, `DAT_00468d78` and `DAT_0045d1ac` — those
   consumers must exist before a restore can be honest.
3. `0x425ab4` only sets `_DAT_00485c70 = 1` and `DAT_004859cd = 1` when the load
   succeeded with `local_8 == 0`; a partially failing load is not rolled back.

---

## 9. Verified export hashes

102 exported functions were re-hashed against `pseudocode/decompile-index.json`
(`sha256` recomputed from each `relative_path` on 25 October 2026); all matched,
and all belong to analysis profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`:

```
0x401000 0x40105d 0x401074 0x40114a 0x401184 0x4013ae 0x401ca6 0x401ee6
0x402285 0x407254 0x407420 0x407527 0x4075fe 0x4077b2 0x407ad5 0x407c5a
0x4083d4 0x4087c1 0x40892d 0x4089b0 0x408a1e 0x408adc 0x408bb2 0x408c81
0x408d3f 0x408dbd 0x40c02c 0x40c329 0x40c7b4 0x40c911 0x40c9ce 0x40cf73
0x40d031 0x40d750 0x40d9f7 0x40dae5 0x40df05 0x40f3b0 0x40f3f0 0x40f4e7
0x40fd7b 0x40ffee 0x410669 0x4117a6 0x4124d9 0x412d35 0x412f77 0x41316f
0x4131a1 0x41393c 0x41399a 0x4139f8 0x413a59 0x416ec0 0x417054 0x417491
0x4177c7 0x417a17 0x417c33 0x417e9c 0x41d6df 0x41dcc9 0x41dd22 0x41df43
0x41e289 0x41efc1 0x4207ef 0x420888 0x4209d2 0x4210af 0x42118c 0x421d5e
0x422770 0x424dc0 0x42515f 0x425218 0x425ab4 0x426261 0x42658f 0x426662
0x426788 0x426892 0x426935 0x4269c6 0x4269ec 0x426b9d 0x426ed0 0x427135
0x42722b 0x42c8a0 0x42d383 0x42d52f 0x42d594 0x42dad9 0x42db22 0x43340a
0x436b01 0x437cad 0x437f76 0x438173 0x438cf9 0x438d78
```

The format core is `0x425ab4`/`0x424dc0` (reader/writer), the read or write
half of each block pair (`0x426261`/`0x425218`, `0x426b9d`/`0x4269ec`,
`0x42722b`/`0x427135`, `0x408adc`/`0x408a1e`, `0x4089b0`/`0x40892d`,
`0x408dbd`/`0x408d3f`, `0x4210af`/`0x42118c`, `0x420888`/`0x4209d2`) plus the
`.lcc` reader (`0x437cad`/`0x437f76`/`0x438173`) and the CD check
(`0x40c7b4`/`0x40c9ce`/`0x41d6df`/`0x40c911`). The remaining addresses are the
consumers quoted in §3 to justify field meanings (`+0x0c`/`+0x14` of the object
record, the dialogue depths, the mid-block globals).

One additional address was read from the PE directly rather than from the
export set: the `objdump` range `0x426d40–0x426f00` (see §3.8), because the
decompiled body of `0x426b9d` drops the loop update.

---

## 10. Next step

The table restore, `0xcc` and `0x70` are implemented in `SAVE_RESTORE.md`; the
carried variable pool and the dialogue window state in `SAVE_VARIABLES.md`;
`0xd1`/`0x6a`/`0xb4`/`0xb5`/`0xbb`/`0x73` in `SOUND_MEDIA_STATE.md`; the
dialogue chains, node lists and save/status requests in `CHAIN_SAVE_STATE.md`.
Still outside the port: the per-scene records (`DAT_00482ad0`), the sound chain,
the `DAT_0047dd70` animation data, and actually writing the save/status files.
