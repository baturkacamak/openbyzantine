# Stale object hitboxes across TK view changes

27 October 2026. User supplied TKXA -> TKXB log repeatedly selecting slot 2,
action 61, and a screenshot of overlapping outlines. No source ISO modified.

## Reproduced

Real TK script from entry, initialized variable pool, no save:
playfield (390,269) selects slot 2/action 61 and loads TKXB. Before the fix,
active slots were 1,2,3,4,5. A second click (275,251) selected old slot 2
again. Its lower id shadowed the newer view's regions.

TK action 61 calls shared label 54 -> 51, executing `0x90[0]`, `0xb8`.
It then enables 4/5 and conditionally 3 and selects background 1 (TKXB).
The missing deactivation was not an intentional overlapping interaction.

## Root cause and correction

The port had changed active-set construction to inspect object +8 (including
save-active objects), but suspend/restore still modified only hotspot_enabled.
Additionally, a false object byte fell back to a stale true mirror value.

`0xb8` now snapshots and clears actual object active bytes, synchronizing the
mirror; `0xb7` restores them and consumes its snapshot. Static-only traces keep
the previous mirror fallback when no object exists. An existing object always
decides whether its hotspot is active. No scene-id exception or hit-test
priority change was introduced, and legitimate overlaps are not removed.

## Evidence

Reused saved pseudocode and verified SHA-256 against decompile-index.json:

- BETRAYAL.EXE: `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
- `0x438cf9`: `7014e4f6df47bf9eeb6ba7303c286d53e40506b39de6f83af26a0c5c845162b5`.
  Walks 350 pointers, snapshots active byte == 1, clears that byte.
- `0x438d78`: `b47bd5f7a4400eb16fbda5f95021a674bdb293cc476ff49da39db640dc85d91f`.
  Consumes snapshot bits and writes active byte 1 on non-null objects.

These are static original-code observations, not original-runtime captures.

## Verification

- `make -C port -j2 all test`: all 14 suites pass. New scene-state tests cover
  save-active objects without mirrors, stale mirrors, restore preserving newly
  active objects, and a consumed snapshot not reviving disabled objects.
- `python3 analysis/rea/verify_tk_hotspots.py /tmp/byz-conv`: actual player and
  shipped TK script/media, SDL dummy devices. TKXA -> TKXB has only 3/4/5;
  (428,201) selects slot 5/action 87 -> TKXC has only 6; exits return through
  TKXB to TKXA, which again has only 1/2.
- `verify_examine_controls.py` now checks parent return against a baseline
  run instead of assuming the old stale count 9. Camera action twice, item
  switching and return are checked. EA's corrected baseline has 7 hotspots.

The tests cover view-switch state and routing, not every rectangle in every
scene. No acoustic check or original-game runtime comparison is claimed.
