# One `DAT_004649e8` record table, decided by the CA asset table

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Analysis profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
(Ghidra 12.1.4). Evidence is the saved per-function exports plus focused
`objdump -d`; no original execution and no binary session.

This step resolves the limitation `FINAL_OPCODES.md` §4 kept open: the port kept
`SceneState::actors` (created on demand) apart from `SceneState::nodes` (restored
from a save), although the original has **one** `DAT_004649e8` table. The tables
are now merged, and whether a record exists at all is decided by the shipped
per-scene animation asset table `engine/<scene>ca` (`DAT_0047dd70`) instead of
being invented.

## 1. Observation — one table, one guard

`0x42db22` sends every record command through `FUN_00431b0f(id, opcode)` and
runs the body only when it returns 0:

| Opcode | Dispatcher case | Body |
| --- | --- | --- |
| `0x64` | `case 100:` (Ghidra prints it in decimal) | `0x4324b0` |
| `0x84` | `0x42dc7e` | `0x4326eb` |
| `0x85` | `0x42dce7` | `0x432904(id, instr, 1)` |
| `0x86` | `0x42de58` | `0x43299d` -> `0x432904(id, instr, 2)` |
| `0x87`..`0x8b` | `0x42dec1`..`0x42e065` | `0x4329e4`/`0x432a63`/`0x432ad2`/`0x432b09`/`0x432c65` |
| `0x8c` | `0x42e172` | `0x432c9c` |
| `0x8e` | `0x42e1d5` | `0x432d14` |
| `0x91` | `0x42dd52` | `0x4325d3` |
| `0x95` | `0x42ddd7` | inline `+0x08` write |

`ANIMATION_RECORD.md` and the port previously treated `0x64` as unguarded; the
disassembly at `0x42dc15`..`0x42dc71` is explicit:

```
42dc15  call 0x436652                 ; raw operand 0
42dc4b  push opcode; push id; call 0x431b0f
42dc5f  test eax,eax
42dc61  jne  0x42dc79                 ; guard failed -> skip the body
42dc67  push instr; push id; call 0x4324b0
```

So an absent record never faults in any of these commands: the dispatcher skips
the body and the script **continues**.

## 2. Observation — what decides existence

`0x431b0f` starts by dereferencing `DAT_0047dd70[id]` **without a NULL check**:

```
431b20  mov eax,DWORD PTR [eax*4+0x47dd70]
431b27  movsx eax,BYTE PTR [eax+0x8]     ; CA type byte
431b2b  test eax,eax
431b2d  je 0x431b4f                      ; type 0 -> try the lazy load
431b46  cmp eax,0x1
431b49  jne 0x431bd4                     ; other type -> no load attempt
431b57  cmp DWORD PTR [eax*4+0x4649e8],0 ; record present?
...
```

`DAT_0047dd70` is the `engine/<scene>ca` table: `0x412f77` loads it with
`FUN_00411dfc(0x47dd70, ...)` and `0x412d35` builds the file names from the
suffix table at `0x45eaf0` (`ca`, `cb`, `cd`, `ch`, `cm`, `cs`, `ct`, `cv`,
`co`, `ci`, `cy`, `cf`, `ce`, `ccs`, `ccz`). `0x411dfc` allocates a 0xc-byte
record per entry: name at +0, source at +4, **file type byte at +8**, state 0 at
+9.

The two creation paths are complementary:

* `0x40fd7b` (called from the scene loader `0x42c8a0` and the save loader
  `0x425ab4`) frees the whole record table through `0x4102ae`, then for every CA
  slot whose `+0x09` state is 0 and whose `+0x08` type is **2 or 3** calls
  `0x40fe70(slot, &DAT_004649e8[slot])` (`0x40fdf9`, `0x40fe1d`, `0x40fe2a`,
  `0x40fe42`).
* `0x431b0f` calls the same `0x40fe70` lazily, but only for type **0 or 1**
  (`0x431b2b`..`0x431b49`).

Any other type byte (the shipped data uses `0xff`) takes neither path, so the
record stays NULL, `0x431b0f` logs through `0x40ac43`/`0x40ac38` and returns 1.

`0x40fe70` builds `anims\<...>` from the asset name and runs `0x410078` /
`0x41cb13`, which end in `0x40ffee`'s `memset(record,0,0x40)` plus `y=0x14`,
`+0x04=10`, `+0x09=0x20`, `+0x2c/+0x30=-1` and the frame list.

**Correction to `ANIMATION_RECORD.md`**: its "record initialization" note says
`+0x14` = frame count. `0x40ffee`'s second parameter is indeed stored as a word
at `+0x14`, but the port does not model the frame count, so the merged record
leaves `+0x14` unset.

## 3. Observation — the shipped CA tables

`CA.ALL` (CD1) splits into 44 per-scene records, 970 entries, largest 322. The
`+0x08` type byte distribution:

| Type | Entries | Meaning (observed) |
| ---: | ---: | --- |
| `0` | 193 | `0x431b0f` lazy `0x40fe70` |
| `1` | 3 | `0x431b0f` lazy `0x40fe70` |
| `2` | 16 | `0x40fd7b` scene start |
| `3` | 66 | `0x40fd7b` scene start |
| `0xff` | 692 | neither: the record is NULL, commands skip |

**Inference** `0xff` is "no animation asset" — the table entry exists only so
the index space stays dense. 278 of 970 entries (28.7%) can produce a record.

Cross-check with the save data: `HSTMED` save `55.btb` restores records at slots
30, 46, 47, 48, 49, 50 while its CA table gives types 3, `0xff`, 2, 2, 2, 2 for
them. Slot 46 (`0xff`) still has a record, which is consistent because the save
is authoritative: `0x425ab4` reads block 17 *and* then calls `0x40fd7b`, which
only creates a record for a slot that is still NULL. The port therefore treats a
restored record as present regardless of its CA type.

`0x439349` installs records from a scene-transition payload as well, so a
non-empty table without a save can only come from a previous scene. That path
stays outside a single-scene trace.

## 4. Port model

* `SceneRecordState` merges the old `SceneActorState` and `SceneNodeState`
  (x/y/+0x04/+0x06/kind/flags/+0x3c trail/+0x18 frame requests, plus the save
  block's marker, `+0x1c`, `+0x20`, `+0x2c`, `+0x30` and `+0x34` extra list).
* `SceneState::records` is that one table; `load_save` fills it from block 17
  and restores every field `0x426b9d` copies (`+0`, `+2`, `+4`, `+6`, `+8`,
  `+9`, the two frame indices, `+0x2c`, `+0x30`, the extra list).
* `SceneState::actor_assets` + `actor_assets_known` hold `engine/<scene>ca`
  (`byzantine-cc --ca <file>`), reusing `parse_named_records` (`+0x08` is
  `NamedRecord::type`).
* The trace's `resolve_record` helper reproduces `0x431b0f`:

| State | Result |
| --- | --- |
| record already known (save or earlier command) | use it |
| no `--ca` supplied | `unresolved_node_asset` stop (unchanged behaviour) |
| id >= CA count | `missing_node_asset` stop: `0x431b0f` would read `NULL+8` |
| type 0..3 | create the `0x40ffee` initialiser record, count `records_created` |
| any other type | skip the body, count `skipped_node_commands`, continue |

* `0x9d`/`0x95`/`0x84`/`0x91` now run the guard **before** resolving their
  operands, matching the original's order (a skipped command never reads a
  variable).
* Every trace prints `records_created` and `skipped_node_commands`; `--census`
  aggregates them and the census report lists both.

## 5. Real traces

| Command target | Result |
| --- | --- |
| `NB` 57.btb 556 `--ca ca/NB` | `steps=9 stopped_at=691 unresolved_sound_name=11 records_created=1 skipped_node_commands=2`, `node_write instruction=699 id=10 value04=5 flags=4` |
| `STAR` 314 `--ca ca/STAR` | `steps=3 stopped_at=329 unresolved_sound_name=3 skipped_node_commands=2` |
| `HSTMED` 55.btb 259 `--ca ca/HSTMED` | `steps=5 stopped_at=283`, `node_write instruction=279 id=48 value04=1 flags=36`, id 50 — records come from the save, 0 created, 0 skipped |

`NB` is the direct before/after: its save carries `nodes=0`, CA slot 9 is
`NBHandr` type `0xff` (skip) and slot 10 is `NBBdSld` type `1` (create). Before
this step the label stopped at the first `0x86` with `unresolved_node_asset=9`
and never reached the slot-10 write.

Unchanged acceptance traces (now with `--ca`): EA 26.btb 602 `steps=24
stopped_at=319`, VT 73.btb 541 `steps=41 stopped_at=620
scene_switch_requested=21`, NB 57.btb 202 `steps=2 stopped_at=229`, VT 73.btb
356 `steps=9 stopped_at=382`, EA 254 `steps=57 stopped_at=2081
unresolved_sound_name=27`.

Traces that reach a record command now need `--ca` to keep going: EA label 458
stops at `0x85` instruction 511 with `unresolved_node_asset=1` without it, and
reproduces its documented `steps=6 stopped_at=516` (plus `records_created=2`)
with `--ca` supplied. That is the intended "existence unknown" stop, not a
regression: the command's behaviour depends on a record the trace cannot know
about.

## 6. Census (44 scenes, 7,693 labels, CA.ALL supplied)

| Outcome | Before | After |
| --- | ---: | ---: |
| `return` | 5,829 | 5,913 |
| `script_end` | 22 | 22 |
| **completed** | **5,851** | **5,935** |
| `unresolved_node_asset` | 76 | **0** |
| `missing_node_asset` (new) | — | **0** |
| `unresolved_variable` | 1,306 | 1,296 |
| `missing_object` | 78 | 80 |
| `scene_switch` | 119 | 119 |
| `unresolved_random` | 245 | 245 |
| `unresolved_disc_request` | 9 | 9 |
| `missing_media_record` | 2 | 2 |
| `missing_dialogue_chain` | 2 | 2 |
| `step_limit` | 5 | 5 |
| unsupported opcode | 0 | 0 |

Diagnostics, not outcomes: **738** records created from the CA table and
**1,925** record commands skipped because the asset type is not 0..3 across all
label traces.

The +84 completed labels come from the merge, not from a new opcode: a label
that ran `0x64` (creating a record in the old `actors` map) and later `0x8a`,
`0x8b` or `0x86` (looking in the old `nodes` map) used to stop with
`unresolved_node_asset`; both now address the same record. `unresolved_variable`
drops by 10 and `missing_object` rises by 2 because those released labels now
reach the next state boundary.

`missing_node_asset` is 0: no shipped script names a slot outside its scene's CA
table, so the original never takes the NULL dereference this outcome protects
against.

## 7. Not claimed

* The animation asset itself was still not modelled in this step; the next step
  (`ANIMATION_ASSET.md`) parses it, so a created record now carries the real
  frame count, per-frame geometry, first-frame offsets and `0x91` frame index.
* `0x40fe70`'s failure path for types 0/1 (a missing `anims` file) cannot be
  evaluated without the asset; the port assumes the load succeeds. `--ca`
  absence is the only "unknown asset" case and stops.
* `0x439349`'s transition-installed records, `0x438de4`'s transition-out
  snapshot and `DAT_004603d0` remain outside single-scene traces.
* `0x8c` (`0x432c9c`) and `0x8e` (`0x432d14`) are unguarded-by-data: not
  reached by any shipped label, so they stay unimplemented dispatcher cases.

## 8. Verified inputs

Target/profile as above. All saved exports re-hashed against
`analysis/rea/pseudocode/decompile-index.json`:

| Address | SHA-256 (first 16) | What it settles |
| --- | --- | --- |
| `0x42db22` | `8df50fc5339a585b` | dispatcher cases, `0x64` guard |
| `0x431b0f` | `f229bde142bc8a3a` | type test, lazy `0x40fe70` |
| `0x40fd7b` | `a994c5ad96f2be05` | scene-start pre-load, type 2/3 |
| `0x40fe70` | `4ca6defafd38ee24` | `anims` load, `+9 = 1` |
| `0x411dfc` | `554520e53cef4b24` | CA record layout (`+8` type, `+9` state) |
| `0x412f77` | `e3b7f0c2326dc4fc` | `0x47dd70` <- `ca` |
| `0x412d35` | `ce0ae287dfeeaf96` | suffix names |
| `0x4102ae` | `6f7755ef3a8edced` | table reset at scene start |
| `0x4102f7` | `5ca250b8a916b5ef` | per-record free |
| `0x40ffee` | `16955dd55a53620b` | initialiser defaults |
| `0x4324b0` | `a72a56ea515a1d8b` | `0x64` body, `+0x09` write |
| `0x43299d` | `68e9e51fdfcfb781` | `0x86` body |
| `0x432904` | `38a13d007cb9f198` | mask pair |
| `0x4326eb` | `06160d5b79b856f7` | `0x84` trail |
| `0x4325d3` | `5663a49198fde61e` | `0x91` frame list |
| `0x426b9d` | `5094e97ef5708b02` | save block 17 reader |
| `0x4269ec` | `a5a6bece74355bbc` | save block 17 writer |
| `0x425ab4` | `c3500b86033e2e90` | save loader calls `0x40fd7b` |
| `0x42c8a0` | `6d09fd55ad59f5e1` | scene loader calls `0x40fd7b` |
| `0x439349` | `66126a8910fc3565` | transition-installed records |
| `0x432401` | `e690b490de31cd80` | `+0x09 |= 0x20` reconcile pass |

All 21 digests match `decompile-index.json` (checked with SHA-256 over the saved
export files). The `.data` suffix table at `0x45eaf0` was read directly from the
target image.

Assembly ranges read from the target (`objdump -d -M intel`): `0x42dc15`..
`0x42dc79` (`0x64` guard), `0x431b0f`..`0x431b49` (type test and lazy load),
`0x40fd7b`..`0x40fe47` (scene-start pre-load), and the `.data` suffix table at
`0x45eaf0`..`0x45eb2b` (`ca`..`ccz`).

## 9. Commands

```sh
make -C port -j2 all test
python3 analysis/rea/build_vm_census.py     # extracts CA.ALL, passes --ca

E=<extracted copy2hd tree>; S=$E/copy2hd/savegame
port/byzantine-cc port/output/cc_all/NB --trace-save $S/57.btb 556 port/output/cb_all/NB \
  --engine $E/copy2hd/engine --lcx $E/copy2hd/engine/SHOW.LCX \
  --lci $E/copy2hd/engine/SHOW.LCI --lcy $E/copy2hd/engine/SHOW.LCY \
  --inventor $E/inventor.txt --hotitems $E/hotitems.txt \
  --gameprefs $E/copy2hd/goldsave/GAMEPREF.000 --ca $E/ca/NB \
  --co port/output/co_all/NB --ci $E/ci/NB --cy $E/cy/NB --cf $E/cf/NB
# steps=9 stopped_at=691 unresolved_sound_name=11 records_created=1 skipped_node_commands=2
# node_write instruction=699 id=10 value04=5 flags=4
```
