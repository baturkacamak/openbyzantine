# Broad saved-state starting pool — 30 October 2026

The user correctly identified the three fixed starting fixtures as a coverage
limit. Long random runs now automatically use all distinct, valid numbered
saves available in the prepared save directory. Current data yields **49 saves
across18 scenes**. This is broader saved-context coverage, not a claim that all
44 engine scenes have independent saved starting states.

## Changes

`port/explore_seeds.py` discovers only actual two-digit `.btb` files, reads their
metadata through the production save dumper and records source hashes. Invalid
saves and exact byte duplicates are reported rather than replaced by guessed
state. Original CP1254 display bytes cannot make metadata parsing fail; only
ASCII metadata fields are interpreted. The catalogue orders contexts by scene
round-robin, so repeated MAP saves do not consume the whole initial budget.
`seed-pool.json` and the report expose counts, names, scenes and exclusions.

`--all-saves` selects this catalogue in the direct explorer. The 2000-run
`explore_random.sh` launcher now selects it by default. An explicit `--seeds`
file retains the previous targeted-fixture workflow. The direct small explorer
preset remains the three diagnostic fixtures unless --all-saves is requested.

Automatic pool seeds use `restore:true`. `--automation-restore-save` restores
the actual saved scene through the existing `travel_to(..., saved_path,
loaded_save)` path, including saved image/panorama, active objects and inventory.
It does **not** replay an arbitrary label0 or scene entry, which could change
the saved progression. The flag requires an automation script and save and
rejects --entry. No per-scene branch or alternative save parser was added.
Original engine/game state is not manufactured or edited by the explorer.

The first broad restore run exposed a new diagnostic-path lifetime bug:
passing a reference to the save path inside options.scene became invalid when
the load replaced that map. It could crash while printing the loaded path
(VC60 / VT72,73 in the initial test). `travel_to` now owns its path argument by
value, fixing the shared ownership rule instead of adding a scene exception.
The initial failing evidence remains under `all-roots/`; the corrected full
pool run has no restore failures. This is a resolved implementation regression,
not a newly discovered original-game defect.

After all initial contexts run, random scheduling first chooses an available
seed uniformly, then one of that seed's queued paths. A branch-rich context
cannot starve the other starting saves. Candidate action shuffling and explicit
choice/path replay remain unchanged. Reusing a starting save is intentional:
controlled comparisons require the same starting state with different inputs.
The mode does not promise a new unique starting state on every launch.

## Validation

Artifacts: `port/output/seed-pool-investigation/`.

* Metadata catalogue:49 candidates,49 distinct valid seeds,18 scenes,
  no skipped records in this data root. All first18 catalogue entries belong
  to different scenes.
* `all-roots-fixed/report.json`:49 fresh player starts,49 restored contexts,
 18 scenes, zero findings. Depth0 deliberately tests restore readiness only;
  it is not a full interaction/playthrough verdict.
* Real long-run launcher profile with count18/seed8675309 and depth0:18 starts,
 18 states,18 different scenes; report retains the49-seed catalogue. Evidence:
  `launcher.log`, `port/output/random-20261030T124118Z-628292/`.
* Asset contract includes genuine EA26, VC60 and VT72 saved starts and then
  loads the **same original bytes** through F9. Scene/background/layer,
  active slots, effective pool, inventory/channels, panorama and disc match.
  These contexts cover the initially exposed path-lifetime failure.
* Unit suite now20 explorer tests: scene-balanced catalogue, exact duplicate/
  invalid-record handling, metadata validation, seeded starting-context
  fairness plus the previous17 contracts.
* `make -C port -j2 all test`: PASS (`final-build-test.log`). Selected CTest:
 4/4 PASS (`ctest.log`). Expanded asset contract: PASS (`asset-contract.log`).
* User game logs/LATEST remains `run-20261029T225825.927020Z-rq6cvhmb`.

No2000-run session was started. The user's unchanged command now starts from
the broader pool:

```
./port/explore_random.sh 2000
```

Each trial still starts from an immutable copy, and original files/user saves
are preserved. The49 contexts include multiple progression/position states
within the same scene; they are not49 different scenes. Scenes without a saved
context can still be reached by actual actions from the available roots.
