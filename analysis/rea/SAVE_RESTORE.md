# Save restore, `0xcc` and `0x70` CONVERSE

Bounded step of 25 October 2026. Scope: restore the four tables a
`savegame/NN.BTB` carries into `SceneState`, make `0xcc` a real object-argument
write, and make `0x70` a real `pc` replacement through the object's `+0x0c`
label. The carried variable pool, dialogue stacks, scene records, rendering,
audio/video, save writing and `engine\<ad>.ccb` asset resolution remain out of
scope for this step; the variable pool and dialogue window were added in the
next step, `SAVE_VARIABLES.md`.

Evidence base (saved exports only, no binary session opened):

- target `analysis/rea/BETRAYAL.EXE`, SHA-256
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
  (matches `pseudocode/decompile-index.json` `target_sha256`);
- analysis profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
  (matches every export header and `analysis_profile_digest`);
- save reader `FUN_00425ab4` and writer `FUN_00424dc0` as documented in
  `SAVE_FORMAT.md`; `port/include/byzantine/save.hpp` and `port/src/save.cpp`
  already parse the image;
- the two dispatchers: `0x42db22` (cases `0x6c`, `0x70`, `0xcc`) and `0x43340a`;
- `0x436652` (raw operand word reader), `0x4319b3` (variable resolver),
  `0x42d383` (builds `DAT_004654e0` from every `0xe1` node);
- `0x432d6e` (the nested conversation runner called by `0x70`), `0x410f09`,
  `0x437bcc`, `0x40c329`, `0x407c5a`, `0x40abb4`;
- shipped `copy2hd/savegame/*.btb` from CD1 for the data checks.

Terms: **observation** = read directly from the export or the shipped bytes;
**inference** = a conclusion that fits code and data but is not stated by
either; **unknown** = not recovered.

---

## 1. What `load_save` restores

`void load_save(SceneState&, const SaveGame&)` in `port/src/cc.cpp` replaces the
four 0x15e-slot tables, matching `0x425ab4`, which frees them through `0x40f3f0`
before reading a new image.

| Save block | `SceneState` target | Fields interpreted |
|---|---|---|
| 18, `DAT_00464300` (0x18) | `objects` | `+0x00/+0x02/+0x04/+0x06` rect, `+0x08` active, `+0x09` kind, `+0x0c` label, `+0x10` name id, `+0x14` argument |
| 22, `DAT_00464f60` (0x14) | `media` | `+0x00` value, `+0x04` frame, `+0x08` timestamp, `+0x0c` media id, `+0x10` active, `+0x11` type |
| 20, `DAT_00486220` (0x3c) | `sounds` | `+0x04/+0x08/+0x0c/+0x10/+0x14/+0x18/+0x1c/+0x20/+0x24/+0x28/+0x2c/+0x30` words, `+0x34` active |
| 17, `DAT_004649e8` (0x40 + tail) | `nodes` | marker, `+0x1c`, `+0x20`, `+0x2c`, `+0x30` and the extra list |

- **Observation** object `+0x0a` is *not* copied. `0x72` never writes it and the
  `0x18`-byte record comes from an un-zeroed `malloc`; `SaveObjectRecord` keeps it
  only as `uninitialized_0a`, and `SceneObjectState` has no such field.
- **Observation** media `+0x12` is *not* copied for the same reason: `0x6a`
  writes `+0x10` (`active`) and `+0x11` (`type`) only.
- **Observation** sound `+0x30` is stored as the file word; `0xffff` is the `-1`
  sentinel that `0x65`/`0xc1` use.
- **Observation** the node record's remaining bytes are the `0x40ffee`
  initialiser defaults, not file state (`SAVE_FORMAT.md` §3.8); only the copied
  fields and the reader's two extra dwords are modelled.
- **Inference** `media.timestamp` is a `timeGetTime()` value that `0x41dcc9`
  refreshes after a reload, so it is restored but not treated as meaningful
  gameplay state.

Not restored in this step: `DAT_0047e2f0` (the carried variable pool),
`DAT_00468cc0`/`DAT_00468d78` (dialogue depths/stacks), `DAT_00482428`
(`scene_index` under a running scene), the `DAT_00482ad0` per-scene records, the
sound chain and the dialogue chains. The variable pool and the dialogue depths/
stacks were restored in `SAVE_VARIABLES.md`; the dialogue chains in
`CHAIN_SAVE_STATE.md`.

## 2. `0xcc` — object argument write

`0x42db22` case `0xcc`:

```
sVar2 = FUN_00436652(*param_1, 0);
uVar3 = FUN_00436652(*param_1, 1);
*(uint *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 0x14) = (uint)uVar3;
```

- **Observation** both operands come from `0x436652`, which reads the encoded
  word. `0x4319b3` (the type-2 variable resolver) is *not* called, so a type-2
  operand here is still a literal slot/value.
- **Observation** there is no NULL check: an absent `DAT_00464300[slot]` is
  dereferenced. The port therefore reports `SceneTrace::missing_object` and
  stops; it never creates the record.
- **Observation** `uVar3` is `uint` assigned from `FUN_00436652`'s `short`
  return, so the encoded word sign-extends: `0x8000` becomes `0xffff8000`.
  Implemented and tested.
- The successful write is retained as `SceneTrace::object_writes`
  (`instruction`, `id`, `value`) so a trace can prove the branch ran.

## 3. `0x70` — CONVERSE

Original path in `0x42db22` case `0x70`:

1. `DAT_0047a280 != 0` takes the else branch (redraw only, no jump); otherwise
2. operand 0 (a raw `ushort` at `*param_1 + 10`) selects `DAT_0046e870`, is
   name-matched by `0x410f09` into `DAT_00485a10`, and that index selects
   `DAT_00482ad0`;
3. `0x437bcc` scans the record's `+0x720` list, `DAT_0046e824` is set, and
   `0x432d6e` runs the conversation until `FUN_0043688b` returns;
4. the runner's result (`local_28`) selects `DAT_00464300`; on `0xffff` nothing
   happens, on `0xfffe` the command returns 1, otherwise
   `*param_1 = DAT_004654e0[object->+0x0c]` and the command returns 2 (pc
   replacement).

- **Observation** `0x42d383` fills `DAT_004654e0` from every script node whose
  opcode is `0xe1`, indexed by its operand-0 word. That is exactly
  `byzantine::resolve_control_flow`'s label map.
- **Observation** the shipped saves confirm the dependency: every `+0x0c` of the
  CD1 object records is a label of the scene the save was written in
  (`SAVE_FORMAT.md` §3.9, 4,716/4,716).
- **Modelled simplification (inference)** the nested conversation runner
  `0x432d6e` and the name/record tables `DAT_0046e870`, `DAT_00485a10` and
  `DAT_00482ad0` are not reconstructed. The port takes operand 0 as the
  conversation's object id and performs the same `+0x0c` label lookup. This is
  the branch the original takes when its runner returns that object; it does not
  simulate the conversation UI or its return codes.
- **Not modelled (unknown)** `DAT_0047a280`'s gate. It is not part of
  `SceneState`; the port always performs the label branch.
- Outcomes: absent object → `missing_object`; `+0x0c` not a label of the script →
  `unresolved_object_label` (including labels above `0xffff`); success →
  `SceneTrace::converse_jumps` (`instruction`, `id`, `label`, `target`).

### Step-limit outcome

Because the conversation branch can return to the same label, a trace can now
loop. `trace_scene_label`'s 10,000-instruction bound used to throw
`CC trace step limit exceeded`; it now sets `SceneTrace::step_limit_exceeded`
and `stopped_at`, so the census reports `step_limit` instead of `error`. Five
shipped labels (IU 375/398, NB 240/242/243) reach this bound. They need the
dialogue/window state the original runner carries; the port does not invent it.

## 4. CLI

```
byzantine-cc <scene.cc> --trace-save <save.btb> <label> <cb>
byzantine-cc <scene.cc> --trace <label> <cb> --save <save.btb>
byzantine-cc <scene.cc> --census <cb> [<scene-cs>] --save <save.btb>
```

`--trace-save` prints `state_origin=save ... objects/media/sounds/nodes`, then
the normal trace output plus one `object_write`/`converse_jump` line per action
and the new outcome fields.

Manual reproduction needs the tables the census extracts (CS/CI/CY/CF splits,
`SHOW.LCI`, `SHOW.LCY`, the inventor name lists and `GAMEPREF.000`); without
them the trace stops on `unresolved_sound_name`/`unresolved_video_name`. The
Python census performs the extraction, splitting and `--save` choice
automatically; the acceptance runs below used the same inputs.

## 5. Acceptance results (shipped data)

Real save 26.btb (scene `ea`, CD1 `copy2hd/savegame`), label 1779:

```
state_origin=save file=analysis/rea/save_sample/26.btb objects=163 media=4 sounds=38 nodes=1
steps=19 stopped_at=633
converse_jump instruction=2246 id=3 label=544 target=628
background_id=5 name=EAPupAll
```

Real save 73.btb (scene `vt`), label 356:

```
state_origin=save file=analysis/rea/save_sample/73.btb objects=178 media=0 sounds=51 nodes=0
steps=9 stopped_at=382
object_write instruction=375 id=19 value=3
object_write instruction=377 id=18 value=4
object_write instruction=378 id=13 value=4
object_write instruction=379 id=5 value=9
object_write instruction=380 id=1 value=7
object_write instruction=381 id=2 value=7
```

`save-test` now also builds a `SaveGame` in memory and covers: table
replacement (not merge), every restored field, the two `0xcc` writes, the
missing-object path (no allocation), the `0x70` jump, the missing-object path,
the non-label path, and the `0xcc` sign extension. With the fixture files it
checks the restore against all four sample images and the real VT/EA traces
above.

`make -C port -j2 all test` passes. `analysis/rea/build_vm_census.py` now
extracts `copy2hd/savegame/*.btb` from CD1, picks one save per scene by
`DAT_00482428` (highest file name per scene, 18 scenes), and passes it as
`--save`; scenes without a save run with an empty object table, which is
reported as `missing_object`, never invented.

| Census | before | after |
|---|---:|---:|
| labels traced | 7,693 | 7,693 |
| completed (`return`) | 4,045 | **4,169** |
| unresolved variable | 2,891 | 2,939 |
| unresolved RNG | 25 | 25 |
| unsupported opcode | 732 | **481** |
| `0xcc` blocking | 194 | **0** |
| `0x70` blocking | 61 | **0** |
| missing object | — | 74 |
| unresolved object label | — | 0 |
| step limit (new outcome) | — | 5 |
| error | 0 | 0 |

New ranking: `0xd1` (93), `0x6a` (74), `0x73` (49), `0x8b` (47), `0x8a` (25),
`0xb4` (24), `0xb5` (24), `0xc8` (20).

## 6. Observation / inference / unknown summary

- **Observation:** `0xcc` reads both operands through `0x436652` and has no
  NULL check; `0x70` replaces pc through `DAT_004654e0[obj->+0x0c]`; `0x42d383`
  indexes that table by each `0xe1` operand; shipped object labels match their
  scene.
- **Inference:** operand 0 of `0x70` is a usable proxy for the conversation
  result in a static trace, because the original's final action after the runner
  returns is the same label lookup. `media.timestamp` is refreshed on load.
- **Unknown:** the conversation runner's own state machine and return codes, the
  meaning of `DAT_0047a280`, `DAT_0046e824`, the `DAT_00482ad0` `+0x720` list,
  the extra-list pairs, and everything the carried variable pool would supply.

## 7. Verified export hashes

22 exports were re-hashed against `pseudocode/decompile-index.json`
(`sha256` recomputed from each `relative_path` on 25 October 2026); all matched
and all belong to analysis profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`:

```
0x407420 0x407b9c 0x407c5a 0x408adc 0x40abb4 0x40c329 0x410f09 0x424dc0
0x425ab4 0x426261 0x426b9d 0x42722b 0x42d383 0x42db22 0x4319b3 0x432d6e
0x43340a 0x436652 0x437bcc 0x437cad 0x438173 0x438cd0
```

No binary session was opened; no cleanup is pending.
