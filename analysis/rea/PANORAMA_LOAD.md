# 0xb9 panorama/node loader

25 October 2026. Static recovery plus port implementation. No original runtime
execution and no live binary session.

## Verified evidence

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`. Every
function below was re-hashed against `pseudocode/decompile-index.json` and
matched:

| Address | Range | Role |
| --- | --- | --- |
| `0x42db22` | `0x42db22-0x43194e` | dispatcher case `0xb9` |
| `0x432295` | `0x432295-0x432302` | panorama setup: node, camera, transition |
| `0x40186e` | `0x40186e-0x401c20` | builds `nodes/<2>/<name>.jpg`, loads, transitions |
| `0x401d29` | `0x401d29-0x401dde` | JPEG load into the panorama buffer |
| `0x401ddf` | `0x401ddf-0x401ee5` | camera consumption of `DAT_0045c064` |
| `0x432133` | `0x432133-0x432294` | script transition id -> engine transition code |
| `0x401c21` | `0x401c21-0x401c62` | `0xba` staging of `DAT_0045c064` (0..0x9bf) |

Strings: `0x45c0b8` = `"%.2s\\%s"`, `0x45c0c0` = `"nodes\\%s.jpg"`,
`0x45c0dc` = `"nodes"`.

## Observed behaviour

```
0xb9 record, force, vertical, [transition]
```

1. **Transition code.** When operand 3's type byte is `2` the code is
   `0x432133(raw(3), 1)`: `1 -> 0x67`, `2 -> 0x65`, `3 -> 0x66`, anything else
   (including `4`) `-> 100`. Otherwise, if `DAT_00460664 != -1`, that value is
   taken **and cleared**; with neither, the code stays `100`.
2. **`force`** is operand 1 resolved through the variable table
   (`FUN_004319b3`). When it is nonzero the original clears
   `DAT_0047e2f0[DAT_0046ee00[raw(1) + scene*600]]` — an indirect table write
   whose meaning is not recovered.
3. **`vertical`** is operand 2 resolved the same way. `0x432295` only uses it when
   `0 < vertical`, storing `_DAT_0047a8dc = FUN_00401c63()` (a yaw/pitch value)
   before the load.
4. **Node.** Raw operand 0 indexes `DAT_00483d90` (CO.ALL). The record's first
   field is the node name; `0x40186e` formats `"%.2s\\%s"` and then
   `"nodes\\%s.jpg"`, i.e. `nodes/<first 2 chars>/<name>.jpg`, with a second
   builder (`"nodes"` + `.jpg`) as fallback.
5. **Load.** If `force != 0` or the path differs from the cached
   `DAT_00468b98`, `0x401d29` decodes the JPEG into the panorama buffer.
6. **Camera.** `0x401ddf` converts `DAT_0045c064` (staged by `0xba`, clamped to
   `0..0x9bf`) into degrees using the loaded width `DAT_0046487c`:
   `clamp(floor(x / width * 360 + 0.5), 0, 360)`, then clears it to `-1`. This is
   the path already reconstructed by `panorama_angle_degrees`/`panorama_geometry`.
7. **Transition.** With `transition != 100` and `!= 0x6a` the original runs the
   `0x418bcf`/`0x42be07`/`0x42bce1`/`0x419cf6`/`0x41920f` blend paths; `0x6a`
   skips the first one. None of the pixel blending is modelled here.
8. Finally `DAT_0045c060 = 1`, `DAT_0048253e = 0xffff`, `0x40d862`, `0x4177c7(0)`
   and `0x41dcc9` — UI/audio housekeeping, retained as part of the load request.

`DAT_00485c6c` selects an alternative `0x40a2c3` path; it is zeroed at scene start
(`0x42c8a0`) and set to 1 only by `0x42d1e2`, so the normal path is modelled and
the alternative is documented as not modelled.

## Port implementation

`port/include/byzantine/cc.hpp` / `port/src/cc.cpp`:

* `ScenePanoramaLoad` retains instruction, raw record id, node name,
  `nodes/<2>/<name>.jpg`, the staged `horizontal` position, `force`, `vertical`,
  `transition` and the `flag_clear` index.
* `SceneState::panorama_records` + `panorama_table_known` hold the CO.ALL record
  list, supplied with `byzantine-cc --co <co-record>`. Missing table stops with
  `unresolved_panorama_node`; an id outside the table throws (the original
  dereferences a null `DAT_00483d90` slot); unsafe names are rejected.
* `SceneState::pending_panorama_transition` is now written by `0xbd` modes 0/2/4
  and consumed by `0xb9`. This corrects the earlier description of
  `DAT_00460664` as a "panorama id": it is a **transition code** (`0x65`, `0x67`,
  `0x6a`, `100`), also written by `0x43340a`/`0x43c8d2`.
* `pending_panorama_x` is consumed, matching `0x401ddf`'s `DAT_0045c064 = -1`.
  The original only reaches that line when the JPEG opened; the port treats a
  known node record as loadable, because JPEG presence is installation data (all
  but 17 of the 182 extracted load commands bind to a JPEG on the ISOs). This
  cannot change any branch: neither staged value is read by a branch opcode.
* Trace reporting: `panorama_loads`, `panorama_flag_clears`,
  `unresolved_panorama_node`.

Not modelled: JPEG decode and the panorama buffer (`0x401d29`), the projection
state update (`0x401ddf`'s callers), every transition blend function, the
`DAT_0047e2f0` table behind the flag clear, `DAT_00485c6c`'s alternative path and
the `0x40d862`/`0x4177c7`/`0x41dcc9` housekeeping.

## Verification

`make -C port -j2 all test` passes, including the three panorama verifiers. New
scene-state cases: node resolution and path, staged-position consumption,
`DAT_00460664` consumption versus the type-2 mapping (which leaves it pending),
`force != 0` flag clear, transition-then-resolve order, missing CO table and
out-of-range id. The variable-metadata test now asserts that EA label 254 reaches
`0xb9` at 2074 and, with the real EA CO record supplied, loads `ea02` at
`nodes/ea/ea02.jpg` with the requested 747 pixel offset.

Real data:

```sh
port/byzantine-cc port/output/cc_all/EA --trace-new-game 254 port/output/cb_all/EA \
  analysis/rea/engine_sample/copy2hd/engine --co port/output/co_all/EA
# steps=57 stopped_at=2081 unresolved_sound_name=27
# panorama_record=3 name=ea02 jpeg=nodes/ea/ea02.jpg force=0 vertical=0
# transition=100 horizontal=747
```

The EA CO table is `0 ea00, 1 ea01, 2 ea03, 3 ea02`, so the raw operand 3 of
instruction 2074 is `ea02` — matching the static `--panoramas` binding.

Census (`build_vm_census.py`, now also supplying `port/output/co_all/<scene>`):
opcode-blocked **876 -> 732** and unknown panorama node **0**, but completed
labels only 4038 -> 4045 because the released labels now stop on carried
variable state (2601 -> 2891): `0xb9` resolves operands 1 and 2 through the scene
variable table, and fresh traces have none of those values. `0xb9` leaves the
blocker ranking entirely; the new top blockers are `0xcc` (194), `0xd1` (93),
`0x6a` (74), `0x70` (61), `0x73` (49), `0x8b` (47), `0x8a` (25), `0xb4` (24).
Opcodes with a port branch: 50 -> 51 of 155.

## Next bounded step

1. `0xcc` (194) needs the save-owned `0x464300` object table (`0x425ab4`), which
   is the same subsystem that would carry the variable state blocking 2,891
   labels.
2. `0xd1` (93) plays a sound through `0x428cbd`; `0x6a` (74) and `0x70` (61)
   need their dispatcher reads.
