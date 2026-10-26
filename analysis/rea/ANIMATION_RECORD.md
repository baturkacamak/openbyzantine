# Actor / animation record behind DAT_004649e8

25 October 2026. Static recovery from saved exports plus focused objdump and
target-byte reads. No original execution.

## Observed

`DAT_004649e8` is an array of pointers indexed by a 16-bit variable id. The
record is created lazily by `0x40fe70`, which builds an animation path from the
variable descriptor at `DAT_0047dd70[id]`:

```
_sprintf(name, "%c%c\%s", desc[0], desc[1], desc)   // 0x45e800
path = "anims" / name                                // 0x45e810, 0x45e808 = ".rle"
0x410078(path, &record)  or  0x41cb13(path, &record)
```

So one record is one variable's animation/actor object, backed by
`anims/<first two chars>/<name>.rle`.

Field offsets written by the recovered commands:

| Offset | Width | Written by | Meaning |
| --- | --- | --- | --- |
| `+0x00` | word | `0x64` (arg 3), `0x84` | x |
| `+0x02` | word | `0x64` (arg 4 + 0x14), `0x84` | y |
| `+0x04` | word | `0x64` (arg 2) | not identified |
| `+0x06` | word | `0x64` clears | not identified |
| `+0x08` | byte | `0x95` | kind byte |
| `+0x09` | byte | `0x64`, `0x85`/`0x86` (masks) | kind-bit/flag byte |
| `+0x18` | dword | `0x40ffee`/`0x410078` | animation frame list head |
| `+0x1c` | dword | `0x64` copy, `0x91` | current animation frame node |
| `+0x3c` | dword | `0x84` | trail list head (8-byte nodes: x, y, next at +4) |
| `+0x20`,`+0x22` | words | `0x64` clears | not identified |
| `+0x24`,`+0x26` | words | `0x64` = x,y | not identified |

`0x84` (`0x4326eb`) appends an 8-byte node `{x, y, next}` to the list at `+0x18`
via `0x44c2f0(8)`, avoiding duplicates, then sets x and/or y from operand 1/2
(type `2` = scene variable, type `@`/`0x40` = constant) and adds 0x14 to y.

`0x91` (`0x4325d3` -> `0x40cf73`) walks the ANIMATION FRAME list (`+0x18`
head, `+0x1c` current, nodes 0x18 bytes with next at +4): operand 1 `0xffff`
clears `+0x1c`; otherwise it resets `+0x1c` to `+0x18` and advances that many
frames, logging through `0x40ac43`/`0x40ac38` when it runs off the end. It does
not touch the `+0x3c` trail list.

`0x85` (`0x432904(id, record, start=1)`) loops over operands 1..7, and for every
operand whose type byte is `2` applies two byte tables to `+0x09`:

```
flags = (table_and[raw_operand] & flags) | table_or[raw_operand]
```

With target bytes read at the recovered addresses:

| Index | `0x460418` (AND) | `0x460428` (OR) |
| ---: | ---: | ---: |
| 0,1 | `fe`, `fe` | `00`, `01` |
| 2,3 | `fd`, `fd` | `00`, `02` |
| 4,5 | `fb`, `fb` | `00`, `04` |
| 6,7 | `f7`, `f7` | `00`, `08` |
| 8,9 | `ef`, `ef` | `00`, `10` |
| 10,11 | `df`, `df` | `00`, `20` |
| 12,13 | `bf`, `bf` | `00`, `40` |
| 14,15 | `7f`, `7f` | `00`, `80` |

`objdump` of `0x432904` confirms byte loads (`mov 0x460418(%edx),%al`), so the
operand word is a byte index, not a scaled index. Scripts use 0..11 (`0x64`:
0..7; `0x85`: 0..11).

`0x64` (`0x4324b0`) applies, guarded by
`table_or[raw arg1] != 0x20 || !(flags & 0x20)`:

```
if arg1 present:  +0x09 = table_or[raw arg1]
if arg2 present:  +0x04 = value(arg2)
if arg3 present:  +0x00 = value(arg3)
if arg4 present:  +0x02 = value(arg4) + 0x14
+0x1c = +0x18; +0x20 = 0; +0x22 = 0; +0x24 = x; +0x26 = y; +0x06 = 0
```

`0x95` sets `+0x08` from operand 1 (type `2` -> scene variable, else constant),
after `0x431b0f` ensures the record exists.

> Correction (26 October 2026): `0x4324b0` writes **`+0x09`**, not `+0x08`.
> `objdump -d` at `0x43251d` is `mov BYTE PTR [ecx+0x9],al`, and the guard at
> `0x4324ea` loads `BYTE PTR [eax+0x9]` and tests `0x20` — the same byte the
> `0x85`/`0x86` mask pair rewrites. `+0x08` is written only by `0x95`. The port
> followed the earlier `+0x08` reading until this correction, which also meant
> `0x84`'s `+0x09 & 0x20` trail guard never cleared; both are fixed in the
> `FINAL_OPCODES.md` step.

## Record initialization

`0x40ffee(record, frame_count)` runs `memset(record, 0, 0x40)` and then writes:

- `+0x14` = frame count (word)
- `+0x02` (y) = `0x14`
- `+0x09` (flags) = `0x20`
- `+0x04` = `10`
- `+0x2c` and `+0x30` = `0xffffffff`
- `+0x18` = `0x18`-byte node, `+0x1c` = same node, node->next = itself

`0x410078`/`0x41cb13` then load frames and add the first frame header's
`local_26` to x and `local_24` to y. Those asset-dependent offsets and the frame
count are not modelled by the portable VM.

## Observed operand shapes (all 44 extracted scenes)

| Opcode | Occurrences | arg 0 | arg 1 | later operands |
| --- | ---: | --- | --- | --- |
| `0x64` | 950 | type 2 (id) | type 2, raw 0..7 | only absent or constant |
| `0x85` | 993 | type 2 (id) | type 2, raw 0..11 | type 2 / absent |
| `0x95` | 198 | type 2 (id) | constant 0..4 | — |
| `0x84` | 193 | type 2 (id) | type 2 or constant | type 2 or constant |
| `0x91` | 44 | type 2 (id) | type 2 or constant | — |

Important: for `0x64`/`0x85` the type-2 operand word is used **raw** as a table
index (`0x4324b0`/`0x432904` call `FUN_00436652`, which does not resolve type 2).
It is not a scene-variable read in those two commands.

## Portable implementation (26 October 2026 — see `RECORD_TABLE.md`)

`SceneState::records` holds one `SceneRecordState` per `DAT_004649e8` slot (the
same table the save restores), initialized with the exact `0x40ffee` constants:
`y=0x14`, `flags=0x20`, `+0x04=10`, rest zero. Existence is decided by the
shipped `engine/<scene>ca` asset table (`--ca`): `0x40fd7b` pre-loads types 2/3
at scene start, `0x431b0f` loads 0/1 on demand through `0x40fe70`, and any other
type (0xff in the shipped data) leaves the slot NULL so the command body is
skipped. The animation loader's frame offsets and frame count are documented as
not modelled *unless* the `anims` container is supplied (`--anims`): then
`0x40fe70`'s real frame count, per-frame geometry and first-frame offsets are
applied (`ANIMATION_ASSET.md`), and the frame-list operations act on that list:

- `0x64` (`0x42db22` `case 100:`) runs the shared `0x431b0f` guard, then sets
  `+0x09`/`+0x04`/x/y, restarts the frame list, and honours the
  `table_or[arg1] != 0x20 || !(flags & 0x20)` guard (see the correction above).
- `0x85` applies the `0x460418`/`0x460428` mask pair for every type-2 operand
  1..7; an index >= 16 is rejected instead of reading past the recovered bytes.
- `0x95` writes the kind byte from a resolved operand.
- `0x84` appends the current x/y to the `+0x3c` trail (deduplicated, skipped
  while flags bit 0x20 is set) and then writes x/y with the `+0x14` y bias.
- `0x91` records a frame restart (`0xffff`) or advance request **until the
  animation asset is loaded**; with `--anims`/a save frame count it models
  `0x40cf73` for real (0xffff clears `+0x1c`, otherwise the head is advanced
  `count` times and an overrun is logged). See `ANIMATION_ASSET.md`.

Census effect: completed entry labels 2,357 -> 3,149; opcode-blocked 3,522 ->
2,431; unknown-variable 1,799 -> 2,098; opcodes with a port branch 29 -> 35.
The remaining top blockers are `0xc4`, `0xbc`, `0xab`, `0x6c`, `0xcc`, `0xb0`,
`0xc1`, `0xb1`, which act on the separate `DAT_00486220`/`DAT_0047a8e0`
scene-object tables and the `DAT_00486f90` dialogue bytes.

## Not claimed

- No original execution; no animation playback was observed.
- Offsets `+0x04`, `+0x06`, `+0x20`, `+0x22`, `+0x24`, `+0x26` are named by
  offset only; their meaning is not established.
- The `0x460418`/`0x460428` bytes are observed data; the C-level table type is
  not recovered. Indices outside 0..15 would read past the observed tables.
- `0x431b0f`'s lazy-load failure branches and `0x40fe70`'s loader errors are not
  modelled.
