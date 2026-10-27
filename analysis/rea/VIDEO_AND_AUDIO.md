# Dialogue call variants, 0xbd video and 0xbf/0xc0/0xc2 audio parameters

25 October 2026. Static recovery plus port implementation. No original runtime
execution and no live binary session.

## Verified evidence

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`. All
functions below were re-hashed against `pseudocode/decompile-index.json` and
matched.

| Address | Range | Role |
| --- | --- | --- |
| `0x42db22` | `0x42db22-0x43194e` | dispatcher cases `0xb2`/`0xb3`/`0xbd`/`0xbf`/`0xc0`/`0xc2` |
| `0x412762` | `0x412762-0x412ae9` | CC reader: instruction nodes, type bytes, operand words |
| `0x40d031` | `0x40d031-0x40d2a0` | `0xbd` target: AVI path, mode gate, player call |
| `0x40d3af` | `0x40d3af-0x40d54c` | AVI player (not reconstructed) |
| `0x427d1a` | `0x427d1a-0x427d48` | pushes sound parameters to the audio objects |
| `0x427c43` | `0x427c43-0x427ca9` | volume (`+0x1c`) update |
| `0x427be3` | `0x427be3-0x427c42` | `+0x10` update |
| `0x427caa` | `0x427caa-0x427d19` | pan from `+0x20`/`+0x24` |
| `0x41d440` | `0x41d440-0x41d459` | `DAT_0045f528[i]`, the gamepref reader |
| `0x41f753` | `0x41f753-0x41f7bd` | loads `gamepref_%03d`, seven 32-bit words |
| `0x412f77` | `0x412f77-0x41316e` | loads `engine/<scene>cf` into `DAT_00484330` |
| `0x425ab4` | `0x425ab4-0x426260` | save loader: allocates the `0x464300` object table |
| `0x40c329` | `0x40c329-0x40c5ef` | only reader of object `+0x14` -> `DAT_004603d4` |

## Observed behaviour

### Instruction layout

`0x412762` builds 0x30-byte nodes: opcode at `+0`, the eight argument type
bytes at `+1..+8`, each operand's low byte at `+10 + i*4` and high byte at
`+12 + i*4`, and the next node at `+0x2c`. That is why the dispatcher tests
`*(char *)(*param_1 + 3)` (operand 2's type) and `+ 4` (operand 3's type).
`FUN_00436652` reassembles the raw 16-bit value, and `FUN_004319b3` resolves a
type-2 operand through the scene variable table.

### 0xb2 / 0xb3 — dialogue call variants

Same record-byte comparison as `0xb0`/`0xb1`, with `FUN_0043194f` saving a return
address before the taken jump:

* `0xb2` calls the label when `record[id][0] == operand 1`;
* `0xb3` calls it when the byte differs.

### 0xbd — video

```
0xbd record, mode, [start, end]
```

* `record` (raw) indexes `DAT_00484330` — the per-scene `engine/<scene>cf` table
  loaded by `0x412f77` (0x15e max) with the usual `{name, source, type}` layout;
  `record->name` is the AVI base name.
* `mode` (raw) selects a folder through `FUN_0040d031`: 0 `video\slide`,
  1 `video\intro`, 2 `video\site`, 3 `video\transit`, 4 `video\char`,
  5 `video\anim`, default `video`. The primary attempt is `avi/<name>.avi`; when
  that file cannot be opened, the mode-specific `video\...\<name>.avi` is tried.
* Operands 2 and 3 are read only when their type byte is `0x40`; they are passed
  to the player as `param_3`/`param_4` of `0x40d3af`.
* Modes 2 and 3 are skipped entirely unless `gamepref[5]` / `gamepref[4]` are
  nonzero (`FUN_0041d440`); `DAT_0045f528` is loaded from
  `savegame/gamepref_%03d` by `0x41f753` (`0x42c8a0` reads index 0 at scene
  start).
* `DAT_00460664` is written for modes 0 (`0x65`), 2 (`0x6a`), 4 (`0x67`) and,
  conditionally, 3; it is the **panorama transition code** that the next `0xb9`
  reads and clears (see `PANORAMA_LOAD.md`), not a panorama node id. For mode 3
  the value depends on `DAT_00464878` (alternate-mode/save state) and
  `DAT_00460668`, so it is not reconstructed.

Data: across the 44 extracted scenes the mode histogram is 5×110, 3×89, 4×38,
0×30, 2×24, 1×2 — 347 video commands, of which 113 are preference-gated. The
shipped default preferences contain `GAMEPREF.000` = `1 1 1 1 1 1 0` and
`GAMEPREF.001` = `0 1 1 0 1 1 0`, i.e. entries 4 and 5 are enabled in both, so
the gated videos do play on the shipped data.

### 0xbf / 0xc0 / 0xc2 — audio parameters

All three write `DAT_00486220[id]` (created by `0x65`) and pass resolved
operands through `FUN_004319b3`:

* `0xbf`: `+0x28 = operand 1`, `+0x2c = operand 2`, and — only when operand 3's
  type byte is 2 — `+0x1c = operand 3`.
* `0xc0`: `+0x1c = operand 1`, and when operand 2 is type 2 also `+0x20`, and
  when operand 3 is type 2 also `+0x24`; then calls `0x427d1a`.
* `0xc2`: `+0x20 = operand 1`, `+0x24 = operand 2`, and when operand 3 is type 2
  also `+0x1c`; then calls `0x427d1a`.

`0x427d1a` forwards the record to three DirectSound setters: `0x427c43` sends
`+0x1c - 10000` as volume, `0x427be3` sends `+0x10`, and `0x427caa` derives a pan
value from `+0x20`/`+0x24` through `0x428b1e`. Audio is not reconstructed, so the
port counts the refresh instead of playing.

### 0xcc needs save state, not a script model

`0xcc id, value` writes `DAT_00464300[id]->+0x14`. `0x464300` is the 0x15e-entry
object table that **only** the save loader `0x425ab4` allocates (0x18 bytes per
slot). `+0x14` is read only by `0x40c329`, which stores it in `DAT_004603d4` and
uses it to decide whether the message window closes (`0x4177c7`). A fresh scene
therefore has no records for `0xcc` to write to, which is why `0xcc` remains the
largest blocker (194 labels) and belongs to the save/load step rather than to a
script-only model.

## Port implementation

`port/include/byzantine/cc.hpp` / `port/src/cc.cpp`:

* `0xb2`/`0xb3` folded into the `0xb0`/`0xb1` case: same comparison, plus the
  24-entry return stack already used by `0x9b`/`0x9d`–`0xa2`.
* `SceneSoundState` gained `field1c`, `field20`, `field24`, `field28`, `field2c`;
  `0xbf`/`0xc0`/`0xc2` resolve every needed operand before writing (atomic on an
  unresolved variable, as `0x84` already does) and count
  `SceneTrace::sound_refresh_requests`.
* `SceneVideoRequest` + `SceneTrace::video_requests` / `video_skipped` hold the
  resolved CF name, `avi/<name>.avi` path, mode, the optional `0x40` operands
  (`position_x`/`position_y`, 0x40d031's param_3/param_4) and the `DAT_00460664`
  transition code for the unconditional modes. `step` is the 1-based
  instruction index so a budgeted replay can stop exactly at the request.
* `SceneState::video_records` + `video_table_known` (CF table) and
  `SceneState::gameprefs` (`std::optional<std::array<std::uint32_t,7>>`) are
  explicit inputs. A missing table stops with `unresolved_video_name`; a missing
  preference array stops with `unresolved_video_setting` instead of assuming the
  gate; an id outside the table throws, like the original's null dereference.
* `byzantine-cc` gained `--cf <path>` and `--gameprefs <28-byte file>`;
  `byzantine-play` pauses on the two new unresolved flags.
* `byzantine-play` **plays the clip** (27 October 2026): `port/src/video.*`
  resolves `avi/<name>.avi` then the mode folder, `scene.sh` fetches a missing
  clip from the user's ISOs (saved disc first), FFmpeg materialises a JPEG+PCM
  cache exactly like the conversation clips, and the frames are drawn with
  `0x40d3af`/`0x43a248`'s placement while the script blocks like `0x413bdd`.
  See `analysis/rea/CF_VIDEO_PLAYBACK.md`.

Modelled: AVI decode/playback through FFmpeg plus the existing JPEG/WAV readers
and the SDL2 mixer, the blocking `0x413bdd` wait, the destination from
`0x40d3af` including the `gamepref[1]` window, `DAT_004859cd = 1` after a played
clip, and the on-demand extraction from the shipped CDs.
Not modelled: the DirectSound setters behind `0x427d1a`, the `mcaft1a1` special
case's `0x40da1f` one-time renderer init, the `local_8` keep-open flag, mode 3's
`DAT_00460668`/`DAT_00464878` branch (so its `DAT_00460664` value stays unset),
mode 1's `0x419d28`/`0x42b280` intro-buffer clear, and the palette/format mode of
`0x43d9c7` (the port always uses the RGB565 surface). `DAT_00460664` is consumed
by `0xb9`.

## Verification

`make -C port -j2 all test` passes (scene-state, variable-metadata, panorama,
records) and the three panorama verifiers pass. New scene-state cases:

* `0xb2`/`0xb3` taken-branch return to the instruction after the call, and no
  call when the comparison does not match;
* `0xbd` request fields for mode 0 (start/end from type-`0x40` operands,
  panorama id `0x65`), mode 3 without a panorama id, the `gamepref[5] == 0` skip,
  the unknown-gate stop, the missing CF table stop and the out-of-range throw;
* `0xbf`/`0xc0`/`0xc2` field writes, the constant-operand-3 rule, the atomic
  unresolved-variable stop and the refresh count.

Real data:

```sh
port/byzantine-cc port/output/cc_all/EXAMINE --trace 288 port/output/cb_all/EXAMINE \
  --cf <CF.ALL/EXAMINE> --gameprefs <GAMEPREF.000> ...
# steps=3 stopped_at=298, background "examine",
# video_record=0 name=exCam mode=0 panorama_id=101

port/byzantine-cc port/output/cc_all/AM --trace 270 port/output/cb_all/AM ...
# steps=8 stopped_at=298 unresolved_sound_name=10, sound_refresh_requests=4
```

The `EXAMINE` 288 trace resolves CF record 0 to `exCam` (mode 0 -> `0x65`);
`EXAMINE` 759 plays CF record 1 `hsain1` in mode 3 with no panorama id and stops
later on variable 26, which the label never sets.

Census (`build_vm_census.py`, now also supplying CF.ALL per scene and
`goldsave/GAMEPREF.000`): completed **3899 -> 4038**, opcode-blocked
**1168 -> 876**, carried-variable 2601 -> 2754, unknown RNG 25, unknown video
name/setting **0**, unknown dialogue table **0**. `0xb2`, `0xb3`, `0xbd`, `0xbf`,
`0xc0` and `0xc2` leave the blocker ranking; opcodes with a port branch
**44 -> 50** of 155. New top blockers: `0xcc` (194), `0xb9` (144), `0xd1` (93),
`0x6a` (74), `0x70` (61), `0x73` (49), `0x8b` (47), `0x8a` (25).

## Next bounded step

1. `0xcc` (194) requires the `0x464300` object table from the save loader
   (`0x425ab4`): reading `savegame/NN.BTB` would unblock it and the whole
   save/load path at once.
2. `0xb9` (144) is the panorama loader; its staging state
   (`SceneState::pending_panorama_x`, `DAT_00460664`) is already modelled.
3. `0xd1` (93), `0x6a` (74) and `0x70` (61) need their own dispatcher reads.
