# Original variable mappings and explicit new-game snapshots

25 October 2026. Implemented a C++17 reader for the original variable metadata
and an opt-in initial-state trace. This identifies EA's missing variables from
the supplied game data and separates known startup defaults from unknown
mid-game history. Original assets were not modified.

## Static binary evidence

Saved REA/Ghidra export, target `BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`,
profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`:

- `pseudocode/functions/0x42c8a0.c` (`FUN_0042c8a0`) clears 3,000 global
  16-bit values at `DAT_0047e2f0`, initializes 50 scene mapping rows with 300
  identity entries each, then loads the campaign metadata before executing it.
- `0x437f76.c` constructs the LCC/LCV/LVM companion paths. `0x438173.c`
  reads the scene list in order, then one 16-bit count and that many 16-bit
  global variable IDs per scene. It separately reads a 16-bit name count and
  NUL-terminated global variable names.
- `0x4319b3.c` resolves a type-2 local operand through the current scene row
  in `DAT_0046ee00`, then reads the mapped global value. Assignment in
  `0x42db22.c` uses the same mapping for writes.

This is static evidence, not an observation of the running Windows game.

## Original-data findings

The C++ parser consumed all of `SHOW.LCC`, `SHOW.LCV` and `SHOW.LVM`:
44 scenes, 1,622 explicit local-to-global mappings, 1,049 global names,
maximum 133 explicit variables in one scene. LCV size is exactly 3,332 bytes:
44 two-byte counts plus 1,622 two-byte IDs.

| Scene | Local ID | Global ID | Original name |
| --- | --- | --- | --- |
| EA | 5 | 8 | `lastNode` |
| EA | 26 | 74 | `flowChk` |
| START | 8 | 8 | `lastNode` |
| AL | 1 | 8 | `lastNode` |

Thus EA variable 5 is a shared global `lastNode`, rather than an isolated
per-scene camera-position variable. This resolves its original name; it does
not reconstruct all preceding assignments. The existing live scene VM still
stores local keys and does not yet carry global aliases between scenes.

## Implementation

- `port/include/byzantine/variables.hpp` and `port/src/variables.cpp` parse
  scene names, per-scene maps and global names. Parsing rejects truncation,
  missing terminators, trailing bytes, duplicate/invalid scene names,
  out-of-range global references and counts beyond the observed engine table
  capacities. Scene lookup accepts ASCII case differences and DOS line endings.
- `byzantine-cc --variables <engine-dir>` reports local/global/name mappings
  for the input CC filename's scene name.
- `--trace-new-game <label> <scene.cb> <engine-dir>` explicitly projects the
  initial zeroed global pool onto that scene's declared local IDs. It prints
  `state_origin=new_game_zero_pool startup_script_executed=false`.
- Ordinary `--trace` remains strict about unknown prior state. The SDL2
  player's behavior is unchanged by this analysis step.

Only explicitly mapped variables are initialized. Original identity fallback
entries beyond the declared count, complete startup execution, saved-game
restoration and cross-scene global writeback are outside this implementation.
In particular, a zero-pool snapshot at an arbitrary label is not proof that
actual gameplay reaches that label with those values.

## Verification

```sh
make -C port -j2 all test
port/byzantine-cc port/output/cc_all/EA --variables analysis/rea/engine_sample/copy2hd/engine
port/byzantine-cc port/output/cc_all/EA --trace-new-game 602 port/output/cb_all/EA analysis/rea/engine_sample/copy2hd/engine
port/byzantine-cc port/output/cc_all/EA --trace-new-game 254 port/output/cb_all/EA analysis/rea/engine_sample/copy2hd/engine
port/byzantine-cc port/output/cc_all/EA --trace 602 port/output/cb_all/EA
```

The build and both test executables pass with warning flags enabled. The new
test covers metadata boundaries, every truncated prefix of synthetic LCV/LVM
fixtures, malformed counts/references/names, real campaign inventory, original
aliases and explicit initial-state construction. The original EA integration
runs 475 → 595 → 602 with a new-game snapshot and reaches unsupported `0xb9`
at instruction 317 without an unresolved variable. An ordinary unknown-state
trace still stops on variable 5 at instruction 312.

CLI observations:

- Fresh zero pool, label 602: 22 steps, stops on `0xb9` at 317.
- Fresh zero pool, label 254: 47 steps, stops on `0xba` at 2212.
- Unknown prior state, label 602: 13 steps, unresolved variable 5 at 312.

All 44 scene layouts were separately listed through the compiled CLI. Full
mappings, trace output and original input hashes are retained in
`variable-metadata-evidence.json`. CMake configuration was updated for the
new module and test, but CMake remains unavailable here; Debian verification
used Make.

## Next boundary

EA's remaining return blocker under the explicit initial snapshot is the
panorama loader `0xb9`, not a guessed default for variable 5. Its implementation
requires the original node-name list, panorama JPEG, projection/camera setup,
and resource effects. The next step is to resolve that node asset binding and
implement a bounded panorama display path with these limits kept explicit.
