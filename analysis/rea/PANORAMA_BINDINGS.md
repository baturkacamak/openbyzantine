# Panorama asset binding and staged horizontal position

25 October 2026. Reused saved REA/Ghidra exports; no original game execution
or new decompilation. `verify_panorama_bindings.py` checks the target SHA-256
and ten selected function hashes against the saved decompile index, then
writes `panorama-binding-evidence.json` with source identity and per-command
asset inventory. Target/profile identifiers are retained in that JSON.

## Static observations

- `0x412d35`: the node-table filename uses the suffix at 0x45eb10. Reading
  the PE section bytes confirms `co`.
- `0x412f77`: loads that table into 0x483d90 using `0x411dfc`, the shared
  two-string plus flag reader. All 44 extracted CO.ALL records parse with
  the existing campaign CB parser (observed flags are 0xff).
- `0x42db22` and `0x43340a`, case 0xb9: raw operand 0 selects the node ID.
  `0x432295` obtains its name through 0x483d90 and calls `0x40186e`.
- `0x40186e`: uses the literal formats at 0x45c0b8 (`%.2s\\%s`) and
  0x45c0c0 (`nodes\\%s.jpg`). This establishes the relative JPEG path.
  The secondary path builder and transition/resource effects are not yet
  reconstructed.
- Both dispatchers' 0xba cases read a raw unsigned 16-bit operand through
  `0x436652` and call `0x401c21`, which clamps the int to 0..2495 and writes
  0x45c064. A raw word 65535 reaches the upper clamp, not the negative clamp.
- `0x401ddf`, called during panorama loading, consumes 0x45c064, derives
  an angular value from its ratio to 0x46487c, and clears it to -1. Exact
  projection state and pixel rendering remain outside this change.

## Implementation and original-data checks

`SceneState::pending_panorama_x` retains the staged value across actions;
`0xba` updates it. **Update:** `0xb9` is now implemented as an explicit load
request (`analysis/rea/PANORAMA_LOAD.md`): it resolves the CO record, builds the
`nodes/<2>/<name>.jpg` path, carries the staged position and the transition code
and consumes `pending_panorama_x` the way `0x401ddf` does. The earlier note that
`0xb9` "remains unsupported" described this file's own step. JPEG decoding and
projection are still outside the trace and the SDL player still commits state
only when an action succeeds.

`static_panoramas` and `byzantine-cc --panoramas <scene.co>` report raw node
IDs, names, source strings and relative JPEG paths, rejecting out-of-range
IDs and unsafe path names. This is static binding, not execution order.

All 44 scene records were checked: 182 load commands, 165 matching a JPEG
path in the six supplied ISO inventories. The 17 unmatched commands are in
MZ2: mz18, mz19, mz21–mz29, mz31–mz36. This is bounded filename absence in
those inventories, not proof these paths execute or resources cannot exist
in another form.

EA has four node names: ea00, ea01, ea03, ea02. Its six load commands all
match supplied JPEG paths. Instruction 317 (the closeup return blocker)
selects ea00. Instruction 2074 selects ea02. An extracted EA00.jpg sample,
inspected with ffprobe, is 2496×768.

Validation: `make -C port -j2 all test` passed with warning flags enabled.
Tests cover raw type-2 operand semantics, upper clamping, unknown initial
camera state, persistence across entry calls, node ID/path binding and bounds,
original EA label 1754 (x=747), and explicit zero-pool EA label 254 (52 steps,
now reaches 0xb9 at 2074 with x=747). Existing return, comparison, hotspot
and variable-metadata tests pass. An initial integration-test assumption
incorrectly expected label 254 to reach ea00 with only lastNode seeded;
the corrected test uses the existing explicit zero-pool fixture and verifies
the actual ea02 path. No defaults were added to ordinary traces.

## Next boundary

Recover projection/camera behavior through `0x401ee6` and resource setup
before adding a bounded panorama renderer. The other 0xb9 effects and 0xbb
remain required for a playable return path.
