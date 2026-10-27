# The game's own mouse cursors (0x40dd7f / 0x40dae5 / 0x40df05)

Status 27 October 2026. Scope: the shipped pointer art and the engine's cursor
selection are implemented in the portable player; the item pointer and the
"item in hand" pointer from the inventory strip now use the same tables.

Target: `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226` (matches
`analysis/rea/pseudocode/decompile-index.json`). Pseudocode used (export hashes
from the same index):

| Address | Function | Export SHA-256 | Role |
| --- | --- | --- | --- |
| `0x40dd7f` | `FUN_0040dd7f` | `80918ff9b340674db9ed00e25ac51ff0b51afbec6d206c4cabadf7e2464c9144` | creates the mouse surfaces, calls `0x40dae5(-1)` |
| `0x40dae5` | `FUN_0040dae5` | `f526a87918f70be0434a4ca4641cd834956f5d9b8a68c3bde6ffb3eb1f3cbd06` | loads the 32 cursors of a theme |
| `0x40df05` | `FUN_0040df05` | `af8620974c3f23d77094273ce5cf256c7cefcbe05759750390118f61a9f33d36` | installs a cursor (table entry or inventory record) |
| `0x40de7b` | `FUN_0040de7b` | `83f72acb1467eeaa2b188b64dcdd9bc582afd3c0a76cd0d0a6ae4baa07be75bb` | small-variant remap |
| `0x40c329` | `FUN_0040c329` | `5a9e9344f5661475134cfcfc8f708404831951a0bc006244f506ddeabcfa7491` | hover dispatcher |
| `0x4083d4` | `FUN_004083d4` | `1ef8d768ac8fe1c444758842dd80d611d7396ddd738dc464917bd93bb215e32b` | inventory-strip hover |
| `0x407bd7` | `FUN_00407bd7` | `38535568f84779e01748f34db4b9559e4f4e388891b5bbc0297811ba26f91cb5` | held item -> cursor id (and +1 = hotitem) |
| `0x407254` | `FUN_00407254` | `1e9277e1eebf04aa40d16596c5e32bc70d225fbd4abb640be1348fb14af32fc8` | loads `inventor/<name>.tga` + `inventor/hotitems/<h-name>.tga` |
| `0x4013ae` | `FUN_004013ae` | `94c3c1e778b6c0a3b194c640c95849023d25508c2bade566095cb30dbbc843e7` | panorama edge bands (cursor ids 0x18..0x1b) — rotation still not modelled |
| `0x40e89f` | `FUN_0040e89f` | `25a7d1dbb5d15320299dcc495964ce0bed54d7bd492181547fd02d4d927b2d82` | resolves the `%s*` wildcard to up to four files |

No new whole-program decompilation was run for this step; the behaviour above is
read from the saved exports. The cursor **data** was read directly from the
target's `.data` section (see below), because the two tables are static data that
the pseudocode only references by address.

## 1. The two tables

`0x40dae5(param)` selects the table and the file pattern:

* `param == -1` -> table `0x45dbe8`, `sprintf("cursors\\%s*.tga")`
* anything else -> table `0x45e090`, `sprintf("cursors\\klio\\%s*.tga")`

`0x42dad9(mode)` is the only caller that passes a real mode (from `0x68`, the
alternate-mode selector `DAT_00464878`); `0x40dd7f` loads the normal set at
startup with `-1`. So the klio set is the **alternate mode** art, and the normal
set is the default.

Each entry is `0x24` bytes: name pointer `+0x00`, hotspot `+0x04`/`+0x08`,
current frame `+0x0c`, frame count `+0x10`, up to four frame objects
`+0x14..+0x20`. Dumped from the file (virtual addresses mapped through the PE
section table):

```
id  normal name  hotspot   klio name   hotspot
 0  default      (24,24)   default     (24,24)
 1  forward      (14,14)   forward     (20,20)
 2  back         (13, 8)   back        (24,24)
 3  -            -         -           -
 4  -            -         -           -
 5  examup       (16,12)   examup      (14,14)
 6  -            -         -           -
 7  -            -         -           -
 8  forward      (14,14)   forward     (20,20)
 9  examdwn      (16,24)   examdwn     (14,14)
10  talk         (16,16)   -           (16,16)
11  exit         (14,18)   exit        (24,24)
12  magnify      (15,14)   magnify     (14,14)
13  take         (19,12)   take        ( 6, 2)
14  manplte      ( 7,13)   manplte     ( 6, 0)
15  mouseoff     ( 0, 0)   mouseoff    ( 0, 0)
16  video        (16,16)   -           (16,16)
17  timer        (16,16)   timer       (11,13)
18  apply        (11,11)   apply       (24,24)
19  apply        (11,11)   apply       (24,24)
20  talk         (16,16)   -           (16,16)
21  info         (24,20)   info        (19,19)
22  diminfo      (24,20)   diminfo     (19,19)
23  default      (24,24)   default     (24,24)
24  up           (11,24)   up          (24,24)
25  down         (11,11)   down        (24,24)
26  left         (24,12)   left        (24,24)
27  right        (11,12)   right       (24,24)
28  hotbig       (24,24)   hotbig      (24,24)
29  map          (11,11)   map         (11,11)
30  hotsmall     (11,11)   hotsmall    (11,11)
31  sload        (16,16)   sload       (16,16)
```

`-` entries have a NULL name pointer in the shipped table (slots 3/4/6/7 in
both sets, and 10/16/20 in the klio set). `0x40df05` would dereference their
empty frame pointers, so the port never installs them.

The art on CD1 (`copy2hd/cursors`, 25 files + 32 klio files) matches these
names and hotspots; the measured sizes confirm the hotspot fields are image
coordinates, e.g. `DEFAULT.TGA` 48x48 hotspot (24,24), `TAKE.TGA` 32x32
(19,12), `MAGNIFY.TGA` 34x37 (15,14), `MAP.TGA` 24x24 (11,11),
`diminfo.tga` 50x40 (24,20), klio `DEFAULT.TGA` 49x49 (24,24), klio
`TAKE.TGA` 20x30 (6,2). `TEMP.TGA` is on the CD but is not referenced by either
table. The extracted dimensions are listed in `port/output/cursors/cursors.txt`
together with 2x PNG previews of the normal set.

`0x40e89f` expands `%s*` (a wildcard, not a literal `*`): the klio set stores
`forward0/forward/forward1/forward2`, `back1..back4` and `INFO1..INFO4` as
multi-frame animation sets, cycled every 250 ms by the `0x40ebe1` thread.

## 2. Which pointer is installed

`0x40c329` is called every frame with the object under the mouse (`0x40c02c`
returns the slot or `0xfffc` top bar / `0xfffd` strip / `0xfffe` nothing):

```
if (no object)     cursor = 0
else:
  kind = object[+0x09]
  compatible = 0x407c5a(object[+0x10], kind)
  if (DAT_0045d1c8 != 0) {                       // an item is held
    if (!compatible)                          cursor = item id
    else if (kind == 0x12 || 0x13 || 0x14)    cursor = item id + 1   // hotitem
    else if (kind == 0x00)                    cursor = item id
    else                                      cursor = kind
  } else                                      cursor = kind
```

So the object's **kind byte is the cursor id**: kind `0x0a` talk, `0x0b` exit,
`0x0c` magnify, `0x0d` take, `0x0e` manplte, `0x12/0x13/0x14` the item-use
kinds, `0x18..0x1b` the panorama arrows, `0x1c` hotbig, `0x1e` hotsmall, etc.
The item ids are the `SHOW.LCI` record indices; `0x407bd7(0)` returns the
record's id (the port resolves it to `inventor/<name>.tga`) and `0x407bd7(1)` is
that id plus one, which `0x40df05` maps through `DAT_00486f90[id/2]` to the
record's `+0x14` sprite, i.e. `inventor/hotitems/<h-name>.tga` (`0x407254`
truncates to eight characters and replaces the first with `h`).

`0x4083d4` is the strip (`y > 0x1a3`, strip visible):

* no item held:
  * over the next arrow `x 0x23b..0x259, y 0x1b1..0x1cb` while `scroll < count-6`
    -> `0x40de7b(0x1c)` (hotbig)
  * over the previous arrow `x 0x25d..0x27b` while `scroll > 0` -> hotbig
  * over a slot whose item is not `DAT_0045d1cc` (the examined item) -> hotbig
  * otherwise 0
* item held: the item pointer, except over the channel area `x 0x46..0x8c` or
  the examine button `x 2..0x3a, y 0x1a8..0x1dc` **and** while the item is on
  the current page, where `0x407bd7(1)` selects the hotitem art.

`0x40de7b` then rewrites some ids **only for the normal table and only outside a
panorama** (`DAT_0045c060 == 0`): `0`, `0x17` -> `0x1d` ("map") and `0x1c` ->
`0x1e` ("hotsmall"). That is the small crosshair used in flat close-ups and the
inventory strip instead of the big 48x48 pointer.

`0x40e29f` clips the pointer at the 640x480 surface edges; the port installs a
real SDL colour cursor, so the host clips it and the composed frame keeps its
pixels (no cursor is baked into screenshots).

## 3. Port implementation

* `port/include/byzantine/cursors.hpp`, `port/src/cursors.cpp`: both tables,
  `cursor_theme`, `cursor_slot` (the `0x40de7b` remap), `hover_cursor`
  (`0x40c329`), `strip_cursor` (`0x4083d4`), `hotitem_media_name` (`0x407254`)
  and `find_cursor_file` (`0x40e89f`'s wildcard: exact stem first, otherwise the
  alphabetically first `.tga` with that stem prefix; the number of matched files
  is reported).
* `port/src/play_main.cpp`: the `Display` installs the pointer through
  `SDL_CreateColorCursor` with the table hotspot (items are centred, like
  `0x40df05`'s `mouse - (width >> 1)`). `update_cursor_at` applies the three
  regions (`y < 0x14` top band = default, `0x14..0x1a3` playfield =
  `hover_cursor`, `>= 0x1a4` strip = `strip_cursor`). It runs on every
  `SDL_MOUSEMOTION` and on every redraw (`present()`), so a click that changes
  the held item or the scene updates the pointer immediately.
* `port/scene.sh` extracts `copy2hd/cursors/*` (CD1 only) into
  `<data>/cursors/` and passes `--cursors`; the player falls back to
  `<backgrounds>/cursors`. A `.cursors_v1` marker backfills older data roots.
* Diagnostics: `--cursors <dir>`, `--cursor-at <x>,<y>` (repeatable; installs
  and prints the pointer for that logical point) and the `cursor …` /
  `cursor_missing …` / `cursor_frames …` lines. A missing cursor-table file is
  reported and the system arrow is used; a missing *item* TGA falls back to the
  default table pointer (an inventory id is never read as a cursor id). No art
  is invented.
* `port/tests/cursors.cpp` (`cursor-test`, part of `make -C port test`): both
  tables and hotspots, the theme rule, the `0x40de7b` remap, every `0x40c329`
  branch (no object, kind cursor, held item, incompatible use, hotitem over
  `0x12`/`0x13`, talk hotspot), the `0x4083d4` strip rules (hidden strip,
  slots, examined item, arrows at the scroll bounds, examine/channel hotitem)
  and the cursor-file wildcard lookup.

## 4. Verification (real data, 27 October 2026)

Data root `/tmp/byz-scene` prepared by `port/scene.sh` from the six shipped
ISOs; `make -C port -j2 all test` passes (14 native suites + logging + the new
`cursor-test`).

Headless (`SDL_VIDEODRIVER=dummy`, real save `26.btb` / `70.btb` / `43.btb`):

```
EA 602 / 26.btb  --cursor-at 282,319   cursor table id=12 name=magnify  size=34x37 hot=15,14
EA 602 / 26.btb  --cursor-at 300,200   cursor table id=0  name=default  size=48x48 hot=24,24
EA 602 / 26.btb  --cursor-at 170,430   cursor table id=28 name=hotbig   size=48x48 hot=24,24
EA 602 / 26.btb  --cursor-at 30,460    cursor table id=0  name=default  (examine button, no item)
EA 602 / 26.btb  --cursor-at 596,446   cursor table id=28 name=hotbig   (next arrow, scrollable)
EA examine ERIDNOTE (flat, normal set)
                 --cursor-at 300,300   cursor table id=29 name=map      size=24x24 hot=11,11
                 --cursor-at 230,430   cursor table id=30 name=hotsmall size=24x24 hot=11,11
CLOCK 0 / 70.btb (klio set, bar page 2 = ISTAR)
                 --bar-select 21 --cursor-at 388,369
                                       cursor item name=hSTAR variant=hotitem
                 --cursor-at 202,378   cursor item name=ISTAR variant=normal
                 --cursor-at 170,430   cursor item name=ISTAR variant=normal
                 --cursor-at 30,460    cursor item name=hSTAR variant=hotitem
HSTMED 0 / 43.btb (flat, klio set)
                 --cursor-at 170,300   cursor table id=0 name=default theme=klio size=49x49
                 --cursor-at 170,430   cursor table id=28 name=hotbig theme=klio size=49x49
CLOCK with inventor/ hidden (missing item art)
                 --cursor-at 388,369   ui_asset_missing name=inventor/hotitems/hSTAR.tga
                                       cursor table id=0 name=default theme=klio
                                       (falls back to the default pointer)
```

The `0x40e89f` wildcard itself is covered by `cursor-test` (a scratch directory
with `INFO1.TGA`/`INFO2.TGA` and an `infox.jpg`: two `.tga` frames, first file
shown, exact stem beaten by an exact match, missing directory reported). No
shipped scene in this data root activates a kind that uses those animated klio
entries, so `cursor_frames` was not produced by real gameplay here.

Interactive window (Xephyr `:9`, 1280x960, real X pointer motion into a
1280x960 window, `--no-audio`): moving the pointer over EA's visible hotspot
(playfield 282,319) logged `cursor table id=12 name=magnify …` and moving off it
logged `cursor table id=0 name=default …`, i.e. the SDL motion path installs the
game's art, not only the diagnostic path. The X server delivered the motion; a
physical-mouse/speaker check is not claimed.

The existing real-data regressions still pass with the new pointer paths:
`verify_ea_layer.py`, `verify_ea_photo.py`, `verify_ea_single_execution.py`,
`verify_examine_controls.py` (all `/tmp/byz-scene`).

## 5. Bounds (explicitly not claimed)

* Multi-frame cursors are not animated: klio's `forward*`, `back*` and `INFO*`
  sets show the first file and log `cursor_frames … files=N`. `0x40ebe1`
  cycles them every 250 ms through the `0x40df05` thread.
* `0x4013ae`'s panorama edge bands are not implemented (the port turns the view
  by dragging, arrow keys and the wheel). Their cursor ids `0x18..0x1b` and art
  are present and tested, but no edge-band pointer is installed because the
  matching auto-rotation does not exist yet.
* The top 0x14-row menu is not modelled, so a pointer over it stays the default.
* The port has no page/channel tabs, so a strip channel switch cannot be
  exercised interactively; `strip_cursor` uses `inventory_page()` and therefore
  follows whatever channel the state has.
* Kinds above the 0x20 table (not produced by the shipped `0x72`/`0x95` writes)
  keep the default pointer instead of indexing an inventory record.
* A klio table slot with NULL art (talk/video) falls back to the normal set's
  entry for the same id, and the fallback is logged; the original would
  dereference a NULL frame pointer.
* `0x40e29f`'s edge clipping is the host's job (SDL colour cursor); the exact
  original blit is not reproduced pixel-for-pixel.
