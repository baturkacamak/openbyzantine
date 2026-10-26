# Carried variable pool and dialogue-state restore

Bounded step of 25 October 2026. Scope: project the save's 3000-word
`DAT_0047e2f0` through the original per-scene index table, and restore the
dialogue window state that `0x408adc` reads. Rendering, audio/video, save
writing and `engine\<ad>.ccb` assets remain out of scope.

Evidence base (saved exports only, no binary session opened):

- target `analysis/rea/BETRAYAL.EXE`, SHA-256
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
  (matches `pseudocode/decompile-index.json` `target_sha256`);
- analysis profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
  (matches every export header and `analysis_profile_digest`);
- `0x42c8a0` (zero pool + identity mapping rows), `0x438173` (LCC/LCV/LVM
  reader), `0x4319b3` (type-2 operand resolver), `0x42db22` (the writes that
  share the same mapping), `0x425ab4` (save reader);
- `0x408adc` (dialogue restore), `0x4089b0`/`0x40892d` (per-record state
  read/write), `0x4077b2` (channel push), `0x407933` (channel pop),
  `0x407420`/`0x407ad5` (redraw/promote), `0x407254` (inventory load),
  `0x40ac43`/`0x40ac38` (error log), `0x40cf73` (node frame index);
- shipped `copy2hd/savegame/*.btb` from CD1 and `SHOW.LCC/LCV/LVM`.

Terms: **observation** = read directly from the export or the shipped bytes;
**inference** = a conclusion that fits code and data but is not stated by
either; **unknown** = not recovered.

---

## 1. The variable key decision

`0x42c8a0` initialises every one of the 50 scene rows with 300 identity words:

```
DAT_0046ee00[scene * 300 + local] = local;            // local 0..299
```

`0x438173` then overwrites the first `count(scene)` entries with the per-scene
LCV global ids (`DAT_0046ee00 + scene*600 + i*2`). `0x4319b3` resolves a type-2
operand with:

```
value = DAT_0047e2f0[ DAT_0046ee00[operand + scene*300] ]
```

- **Observation** the mapping row is 300 words (600 bytes) per scene and is
  initialised to identity before LCV is applied (`0x42c8a0`).
- **Observation** the pool is 3000 words; all LCV global ids are below the LVM
  name count (parser bound), so every mapping is in range.
- **Decision** the port keeps `SceneState::variables` keyed by the raw local
  operand (its existing key). `load_save_variables` fills all 300 locals:

```
global = local < declared_global_ids.size() ? declared_global_ids[local] : local
state.variables[local] = save.node_values[global]
```

  This is exactly the original's lookup, just resolved once at load time. It is
  **not** a copy of 3000 words into a raw map: EA local 5 comes from global 8
  (`lastNode`), local 26 from global 74 (`flowChk`), local 133 from identity
  133.
- **Inference** filling the identity tail is correct for the save path because
  the save carries all 3000 global words. `new_game_scene_state` keeps its older,
  stricter declared-only projection; that is a separate entry point and was not
  changed here.

## 2. Dialogue window state (`0x408adc`)

The reader consumes, in order:

1. `DAT_0045d1c0` (u32), `DAT_0045d1c4` (u32), `DAT_0045d1bc` (u32);
2. `DAT_00468cc0` — ten `u32` channel depths;
3. `DAT_00468d78` — ten 350-entry `u32` stacks (0x578 bytes per channel);
4. one `i32` current-record index;
5. `0x4089b0` — repeated `(byte, byte)` pairs until the first byte is `0xff`.

- **Observation** `0x4077b2` pushes a new id by shifting every existing entry up
  and writing the new id at index 0, so saved index order is **top-first**. The
  port's `dialogue_channels[c]` is restored in that order (index 0 = top).
- **Observation** `0x407933` removes the first stacked entry matching an id, so
  the stack is an ordered list, not a set.
- **Observation** `0x40892d` writes `0x00` followed by the record's state byte
  for every non-NULL `DAT_00486f90` slot and stops at the first NULL with
  `0xff`. `0x4089b0` ignores the first byte except as the terminator and stores
  the second byte at record `+0x00`. Therefore the pair **position** is the
  record id and the **second byte** is the state; the `0x00` is a presence
  marker, not data.
- **Observation** `0x408adc` sets `DAT_0045d1c8 = 0` first; an index of `-1`
  keeps it clear, otherwise it selects `DAT_00486f90[index]`. An unallocated
  slot is a NULL pointer, i.e. also no current line.
- **Decision** the port derives the record capacity from
  `dialogue_marks.size()` (the writer's own contiguous count) instead of
  hard-coding 126. A stacked id at or above that count has no record and is
  rejected while loading.
- **Not modelled (this step)** `DAT_0045d1c0` was left untouched because
  `SceneState` had no consumer; the following step added `dialogue_visible`
  (`0xb4`/`0xb5`) and now restores it. The trailing `FUN_00407420(0)` redraw
  from `0x408adc` is a renderer request; the portable trace does not synthesise
  a display event for it.

## 3. `0x75` underflow: error log, not an abort

Restoring real values made the unsigned-underflow branch of `0x75` reachable in
11 shipped labels (EA and GC). The original calls `0x40ac43`/`0x40ac38` (the
engine error log) and skips the write; it does **not** stop the script.

The port used to throw `CC subtraction would underflow variable N`. It now
counts `SceneTrace::subtraction_underflows`, leaves the destination unchanged
and continues, which is what the original branch does. The census went from 11
`error` outcomes to 0.

## 4. CLI

```
byzantine-cc <scene.cc> --trace-save <save.btb> <label> <cb> --engine <engine-dir>
byzantine-cc <scene.cc> --trace <label> <cb> --save <save.btb> --engine <engine-dir>
byzantine-cc <scene.cc> --census <cb> [<scene-cs>] --save <save.btb> --engine <engine-dir>
```

`--engine` is the directory holding `SHOW.LCC`, `SHOW.LCV` and `SHOW.LVM`. It is
optional: without it the four save tables and the dialogue state are restored,
but the variable pool stays unknown (the previous strict behaviour). With it,
the save's `DAT_00482428` scene is checked against the traced CC's scene name;
a mismatch is a hard error instead of silently using the wrong row.

`--save-dump` now runs the same `load_save` contract at the end and prints
`restore objects=... media=... sounds=... nodes=... dialogue_records=...
dialogue_stacked=...`, so a malformed depth/id is visible from the dump too.

## 5. Shipped-data checks

All 77 shipped save images in CD1 `copy2hd/savegame/` (74 numbered + `autosave`
+ `tempsave` as `.btb`, plus `NEW.CAN`) restore without an error:

| check | result |
|---|---|
| dialogue record bytes (`0x40892d` pairs) | 126 in every image |
| deepest channel (`DAT_00468cc0`) | 20 (limit 350) |
| stacked ids per image | <= 35 total, every id < 126 |
| channel selector `DAT_0045d1c4` | 0..7 (ten channels) |
| scroll `DAT_0045d1bc` | 0 in every image |
| identity/LCV mapping range | every global < 3000 |

- **Observation** the 126 record bytes independently match the 126
  `DAT_00486f90` records from `DIALOGUE_RECORDS.md`.
- **Unknown** whether every possible save always has `marks.size()` equal to the
  LCI record count; the shipped set does. The port treats the file count as the
  capacity and rejects stacked ids beyond it.

## 6. Acceptance results (shipped data)

EA save `26.btb` with `--engine`, labels 475 / 595 / 602:

```
state_origin=save file=analysis/rea/save_sample/26.btb objects=163 media=4 sounds=38 nodes=1 dialogue_records=126
variables_restored=300 declared=74 scene=ea

475: steps=3  stopped_at=534  background 0 EAEMShMs
595: steps=10 stopped_at=708  background 7 EAEmShCU
602: steps=23 stopped_at=318  unsupported_opcode=0xbb
     panorama_record=0 name=ea00 jpeg=nodes/ea/ea00.jpg
```

- `602` no longer stops on an unresolved variable; with the restored pool it
  passes the panorama load and reaches the next unimplemented opcode `0xbb`.
  Under a zero pool it stopped at `0xb9` at 317; with a fresh unknown state it
  stopped on variable 5. Neither path is used now.
- `475` and `595` complete without an unresolved variable.

The five `step_limit` labels did not change. They are self-referential `0x70`
conversation returns, not variable-dependent:

```
IU label 375: instruction 459 0x70(object 5) -> label 375 -> target 456 (its own start)
IU label 398, NB labels 240/242/243: same pattern
```

The original's conversation runner `0x432d6e` blocks for player input and only
then returns an object id (or `0xfffe`/`0xffff`); the port takes operand 0 as
the result and immediately re-enters the label, so the 10,000-instruction bound
is the honest outcome. This is the documented simplification from
`SAVE_RESTORE.md` §3, not a missing variable.

## 7. Census

| Census | after table restore (previous step) | after variable + dialogue restore |
|---|---:|---:|
| labels traced | 7,693 | 7,693 |
| completed (`return`) | 4,169 | **5,438** |
| unresolved variable | 2,939 | **1,267** |
| unresolved RNG | 25 | **223** |
| unsupported opcode | 481 | 686 |
| `missing_object` | 74 | 74 |
| `unresolved_object_label` | 0 | 0 |
| `step_limit` | 5 | 5 |
| `error` | 0 | 0 |

- All 1,267 remaining `unresolved_variable` labels are in the **26 scenes with
  no shipped CD1 save**. All 18 saved scenes resolve every variable (0
  unresolved). No value is invented for the unsaved scenes.
- `unresolved_random` rose because released labels now reach `0x7d RANDOM`
  (223). The CRT `_rand` state is not part of the save format, so the port
  correctly reports it as unknown instead of assuming `0xbeef`.
- The unsupported-opcode total rose from 481 to 686 because 1,261 labels moved
  past their variable stop and now reach the next real blocker. New ranking:
  `0xd1` (164), `0x6a` (98), `0x73` (70), `0x8b` (63), `0x8a` (37), `0xb4` (31),
  `0xb5` (24), `0xbe` (24), `0xbb` (22), `0xc8` (20).

## 8. Tests

`make -C port -j2 all test` passes.

- `save-test` (synthetic save): variable projection with declared ids
  `{3,1,2999}` and identity tail; dialogue stack order; record states; current
  record index, `-1` and unallocated-slot clearing; depth > 350 and orphan id
  rejection; existing object/media/sound/node tests.
- `save-test` (four real images): restore table counts, dialogue record count,
  selector/scroll, per-channel depth and current-record agreement.
- `variable-metadata-test` (real SHOW.LCV): builds a 3000-word pool
  `1000+i`, maps EA's 133 declared ids, and checks local 5 -> 8, 26 -> 74,
  133 -> identity, 299 -> identity, i.e. the values are `1008`, `1074`, `1133`,
  `1299` and not the raw local offsets.
- `scene-state-test`: the `0x75` underflow path now asserts the counter and the
  unchanged destination instead of a thrown error.

All three panorama verifiers still pass.

## 9. Verified export hashes

17 exports were re-hashed against `pseudocode/decompile-index.json`
(`sha256` recomputed from each `relative_path` on 25 October 2026); all matched
and all belong to analysis profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`:

```
0x407254 0x407420 0x4075fe 0x4077b2 0x407933 0x407ad5 0x4083d4 0x40892d
0x4089b0 0x408adc 0x40ac38 0x40ac43 0x40cf73 0x425ab4 0x42c8a0 0x4319b3
0x438173
```

No binary session was opened; no cleanup is pending.
