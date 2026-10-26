# Cross-scene travel (`0x73`) and the carried variable pool

Status 26 October 2026. Scope: a hotspot action that ends in `0x73` now loads
the target scene, runs its entry and keeps the one piece of state the original
carries across a scene switch. Verified pair: **MAP ↔ MR** (map → morris), both
directions driven by the shipped hotspot tables and a shipped savegame.

Target: `BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
(`analysis/rea/pseudocode/decompile-index.json`). All function hashes below are
the first 16 hex digits of the indexed export SHA-256.

## 1. What `0x73` actually does

`0x42db22` (`8df50fc5339a585b`) case `0x73`:

```
uVar3 = FUN_00436652(*param_1,0);          // raw operand 0
uVar4 = (ushort)DAT_00482460[uVar3];       // 50-entry LCX table
if (param_2 == 0 && DAT_00482460[uVar3] != 0xffff) {
    local_8 = 0;                           // dispatcher result = 0
    DAT_00482428 = uVar4;                  // the target scene index
    if (DAT_004603d0 != NULL) {            // transition snapshot (not modelled)
        DAT_00482428 = (ushort)*DAT_004603d0;
        DAT_00460660 = 1;
    }
}
```

- `0x42db22` returns `local_8`; a valid table entry returns **0**, an
  initialised `0xffff` entry leaves `0xffff`.
- `0x42d594` (`546a5c43ede36e64`) maps result `0` to `DAT_0047a230 = NULL`, then
  returns 0. Result `0xffff` continues with the next instruction.
- `0x42c8a0` (`6d09fd55ad59f5e1`, the scene loop) repeats its inner
  `do { … } while (result == 0xffff || result == 2)` only for `0xffff`/2, so a
  0 result leaves the inner loop. The outer `while (result == 0)` then reloads
  the scene selected by `DAT_00482428`:
  `FUN_0044ba00(&DAT_0046e260, &DAT_00483bc0 + DAT_00482428 * 9)` (36 bytes per
  entry) followed by `0x412d35`, `0x4124d9`, `0x412f77`, `0x408662`, `0x40f3f0`,
  `0x40f4e7`, the CC file read, `0x40f3b0`, `0x42d52f`, `0x40fd7b`, `0x4117a6`,
  `0x410607`, `0x42d383`.
- `DAT_00483bc0` is filled by `0x438173` (`aad99d1a46f68fdc`) from
  `engine/<chapter>.lcc`: one lowercased name per line, indexed by
  `DAT_00482528`. **So the LCX value is a SHOW.LCC scene index**, which is how
  the port turns it into a scene name (`VariableMetadata::scenes[index]`).
- The dispatcher's entry address: `0x42d594` does `DAT_0047a230 = param_1` when
  its second argument is not 2, and `0x42c8a0` passes `DAT_0047a238`, the
  instruction-list head. A save only resumes through `0x41e289`
  (`a89ac0f7d4449865`) on the first loop iteration (`local_14`); every later
  iteration, including the one after `0x73`, starts at the **first instruction**
  of the reloaded script. The first instructions of every shipped scene are
  `0x68` and the `0x72`/`0x7a` hotspot declaration pairs, which is why a label
  trace never sees them.

## 2. What crosses a scene switch, and what does not

`0x42c8a0` clears the 3000-word pool `DAT_0047e2f0` (and `DAT_00477350`,
`DAT_0047ae80`, `DAT_00485a10`, `DAT_00482540`, `DAT_00464888`) **once**, before
the outer scene loop. `DAT_0047e2f0` is therefore the state that survives.

Everything else is rebuilt per load:

| Table | Freed / rebuilt by | Evidence |
| --- | --- | --- |
| `DAT_00464300` objects | `0x40f3f0` (`070b3b5fc5f201bc`) in the loop | frees `+0x00` slots 0..0x15d |
| `DAT_00486220` sounds | same function | frees, then `0x4117a6` (`7a50d19486b6ed83`) re-creates them from the new script's `0x65` instructions |
| `DAT_00486a10` dialogue chains | same function | frees |
| `DAT_00464f60` media | same function | frees |
| `DAT_004649e8` records | `0x40fd7b` (`a994c5ad96f2be05`) → `0x4102ae` (`6f7755ef3a8edced`) | frees the table, then pre-loads CA type 2/3 |
| `DAT_00486f90` dialogue records | `0x408662` (`b0030e9408f8f970`) in the loop | one 0x18-byte record per SHOW.LCI name, `+0x00 = 0` |
| `DAT_004654e0` label map | `0x40f4e7` + `0x42d383` | per script |

`0x42c8a0` initialises `DAT_00464878` (alternate mode) to `-1` once as well;
`0x68` writes it per scene (see §4).

The per-scene local→global row is `DAT_0046ee00[scene*300 + local]`: 300
identity entries per scene, overwritten for the first `count` entries by the LCV
ids (`0x438173`). `0x4319b3` resolves a type-2 operand through that row and every
variable opcode reads/writes `DAT_0047e2f0` through it. A switch therefore
carries the **pool**, not the local numbering: the same global is a different
local number in the next scene (MAP local 18 ↔ MR local 18 → global 10; MAP
local 25 → global 2).

## 3. Implementation

- `variables.hpp/cpp`: `export_variable_pool(state, layout)` and
  `apply_variable_pool(state, pool, layout)`.
  - `SceneState::variable_pool` holds the carried 3000-word `DAT_0047e2f0`.
  - `SceneState::variables` stays the current scene's 300-local projection (the
    key the VM already uses for type-2 operands).
  - `load_save_variables` now delegates to `apply_variable_pool`, so the save's
    whole 3000-word block becomes the carried pool, not just the 300 locals.
- `SceneState::variable_writes` records the local operands the script stored, in
  order (`0x67`, `0x74`, `0x75`, `0x7d`, `0x7f`). Export replays them over the
  arrival pool. This matters because `0x46ee00` can map a declared local and an
  identity local onto one global (MAP local 3 → global 126 and identity local
  126 → global 126): a blind projection would let the untouched identity copy
  overwrite the value the script wrote.
- `cc.cpp`: `trace_scene_from(script, entries, state, start_pc, budget)` is the
  single runner; `trace_scene_label` resolves a label first and the new
  `trace_scene_entry` starts at instruction 0 (the `0x73` path).
- `play_main.cpp`: `plan_hotspot` accepts a probe that ends in `0x73`
  (`plan.travel`), `follow_switches` performs the engine's outer loop (bounded to
  8 hops, reported), and `travel_to` does:
  1. resolve the index through SHOW.LCC (`--engine`), refusing an out-of-range
     index without inventing a name;
  2. derive the target's `cc/cb/co/ci/cy/cf/cs/ca` files from the current ones
     (same directories, case-insensitive stem) and drop `--save`;
  3. export the pool and import it into the target's LCV row;
  4. keep `panorama_size` (0x401256 fixes the node size once) and the current
     node object, as `DAT_00464880` survives a scene load;
  5. pre-load the CA type 2/3 records (`0x40fd7b`), run the new scene from
     instruction 0 through the same audio-gated runner as a click, then
     compose/present and print `travel …` / `entry …`.
- `byzantine::trace_blocker` (cc.cpp) is the single success/failure classifier for
  an action probe (§6); `SceneCompose` gained `skipped_snap`/`skipped_hidden`, and
  `compose_scene` applies `0x43b216`'s `+0x09 & 0x20` / `& 0x40` gate (§7).
- `0x68` and `0x7a` became real branches: `0x68` writes `DAT_00464878` (0 when
  operand 1 is a type-2 zero, else -1) and counts `alternate_mode_changes`;
  `0x7a` is a dispatcher no-op (case `0x7a` only copies operand 0 into scratch,
  the rectangle is `0x72` and the enable is `0x6d`/`0x6e`).

## 4. Why MAP ↔ MR

The shipped transition graph (LCX value in brackets), scanned from all 44 CC
files, includes MAP → am(2), as(5), bb(6), bc(7), gc(13), gt(18), hl(21),
iu(9), mr(27), nb(3), ea(11), sp(34), sm(33), tk(32), ft(17), vb(25);
MR → map(25) at labels 96/160/265/430. Both scenes have shipped savegames.

- MAP entry (`label 87` save state, then instruction 0 on a later visit) enables
  the room hotspots 34..49. Clicking 47 (rect `163,258 58x85`) runs action 246,
  which enables the exit hotspot 13 on the same rectangle; clicking it runs
  action 257 → `0x73` index 27 (mr).
- MR entry runs 100 instructions, loads `backs/mr/MRDOORCL.jpg` and enables
  hotspots 38 and 40; clicking 38 (rect `539,300 100x100`) runs action 96 →
  `0x73` index 25 (map).

Scenes whose entry stops on a missing RNG seed (`unresolved_random`, e.g. IU,
NB, EA) were rejected for this pair: the CRT `_rand` state at `0x462ef8` is
seeded once at startup (`0x415a4e` → `0x44d060(0xbeef)`) and advanced by every
`0x7d`, so a savegame cannot reconstruct it. The port keeps reporting
`unresolved_random` instead of inventing a stream.

## 5. Real results

Headless round trip (three clicks; `SDL_VIDEODRIVER=dummy`):

```
click_at 190,300 -> 190,320 slot 47 action 246 click_result steps=6 sounds=0 fingerprint=0xd39e636e06abe575
click_at 190,300 -> 190,320 slot 13 action 257 scene_switch=27 click_result steps=9 dialogue_visible sounds=0 fingerprint=0xaeeecd8e12a72634
travel index=27 scene=mr from=MAP pool=3000 locals=300 pool_changed=1 (2) from_locals_digest=0xc22762db55d8c365 pool_digest=0x7c4d665dc04d88b7 locals_digest=0x23569faa90b9e0fb
entry scene=MR steps=100 stopped_at=89 hotspots=2 records=0 locals_digest=0xee2eed4267652f29
click_at 589,330 -> 589,350 slot 38 action 96 scene_switch=25 click_result steps=14 sounds=0 fingerprint=0xce498ba10ab168e4
travel index=25 scene=map from=MR pool=3000 locals=300 pool_changed=4 (10,79,80,718) from_locals_digest=0x4543b0f6ebe4b36c pool_digest=0xd290910988b7940c locals_digest=0xe6aae5f275869f89 record_preload=17
entry scene=MAP steps=158 stopped_at=137 hotspots=11 records=17 locals_digest=0xa5a85cb2a2915aea
composed background=MAPFINM1.jpg drawn=1 skipped_no_frame=0 skipped_snap=16 skipped_hidden=0 pixels=3373 drawn_ids=0
```

- `pool_changed` is the number of the 3000 pool words that differ from the pool
  the scene received when it was loaded. MAP's own visit (label 87 + the two
  clicks) wrote **1** word (global 2 = 19, MAP local 25). MR's entry and its
  return action wrote **4** (globals 10, 79, 80, 718), i.e. **2996/3000 pool
  words are bit-identical** across the round trip.
- Global 10 is shared (MAP local 18 = MR local 18). MR read the 1 MAP had written
  and stored 0; MAP's arrival projection consequently reads 0 for local 18
  before its own entry runs. Global 2 (MAP's own value 19) is untouched by MR and
  still 19 on return.
- MAP's entry then writes its own locals (18 → 1, 25 → 19→20): the shipped
  script increments a visit counter, exactly as the original would on reload. That
  is scene logic, not carry loss.
- The MAP frame after the return draws 1 record / 3373 pixels (`maphl`) from the
  17-record preloaded table, while resuming `tempsave.btb` draws 11 / 62,263
  pixels from its 27-record saved table (§7 has the full comparison and the
  reason: the 10 room records only exist in the save; the 16 inactive `…Hi`
  preloads are hidden by the `0x20` flag in both cases).
- Interactive run (same clicks through the mouse handler, `SDL_AUDIODRIVER=dummy`)
  prints the same numbers and keeps the window alive; the MR frame is
  `composed background=MRDOORCL.jpg`.
- Scene layer after a switch: the node object in `DAT_00464880` survives, but the
  visible layer comes from the new scene's own entry trace, so a background
  (`0x69`) replaces a node carried from another scene. Verified with MAP → AM
  (panorama) → MAP: after the return, the click test uses the plain screen point
  and the frame is `composed background=MAPFINM1.jpg`. A label *inside* one scene
  that loads a background without a node load still keeps the current node,
  which preserves the documented EA 602 click frame
  (`composed panorama=nodes/ea/EA00.jpg … yaw=180`). Reaching AM's return hotspot
  85 needs the camera turned (it is not in the entry view at yaw 180); the check
  used `--staged-x 1148`.

Regression checks kept: MAP 87 `--trace-save` → `steps=70 stopped_at=137`
`tempsave_requests=1`; MAP `--click 34` → action 269, 6 steps, 12 active slots;
VT 72.btb 497 with audio → `gate stopped_after_step=4 of=15`,
`audio step=4 id=35 name=voTins1 duration_ms=4709`, and `--click 14` →
`click_result steps=12 dialogue_visible text=1 sounds=1
fingerprint=0x849a49bff1a28319 active_slots=2`; EA 602 panorama →
`panorama_load instruction=317 staged_x=none yaw=180`,
`click_at 246,294 -> 2423,496 slot 4 action 586 steps=11`. The EA frame now
reports `drawn=0 skipped_snap=1` (its only record, `EACaset2`, keeps the `0x20`
flag), and MAP `--click 34` draws 12 records / 70684 pixels (`mapTkHi` is
activated).

## 6. A failed click is not a success

`plan_hotspot` runs the action on a copy and only commits when the probe is a
real end. The original classifier named eight reasons; a click whose label
stopped on `unresolved_dialogue_channel` (e.g. a missing `--cy`), an
`unresolved_save_setting`, an `unresolved_video_*`, a `disc_request` or a
`step_limit` was reported as a success and its partial state was committed.
`byzantine::trace_blocker` (cc.cpp, tested in `scene-state-test`) now classifies
`SceneTrace` in one place:

- a blocker: `unsupported_opcode`, `unresolved_variable`, `unresolved_random`,
  `unresolved_sound_name`, `missing_object`, `unresolved_object_label`,
  `unresolved_dialogue`, `unresolved_dialogue_channel`,
  `unresolved_dialogue_inventory`, `unresolved_video_name`,
  `unresolved_video_setting`, `unresolved_node_asset`, `missing_node_asset`,
  `disc_request`, `missing_media_record`, `missing_dialogue_chain`,
  `unresolved_system_input`, `unresolved_save_setting`,
  `unresolved_scene_table`, `unresolved_panorama_node`,
  `step_limit_exceeded`;
- **not** a blocker: `0x6f` return, `0x79` script end, a `0x73` scene switch, and
  the counters that only log a skipped command (`skipped_node_commands`,
  `anims_load_failures`, `missing_media_requests`, `subtraction_underflows`,
  `dialogue_name_fallbacks`, `video_skipped`, `media_timestamps_unavailable`,
  refresh/display requests).

Real check (MAP 87 with every table except `--lcx`):

```
click_at 190,300 -> 190,320 slot 47 action 246 click_result steps=6 …
click_at 190,300 -> 190,320 slot 13 action 257 stopped: unresolved_scene_table=31 at instruction 302
```

The second click no longer prints `click_result`, does not travel and does not
commit the partial state. The new scene's entry line uses the same classifier
(`entry … stopped: unresolved_random`), so an incomplete entry cannot look like a
clean one either.

## 7. MAP's frame after the switch (render comparison)

Two things were compared: the record table and the draw gate.

**Table.** `0x40fd7b` really pre-loads the CA type 2/3 records, and MAP's
`engine/map ca` has **17 type-3 entries** (`maphl`, `mapTkHi`, `mapGTHi`, …).
Without the preload the port had 0 records after the switch; it now creates 17
(`record_preload=17`). For all **17** slots the save also holds, the preloaded
record's x/y and frame count match the save exactly (17/17: e.g. `maphl`
`143,63` frame 1, `mapTkHi` `338,231` frame 1). The save's other 10 records are
the lazily created room records (`maptk`, `mapea`, `mapSp`, `mapnb`, `mapsm`,
`mapiu`, `mapbc`, `mapgb`, `maphs`, `mapam`; CA type `0xff` = `0x42d52f`'s
"load lazily" marker) that were created during play and restored by `0x425ab4`.
A scene switch frees the table (`0x4102ae`), so they are **not** part of a fresh
load — the original drops them too.

**Draw gate.** `0x43b216`'s normal pass only draws a record when
`(+0x09 & 0x20) == 0` and `(+0x09 & 0x40) == 0`. `0x40ffee` gives every new
record `0x20`, and the separate snap pass needs a non-zero `+0x20` that
`0x40ffee` leaves at 0 (`0x43264d` and the trail path are not modelled). The
port used to draw every record with a frame, i.e. it painted records the original
keeps hidden. `compose_scene` now applies the gate and counts the skipped ones
(`skipped_snap`, `skipped_hidden`).

Measured frames:

| Frame | Table | Drawn | Ids | Pixels |
| --- | --- | --- | --- | --- |
| MAP 87 from `tempsave.btb` | 27 (17 preload-equivalent + 10 lazy) | 11 | 33,28,26,24,16,14,12,10,8,2,0 | 62,263 |
| MAP after MR → MAP | 17 (preload only) | 1 | 0 (`maphl`, activated by the entry's `0x64`) | 3,373 |
| MAP after the same entry + `--click 34` | 27 | 12 | the 11 above + 3 (`mapTkHi`) | 70,684 |

The save's own record bytes explain the 11: the 10 lazy room records and
`maphl` carry `flags=0x4` (drawn), the 16 inactive `…Hi` preloads carry
`flags=0x20` with `+0x20 = 0` (hidden, and `+0x0a`/`+0x0c` are 0 so the
offset/blend path does not apply either). The two frames differ in 55,409
pixels: exactly those 10 room records, which the original also does not show
after a scene switch. Clicking a room (slot 34) activates
`mapTkHi` (`0x64` clears `0x20`), which the original draws the same way.

Correction to earlier notes: the old `drawn=27 pixels=163079` MAP frame and the
EA `drawn=1 pixels=13570` panorama frame counted records with the `0x20` flag
(EA's `EACaset2` is a type-2 preload with `flags=0x20` and `+0x20=0`). Those are
not visible in the original; the current values are MAP `drawn=11
pixels=62263` and EA `drawn=0 skipped_snap=1`.

## 8. Tests

- `variable-metadata-test`: A/B layouts with a shared global, exact round trip,
  "a scene that writes nothing hands its arrival pool on unchanged", the
  declared/identity alias case, and rejection of a short pool and a 301-entry row.
- `scene-state-test`: `0x68` mode 0 / -1 / unchanged with the change counter;
  `trace_scene_entry` starting at instruction 0 through a `0x7a` no-op;
  `trace_blocker` for every new blocker and for the continuing counters;
  `preload_scene_records` (CA 2/3 only, 0xff not preloaded, a missing asset
  leaves the slot NULL); `compose_scene`'s `0x20`/`0x40` skip counters.
- `make -C port -j2 all test` passes all nine suites; the panorama verifiers pass.

## 9. Not modelled (reported, never invented)

- The transition snapshot path `DAT_004603d0`/`DAT_00460660` (including
  `0x439349`, and `0x73`'s override) and `0x42dad9`'s redraw helpers.
- The other pools cleared once before the loop (`DAT_00477350`, `DAT_0047ae80`,
  `DAT_00485a10`) and `DAT_00482540`'s dialogue-list state; only
  `DAT_0047e2f0` is recovered well enough to carry.
- Reading an aliased global through the *other* local inside one scene: the VM
  still keys reads by local, so a read-modify-write through an alias can see a
  stale copy. Exports are exact because they replay the recorded writes.
- Scenes whose entry needs the CRT `_rand` stream (IU, NB, EA, …) stop honestly
  at `unresolved_random`; some entries also need variables only a full
  playthrough would have.
- `0x73`'s transition effects (`0x418bcf`, `0x42bce1`, `0x42be07`, `0x419cf6`,
  `0x418eb2`, `0x419a51`, codes `0x65..0x6a`).
- The snap pass: records with `+0x09 & 0x20` that carry a non-zero `+0x20` are
  drawn once by `0x43b216` through the mode-1 rect and then cleared
  (`0x43264d`, `+0x20 = +0x22 = 0`, `+0x29`/`DAT_00482acd`). The port has no
  `+0x20`/`+0x22`/`+0x24`/`+0x26` fields, so it skips such records and reports
  them in `skipped_snap` instead of guessing their rect. The `0x40` conversion
  (clear `0x40`, set `0x20`, clear `+0x20`/`+0x22`) is likewise not modelled;
  such records are counted in `skipped_hidden`.
- The `flags & 0x10` offset path (`+0x0a`/`+0x0c` non-zero: draw, move, draw
  again and possibly re-arm `0x20`) and the `0x42b5b2` destination-read blend.

## 10. Verified exports

| Address | SHA-256 (16) | Role |
| --- | --- | --- |
| 0x42c8a0 | 6d09fd55ad59f5e1 | scene loop, one-time pool clear, per-load calls |
| 0x42d594 | 546a5c43ede36e64 | dispatcher; result 0 ends at the reload |
| 0x42db22 | 8df50fc5339a585b | cases 0x68, 0x73, 0x7a |
| 0x42dad9 | 2665224675174a42 | stores `DAT_00464878` |
| 0x40f3f0 | 070b3b5fc5f201bc | frees objects/sounds/chains/media per load |
| 0x40f524 | 10036e1d6c8893de | same frees at startup |
| 0x40fd7b | a994c5ad96f2be05 | record table free + CA type 2/3 preload |
| 0x4102ae | 6f7755ef3a8edced | record table free |
| 0x4117a6 | 7a50d19486b6ed83 | sound rows from the new script's 0x65 |
| 0x408662 | b0030e9408f8f970 | dialogue records per load |
| 0x438173 | aad99d1a46f68fdc | LCC list → `DAT_00483bc0`, LCV → `0x46ee00` |
| 0x412762 | 96dc07ef7781917e | instruction decoder (type/operand layout) |
| 0x436652 | 3d298553bbe3de0e | raw operand word reader |
| 0x41e289 | a89ac0f7d4449865 | first-load save resume (not used after a switch) |
| 0x43299d | 68e9e51fdfcfb781 | 0x86 writes record +0x04 |
| 0x432904 | 38a13d007cb9f198 | type-2 mask walk (`+1+n` types, `+0x0a+4n` values) |
| 0x401256 | 14244ab4d11bb8b5 | fixed panorama node size at startup |
