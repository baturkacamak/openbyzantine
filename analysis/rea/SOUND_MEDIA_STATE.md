# Sound start, media definition, dialogue window and scene switch

Bounded step of 25 October 2026. Scope: `0xd1` audio start, `0x6a` media
definition, `0xb4`/`0xb5` dialogue visibility, `0xbb` channel selection and
`0x73` scene switch. Audio output, rendering, AVI/JPEG decoding and cross-scene
execution remain out of scope.

Evidence base (saved exports only, no binary session opened):

- target `analysis/rea/BETRAYAL.EXE`, SHA-256
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
  (matches `pseudocode/decompile-index.json` `target_sha256`);
- analysis profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`
  (matches every export header and `analysis_profile_digest`);
- `0x42db22` cases `0x6a` (line ~183), `0xb4`/`0xb5` (line ~1107), `0xbb`
  (line ~1177), `0x73` (line ~367), `0xd1` (line ~1490);
- `0x428cbd` (nine-argument audio/dialogue start), `0x4073f0` (channel select),
  `0x4075a1` (message-area redraw), `0x407420` (message-window redraw),
  `0x408c81` (CY name → channel), `0x41d440` (gamepref accessor);
- `0x42d594` (dispatcher return-code consumer), `0x42c8a0` (50-entry
  `DAT_00482460` initialisation), `0x438173` (LCX reader), `0x424dc0` (the save
  writer that stores the media records);
- shipped `engine/SHOW.LCX` and `port/output/cc_all/*`.

Terms: **observation** = read directly from the export or the shipped bytes;
**inference** = a conclusion that fits code and data but is not stated by
either; **unknown** = not recovered.

---

## 1. `0x6a` — media definition (`DAT_00464f60`)

The instruction has four operands (`kArgumentCounts[0x6a] = 4`):

| operand | reader | use |
|---|---|---|
| 0 | `0x436652` raw | slot id (`DAT_00464f60[id]`) |
| 1 | `0x4319b3` resolved | `+0x00` value |
| 2 | `0x436652` raw | `+0x0c` media id |
| 3 | type byte at `+4`, low byte at `+0x16` | `+0x11` type when its type is 2, else 0 |

- **Observation** the slot is allocated with `0x44c2f0(0x14)` only when it is
  NULL; an existing record is reused and every field below is still written.
- **Observation** `+0x10 = 1`, `+0x04 = 0` and the `timeGetTime()` value is
  written to `+0x08`.
- **Unknown** the wall-clock timestamp. The port stores `timestamp = 0` and
  counts `SceneTrace::media_timestamps_unavailable`; it does not pretend to
  reproduce `timeGetTime()`.
- **Observation** `+0x11` takes only the **low byte** of operand 3, not its
  word. The port masks with `& 0xff`.
- `SceneTrace::media_definitions` retains `(instruction, id, value, media_id,
  type)` so the branch is visible.

## 2. `0xd1` — audio start (`0x428cbd`)

`0x42db22` case `0xd1` builds the nine-argument call:

```
FUN_00428cbd(raw(0), raw(1), text, 0, -1, 0, 0xffffffff, 0, 0xffffffff)
```

with `text = raw(2)` only when operand 2's type byte is 2, otherwise `-1`.

`0x428cbd(param_1..param_9)` reads `DAT_00486220[param_1]`; a NULL record or
`(record->+0x18 == 0 && gamepref(2) == 0)` returns `0xffff`. Otherwise it calls
`0x427de1(id, 0)`, optionally draws text `DAT_00483640[text]` with the
`DAT_0045f214` string, and runs a blocking loop that reads the dialogue window
(`0x43c8d2`) while `param_2 == 0` and polls `0x428143(id)` until playback ends.

- **Observation** parameter 1 is the sound record, parameter 2 the mode
  (`0` = interactive dialogue), parameter 3 the text id.
- **Not modelled** the blocking loop, `0x427de1` playback, the text renderer and
  `DAT_0045f214`. The port records `SceneSoundStart{instruction, id, mode, text,
  record_present, allowed}` and continues. `allowed` is the `+0x18`/
  `gamepref[2]` gate; it is nullopt when the record is absent or gameprefs were
  not supplied.
- **Inference** the command is request-only in the same sense as `0xbf`/`0xc0`:
  the state is observable, but no device is opened and no wait is simulated.

## 3. `0xb4`/`0xb5` — dialogue visibility (`DAT_0045d1c0`)

- `0xb4`: `DAT_0045d1c0 = 0`, then `0x4075a1(1)` (clear/blit the message area).
- `0xb5`: `DAT_0045d1c0 = 1`, then `0x407420(1)` (redraw the message window).
- **Observation** both have **zero operands** (`kArgumentCounts = 0`).
- The port adds `SceneState::dialogue_visible` (BSS default `false`, matching
  the original zeroed BSS), sets/clears it and counts one
  `dialogue_refresh_requests` per command.
- `load_save` now restores `dialogue_visible = field_45d1c0 != 0`; this field
  had no consumer before this step.

## 4. `0xbb` — select dialogue channel

`0x42db22` case `0xbb` calls `0x4073f0(raw(0))`:

```
DAT_0045d1c4 = FUN_00408c81(raw(0));
DAT_0045d1bc = 0;
FUN_00407420(0);
```

`0x408c81` is the same CY name → channel resolver already used by `0xab`'s
operand 2, so the port calls `resolve_name_index` with the global `SHOW.LCY`
list and the per-scene `CY` table.

- **Observation** the scroll is reset to 0 and the message window is redrawn.
- **Observation** an absent name takes the `0x408c81` fallback (id 0) and is
  counted as `dialogue_name_fallbacks`; without the tables the trace stops with
  `unresolved_dialogue_channel`.
- The port sets `current_dialogue_channel`, clears `dialogue_scroll` and counts
  one refresh request.

## 5. `0x73` — scene switch (`DAT_00482460`)

`0x42db22` case `0x73`:

```
uVar3 = raw(0);
uVar4 = DAT_00482460[uVar3] & 0xffff;
if (DAT_00482460[uVar3] != 0xffff) {
    DAT_00482428 = uVar4;
    if (DAT_004603d0 != NULL) { DAT_00482428 = *DAT_004603d0; DAT_00460660 = 1; }
}
```

- **Observation** `DAT_00482460` is a 50-entry array (`0x42c8a0` fills entries
  0..49 with `0xffff`) and is filled from `engine/%s.lcx` with the chapter name
  (`0x438173`, suffix `lcx` at `0x4615b8`).
- **Observation** the LCX file is `u16 count` + `count * u16`; shipped
  `SHOW.LCX` has 44 entries (bytes `00 00 19 00 0e 00 …`), i.e.
  index 1 → scene 25 (`map`), index 2 → 14 (`gc`), index 9 → 21 (`iu`),
  index 10 → 42 (`vt`). Raw `0x73` operands in the 44 scenes are 1..42.
- **Observation** `DAT_004603d0` is a nullable transition snapshot built by
  `0x438de4`; when present it overrides the target. It is **not modelled**, so
  the port reports the LCX target.
- **Modelled behaviour** the port does **not** continue in the old script: after
  a switch it records `SceneTrace::scene_switch_requested` (the target
  `DAT_00482428`) and stops. Continuing would keep using the old scene's
  variable row, which this layer cannot remap mid-script.
- Entries from the file count to index 49 are valid "no entry" (the original
  reads the initialised `0xffff`) and leave the scene unchanged. An index of 50
  or more is an out-of-bounds read and is rejected. Without the LCX table the
  trace stops with `unresolved_scene_table`.
- `parse_scene_switch_table` (records.cpp) rejects a count above 50, a size
  mismatch and truncation; `--lcx <SHOW.LCX>` supplies it.

## 6. CLI

```
byzantine-cc <scene.cc> --trace ... --lcx <SHOW.LCX>
byzantine-cc <scene.cc> --census ... --lcx <SHOW.LCX>
byzantine-cc <scene.cc> --trace-save <save> <label> <cb> --engine <dir> --lcx <SHOW.LCX>
```

`--trace`/`--census` print one `dialogue_visible=true|false` line and, when
relevant, `sound_start`, `media_definition`, `media_timestamps_unavailable`,
`scene_switch_requested` and `unresolved_scene_table`.

## 7. Acceptance (shipped data)

VT save `73.btb`, label 541 (with LCX), reaches the switch:

```
steps=41 stopped_at=620 scene_switch_requested=21
dialogue_visible=false
```

Instruction 620 is `0x73` with raw operand 9; `SHOW.LCX[9] = 21` (`iu`), which
is exactly the reported target. The same trace executes `0xb4` at instruction
611 (visibility false).

VT label 497 reaches a real `0xd1`:

```
sound_start instruction=560 id=35 mode=0 record=true text=2 allowed=true
```

EA save `26.btb`, label 389 reaches a real `0x6a`:

```
media_definition instruction=421 id=1 value=50000 media_id=401 type=0
media_timestamps_unavailable=1
```

EA label 602 now **completes** (return at 319) instead of stopping on `0xbb`
(318): `0xbb` selects channel 0 and the script returns.

## 8. Census

| Census | previous step | this step |
|---|---:|---:|
| labels traced | 7,693 | 7,693 |
| completed (`return`) | 5,438 | **5,702** |
| unresolved variable | 1,267 | 1,278 |
| unresolved RNG | 223 | 238 |
| unsupported opcode | 686 | **289** |
| `missing_object` | 74 | 74 |
| `scene_switch` | — | 107 |
| `unresolved_scene_table` | — | 0 |
| `step_limit` | 5 | 5 |
| `error` | 0 | 0 |

New ranking: `0x8b` (63), `0x8a` (37), `0xbe` (28), `0xc8` (22), `0xcb` (20),
`0xc6` (19), `0xd2` (19), `0x80` (17), `0xd4` (17), `0x8d` (9), `0xc7` (9).
`0xd1`, `0x6a`, `0xb4`, `0xb5`, `0xbb` and `0x73` are no longer blocking
opcodes; `0x73` labels move to the explicit `scene_switch` outcome.

The small rises in `unresolved_variable`/`unresolved_random` come from labels
that are now released past these commands and reach the next real blocker.

## 9. Tests

`make -C port -j2 all test` passes.

- `scene-state-test`: `0x6a` field writes (resolved value, raw id, low-byte
  type, zero timestamp and the counter), `0xd1` request fields and the
  `+0x18`/gamepref gate (true, false, absent record), `0xb5`/`0xbb`/`0xb4`
  visibility/channel/scroll/refresh, `0x73` target/no-entry/missing-table.
- `records-test`: LCX parser (`3 entries → {0, 25, 0xffff}`), empty table,
  truncated count, count > 50 and size mismatch.
- `save-test`: `dialogue_visible` restore on the synthetic save and on all four
  real fixtures.

The three panorama verifiers still pass.

## 10. Observation / inference / unknown

- **Observation:** `0x6a` has four operands and writes `+0x00/+0x04/+0x08/
  +0x0c/+0x10/+0x11`; `0xd1` calls `0x428cbd` with the nine arguments above;
  `0xb4`/`0xb5` write `DAT_0045d1c0`; `0xbb` resolves a CY channel and resets
  the scroll; `0x73` indexes a 50-entry LCX-backed table and writes
  `DAT_00482428`.
- **Inference:** `0xd1` is best treated as request-only until playback and the
  dialogue loop are reconstructed; `0xb4`/`0xb5`'s two redraw routines are
  equivalent observable refresh requests.
- **Unknown:** `timeGetTime()`, the `0x428cbd` blocking loop and its dialogue
  return codes, the `DAT_0045f214` string contents, the `DAT_004603d0`
  transition-snapshot override, and what happens after a scene switch (the
  original continues the current script with the new `DAT_00482428` row).

## 11. Verified export hashes

12 exports were re-hashed against `pseudocode/decompile-index.json` (`sha256`
recomputed from each `relative_path` on 25 October 2026); all matched and all
belong to analysis profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`:

```
0x4073f0 0x407420 0x4075a1 0x408c81 0x41d440 0x424dc0 0x42515f 0x428cbd
0x42c8a0 0x42d594 0x42db22 0x438173
```

No binary session was opened; no cleanup is pending.
