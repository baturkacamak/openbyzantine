# Inventory item use on scene objects (0x407c5a / 0x40c329)

Date: 26 October 2026.

This step adds the shared "use the selected inventory item on a scene object"
path. It is the missing half of the inventory strip whose selection and examine
close-up landed in the previous step. The gate, the item data and the script
execution are common code; no scene branch was added.

## Evidence

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.
Pseudocode hashes from `pseudocode/decompile-index.json`:

| address | digest (first 16) | role |
| --- | --- | --- |
| `0x407c5a` | `862c7fc71bc4f5da` | item/object compatibility |
| `0x407bd7` | `38535568f84779e0` | item cursor id (not a mutation) |
| `0x408bb2` | `b595c3b398d30523` | CI name id -> dialogue record id |
| `0x408076` | `0a9377f19d5045b6` | strip click: select / swap / examine |
| `0x407ad5` | `abfd5c889890e24d` | promote the selected record to state 3 |
| `0x4077b2` | `50e68fbad69f4732` | push a line onto a channel (inventory add) |
| `0x407933` | `4139a519aaad2efa` | remove a line from a channel (inventory remove) |
| `0x40c329` | `5a9e9344f5661475` | object-click dispatch and item gate |
| `0x43c8d2` | `09a9db4ea6321c2e` | run the object's `+0x0c` label |
| `0x42db22` | `8df50fc5339a585b` | opcode dispatcher; case `0x72` object create |
| `0x438afa` | `da20d306dd0993e8` | scene name -> examine group |
| `0x438c80` | `2661f41141dca5c5` | examine group -> close-up scene index |
| `0x439349` | `66126a8910fc3565` | restore the close-up snapshot |
| `0x4085e7` | `e20f05040ada55e7` | clear `DAT_0045d1cc`, record back to 1 |

## The original flow

`0x40c329` is the scene click dispatcher. For an object hit it does, in order:

1. `DAT_004603d4 = DAT_00464300[id]->+0x14` (the examine text id; `0x4177c7(0)`
   was already called to clear the window);
2. `compatible = 0x407c5a(object->+0x10, object->+0x09)`;
3. when an inventory record is selected (`DAT_0045d1c8 != 0`): an incompatible
   object sets the dispatcher result to `0xfffe`, so `0x43c8d2` never reaches
   `DAT_004654e0[object->+0x0c]` — the object's action label does **not** run.
   Compatible objects run the same label as a normal click.

`0x407bd7` was checked again: it returns `record+4` (the item's cursor id,
`id*2+0x20`) or `+4 + 1` for the "hot" variant; it does not consume anything.
Consumption is the scene script's own `0xab` mode 0/2 (or `0x407933`), i.e. the
game's authored behaviour, not a port rule.

### 0x407c5a in the port

`byzantine::item_use_compatible(state, object, selected)` reproduces the switch
exactly:

| kind | no name id (`+0x10 == -1`) | with a name id |
| --- | --- | --- |
| `0x00,0x01,0x02,0x05,0x08,0x09,0x0c` | true (exits etc.) | compare record |
| `0x03,0x04,0x06,0x07` | false | false |
| `0x0a,0x0b,0x0d,0x0e,0x10` | true only if nothing is selected | compare record |
| `0x0f,0x11,0x18..0x1b,0x1d,0x1e` | false | false |
| `0x12` | false | compare record |
| `0x13,0x14` | false | search `DAT_00486a10[chain]` ids |
| default | compare record | compare record |

"compare record" is `DAT_00486f90[name_id] == DAT_0045d1c8`, i.e. the stored
name id equals the selected dialogue-record id. The port's selection is
`SceneState::current_dialogue_record`, the same id the strip highlight uses.

### Object name ids (0x72 operand 7)

`0x42db22` case `0x72` reads operand 7 only when its type byte is 2. Kinds
`0x13`/`0x14` store that word as a dialogue-chain id. Every other kind passes
it through `0x408bb2`, so `+0x10` becomes a **dialogue record id** (SHOW.LCI
index). The port now resolves that through the per-scene CI table instead of
leaving `+0x10` unset. Without the CI table the field stays `-1` and is
reported, never guessed.

## Real puzzle used for acceptance

CLOCK, save `70.btb` (CD1). Data, not a guess:

* object `26` is active after entry: rect `(388,369,38x36)`, kind `0x12`,
  label `202`, `+0x10 = 21`;
* the scene's page 2 is CY `vTk` (SHOW.LCY index 2) with exactly one item,
  record `21 = ISTAR` (SHOW.LCI);
* CLOCK CI id 0 is `ISTAR`, so the `0x72` operand-7 CI id 0 resolves to
  record 21;
* `0x407c5a(21, 0x12)` is true exactly when ISTAR is selected.

Label `202` (shipped script):

```
0x6e 26                 disable the star hotspot
0xab CI 0 mode 2 CY 0   remove ISTAR from channel vTk  (consume)
0x67 0 -> bHveStar
0x9b 145                starChck == 0 -> label 155
                       155: create record 3, enable object 27
```

The port does not create record 3 because CA CLOCK entry 3 `vtoStar` has type
`0xff` (the original's `0x431b0f` guard skips it too). The visible/observable
result is therefore the removed item and the hotspot swap, plus the record-21
state byte 2 that the scene entry reads.

### Headless acceptance

```
CLOCK 70.btb --entry --bar-select 21 --click 26
bar_page page=2 scroll=0 count=1 ids=21,
bar_select id=21 name=ISTAR selected=yes
item_use slot=26 item=21 name=ISTAR object_kind=0x12 object_name=21 compatible=yes
click slot 26 action 202 handled=true
click_result steps=9 … fingerprint=0xbafbe1f79b0f276a
bar_state page=2 scroll=0 count=0 selected=21 examined=none ids=
scene=CLOCK background=vtClock hotspots=7 slots=0,7,24,25,27,30,31,
```

Before/after: hotspot 26 is gone, hotspot 27 appears, page 2 is empty.
Log: `port/output/recordings/item_use/clock_headless_use.log`.

### Wrong item

Page 2 of CLOCK only holds ISTAR, so the wrong-item case is a non-item hotspot.
Clicking object 24 (kind `0x0e`, no name id) while ISTAR is selected:

```
item_use slot=24 item=21 name=ISTAR object_kind=0xe object_name=-1 compatible=no
click at 220,380 slot 24 action 477 item_use rejected item=21 kind=0xe
item_use_text window=examine cleared=false
```

No `click_result`, no label run, no variable/object/record change. The HSTMED
control gives the byte-identical-frame proof: selecting record 66 (`ihstd`, the
wrong item for a crowbar tile) and clicking object 8 rejected the use, and the
composed frame was byte-identical to the same selection without the click. The
examine window is cleared exactly as `0x4177c7(0)` does before the gate.

### Leaving the scene and returning

Real script path, both hops are shipped hotspots:

1. use ISTAR on object 26 (above);
2. click object 30 (label 166) -> `0x73 [10]` -> LCX 42 = VT;
3. in VT click slot 114 (label 1374) -> `0x73 [8]` -> LCX 6 = CLOCK.

After the return:

```
entry scene=CLOCK steps=137 stopped_at=76 hotspots=7 records=1
object slot=26 … active=0 kind=0x12 label=202 name_id=21 name=ISTAR
object slot=27 … active=1 kind=0xe label=494
scene=CLOCK background=vtClock hotspots=7 slots=0,7,24,25,27,30,31,
```

The control run without the use (same two hops) returns with object 26 active
and object 27 inactive. The difference is the process-global inventory: `0xab`
mode 2 wrote record 21's state byte to 2, the pool carries the item state, and
CLOCK's entry label 137 runs `0xb0 CI 0 == 2` -> `145` -> `155`, so it rebuilds
the placed-star hotspot. Logs:
`clock_roundtrip.log` and `clock_roundtrip_without_use.log`.

### Real window

Isolated Xephyr `:8` at 1280x960, real X11 clicks (`xdotool`):

```
bar_item slot=0 id=21 name=ISTAR selected=yes current=21
mouse_click phase=down logical=407,369 window=1280x960 output=1280x960 logical_size=640x480
item_use slot=26 item=21 name=ISTAR object_kind=0x12 object_name=21 compatible=yes
click at 407,369 slot 26 action 202
click_result steps=9 … active_slots=7
```

Screenshot `clock_after_use.png` shows the clock scene and an empty page-2
strip. A second real-window run clicked object 24 with ISTAR selected and was
rejected (`clock_real_wrong_item.log`).

## Second scene (different item and scene group)

HSTMED, save `43.btb`: ten active kind-`0x12` objects named record
`72 = icrwbar` (a tool). Clicking object 8 (label 1107) with the crowbar
selected:

```
item_use slot=8 item=72 name=icrwbar object_kind=0x12 object_name=72 compatible=yes
click slot 8 action 1107 handled=true
click_result steps=16 … fingerprint=0xbd5afafd5350a333
composed background=VBMEDDWN.jpg drawn=9 … pixels=11547 drawn_ids=41,40,37,36,34,38,35,33,39
scene=HSTMED background=vbmeddwn hotspots=11 slots=0,1,2,3,4,5,6,7,41,42,43,
```

Before: `VBHSTSCU.jpg`, 10 records, 14237 pixels, 12 hotspots. The tool is not
consumed (the script only removes it when authored to); hotspot 8 is disabled
and the background/record set changes visibly. Log:
`port/output/recordings/item_use/hstmed_crowbar_use.log`.

## Port changes

* `port/include/byzantine/inventory.hpp` / `src/inventory.cpp`:
  `item_use_compatible`.
* `port/src/cc.cpp`: `0x72` operand-7 CI resolution for non-`0x13/0x14` kinds.
* `port/src/play_main.cpp`:
  * `plan_hotspot` applies the gate before the action label runs and reports
    `item_use … compatible=yes|no`; a rejection never traces or commits the
    label.
  * `clear_text_after_rejected_use` mirrors `0x4177c7(0)` (examine window only).
  * `--click-at` with a stored `y >= 0x1a4` now goes through the real strip
    handler (`handle_bar_click`), so headless runs can click inventory slots
    and the examine button with the same path as the mouse.
  * diagnostics `--print-objects` (live objects + strip state) and
    `--print-variables` (local/global/name/value).
* `port/src/cc_main.cpp`: `--save-dump` prints each channel's item ids.
* Tests: `inventory-test` covers every `0x407c5a` kind branch, the chain rule
  and the exit case; `scene-state-test` covers `0x72` CI resolution and the
  "no CI table -> unset" behaviour.

`make -C port -j2 all test` passes all suites.

## Boundaries

* `0x407bd7`'s cursor id is computed but not drawn: the port has no cursor
  atlas yet, so the item/hotitem cursor image is not claimed.
* The gate uses the selected strip record (`DAT_0045d1c8`); `DAT_0045d1cc`
  (the close-up item) is only its examine target. This matches the original.
* A wrong item clears the examine window (as `0x4177c7(0)` does) but writes
  nothing else. The object's `+0x14` text id is not applied on a rejected use,
  because the port only shows text for a committed click; the original stores
  the id before the gate but does not run the label either.
* `0x13`/`0x14` compatibility needs save block 24 chains; a missing chain is a
  clean "not compatible", not an allocation.
* Item combining (`0x407bd7`'s other callers/`0x407c1d`) and the cursor drawing
  remain open.
