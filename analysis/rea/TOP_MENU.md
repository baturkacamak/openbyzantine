# Top bar and its menu screens

Status: implemented and verified on the shipped data (27 October 2026).
Target `BETRAYAL.EXE` SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
(analysis profile `e1ad533d…`, Ghidra export set
`analysis/rea/pseudocode/functions`).

## 1. Reported symptom

Pinned run `run-20261027T170925.613868Z-lu9b3p1r` (`MR` label 0,
`30.btb`, normal window) delivered three clicks in the top 20-pixel band. The
player answered:

```
ui_click top_bar x=35 y=8 consumed (top menu is not modelled)
ui_click top_bar x=5 y=8 consumed (top menu is not modelled)
ui_click top_bar x=26 y=6 consumed (top menu is not modelled)
```

The band was not drawn and every click was swallowed. This report covers the
real bar, its hover/pressed states, the four destination screens and their
actions.

## 2. Original behavior (addresses and digests)

| function | digest (first 16) | role |
| --- | --- | --- |
| `0x408e3f` | `2079ca547af7db29` | loads `setupart/top_bar.tga`, `setupart/hotbar.rle` (5), `setupart/dwnbar.rle` (5) |
| `0x408ee2` | `aa5c0d45e3e46a5e` | frees the bar and both RLE sets |
| `0x408f32` | `079dc12256352f9f` | `param 1`: blit `top_bar.tga` at (0,0) 640x20; `param 0`: clear the same rectangle; resets the hover index |
| `0x408fe7` | `8ebe38773884234e` | hover; boundaries `0, 0xb5, 0x144, 0x1f4, 0x280`; index 1..4; draws base + highlighted frame; cursor `0x1c` |
| `0x4090c4` | `d6cbfcea1d6b4c53` | frame index <5 = HOTBAR, `index-5` = DWNBAR; uses each frame's own x/y |
| `0x4091e5` | `e0b2de5170bd36d9` | click flash: DWNBAR frame, `Sleep(100)`, HOTBAR frame |
| `0x40c02c` | `ef06dfae27973bb1` | `y < 0x14` returns `0xfffc` (top band) |
| `0x40c329` | `5a9e9344f5661475` | first entry draws the bar (`0x408f32(1)`), later entries run `0x408fe7`; leaving clears it (`0x408f32(0)`) |
| `0x40d750` | `ad0e815e270816d2` | `DAT_0045d1d4 != -1` -> click event 3 |
| `0x43c8d2` | `09a9db4ea6321c2e` | case 3: `0x4091e5` flash, save the script pointer, end the scene loop |
| `0x41de4b` | `6a7d8fe78b3a8317` | button 1/2/3/4 -> screen mode 1/3/2/0 |
| `0x41e289` | `a89ac0f7d4449865` | the screen loop |
| `0x41f9ea` | `17a8a34d1bfe9df3` | screen art: mode 0 `preffs.jpg`, 1/2 `setup00.jpg`, 3/4/5 `s_note00/01/02.jpg` |
| `0x41fe46` | `dc26e809348f0e7c` | static control enable sets |
| `0x42044d` | `8774f29932bc832c` | save-slot rows and page arrows |
| `0x4202ff` | `7d67a1cf93101af7` | save slot text: centred x=0x1de, y=0x42+0x1e*i, font slot 2 |
| `0x421ffc` | `02140f3efbe34e27` | KEŞİF list: centred x=0x1d0, y=0x55+0x28*i, font slot 2 |
| `0x422101` | `9fea29eb3e858d99` | KEŞİF row hit and redraw |
| `0x420b99` | `b6afc2850ab29651` | control 0x24's bottom = `0x4b + (status-0x33)*0x28` |
| `0x4273f4` | `9aa6891fe5b5a62d` | `savegame/status.000` dword; missing = `0x34` |
| `0x41d7ff` | `02c165ff8a68dbf2` | loads ROLL/on_off/yellow RLE and MESSAGES.TXT contact/name rows |
| `0x41f065` | `2e7ae322149f960f` | slot list: `autosave.btb` then every `btb.sav` line |
| `0x41f41c` | `845fd8b0291ec946` | reads each save's disc, scene and description |
| `0x41effe` | `992805e8e898aae8` | first free `NN.btb` (save with no slot selected) |
| `0x41f753` | `16b8e272bc7f9d37` | reads `savegame/gamepref_%03d` (7 dwords) |
| `0x41f7be` | `a5a47ec29a1bc509` | writes `savegame/gamepref_000` (7 dwords) |
| `0x41fc87` | `6e82e9b04ffd2390` | plays `avi/<name>.avi` through `0x40d031` |
| `0x41fcf3` | `64340d1fab433d56` | credits screen (`setupart/credbk.jpg`) |

### 2.1 The bar is a roll-over UI

`0x40c329` is called every scene iteration. When the mouse is in rows
`0..0x13`, `0x40c02c` returns `0xfffc`; the first such call blits the whole bar
and later calls only update the hovered button. Moving the pointer back into
the playfield calls `0x408f32(0)`, which clears the band. This is why the bar
looks absent in normal play until the pointer reaches the top edge.

`0x408fe7` counts how many of the five x boundaries are `<= x`; the result is
`1..4` (`DAT_0045d1d4`). `0x4090c4(index,1)` draws `HOTBAR.RLE[index]` at the
frame's own `+0x06/+0x08` origin (182,0 / 325,0 / 501,0 for the other buttons).
`0x4091e5` flashes `DWNBAR.RLE[index]` for 100 ms before reopening the HOTBAR
image. Clicking when `DAT_0045d1d4 != -1` makes `0x40d750` report event 3;
`0x43c8d2` case 3 finishes the flash, stores the script pointer and ends the
scene loop.

### 2.2 The four screens

`0x41de4b` maps the stored button to `0x41e289`'s mode:

```
button 1 (YÜKLE/KAYDET) -> mode 1 -> setup00.jpg, save/load screen
button 2 (NOTLAR)       -> mode 3 -> s_note00.jpg, notes page
button 3 (KEŞİF)        -> mode 2 -> setup00.jpg, movie/simulation replay list
button 4 (TERCİHLER)    -> mode 0 -> preffs.jpg, seven preferences
```

`0x41fe46` enables the controls of the 0x28-rect table at `0x45f548`
(16 bytes each: left, top, right, bottom):

```
mode 0 (prefs)      30,35,4,5,6..12
mode 1 (save/load)  0,3,4,5,14,16,17,39 + 0x12..0x1b (existing slots)
                    + rows 1/2 when a slot is selected, row 5 then disabled
mode 2 (keşif)      4,5,14,15,17,30,36,39
mode 3 (NOTLAR)     4,5,14,30,31
mode 4 (TEMAS)      14,30,32,33,34
mode 5 (spread)     14,29,30,33
```

The left-page menu rows are 0..5; their bullets are `on_off.rle` frames 0..5
and are drawn only for enabled rows (`0x41f98e`). Preferences are rows 6..12;
when a stored value is 0 the corresponding `on_off.rle` frame 6+i is drawn
(`0x41fe46` case 0, `0x41f94e`), otherwise the baked art shows the enabled word.

Save/load screen: `0x41f065` builds the slot list as `autosave.btb` followed by
the lines of `savegame/btb.sav`; `0x41f41c` reads each image's description. The
ten rows are drawn centred at x=0x1de, y=0x42+0x1e*i. Selecting a slot enables
OYUN YÜKLE/OYUN İPTAL and disables OYUNA DEVAM (`0x42044d`). `0x1c`/`0x1d` are
the next/previous page arrows.

KEŞİF screen: `0x421ffc` draws `MESSAGES.TXT` rows 0x34..0x3a centred at
x=0x1d0, y=0x55+0x28*i, limited by `status.000` (default 0x34). The list rows
are: credits, intro movie, the four simulation scenes, epilogue (see
`0x41e289` case 0x24):

```
0x34 Hazırlayanlar          -> 0x41fcf3 (credbk.jpg)
0x35 Giriş filmi tekrar     -> 0x41fc87("mintro")
0x36 Topkapı Simülasyonu    -> scene vtdisc
0x37 Yerebatan Simülasyonu  -> scene vbdisc
0x38 Efes Simülasyonu       -> scene vcdisc
0x39 Ayasofya Simülasyonu   -> scene vhdisc
0x3a Epilog tekrar          -> 0x41fc87("mepilog")
```

Preferences: `0x41f753(0)` loads `savegame/gamepref_000` into seven dwords;
`0x41f7be` writes them back. The screen toggles a value and redraws the
`on_off.rle` overlay. The port keeps the same file name and layout, and
`scene.sh` now prefers `savegame/gamepref_000` over the CD's
`GAMEPREF.000`, so a toggle survives the next launch.

## 3. Port implementation

New files:

- `port/include/byzantine/menu.hpp`, `port/src/menu.cpp`: the bar boundaries,
  `0x41de4b` mapping, all 0x28 control rectangles, the `FUN_0041fe46` /
  `0x42044d` enable rules, the save-row and KEŞİF-row hit tests.
- `port/tests/menu.cpp` (`menu-test`): boundary cases, mode mapping, rectangle
  spot checks, enable sets, page/row hit tests.

Player changes (`port/src/play_main.cpp`):

- `Display` loads `setupart/TOP_BAR.TGA`, `HOTBAR.RLE`, `DWNBAR.RLE`,
  `on_off.RLE` (the TGA is a full-screen mask whose unused area is the magenta
  key; only the bar's covered pixels are blitted) and draws them in
  `build_screen` after the playfield/strip (`0x408f32`).
- Mouse motion in rows 0..0x13 sets the visible bar and the hovered index and
  repaints; leaving clears it (`0x40c329`/`0x408fe7`). The top-band pointer is
  the game's `hotsmall` cursor (`0x1c` -> `0x1e`).
- A click in the band draws the DWNBAR frame, waits 100 ms and opens
  `MenuUi::open_screen(top_bar_menu_mode(button))` (`0x4091e5`).
- The menu draws the shipped JPEG for the mode, the enabled-row bullets, the
  preference overlays, the save-slot list, the KEŞİF list, a delete/quit
  confirmation box (real MESSAGES.TXT lines 9/11 and EVET/HAYIR 0x4c/0x4d) and
  a notice box for unimplemented actions. The list text uses font slot 2
  (`fonts/setup.sfn` + `setup.tga`), the same slot `0x4202ff`/`0x421ffc` ask for.
- Actions:
  - OYUNA DEVAM (5) closes the screen.
  - OYUNDAN ÇIK (4) asks line 9, then ends the session.
  - OYUN YÜKLE (1) parses the selected `NN.BTB` and reloads that scene through
    the same travel path, with the save's variable pool and inventory. The entry
    starts at instruction 0 because the port does not model the saved script
    pointer (`0x426935`); the save's scene index, state and media are real.
  - YENİ OYUN (3) loads `new.can` the same way.
  - OYUN İPTAL (2) asks line 11 and removes the selected save, then refreshes
    the list (50 -> 49 in the test).
  - OYUN KAYIT (0) reports that the writer `0x424dc0` is not reconstructed.
  - Preferences 6..12 flip the dword, redraw the off overlay and write
    `savegame/gamepref_000`.
  - Tabs 0x0e/0x0f/0x10/0x11/0x1e/0x21 and page areas 0x1f/0x20/0x22/0x23
    switch screens exactly as `0x41e289`'s switch does.
  - KEŞİF 0x24: the four simulation rows travel to the named scenes and follow
    their entry `0x73` chain (`vtdisc -> vt`, observed in the test); credits
    show `CREDBK.JPG`; intro/epilogue use the existing FFmpeg -> JPEG/WAV clip
    pipeline (`play_decoded_clip`, 2x 640x400 at row 0x14).
- Diagnostics: `--menu <0..5>`, `--menu-click-at <x>,<y>` (repeatable),
  `--menu-shot <ppm>` and `--bar-show <0..4>` (headless bar state). They drive
  the same `MenuUi` the window uses.

## 4. Verification

- `make -C port -j2 all test` passes all native suites, `tests/logging.py` and
  the new `menu-test`.
- Real-data regressions still pass: `verify_ea_layer.py`, `verify_ea_photo.py`,
  `verify_ea_single_execution.py`, `verify_mr_arrival.py`.
- Headless screenshots (`port/output/top-menu/`):
  `bar-hover-loadsave.png`, `bar-hover-kesif.png` (the bar with the bright
  HOTBAR frame), `save-load.png`, `save-load-page2.png`, `preferences.png`,
  `preferences-auto-off.png` (the "kapat" overlay after toggling), `kesif.png`,
  `credits.png`, `notes.png`, `quit-confirm.png`,
  `save-not-reconstructed.png`.
- Real SDL window on an isolated Xephyr `:8` (1280x960, 2x logical 640x480):
  `window-save-load.png` (slot 1 selected, OYUNA DEVAM correctly disabled) and
  `window-loaded-vh.png` after OYUN YÜKLE switched `MR` to `VH`
  (`travel index=39 scene=vh … save=…/25.btb`, 6 hotspots).
- Selected log lines from the deterministic runs:

```
menu_open mode=1 name=save_load page=0 slots=50
menu_slot index=1 file=25.btb description=diadem selected=yes
menu_click x=140 y=305 control=1 mode=1
menu_load slot=1 file=25.btb description=diadem scene_index=39 reason=oyun_yukle
travel index=39 scene=vh from=MR ... save=/tmp/byz-scene/copy2hd/savegame/25.btb
menu_load committed scene=VH hotspots=6

menu_new_game file=NEW.CAN scene_index=0
travel index=0 scene=start from=MR ... save=.../NEW.CAN

menu_pref index=0 value=0
menu_prefs_write file=".../copy2hd/savegame/gamepref_000"   (dword 0 becomes 0)

menu_page page=1                                           (0x1c next page)
menu_delete slot=0 file=autosave.btb removed=yes           (after EVET)
menu_kesif_click index=54 text=Topkapı Simülasyonu tekrar
menu_kesif scene=vtdisc index=43
travel index=42 scene=vt from=VTDISC ...                   (entry 0x73 chain)
```

A live Xephyr run with real pointer events (the host pointer was inside the
window) also produced `ui_click top_bar x=225 y=11 button=2` ->
`menu_open mode=3 name=notes` -> OYUNA DEVAM (`control=5`) -> `ui_click top_bar
x=357 y=7 button=3` -> `menu_open mode=2 name=kesif`, which is the reported
interaction repaired end to end.

## 5. Not implemented / differences

- **Save writer (`0x424dc0`)**. OYUN KAYIT only reports the gap; the port still
  cannot write a valid 25-block `.btb`. Nothing is silently half-saved.
- **Save script pointer.** A load enters the scene at instruction 0 with the
  save's state; the original restarts at the saved pointer (`0x426935`).
- **Cross-disc animation overlay.** The launcher computes
  `--anims-overlay` for the starting save's disc. Loading a save from another
  disc keeps that overlay and logs `menu_load anims_overlay_disc_mismatch`.
- **Credits scroll.** `CREDBK.JPG` is shown; `0x422500`'s scroll is not.
- **TEMAS LİSTESİ text.** The shipped page art is drawn, but the contact names
  (`0x420c51` + `0x41d7ff`'s MESSAGES rows / notebook.000 dialogue ids) are not
  reconstructed. The notebook pages 1/2 are likewise art-only.
- **Hover highlight art.** ROLL.RLE (15 frames) and yellow.rle (4 frames) are
  loaded by `0x41d7ff` for `0x41f90e`/`0x420587`; the port changes the pointer
  and logs `menu_hover`, but does not draw those frames yet.
- **İPUCU (`0x41e002`)** is reported as not implemented from its tab.
- **Movie placement** always uses mode 1's 2x 640x400 (gamepref[1] on); the
  small-window branch is not selected from the menu. The movies themselves use
  the existing decoded-clip path; a full 70 MB intro was not replayed here.
- The menu's confirmation boxes and the notice box are port layouts; their
  messages, labels and button ids are the shipped ones.
