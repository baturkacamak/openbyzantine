# Per-scene engine tables and the sound-name table (DAT_0047a8e0)

25 October 2026. Static recovery from saved exports plus reads of the preserved
CD1 ISO. No original execution.

## Table loader

`0x412f77` loads eleven per-id record tables through `0x411dfc(table, name,
count)`:

| Table | Records | Name global | Count |
| --- | --- | --- | --- |
| `0x47dd70` | variable descriptors | `0x4859a0` | `0x4859d8` |
| `0x484e80` | — | `0x46e800` | `0x477348` |
| `0x46e870` | — | `0x4859e0` | `0x46e24e` |
| **`0x47a8e0`** | **sounds** | `0x483630` | `0x483048` |
| `0x483080` | — | `0x47fa60` | `0x46e862` |
| `0x4848b0` | — | `0x484310` | `0x48306e` |
| `0x46e280` | — | `0x47a2a0` | `0x4835fc` |
| `0x483d90` | — | `0x46e240` | `0x46e814` |
| `0x485400` | — | `0x47a270` | `0x485a00` |
| `0x47a2b0` | — | `0x47ae60` | `0x483bb8` |
| `0x484330` | — | `0x482440` | `0x47a25e` |

`0x412d35(prefix)` builds each file name from a two-character scene prefix plus
the suffix strings at `0x45eaf0..0x45eb1c`: `ca, cb, cd, ch, cm, cs, ct, cv,
co, ci, cy, cf, ce, ccs`. `0x411dfc` opens `engine/<name>` (or the same name
from an ALL archive through `0x411b4b`) and parses repeated
`<name>\0<source>\0<type byte>` records into 12-byte entries:

- `+0` = copied name string
- `+4` = copied source string (for example `EA.CCG`)
- `+8` = type byte from the file
- `+9` = 0 (later overwritten by `0x4117a6`)

## Recovered data: `CS.ALL` is the sound table

The extracted `copy2hd/engine/CS.ALL` (CD1) contains one entry per scene (44),
and each entry parses cleanly in that record format. Example `EA` (38 records):

```
voemtap2, EA.CCG, 2
eadrOpn,  EA.CCG, 1
eadrCls,  EA.CCG, 1
eaphnrng, EA.CCG, 1
...
```

The type bytes across `EA`/`VH` are `0, 1, 2`. `VH` contains a record named
`endgame` — exactly the string `0x42db22` case `0xc4` compares against
(`__strcmpi(name, "endgame")`, which sets `DAT_0046066c = 1`). That confirms
`0x47a8e0` is the per-scene sound-name table and that record order is the id
used by the scripts.

Existing port tooling can enumerate the same entries:

```sh
port/byzantine-all <cd>/copy2hd/engine/CS.ALL /tmp/cs
strings -n 4 /tmp/cs/EA
```

## Consumers

- `0x65` defines an instance: `DAT_00486220[id]` gets a 0x3c-byte record with
  `+0x04` enabled flag, `+0x08`, `+0x0c`, `+0x10`, `+0x14`, `+0x18`, `+0x1c =
  10000`, `+0x20/+0x24 = 50`, `+0x30 = -1`.
- `0x4117a6` scans the runtime script records for `0x65`, writes the enabled
  flag to `0x47a8e0[id]+8`, and on the disabled path calls `0x4283a6`.
- `0x4283a6` opens `sounds\...` (`.wav`/`.raw`) using the record name and stores
  the result; this is audio playback state, not a script variable.
- `0xc4` (283 blocked labels) for each type-2 operand calls `0x427de1(id, 1)`,
  sets `DAT_00486220[id]+0x34 = 1` when `+0x14 != 0`, and flags `endgame` by
  name.
- `0xbc` (281) calls `0x4281b1`/`0x4277ed` depending on `+0x2c` and clears
  `+0x34`.

## Port reader (done)

`port/include/byzantine/records.hpp` + `port/src/records.cpp` implement
`parse_named_records`: the `<name>\0<source>\0<type>` layout, the `0x15e`
record limit, and rejection of empty names, missing terminators and a missing
type byte. `port/tests/records.cpp` covers two-record input, the `endgame`
entry, empty input, four malformed inputs and the record limit. It is wired into
Make and CMake; `make -C port -j2 all test` and the new `records-test` pass.

## VM implementation and census effect

`SceneState::sounds` (0x65 records), `sound_records` (the per-scene CS entry)
and `endgame_requested` are implemented:

- `0x65` stores `+0x04/0x08/0x0c/0x10/0x14/0x18`, using the resolved operands
  1/2 and the raw operands 3/4/5/6 (`record[7] == 2` selects operand 6).
  The `FUN_0041d440(2)` sound-device check that can clear `+0x04` is NOT
  evaluated and is documented in the code.
- `0xc4` activates every type-2 operand whose `+0x14 != 0`, compares the sound
  name with `endgame`, and stops with `unresolved_sound_name` when no CS entry
  was supplied (it must not skip a potential endgame trigger).
- `0xbc` clears `+0x34` for every type-2 operand.

`byzantine-cc --census <cb> [<scene-cs-entry>]` now loads the CS entry, and
`build_vm_census.py` extracts `copy2hd/engine/CS.ALL` from the CD1 ISO with 7z,
splits it with `byzantine-all`, and passes each scene's entry when available.

Census on all 44 scenes (7,693 labels): completed 3,149 -> **3,418**,
opcode-blocked 2,431 -> **2,044**, unknown-variable 2,098 -> 2,215, unknown
sound table 0 (every scene had a CS entry; five are legitimately empty).
`0xc1` was then added (`+0x30`, `3000 -> -1`): completed 3,439,
opcode-blocked 1,909, unknown-variable 2,329. New top blockers: `0xab` (310),
`0x6c` (236), `0xcc` (178), `0xb0` (153), `0xb1` (140), `0xbd` (110), `0xb6`
(91), `0xb9` (91).

## Not claimed

- Only the `cs` table's purpose is confirmed; the other ten suffixes are listed
  from the loader but their record meaning is not established here.
- The `sounds\...` file opening, volume/loop fields and DirectSound playback are
  not reconstructed.
- No original execution; the table parse is verified against the ISO bytes and
  the script semantics, not against runtime behavior.
