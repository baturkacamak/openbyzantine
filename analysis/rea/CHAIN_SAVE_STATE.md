# Dialogue chains, node lists, save requests and the `0xc8` system command

Bounded step of 25 October 2026. Scope: `0x80` media-id write, `0x8a`/`0x8b`
node list commands, `0xbe`/`0xd2` save/autosave requests, `0xc6`/`0xc7` dialogue
chains, `0xc8` system/input sub-commands, `0xcb` panorama rotation and
`0xd4`/`0xd5` flag writes. Writing a save, audio, rendering and asset loading
remain out of scope.

Evidence base (saved exports only, no binary session opened):

- target `analysis/rea/BETRAYAL.EXE`, SHA-256
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
  (matches `pseudocode/decompile-index.json` `target_sha256`);
- analysis profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
  (matches every export header and `analysis_profile_digest`);
- `0x42db22` cases `0x80`, `0x8a`/`0x8b`, `0xbe`/`0xd2`, `0xc6`, `0xc7`
  (decimal `case 199`), `0xc8` (decimal `case 200`), `0xcb`, `0xd4`/`0xd5`;
- helpers `0x431b0f` (node guard/creation), `0x432b09` (extra-list append),
  `0x432c65` (+0x2c write), `0x40fe70` (node create), `0x42515f`
  (tempsave/autosave), `0x41d440` (gamepref), `0x427389` (status.000),
  `0x42a6aa`/`0x42a579` (input/timer), `0x402285` (panorama rotation),
  `0x408bb2` (CI name → dialogue id), `0x438173` (LCX reader);
- save reader `0x425ab4` and block 24 (`DAT_00486a10`).

Terms: **observation** = read directly from the export or the shipped bytes;
**inference** = a conclusion that fits code and data but is not stated by
either; **unknown** = not recovered.

---

## 1. Locating `0xc8`

`0xc8` has no `case 0xc8:` text because Ghidra prints that block as decimal
`case 200:`. Its real target was confirmed from the dispatcher's jump table:

```
0x431751: jmp DWORD PTR [eax*4 + 0x431758]   ; eax = opcode - 0x64
```

Reading the table at `0x431758 + (0xc8-0x64)*4` gives `0x42e643`, and the
export's `case 200:` block at that address is the same sub-switch. No behaviour
was guessed from the assembly; the pseudocode supplies the sub-command table.

## 2. `0x80` — media id

`DAT_00464f60[raw(0)]->+0x0c = raw(1)`. A missing slot takes `0x40abb4`; the
record is never allocated. The port counts `missing_media_requests` and records
`SceneTrace::media_id_writes`.

## 3. `0x8a`/`0x8b` — node list commands

- `0x8b` → `0x432c65`: `DAT_004649e8[raw(0)]->+0x2c = raw(1)`.
- `0x8a` → `0x432b09`: append one 0xc-byte list entry to `node->+0x34`:
  `entry+0x00 = raw(2)`, `entry+0x04 = resolved(1)`, `entry+0x08 = next`.
  The save's 8-byte `(first, second)` pair corresponds to offsets `+0x00` and
  `+0x04`, so the port stores `first = raw(2)`, `second = resolved(1)`.
- **Observation** both first call `0x431b0f(node, opcode)`. It creates the node
  through `0x40fe70` only when `DAT_0047dd70[node]` has type 0/1 and the node is
  NULL; a failed creation logs through `0x40ac43`/`0x40ac38` and returns 1.
- **Not modelled** `DAT_0047dd70` (the animation-name table) and `0x40fe70`.
  A node that the save did not provide is reported as
  `SceneTrace::unresolved_node_asset` and stops; the animation is not invented.
- **Observation** the extra list in the save is read as a control byte followed
  by 8-byte entries; appended entries get `control = 1` (a present marker). The
  save writer's count/reader control mismatch is documented in `SAVE_FORMAT.md`
  §3.8 and does not affect this in-memory path.

## 4. `0xbe`/`0xd2` — save/autosave requests

`0x42db22` cases `0xbe`/`0xd2` share the target `0x42e607`:

```
if (DAT_00460670 == -1) goto end;
if (FUN_0041d440(0) != 1) goto end;
FUN_0042515f(opcode);   // 0xbe: tempsave, 0xd2: autosave copy
```

- **Observation** `DAT_00460670` is the `examine` scene index set by `0x438173`
  (`-1` when the chapter has no `examine` entry); `gamepref[0]` must be **1**.
- **Observation** `0x42515f(0xbe)` formats a name and calls `0x424dc0` to write
  `savegame\tempsave.btb`; the other branch copies tempsave to `autosave.btb`.
- Save writing is a non-goal: the port records `tempsave_requests` /
  `autosave_requests` and continues, because the command has no script-state
  side effect. Without the engine scene list or gameprefs the trace stops with
  `unresolved_save_setting`.

## 5. `0xc6`/`0xc7` — dialogue chains (`DAT_00486a10`)

Block 24 is `0x68` bytes per present slot: 25 `u32` record ids at `+0x00` and a
count at `+0x64`; absent ids are `0xffffffff`.

- `load_save` now restores them into `SceneState::dialogue_chains` and rejects a
  count above 25 or an id without an allocated record.
- `0xc6` creates/reset-initialises the chain (count 0, 25 ids `0xffffffff`) and
  appends the resolved ids of operands 1..7: each is a raw CI name id mapped by
  `0x408bb2` (the port's `resolve_name_index`). It appends to an existing chain
  rather than resetting it, exactly as the original.
- `0xc7` resolves each operand and removes one matching id, shifting the tail
  down and decrementing `+0x64`. An absent chain is a NULL dereference in the
  original; the port reports `SceneTrace::missing_dialogue_chain` and stops.
- **Inference** removal is one id per operand because `0x408bb2` returns one
  record id per CI name.

## 6. `0xc8` — system/input sub-commands

The resolved operand 0 selects a sub-command (`0x1e..0x3a`):

| sub | original action | port |
|---|---|---|
| `0x1e` | `DAT_0045d1d0=0`; `0x42a6aa` reseeds `_rand` from local time | clears `dialogue_input_active`; reports `unresolved_random` and stops |
| `0x1f` | if operands 2/3/4 are type 2: `0x42a579` input query, write three variables through `0x46ee00` | `unresolved_system_input` and stop; no-op when the operand types differ |
| `0x20` | `0x42a6aa`; `DAT_0045d1d0=1` | sets `dialogue_input_active` |
| `0x21..0x31`, `0x34`, `0x35` | no-op | no-op |
| `0x32`/`0x33` | `DAT_00460668 = 1`/`2` | writes `field_460668` |
| `0x36..0x39` | `0x427389(sub)` | records `status_write_requested` |
| `0x3a` | `DAT_0045f490=2`, then `0x427389(sub)` | writes `field_45f490` and records the status request |

- **Observation** `0x42a6aa(0x1e)` is `GetLocalTime` → `FUN_00451cb0` →
  `DAT_00462ef8 = value` (the CRT `_rand` state) → `_rand()`. The resulting RNG
  stream is not reproducible, so the port must not continue as if it were.
- **Observation** `0x42a6aa(0x1f)` calls `0x42a579` and writes its two output
  words and the input id into three `DAT_0047e2f0` entries selected by the raw
  operands 2/3/4. That is the input/event triplet; the port stops instead of
  inventing it.
- **Unknown** `FUN_00429af7` (sub 0x20) and `FUN_0042913b` (sub 0x1e);
  `0x4273f4` (the status comparison inside `0x427389`).

## 7. `0xcb` — panorama rotation request

`0x42db22` case `0xcb` calls `0x402285(resolved(0), step)`. `step` is `10`
unless operand 1's type byte is `0x40`, in which case its constant word is used.
`0x402285` computes the angle increments and calls `0x401ee6` (the renderer);
the port records `ScenePanoramaRotation` instead.

## 8. `0xd4`/`0xd5` — `DAT_004859cd`

`0xd4` writes 1 and `0xd5` writes 0. `0x425ab4` also sets it to 1 after a
successful load, so `load_save` sets `flag_4859cd = true`. The exact consumer
meaning (panorama/display reload suppression is the current inference) remains
unrecovered; only the writes are modelled.

## 9. Save additions

`load_save` now also restores:

- `dialogue_chains` from block 24 (`count` + 25 ids, validated);
- `field_460668` from the mid-block dword (written by `0xc8` sub 0x32/0x33);
- `flag_4859cd = true` after a successful load.

`--save-dump` prints each chain (`count` and ids) and validates it through the
same `load_save` contract.

## 10. Acceptance (shipped data)

- NB save `57.btb`, label 202: `0xc6` creates chain 0 with one id:
  `dialogue_chain id=0 count=1 ids=20`; the save's existing chain 2 stays
  `count=2 ids=43,123`.
- NB save `57.btb`, label 257: `0xc7` targets chain 0, which the save does not
  contain (the label is traced without its earlier `0xc6`), so it reports
  `missing_dialogue_chain=0` — the original would have dereferenced NULL.
- VT save `73.btb`, label 1818: `system_command instruction=2272 sub=0x36`
  followed by `status_write_requested=54`, then the trace continues.
- The save-gate commands are exercised by the census (gamepref[0]=1 and an
  `examine` scene are present) without writing a file.

## 11. Census

| Census | previous step | this step |
|---|---:|---:|
| labels traced | 7,693 | 7,693 |
| completed (`return`) | 5,702 | **5,818** |
| unresolved variable | 1,278 | 1,302 |
| unresolved RNG | 238 | 245 |
| unsupported opcode | 289 | **58** |
| `missing_object` | 74 | 78 |
| `unresolved_node_asset` | — | 70 |
| `missing_dialogue_chain` | — | 2 |
| `scene_switch` | 107 | 115 |
| `step_limit` | 5 | 5 |
| `error` | 0 | 0 |

New ranking: `0x79` (22), `0x8d` (9), `0xd0` (8), `0x86` (7), `0xcf` (7),
`0xd3` (3), `0x7f` (2).

- The 70 `unresolved_node_asset` labels are `0x8a`/`0x8b` on nodes the save did
  not provide; creating them needs `DAT_0047dd70`/`0x40fe70`.
- The two `missing_dialogue_chain` labels are real static misses: the label is
  traced without the earlier `0xc6` that creates the chain.
- Small rises in unresolved variable/RNG are released labels reaching the next
  real read (including `0xc8` sub 0x1e reseeding the RNG).

## 12. Tests

`make -C port -j2 all test` and the three panorama verifiers pass.

- `scene-state-test`: `0x80` write/missing, `0xd4`/`0xd5`, `0x8b`/`0x8a` +
  missing node, `0xbe`/`0xd2` gate (present, gamepref 0, unknown), `0xc6`/
  `0xc7` append/remove/missing, `0xc8` sub-commands (0x32/0x33/0x36/0x1e/0x1f/
  no-op), `0xcb` rotation with explicit and default steps.
- `save-test`: dialogue-chain restore, `field_460668`, `flag_4859cd`, plus the
  existing table/dialogue checks; four real fixtures.
- `records-test` and `variable-metadata-test` are unchanged and pass.

## 13. Observation / inference / unknown

- **Observation:** the exact field writes and call arguments above; the `0xc8`
  sub-command table; the `0xbe`/`0xd2` gate; the LCX-backed `0x73` from the
  previous step.
- **Inference:** `0x8a`/`0x8b`'s entry offsets; `DAT_004859cd`'s meaning;
  `0xc8` sub 0x20's `FUN_00429af7` purpose.
- **Unknown:** `DAT_0047dd70`, `0x40fe70` frame data, `0x42a579` input triplet,
  `FUN_00429af7`, `FUN_0042913b`, `0x4273f4`, and the status/autosave file
  contents (writing is out of scope).

## 14. Verified export hashes

18 exports were re-hashed against `pseudocode/decompile-index.json` (`sha256`
recomputed from each `relative_path` on 25 October 2026); all matched and all
belong to analysis profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`:

```
0x402285 0x408bb2 0x408c81 0x40abb4 0x40fe70 0x41d440 0x42515f 0x425ab4
0x427389 0x42a579 0x42a6aa 0x42c8a0 0x42db22 0x4319b3 0x431b0f 0x432b09
0x432c65 0x438173
```

No binary session was opened; no cleanup is pending.
