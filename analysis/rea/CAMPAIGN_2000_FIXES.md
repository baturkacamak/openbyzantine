# Resolution of the 2000-run findings (30 October 2026)

User requested fixing all20 findings in
`port/output/random-20261030T130724Z-672018/`. The raw campaign and its bundles
remain unchanged. Initial classification is in `CAMPAIGN_2000_REVIEW.md`.

## Result

All20 original public replay capsules now finish without a finding. The same
20 action/choice fixtures also pass the reusable asset regression suite, with
byte-identical original seed saves and preferences verified against each
campaign capsule.19 original roundtrip cases and an additional active Morris
board save are restored in separate OS processes, using the production
`--restore-save` path. An actual Morris token selection/placement produces a
computer response; its board survives persistence and its ordinary exit returns
to VC with usable hotspots.

A further seeded broad sweep ran113 processes:103 discovery trials and10
save checks, all10 passing, no findings. It reached100 detailed states and76
progress/context signatures across21 scene identifiers, then stopped at the
configured max_states. It did not rerun all2000 attempts or exhaust the game.

## Corrections

| Original findings | Shared correction |
| --- | --- |
| F0001, F0002, F0004, F0011, F0014 | Save directory now belongs to stable host options initialized at startup, independently of the mutable scene-table map. Travel, F5/F9 and menu operations retain it, so trials cannot read another trial's quicksave. |
| F0009, F0013, F0020 | Persistence comparison retains inventory status/ownership but excludes the lazy inventory-loaded asset-cache flag. No possession difference is suppressed. |
| F0005, F0006, F0016 | Port companions persist the actual RNG state; loading a port save reproduces random-dependent subsequent actions rather than reusing the post-action stream. Original saves still cannot recover an unstored RNG state. |
| F0003 | Port companions preserve per-conversation memory on saved-session restoration. Ordinary scene travel retains the original memory-reset behavior. |
| F0007, F0012, F0018, F0019 | Companions preserve examined-item/image identity and parent scene context, including original-format parent state plus runtime metadata. Restoring does not run guessed labels or display an intermediate parent frame. |
| F0008, F0010, F0017 | Exact active camera yaw/pitch and hotspot gates are restored. The originals lost vertical angle; the new restore also accounts for advance() subtracting the vertical delta. The same screen click consequently reaches the same object/action. |
| F0015 | The previously unresolved system calls now initialize, execute and release the recovered Morris board instead of stopping or skipping the command. |

F5 rewinds transactional hover preview before saving so the save does not
contain an unpaired transient highlight. The inventory cache remains lazily
loaded. Automation describes only the active panorama; an old panorama cached
behind a flat background is not persistence state. Detailed fingerprintv5 and
progress projectionv2 include the live Morris board, preventing incorrect
search merges. Old-key replay prefix validation is explicitly disabled, as in
the existing versioned policy; actual action/roundtrip outcomes are checked.

## Port persistence contract

Original BTB bytes retain their existing format. F5, manual save and autosave
also write a versioned `.btb.byzport` companion using bounded little-endian
field serialization, never native struct memory or pointers. It contains exact
BTB byte identity, camera, local write bookkeeping, hotspot/suspended gates,
conversation keys/slots, item selection/use, RNG, Morris state and optional
examine parent. Assets/tables are resolved from the loaded chapter; the parent
is rebuilt without injecting invented state or running entry labels. A stale
companion for a different BTB is explicitly rejected. Keep both files together
when moving port saves. Menu deletion also removes the companion.

`--restore-save` is a production startup option, valid with --save and without
--entry. It restores the saved image/state directly in interactive and --shot
paths, bypassing entry evaluation. `continue.sh` now uses it instead of --entry.
The older automation-only alias remains supported. Original saves without
companions still use the original-format loader. The companion is a **port
feature**, not a claim that the original format persisted RNG or conversation
memory. Explorer seed/replay bundles retain and hash a companion when present.

## Verified executable evidence for Morris

Original target SHA256:
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Target and30 selected exports were checked against
`analysis/rea/pseudocode/decompile-index.json`; evidence is retained in
`port/output/campaign-2000-review/verified-exports.json`.

- `0x42db22` case200 validates typed output operands and writes computer point,
  result code and mill line to the variable pool.
- `0x42a6aa` initializes with a host-time-seeded CRT stream, discards one draw,
  resets the nine-turn counter, executes turns or frees the board.
- `0x42913b`/`0x42a7b0` supply the exact24-node peer,score,flag table.
- `0x42a579`, `0x42a005`, `0x42a10e`, `0x42a217`, `0x42a349`, `0x42a46b`,
  `0x42a555`, `0x429d47` supply terminal checks, ordered threat selection,
  corner scoring, maximum-score selection, CRT tie-breaking and line IDs.
- `0x42a770`..`0x42a990` provide accessors. Ghidra assigns the same MFC-style
  name to two different flag setters. Verified target disassembly establishes
  the first call in0x42a579 targets0x42a9b0 (human bit2) and the later call
  targets0x42a990 (computer bit1); see `morris-dispatch-disassembly.txt`.

The normal host supplies clock time as the initialization seed; automation
supplies a logged deterministic0xbeef seed. Context-free VM probes without a
seed or initialized board still report the missing context. On terminal
branches where the native computer-output local is unwritten/undefined, the
port does not fabricate a move. The module contains recovered data/rules,
no scene-name special cases. No fresh whole-program decompilation was run.

## Validation and retained evidence

All paths below are under `port/output/campaign-2000-review/`:

- `validated-build-test.log`: `make -C port -j2 all test` passed. Explorer26
  unit checks include ownership/cache distinction and board fingerprints;
  C++ checks cover companion roundtrip/truncation/version errors, initialization,
  human/computer wins, typed VM output writes and board release.
- `public-replays-final/summary.json`:20/20 old public capsules clear.
- `production-regressions/summary.json`, `production-regressions.log`:
 20/20 retained fixtures; separate-process restores use --restore-save;
 actual human/computer turn, persisted board and usable exit pass.
- `production-headless/`: production --restore-save plus --shot restores the
 saved examined key. Exported screenshot inspected: key image and inventory
 are visible, rather than generic examine background.
- `explorer-assets.log`: existing MAP/answer-branch/save-start/replay contracts
 pass. `ctest.log`: selected CMake scene-state/save/exploration3/3 pass.
- `verification-sweep/report.json`:113 launches,100 detailed/76 progress
 contexts,21 scenes,10/10 save checks,0 findings.
- `fixed-result.json`: consolidated results and final player hash.

Dummy SDL/no-audio/no-video checks establish script/input/persistence behavior;
they do not validate physical speakers, monitor behavior or equivalence to an
original-runtime playthrough. Normal game logs/LATEST remains
`run-20261029T225825.927020Z-rq6cvhmb.log`; EXPLORATION_LATEST still points to
the user's original2000-run campaign. Original game inputs are unchanged.
