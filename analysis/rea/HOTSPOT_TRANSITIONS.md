# Hotspot suspend/restore — bounded reconstruction step

Date: 25 October 2026.

Implemented `0xb7` and completed the persistent snapshot used by the already
supported `0xb8`. This supports restoring scene interactions after temporary
views; it does not implement the remaining panorama, media or cross-scene flow.

## Static evidence

Reused the saved REA/Ghidra export, without reopening or executing the EXE:

- Target: `analysis/rea/BETRAYAL.EXE`, SHA-256
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
  Rechecked against the recorded target digest after implementation.
- `pseudocode/functions/0x42db22.c`: case `0xb8` calls `FUN_00438cf9`;
  case `0xb7` calls `FUN_00438d78`.
- `pseudocode/functions/0x438cf9.c`: walks 350 hotspot pointers. For each
  active hotspot, records a set bit in `DAT_00464888` and clears byte +8;
  missing or disabled hotspots instead write a zero snapshot bit. Each call
  replaces the snapshot.
- `pseudocode/functions/0x438d78.c`: consumes each set snapshot bit and sets
  byte +8 of the corresponding non-null hotspot pointer to 1. It leaves
  other active hotspots alone. A second restore has no remaining saved bits.
- Both functions belong to saved analysis profile
  `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`;
  export inventory: `pseudocode/decompile-index.json`.

These are static decompiler observations, not observations of the original
running game. The portable mapping uses declared static hotspot slots rather
than original process pointers and preserves the snapshot across actions in
one `SceneState`. The player already copies this state before attempting an
action, so the snapshot also participates in its existing commit-on-success
behavior.

## Implementation and verification

`SceneState::suspended_hotspots` stores saved enabled bits. `0xb8` replaces
that snapshot and disables active hotspots. `0xb7` merges saved valid slots
into the enabled set and consumes the snapshot.

Commands run successfully:

```sh
make -C port -j2 all test
port/byzantine-cc port/output/cc_all/STAR --trace 1282 port/output/cb_all/STAR
```

The build regenerated the CLI and SDL2 player with `-Wall -Wextra -Wpedantic`.
Tests cover suspend/restore across separate entry calls, preservation of
intervening enables, one-time consumption, and replacement by a second
suspend. They also execute original STAR label 1282 (instructions 1520–1521:
`0xb7`, return), using an explicitly seeded snapshot of a declared STAR slot.
This is a command integration test, not proof of the game's full path into
that label. The CLI trace completes with `steps=2 stopped_at=1521` and no
unsupported opcode. Existing EA transition and variable tests still pass.
The first synthetic fixture run failed because its hotspot declarations were
missing their eighth operand; correcting the fixture to the parsed format
made the full suite pass.

Original game assets and saved binary evidence were not modified. CMake is
still unavailable on this host; verification used the Debian Makefile.

## Remaining boundary

EA 475 → 595 still works and the return still explicitly reports missing
variable 5. The saved `0xb9` branch includes resource/scene setup and external
state; it cannot safely be reduced to assigning a guessed panorama value.
The next EA step remains recovering that initialization path and its required
state, or implementing another command needed along it.

## 0x72 object declaration (26 October 2026)

`0x42db22` case `0x72` also allocates `DAT_00464300[slot]` when that pointer is
NULL. The fields come from the same instruction: x/y (`operand 2 + 0x14`)/w/h,
`+0x08 = 0`, label = operand 5, kind = operand 6, `+0x14 = 0xffffffff`, and
`+0x10` from operand 7 (direct for kinds 0x13/0x14, otherwise
`FUN_00408bb2` string-table resolution). An existing pointer is not replaced;
the original only logs `0x40abb4`.

The port previously parsed the rectangle for clicks but never created the
object, so a saved scene whose entry `0x72` precedes `0xcc` stopped on
`missing_object` (VB slot 4 via MAP action 497). `trace_scene_from` now creates
the object from `static_hotspots` (same y offset), and `0x6d`/`0x6e` keep
`+0x08` in sync. A save object or earlier creation is not overwritten.
`FUN_00408bb2`'s name-list resolution is not modelled; no current consumer reads
`object.name_id`, so such objects keep `0xffffffff` instead of a guessed id.

Verification: `scene-state-test` (fields, 0x6d/0x6e, no overwrite, kind 0x13
direct name) and the 17-destination MAP census, where VB now returns at
instruction 217 with 4 hotspots.
