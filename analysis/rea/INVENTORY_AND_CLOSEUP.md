# Common inventory strip, item close-up and cursor camera

Date: 26 October 2026.

This step follows the AM conversation's normal return: the player must be able
to continue with the real mouse, not only watch the conversation end. It adds
the original 640x480 surface, the bottom inventory strip, item selection and
examine close-up, the close-up return snapshot, and vertical mouse look. It does
not add a scene or a scene-specific patch.

Evidence uses the shipped saves and assets:

- AM `62.btb` (`Inuam3a` conversation, item 11 `ibeering`);
- VT `73.btb` (same engine path, item 11 -> `EXAMVT`);
- `inventor/INSTRIP.TGA`, `inventor/*.tga`, `inventor/examine/*.jpg`,
  `copy2hd/setupart/*` from the CDs;
- `setupart/TOP_BAR.TGA` only as evidence for the still-missing top menu.

## What the original does

The earlier `SAVE_FORMAT.md` pass called save blocks 8–12 "dialogue state".
That was wrong. The functions that consume the same tables are the inventory
strip:

| function | digest (first 16) | role |
| --- | --- | --- |
| `0x4083d4` | `1ef8d768ac8fe1c4` | strip hover/cursor: six 0x3c slots, examine button, page arrows |
| `0x407420` | `6e9ceb4b85a2450d` | draws at most six item icons at `x=0xa0+0x3c*i`, `y=0x1a4+6` |
| `0x408076` | `0a9377f19d5045b6` | item-slot click: select/swap/page; sets state 4 for examine |
| `0x407f6a` | `b2dd03c1f6cc3268` | examine button (`x=2..0x3a`, `y=0x1a8..0x1dc`) |
| `0x407df9` / `0x407e74` | `d7553e3d03042b61` / `60e3c475c6a5b87e` | next / previous page arrow |
| `0x407527` | `e732fd9ca47434df` | scroll one item, clamped to `count-6` |
| `0x4075fe` | `8c476791914d97ed` | blits the 60-px strip at `0x1a4` when visible |
| `0x408adc` | `d1329185effa50d5` | restores visible/page/scroll/counts/items/state |
| `0x40860e` | `28e4dc0352c44094` | selected item -> 8-char stem, first char `'e'` |
| `0x41045b` | `bfd535306a102e94` | `inventor\examine\<stem>.jpg` for transition code 4 |
| `0x438173` | `aad99d1a46f68fdc` | stores the five examine scene indices from SHOW.LCC |
| `0x438afa` | `da20d306dd0993e8` | scene-name group -> examine group |
| `0x438c80` | `2661f41141dca5c5` | group -> close-up scene index |
| `0x438de4` | `710d30c0b6ec1f13` | capture the parent state before the close-up |
| `0x439349` | `66126a8910fc3565` | restore that captured state on return |
| `0x42db22` | `8df50fc5339a585b` | `0x69` transition 4; `0x73` uses the snapshot when set |
| `0x4013ae` | `94c3c1e778b6c0a3` | cursor-band yaw/pitch; `0x50..0x168` is the neutral band |
| `0x40c02c` | `ef06dfae27973bb1` | rows `0x14..0x1a3` are scene; `>=0x1a4` is the strip |
| `0x408e3f` / `0x408f32` | `2079ca547af7db29` / — | top menu bar assets and draw (not modelled) |

`0x4083d4`/`0x407420` use `DAT_00468cc0[page]` (count), `DAT_00468d78`
(10×350 item ids), `DAT_0045d1c4` (page), `DAT_0045d1bc` (scroll),
`DAT_0045d1c8` (selected item) and `DAT_00486f90` (item records). Those are
exactly save blocks 8–12. `0x40860e` and `0x41045b` prove the close-up image is
`inventor/examine/<first 8 chars, first replaced by e>.jpg`; the port uses the
existing JPEG decoder.

The scene group table is original data, not a port special case:

```
vh -> examvh
vb, hstmed -> examvb
vc, lintel, morris -> examvc
vt, clock, voice, star, mz2 -> examvt
other -> examine
```

`0x438afa` returns group 5 for `va`, but `0x438173` stores only five groups
(0..4), so that read is out of range. The port leaves `va` unresolved instead of
guessing a sixth scene.

## Implemented common functions

- **640x480 normal surface.** `Display` keeps the 640x400 playfield and builds
  a 640x480 screen with the playfield at row `0x14`; all normal mouse clicks are
  converted back to playfield coordinates. `--click-at`/`--click` remain
  playfield coordinates for existing tests.
- **Inventory data across scenes.** `carry_inventory` keeps save block 8–12
  (`dialogue_channels`, item states, visible/page/scroll/selection/use item)
  across `0x73` like the process-global original tables. `0xbb` still selects a
  page during scene entry (VT entry does this, so the save's page 2 becomes page
  0 before the strip is drawn; that is the shipped script's outcome).
- **Strip drawing.** `Display::build_screen` draws `inventor/INSTRIP.TGA` at
  `0x1a4` and the six real item icons from `SHOW.LCI` names, with the selected
  slot outlined. Icons are decoded once through the already-used FFmpeg; a
  missing TGA is reported (`ui_asset_missing`) and leaves a plain slot.
- **Strip input.** Clicks in rows `>=0x1a4` are consumed before the scene hit
  test. Slot hit boxes are `x=0xa0+0x3c*i`, `y=0x1a6..0x1df`; examine button
  `x=2..0x3a`, `y=0x1a8..0x1dc`; page arrows `x=0x23b..0x259` and
  `x=0x25d..0x27b`, `y=0x1b1..0x1cb`.
- **Item selection/examine.** `inventory_select` toggles the current item and
  marks a state-3 entry seen; `inventory_mark_using` sets state 4 and
  `current_use_item` (DAT_0045d1cc). `0x69` with transition 4 now requests
  `examine_image` instead of a CB background; `refresh_background` resolves the
  real `inventor/examine` JPEG.
- **Close-up scene entry/return.** `enter_examine` uses the recovered scene
  group table to travel to `EXAMINE`/`EXAMVT`/..., captures the whole
  `SceneView` and arms `return_armed`. A `0x73` while armed restores that view,
  merges the examine scene's inventory edits, and reports
  `closeup_return scene=… from=…`. This is the DAT_004603d0 override; the port
  stores a whole-view snapshot rather than the original's individual subsystem
  tables.
- **Vertical mouse look.** Left-drag now sends both yaw and pitch (`0.15°` per
  vertical pixel, a port input decision); the wheel sends ±5°. Arrow keys still
  work. This is not the original cursor-edge band logic of `0x4013ae`.

## Acceptance run

AM `62.btb`, physical X11 mouse on an isolated 1280x960 X server:

1. rotate to slot 88, click it; rotate to slot 92, click and run `Inuam3a`;
2. answer choice 0 with a real answer-row click;
3. the last clip plays the shipped `na1nur04.avi` + PCM;
4. after `conversation_end result=65535`, the normal AM view returns with the
   strip visible (`click_result steps=2 … active_slots=0`);
5. vertical drag changes pitch `-1.5, -3, -4.5, -6, -7.5, -9`;
6. click item slot 1 (`id=11 ibeering`), then the examine button;
7. `closeup_enter scene=EXAMINE parent=AM item=11 index=8`, background
   `EBEERING.jpg`;
8. click the close-up exit hotspot (`slot 0 action 269`, logical playfield
   `589,370`); `closeup_return scene=AM from=EXAMINE … bar=visible`;
9. click another strip item (`slot=2 id=6 name=IHOMER`) to show normal input
   continues.

Recording: `port/output/recordings/am_bar_examine_exit.mp4` — 68.03 s,
1280x960 H.264 30 fps + AAC 48 kHz stereo (mean −20.8 dB, max −0.6 dB);
`am_bar_examine_exit.log` is the player log, `_contact.png` a frame sheet.

Same system in a different scene, also physical mouse:

EA `26.btb`, item 30 (`iridnote`), which is on page 0:

1. click slot 0 (`(190,450)`), examine button (`(30,450)`);
2. `closeup_enter scene=EXAMINE parent=EA item=30 index=8`, background
   `ERIDNOTE.JPG`;
3. click the exit hotspot at playfield `(589,349)` -> `closeup_return scene=EA
   from=EXAMINE`, `hotspots=2`, `bar=visible`.

Recording: `port/output/recordings/ea_bar_examine_exit.mp4` (12 s), log
`ea_bar_examine_exit.log`, frames `ea_bar_examine.png`, `ea_closeup.png`,
`ea_returned.png`.

A third scene was checked headless:

```
VT 73.btb + bar item 11 -> travel index=12 scene=examvt
background EBEERING.jpg, entry steps=38, slots=5
click exit 589,349 -> closeup_return scene=VT from=EXAMVT
hotspots=2, bar=visible
```

Commands: `--bar-select 11 --bar-use --click-at 589,349` (the close-up exit
coordinates are playfield coordinates; the physical runs add the 0x14 row).

## Subsystem status

**Inventory/toolbar — common, partial.**
Now: save blocks 8–12, 640x480 strip, six item icons, selection, scroll,
examine, visible state and carry across `0x73`. Missing: the top menu
(`0x408e3f`/`0x408f32`/`0x408fe7`/`0x4090c4`: save/load, notes, explore,
options), the item “use on scene object” compatibility of `0x407c5a` and the
consume/combine path of `0x407bd7`, and the remaining strip page tabs.

**Close-up exit — common, implemented.**
Now: transition-4 `0x69` image, original scene-group table, snapshot return,
close-up exit hotspot, atomic item switch inside the close-up
(`analysis/rea/EXAMINE_SWITCH.md`) and the `0x4085e7` examine-state clear on
return. Missing: the exact original snapshot table layout (the port snapshots
the whole `SceneView`), the `va` out-of-range group, and close-up
animations/transitions.

**Camera — common, partial.**
Now: yaw and pitch via left-drag, wheel and arrow keys; the same
`PanoramaView::advance` for all panorama scenes. Missing: the exact original
cursor-edge bands (`0x4013ae`), their acceleration/dead zone, and cursor zone
look buttons.

**Conversation — common, complete for this flow.**
Now: `0x70` pause/resume, answer lists, ESC menu, media/audio, and immediate
restore of the normal 640x480 view after the result. Missing: any original
out-of-script close-up UI beyond the strip, and the exact original transition
effects around the conversation.

**Item use — common, partial.**
Now: selection, state 3→1, state 4 examine flag, examine image and close-up
travel, and the `0x407c5a` item-on-object gate with the shipped script's own
consume (`0xab`) — see `analysis/rea/ITEM_USE.md`. Missing: item combining and
the `0x407bd7` cursor drawing.

**Scene transitions — common, partial.**
Now: `0x73`/LCX resolution, target sibling tables, variable-pool carry,
inventory carry, and parent snapshot return. Missing: the original
object/media/dialogue-chain snapshot contents (whole-view snapshot is the port
approximation), and the map/exit UI beyond the strip.

## Boundaries

- The top 20 rows are currently black; top-menu clicks are consumed but no menu
  is drawn. This is deliberate: drawing `TOP_BAR.TGA` without its
  save/load/notes/explore/options actions would be decoration.
- The strip's item-on-object path is not claimed. The bar is interactive for
  selection/examine, not a complete inventory system.
- The close-up snapshot is a port representation of `DAT_004603d0`; original
  subsystem-by-subsystem restoration order is not bit-identical.
- `inventor/examine` files are read from the extracted data root; `scene.sh`
  now extracts them from every CD and the inventory TGAs, with `.ui_v1`
  backfill. The AM test data was manually backfilled before the script change.
- Physical verification is the AM conversation/bar/examine flow and the EA
  bar/examine flow. VT/EXAMVT is a headless engine-path check with the same code
  and original data.
