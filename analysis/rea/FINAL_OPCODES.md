# Final opcode blockers — `0x79`, `0x7f`, `0x86`, `0x8d`, `0xcf`, `0xd0`, `0xd3`

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Analysis profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
(Ghidra 12.1.4). Evidence is the saved per-function exports plus focused
`objdump -d` of the target; no original execution and no binary session.

Result: **every opcode that a shipped scene entry label actually reaches now has
a `port/` branch.** The census blocking ranking is empty (58 labels → 0), and the
seven opcodes below either execute or stop with a named state boundary.

## 1. Observation — dispatcher table and case addresses

`0x42db22` switches on `*(u8*)instruction` through
`jmp DWORD PTR [eax*4+0x431758]` after `sub eax,0x64; cmp eax,0x71`
(`0x431737`..`0x431751`). Reading the 114 dwords at `0x431758` gives every case
address; the ones in this step:

| Opcode | Case | Callee |
| --- | --- | --- |
| `0x79` | `0x431063` | inline (`DAT_0045f490`) |
| `0x7f` | `0x430ce2` | inline (`DAT_00464f60`, `0x46ee00`) |
| `0x86` | `0x42de58` | `0x431b0f` → `0x43299d` → `0x432904` |
| `0x8d` | `0x430f16` | `0x4319b3` → `0x431a69` → `0x40c7b4`; then `0x409513` |
| `0xcf` | `0x42f882` | `0x427a01` per type-2 operand |
| `0xd0` | `0x42f878` | `0x4278ab` |
| `0xd3` | `0x42f6c9` | `0x428cbd` (nine arguments) |

## 2. Observation — behaviour per opcode

### `0x79` — end of script

```
431063  xor  eax,eax
431065  mov  al,BYTE PTR [ebp+0xc]     ; param_2 (sub-script call mode)
431068  test eax,eax
43106a  jne  0x43108d                  ; called: leave local_8 = 0xffff
431070  mov  WORD PTR [ebp-0x4],1      ; local_8 = 1
431076  cmp  DWORD PTR ds:0x45f490,0
43107d  jne  0x43108d
431083  mov  DWORD PTR ds:0x45f490,1
```

* **Observation** the case writes `DAT_0045f490 = 1` **only when it is 0**, and
  sets the dispatcher result to `1` in main-script mode.
* **Observation** in the caller `0x42d594` (`FUN_0042db22(&DAT_0047a230,'\0')`),
  result `1` takes `DAT_00484e28 = DAT_0047a230; DAT_0047a230 = NULL`, i.e. it
  saves the resume instruction and ends the run. Result `0xffff` would advance
  to the next instruction.
* **Inference** `0x79` is "return to the caller script": `0x41e289` restores
  `DAT_0047a230 = DAT_00484e28` in its `param_1 == 6` branch, and `0xb2`/`0xb3`
  (`0x43194f`) push a return address the same way.
* Port: `SceneState::field_45f490` is written only from 0 and
  `SceneTrace::script_end_requested` stops the trace at that instruction.
  `script_end` is a terminal outcome, not an unsupported opcode.

### `0x7f` — media value − frame into a scene variable

```
430cfe  mov  eax,DWORD PTR [eax*4+0x464f60]   ; media record
430d05  mov  eax,DWORD PTR [eax]              ; dword +0x00
430d17  sub  eax,DWORD PTR [ecx+0x4]          ; dword +0x04
430d1a  mov  WORD PTR [ebp-0x10],ax           ; truncate to 16 bits
430d33  cmp  DWORD PTR [eax*4+0x464f60],0     ; NULL check AFTER the load
```

* **Observation** operand 0 (raw `0x436652`) indexes `DAT_00464f60`; operand 1
  (raw) is the destination local, mapped through
  `0x46ee00[raw(1) + DAT_00482428*600]`. The stored word is
  `(dword +0x00 − dword +0x04) & 0xffff` — a 32-bit subtraction, not the
  `(short)-(short)` the decompiler prints.
* **Observation** the record is dereferenced **before** the NULL check, so an
  absent slot faults in the original; the `0x40abb4` log at `0x430d41` is only
  reached after a successful load.
* Port: `variables[raw(1)] = u16(value − frame)`; an absent record is the
  explicit `missing_media_record` stop, never a skipped write.

### `0x86` — node `+0x04` write and `+0x09` mask

* **Observation** `0x42de58` resolves raw operand 0, calls `0x431b0f(id, opcode)`
  (returns 0 when `DAT_004649e8[id]` exists, 1 after a failed `0x40fe70` asset
  creation) and only then `0x43299d(id, instruction)`.
* **Observation** `0x43299d` writes `node+0x04 = (short)0x436652(instr,1)` and
  then calls `0x432904(id, instr, 2)`: for operands **2..7** whose type byte is
  `2`, `+0x09 = (and[raw] & +0x09) | or[raw]` with the `0x460418`/`0x460428`
  tables already recovered in `ANIMATION_RECORD.md`.
* **Observation** in the shipped scripts `0x86` always uses a constant operand 1
  and never a type-2 mask: 23 instructions — 14×`0`, 3×`5`, 2×`10`, 2×`1`, 2×`2`.
* Port: the record gained `field04` and `flags` (defaults `10`/`0x20` from
  `0x40ffee`) restored from the save's `0x40`-byte record (`raw[4..5]`, `raw[9]`,
  both in `0x426b9d`'s copy list). A record the save does not provide keeps the
  existing `unresolved_node_asset` outcome, and every write is recorded in
  `SceneTrace::node_writes`. (The next step merged this record into the single
  `SceneState::records` table and made existence a CA-asset decision; see
  `RECORD_TABLE.md`.)

### `0x8d` — CD request

* **Observation** `0x42db22` resolves operand 0 with `0x4319b3`, calls
  `0x431a69(value)`, stores its result as the dispatcher result, and then calls
  `0x409513(instruction)` unconditionally.
* **Observation** `0x431a69` → `0x40c7b4(disc)`: for `disc == 7` it first tries
  `0x40c6d6(1)` and returns `0xffff` with `DAT_0045da78 = 1` when CD 1 is
  present, otherwise retries as disc 2. `0x40c7b4` enumerates CD-ROM drives
  (`0x40c911`), verifies the volume (`0x40c9ce` → `0x40c70e`), sets
  `DAT_0045da78`, and can show the "Now accessing CD %d" box. It returns 0 on
  success and 1 when the retry loop ended (user cancelled).
* **Observation** success → `0xffff` (the caller continues); failure → `1` (the
  caller ends the script) with `DAT_0045f490 = 3`, or `1` for the disc-7
  fallback with `DAT_0045f490 = 1`.
* **Observation** `0x409513` is the debug-path instruction logger: it runs only
  when `DAT_00485c6c != 0` and calls `0x409457(&DAT_0045d3b8,1)` /
  `0x40a4bc`/`0x40a40d` / `0x4094f2`, which print through `0x4599c0`.
* **Unknown** which CD is mounted is host state. Port: `disc_request` records
  the resolved disc and stops (`unresolved_disc_request`).
* **Data** the opcode is only used as a disc selector: `MAP` `0x8d` ×6 with
  constants `1..6` and `START` ×1 with `7` (7 instructions, reached by 9 census
  labels).

### `0xcf` / `0xd0` — release and stop-all

* **Observation** `0xcf` loops `i = 0..7`, reads the type byte at
  `instruction+1+i`, and for type `2` calls `0x427a01(raw word at
  instruction+0xa+4i)`. `0x427a01` creates a `0x5c`-byte playback session only
  when `0x428143(id) == 0` ("not playing"), `DAT_00485c90[id] != 0` (a
  DirectSound object created by `0x427de1` → `0x422b04`/`0x4283a6`) and
  `sound+0x14 != 0`. It then copies 15 dwords of the sound record, sets
  `DAT_00485c90[id] = 0` and `sound+0x34 = 0`.
* **Observation** `0xd0` calls `0x4278ab`, which walks the `DAT_0046019c` session
  list, stops and frees each entry and sets the head to `NULL`.
* **Unknown / non-goal** the DirectSound objects. The port records one
  `SceneSoundRelease {id, record_present, field14}` per type-2 operand and counts
  `sound_stop_all_requests`; it does not pretend the device gate was evaluated.
* **Data** `0xcf` ×5 instructions (`VC` id `1`; `VT` ids `0`, `11`, `10`, `0`),
  `0xd0` ×7 (`CLOCK` 1, `HSTMED` 1, `MZ2` 3, `VOICE` 2).

### `0xd3` — timed sound/dialogue session

* **Observation** `0x42f6c9` calls
  `0x428cbd(raw(0), 0, -1, resolved(1), raw(2), p6, p7, p8, p9)`. `p6`/`p7`
  come from operands 3/4 only when operand 3 has type byte `0x40`, `p8`/`p9`
  from operands 5/6 only when operand 5 does; the unset defaults are
  `0, 0xffffffff`.
* **Observation** in `0x428cbd`, `local_34[0..2] = {p4, p6, p8}` are
  millisecond timeouts (`local_34[3]` is the run flag) and
  `local_10[0..2] = {p5, p7, p9}` are handed to
  `0x431ec2(id, 0x66, 0)` when their timeout elapses. `0x431ec2` is the
  scene/panorama transition loader (transition codes `0x64`–`0x6a`, `100`).
  `param_2 = 0` also runs the dialogue loop through `0x43c8d2`.
* **Observation** the `+0x18`/`gamepref[2]` gate is the same one `0xd1` uses:
  `record+0x18 == 0 && gamepref[2] == 0` → return `0xffff` without playing.
* **Data** `0xd3` ×6: `AM` `(24, 15000, 18, 18000, 19)` ×2, `HSLOWER`
  `(24, 19000, 2)` ×2, `HSUPPER` `(5, 14000, 5)` ×2.
* Port: `SceneSoundSession` retains the id, the three (timeout, follow-up)
  slots, the record and the gate. The `0x431ec2` transition is not executed.

## 3. Corrected port defect — `0x64` writes `+0x09`, not `+0x08`

`objdump -d` at `0x43251d` is `mov BYTE PTR [ecx+0x9],al`, and the guard at
`0x4324ea` loads `BYTE PTR [eax+0x9]` and tests `0x20`. `ANIMATION_RECORD.md`
had recorded `0x64` as writing the `+0x08` kind byte; the port therefore kept
`+0x09 = 0x20`, which also meant `0x84`'s `+0x09 & 0x20` trail guard never
cleared and `0x85`'s masks started from `0x20` instead of the kind bit.

Fixed: `0x64` writes `flags` (`+0x09`), `0x95` remains the only `+0x08` writer,
and the scene-state test now expects `flags = 0x03` and one `0x84` trail entry
`{100, 70}` for the same script (the original records the pre-write position
once bit `0x20` is cleared). `ANIMATION_RECORD.md` carries the correction.

## 4. Known limitation (RESOLVED in `RECORD_TABLE.md`)

The original has **one** `DAT_004649e8` table for actors and nodes: `0x64`,
`0x84`, `0x85`, `0x91`, `0x95`, `0x86`, `0x87`..`0x8b` all dereference the same
0x40-byte record. This step still kept `SceneState::actors` (created on demand)
apart from `SceneState::nodes` (restored from the save), so a script that mixed
`0x85` and `0x86` on one id was modelled on two records.

Resolved by the next step: the tables are merged into `SceneState::records` and
record existence is decided by the shipped `engine/<scene>ca` asset table
(`--ca`), as `0x40fd7b`/`0x431b0f` do. See `analysis/rea/RECORD_TABLE.md`.

## 5. Real traces (shipped data)

Engine tables extracted from CD1 by `build_vm_census.py`'s member list; saves
from `copy2hd/savegame`.

| Command target | Result |
| --- | --- |
| `AM` `--trace-save 66.btb 1010` | `steps=2 stopped_at=1249 script_end_requested=true` |
| `AM` `--trace-save 66.btb 1183` | `sound_session instruction=1484 id=24 record=true allowed=true timeout0=15000/18 timeout1=18000/19`, then returns at 1486 |
| `HSTMED` `--trace-save 55.btb 259` | `node_write instruction=279 id=48 value04=1 flags=36`, `instruction=280 id=50 value04=1 flags=36`, returns at 283 |
| `NB` `--trace-save 57.btb 556` | `unresolved_node_asset=9` (57.btb carries `nodes=0`) |
| `STAR` `--trace 314` | `unresolved_node_asset=10` (no shipped save) |
| `MAP` `--trace-save tempsave.btb 134` | `disc_request=1` |
| `VC` `--trace-save 60.btb 133` | `sound_release instruction=137 id=1 record=true field14=1`, returns at 138 |
| `CLOCK` `--trace-save 71.btb 504` | `sound_stop_all_requests=1`, returns at 639 |
| `EA` `--trace-save 26.btb 757` | `missing_media_record=5` (26.btb has 4 media records) |

`HSTMED` node `+0x09 = 36 (0x24)` and `+0x04 = 1` come from the save bytes
(`raw[9]`, `raw[4..5]`), verified with a small reader against `55.btb`.

## 6. Census (44 scenes, 7,693 labels)

`python3 analysis/rea/build_vm_census.py`:

| Outcome | Before | After |
| --- | ---: | ---: |
| `return` | 5,818 | 5,829 |
| `script_end` (new) | — | 22 |
| **completed** | **5,818** | **5,851** |
| unsupported opcode | 58 | **0** |
| `unresolved_variable` | 1,302 | 1,306 |
| `unresolved_random` | 245 | 245 |
| `missing_object` | 78 | 78 |
| `unresolved_node_asset` | 70 | 76 |
| `scene_switch` | 115 | 119 |
| `step_limit` | 5 | 5 |
| `missing_dialogue_chain` | 2 | 2 |
| `unresolved_disc_request` (new) | — | 9 |
| `missing_media_record` (new) | — | 2 |

Blocking-opcode rows: `0x79` 22, `0x8d` 9, `0xd0` 8, `0x86` 7, `0xcf` 7,
`0xd3` 3, `0x7f` 2 → **none**. The four extra `unresolved_variable` and four
extra `scene_switch` labels are released labels reaching the next state
boundary. Opcodes with a `port/` branch: 70 → **77 of 155**. Every stop is now a
named state boundary: carried variables (1,306), RNG seed (245), object table
(78), node asset (76), instruction bound (5), scene switch (119), media record
(2), dialogue chain (2), CD request (9), script end (22), return (5,829).

Per-scene rows that changed: `AM` 1 `script_end`, `BC` 3, `EA` 5 +
1 `missing_media_record`, `FT` 5 + 1, `GC` 1, `IUOVER` 2, `NB` 4 + 3
`unresolved_node_asset`, `MR` 1, `MAP` 6 `unresolved_disc_request`, `START` 3,
`STAR` 2 `unresolved_node_asset`, `HSTMED` 2.

## 7. Tests

`make -C port -j2 all test` passes (scene-state, variable-metadata, panorama,
records, save with four fixtures). New scene-state cases:

* `0x79` sets the flag and stops; a pre-existing `3` is not overwritten.
* `0x7f` writes `60` for `100 − 40`, `0xffff` for `0 − 1`, and stops with
  `missing_media_record` without writing.
* `0x86` writes `+0x04`, masks `+0x09` (`0x20` → `0x21`), records
  `node_writes`, and stops on an absent node.
* `0x8d` records the resolved disc; an unresolved operand is
  `unresolved_variable`.
* `0xcf` records one release per type-2 operand (and only those); `0xd0` counts.
* `0xd3` keeps `timeout0=15000/18`, `timeout1=18000/19`, no third slot, and
  `allowed=true` from `+0x18`.

The three panorama verifiers pass unchanged
(`verify_panorama_bindings`, `verify_panorama_camera`,
`verify_panorama_projection`).

## 8. Verified inputs

Target and profile as above; all 20 saved exports re-hashed against
`analysis/rea/pseudocode/decompile-index.json`:

| Address | SHA-256 (first 16) |
| --- | --- |
| `0x42db22` | `8df50fc5339a585b` |
| `0x431a69` | `c2404c4cc8c7f3eb` |
| `0x40c7b4` | `a6ef5837f169597f` |
| `0x40c9ce` | `92129e7865232432` |
| `0x431b0f` | `f229bde142bc8a3a` |
| `0x43299d` | `68e9e51fdfcfb781` |
| `0x432904` | `38a13d007cb9f198` |
| `0x427a01` | `ee7f1168e6b33c1c` |
| `0x4278ab` | `0131d714134b6a0d` |
| `0x427de1` | `551daf477744ef8c` |
| `0x428143` | `d3d67f849fbfa5ab` |
| `0x428cbd` | `8048e3e4dffeab96` |
| `0x431ec2` | `1bc7fa6a2fe3fc6f` |
| `0x42d594` | `546a5c43ede36e64` |
| `0x40ffee` | `16955dd55a53620b` |
| `0x4324b0` | `a72a56ea515a1d8b` |
| `0x409513` | `54be899fee83bcbd` |
| `0x41e289` | `a89ac0f7d4449865` |
| `0x40c6d6` | `9426f7ccccce43ef` |
| `0x40c70e` | `775757b8f089c723` |

Assembly ranges read from the target (objdump, `-M intel`): `0x431737`–`0x431751`
(dispatcher table), `0x431063`–`0x43108a` (`0x79`), `0x430ce2`–`0x430da6`
(`0x7f`), `0x42de58`–`0x42debb` (`0x86`), `0x430f16`–`0x430f5a` (`0x8d`),
`0x42f878`–`0x42f8ea` (`0xd0`, `0xcf`), `0x42f6c9`–`0x42f7ce` (`0xd3`),
`0x4324b0`–`0x4325ce` (`0x64` `+0x09` write and guard).

## 9. Commands

```sh
make -C port -j2 all test
python3 analysis/rea/build_vm_census.py          # 44 scenes, blocking_total 0

E=<extracted copy2hd tree>; S=$E/copy2hd/savegame
port/byzantine-cc port/output/cc_all/AM --trace-save $S/66.btb 1010 port/output/cb_all/AM \
  --engine $E/copy2hd/engine --lcx $E/copy2hd/engine/SHOW.LCX \
  --lci $E/copy2hd/engine/SHOW.LCI --lcy $E/copy2hd/engine/SHOW.LCY \
  --inventor $E/inventor.txt --hotitems $E/hotitems.txt \
  --gameprefs $E/copy2hd/goldsave/GAMEPREF.000 --co port/output/co_all/AM \
  --ci $E/ci/AM --cy $E/cy/AM --cf $E/cf/AM
# steps=2 stopped_at=1249 script_end_requested=true

port/byzantine-cc port/output/cc_all/HSTMED --trace-save $S/55.btb 259 port/output/cb_all/HSTMED \
  ...same tables...
# node_write instruction=279 id=48 value04=1 flags=36
```

## 10. Non-goals kept

Audio output, DirectSound session objects, AVI/JPEG decode, the `0x431ec2`
transition, CD-ROM detection, the debug logger (`0x409513`), save/status writes,
rendering, and unifying the actor/node tables.
