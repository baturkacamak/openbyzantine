# Dialogue records, channel stacks and 0xab/0xb0/0xb1/0xb6

25 October 2026. Static recovery plus a bounded port implementation. No original
runtime execution and no live binary session; everything below comes from the
saved REA/Ghidra exports, the EXE string/data image and the game CDs.

## Verified evidence

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`. Every
function below was re-hashed against `pseudocode/decompile-index.json` (all
match):

| Address | Range | Role |
| --- | --- | --- |
| `0x42db22` | `0x42db22-0x43194e` | opcode dispatcher (cases `0xab`, `0xb0`, `0xb1`, `0xb6`) |
| `0x4077b2` | `0x4077b2-0x4078f3` | start a dialogue line, push on the channel stack |
| `0x407933` | `0x407933-0x407ad4` | end a line, remove from the channel stack |
| `0x407ad5` | `0x407ad5-0x407b9b` | promote a line to state 3 and reset stale prompts |
| `0x407254` | `0x407254-0x4073c2` | inventor/hotitems TGA load that gates the stack push |
| `0x407b9c` | `0x407b9c-0x407bd6` | set the current record pointer |
| `0x407420` | `0x407420-0x407526` | redraw the message window |
| `0x408662` | `0x408662-0x4087c0` | allocate the dialogue record array |
| `0x408bb2` | `0x408bb2-0x408c80` | CI name id -> active dialogue list index |
| `0x408c81` | `0x408c81-0x408d3e` | CY name id -> channel list index |
| `0x411dfc` | `0x411dfc-0x412205` | generic `<name>\0<source>\0<type>` record loader |
| `0x412d35` | `0x412d35-0x412f76` | builds `engine/<scene><suffix>` names |
| `0x412f77` | `0x412f77-0x41316e` | loads the eleven per-scene engine tables |
| `0x437cad` | `0x437cad-0x437f75` | `.LCC` boot branch, derives LCV/LVM/LCX/LDM/LCI/LCY |
| `0x438173` | `0x438173-0x438af9` | loads the boot lists/records (LCI/LCY here) |

## Observed model

### Records

`0x408662(count)` allocates `count` 0x18-byte records in `DAT_00486f90` and
writes `+0x00 = 0`, `+0x04 = id*2 + 0x20`, `+0x08 = id`, `+0x0c/+0x10/+0x14 = 0`
for each. Slots above `count` stay null. The count comes from
`0x411dfc(0x485400, ..., &DAT_00485a00)` (`0x412f77`, suffix `ci`), which
`0x42c8a0` passes to `0x408662`; the `.LCC` boot path (`0x438173`) instead
passes the number of `engine/SHOW.LCI` entries. `0x408662` runs once
(`DAT_0045d1d8` guard) and `0x40885b` frees the array on scene teardown.

Only `+0x00` is read or written by the dialogue opcodes; it is the byte `0xb0`/
`0xb1` branch on. `0x407254` fills `+0x0c`/`+0x14` with inventor images.

### Name ids

The script operand is an index into the per-scene table, not the record id:

* `0x408bb2(id)`: compares `DAT_00485400[id].name` (suffix `ci`) with the
  active list `DAT_00482540` (up to `0x15e` entries) using `__strcmpi` and
  returns that list index.
* `0x408c81(id)`: the same over `DAT_0047a2b0[id].name` (suffix `cy`) against
  the ten-entry list `DAT_0047a8b0`.
* If the list's first entry is null the helper returns the raw id unchanged.
  A name that is absent from a non-empty list logs through `0x40ac43` and
  yields `0`.

`0x438173` fills both lists by reading `<name>\0<source>\0<type byte>` records
from `engine/SHOW.LCI` and `engine/SHOW.LCY` (0x437cad replaces the last
characters of the boot name `SHOW.LCC`, 0x4615ac/0x4615c0/0x4615c4 = `lcc`,
`lci`, `lcy`). The shipped CD1 confirms both files. **Data check:** SHOW.LCI has
126 records and every one of the 366 CI names of the 44 extracted scenes is
present in it (case-insensitively); SHOW.LCY has 8 records and every per-scene
CY name is present. So the dialogue record id space is the global zero-based
SHOW.LCI order and the channel id space is the SHOW.LCY order.

### Opcodes

`0x42db22` reads every operand through `0x436652` (raw 16-bit fetch, no variable
resolution):

* `0xab name, mode, channel`
  * resolve `name` with `0x408bb2`, `channel` with `0x408c81`;
  * `mode == 1` -> `0x4077b2(channel, id)`; `mode == 0 || mode == 2` ->
    `0x407933(channel, id)`;
  * then unconditionally `record[id][0] = (byte)mode`.
* `0xb0 name, value, label, -`: jump to `label` when `record[id][0] == value`.
* `0xb1 name, value, label, -`: jump when `record[id][0] != value`.
  The byte is zero-extended, so a `value` above 255 never matches.
* `0xb6 name, channel`: `0x4077b2(channel, id)` then `0x407ad5(record[id])`.

`0x4077b2` ignores a record that is already `1`, otherwise writes `1` and then:

* if the record is the current one (`DAT_0045d1c8`), `0x407b9c(0)` clears the
  current pointer and redraws an empty window;
* else if `0x407254` reports success, the id is shifted onto the front of the
  channel stack (`DAT_00468cc0` count, `DAT_00468d78` + channel*0x578, 0x15e
  entries);
* and, when the channel is the current channel (`DAT_0045d1c4`), `0x407420(1)`
  redraws.

`0x407254` truncates the record name to 8 characters, loads
`inventor/<name>.tga` and then `inventor/hotitems/<name with a leading 'h'>.tga`
and returns success only when both loads succeed. This is media data, not script
state: 325 of the 366 CI records have both TGAs on the shipped CD1, 41 do not,
so the push genuinely depends on it.

`0x407933` returns immediately for states `0` and `2`; for state `3` it clears
the current pointer first; it removes the first matching id from the channel
stack, decrements `DAT_0045d1bc` when the stack is shorter than
`DAT_0045d1bc + 6`, and redraws when the channel is current.

`0x407ad5` sets the current pointer to the record (`0x407b9c`), walks every
channel stack and returns any state-3 record to state `1`, then writes `3` to
the promoted record and redraws.

## Port implementation

`port/include/byzantine/cc.hpp` / `port/src/cc.cpp`:

* `SceneDialogueTables` holds the four lists: global `names` (SHOW.LCI, ids),
  per-scene `scene_names` (CI, script operands), global `channels` (SHOW.LCY)
  and per-scene `scene_channels` (CY). Empty globals reproduce the original's
  identity fallback; a missing per-scene table makes the trace stop instead of
  reading a phantom record.
* `SceneDialogueRecord{state, inventory_loaded}`, `SceneState::dialogue_records`,
  `dialogue_channels[10]`, `current_dialogue_channel` (BSS zero),
  `current_dialogue_record`, `dialogue_scroll`, and the two media name sets with
  `inventory_assets_known`.
* Trace reporting: `unresolved_dialogue`, `unresolved_dialogue_channel`,
  `unresolved_dialogue_inventory`, `dialogue_name_fallbacks`,
  `dialogue_refresh_requests`, `dialogue_display_requests`.
* `byzantine-cc` gained `--lci`, `--ci`, `--lcy`, `--cy`, `--inventor`,
  `--hotitems`; the argument parser now accepts them in any order after the
  mode, and `--trace`/`--trace-new-game` keep `<label>` first as documented.
  `byzantine-play` now pauses on the three dialogue-unresolved flags exactly as
  it already did for a missing variable, instead of committing partial state.

Behaviour that is deliberately not modelled: the renderer behind `0x407420`/
`0x407b9c`/`0x407ad5` (counted as requests), the record fields `+0x04`/`+0x08`/
`+0x0c`/`+0x10`/`+0x14` beyond the inventory flag, save/restore of
`DAT_0045d1c0`/`0x45d1c4`/`0x45d1bc`/`0x468cc0`/`0x468d78` (`0x408adc`), the
`0x408662` once-only guard across scenes, and the `0xb2`/`0xb3` call variants of
the same comparison.

## Verification

`make -C port -j2 all test` passes (scene-state, variable-metadata, panorama,
records). New scene-state cases cover: the media-gated start (`state` written
before the load, no push), start with mappable media, mode-0 stop with stack
removal and scroll shrink, the state-2 early return, CI/CY name fallback to
record/channel 0, rejection of an out-of-range CI name id, both branch
directions of `0xb0`/`0xb1`, and the two `0xb6` promotions that reset the
previous state-3 line through the channel stack.

Real data, first CLI run with the extracted tables:

```sh
port/byzantine-cc port/output/cc_all/EA --trace 458 port/output/cb_all/EA \
  --ci <CI.ALL/EA> --cy <CY.ALL/EA> --lci <SHOW.LCI> --lcy <SHOW.LCY> \
  --inventor <names> --hotitems <names>
# steps=6 stopped_at=516, dialogue_refresh_requests=1
# dialogue_id=28 state=1 inventory_loaded=true, dialogue_channel=0 stack=28
```

EA CI name id 2 is `ienvlop`, SHOW.LCI index 28 is `ienvlop`, and both
`inventor/ienvlop.tga` and `inventor/hotitems/henvlop.tga` exist. Without the
tables the same trace stops at instruction 514 with `unresolved_dialogue=2`.
`VA` label 265 (`0xb0` on name id 2, value 0) jumps because record 80 is `0`,
then enables hotspot 6 and returns at 301.

Census (`analysis/rea/build_vm_census.py`, now supplying CS/CI/CY/SHOW.LCI/
SHOW.LCY and the TGA name listings from CD1): completed labels **3510 -> 3899**,
opcode-blocked **1754 -> 1168**, carried-variable **2404 -> 2601**, unknown RNG
16 -> 25, unknown dialogue table/media **0**, opcodes with a port branch
**40 -> 44** of 155. `0xab`, `0xb0`, `0xb1` and `0xb6` disappear from the
blocker ranking; the new top blockers are `0xcc` (188), `0xbd` (171), `0xb9`
(108), `0xbf` (105), `0xc0` (92), `0xd1` (91), `0xb2` (52), `0x70` (47).

## Next bounded step

1. `0xb2`/`0xb3`: identical comparison plus the `0x43194f` return-address push
   (52 + 3 blocked labels).
2. `0xcc` (188) and `0xbd` (171) need their own dispatcher read; `0xb9` (108)
   remains the panorama loader.
