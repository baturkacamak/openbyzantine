# Completed 2000-run campaign review (30 October 2026)

Pinned user campaign before further testing:
`port/output/random-20261030T130724Z-672018/` (seed2522863484).
Normal game `logs/LATEST` remains `run-20261029T225825.927020Z-rq6cvhmb.log`.
Original campaign report/bundles were not edited. Review outputs are separate
under `port/output/campaign-2000-review/`.

## Observed coverage

2000 launches in2804.287 seconds (46m44s), stopping at max_runs. These comprise
1922 discovery trials,58 save checks and20 finding confirmations.49 authentic
save contexts initialized.31 scene identifiers were observed, including examine
screens,1590 distinct detailed states and444 progress/context signatures.
331 detailed duplicates;17661 queued paths remain. Depths0..8 were observed;
this is not a completed playthrough. Audio/video were disabled and SDL dummy
devices used. All2000 process results show exit0, timed_out=false; this does
not preclude script blockers, which are detected from explicit observations.
16 answer observations span5 scene/record/group tuples.

## Findings and initial triage

20 unique finding signatures:1 runtime error and19 review candidates; campaign
confirmation reproduced every signature.58 save checks:28 passed,30 flagged;
duplicate failures collapse into these finding signatures.

| IDs | Observation | Interpretation / next work |
| --- | --- | --- |
| F0015 | MORRIS entry stops at489 after429 steps, `unresolved_random` | Confirmed explicit runtime blocker. Fresh public replay also reproduces it with unchanged player and prefix validation enabled. Three actual clicks from saved VC context reach it. |
| F0001, F0002, F0004, F0011, F0014 | F5 writes trial-local quicksave; after scene travel F9 searches shared private assets/savegame instead | Five invalid save/load comparisons: first two cannot find a quicksave; others load an unrelated earlier trial's quicksave. Fix stable save-directory ownership and trial isolation before treating these as save-state loss. |
| F0009, F0013, F0020 | Only inventory_loaded changes1→0; item status is unchanged | Cache-loading difference, no observed loss of possession. Raw save comparator is too strict for this field. |
| F0005, F0006, F0016 | saved/loaded effective pool matches; repeating input produces different random-dependent words | RNG state differs before/after load. Source explicitly preserves process-global RNG across scene loads. Not enough evidence to call this save corruption. Comparator must distinguish nondeterministic replay from persistence. |
| F0003 | MR conversation entry0 exists before load, absent after | Existing scene loader resets conversation memory; observed source rule. Whether this loses required behavior needs a behavioral check, not raw map equality. |
| F0007, F0012, F0018, F0019 | Examine image becomes generic `examine`; repeating the exit fails to return to parent as before | Strong restore candidates: background/return context should be investigated together. No original-runtime comparison performed. |
| F0008, F0010, F0017 | Same click after load changes travel, active slots or AM conversation behavior | Remaining interaction replay candidates; investigate selected item, hover/select, view context and dialogue state. Not yet isolated to exact cause. |

The5 path mismatches are directly established by F5/F9 log paths, not merely
inferred from differing scene names. `savegame_directory(options)` derives its
path from options.scene[--save], falling back to backgrounds/copy2hd/savegame;
`travel_to` replaces options.scene. The shared directory is inside the campaign
analysis copy, not the user's original saves. This means some periodic checks
were not isolated as intended. Do not claim all19 candidates are engine bugs,
or that confirmation alone makes a shared-state-dependent replay self-contained.

The MORRIS blocker also exists explicitly in current cc.cpp: system subcommand
0x1e sets unresolved_random and stops. This review uses current source and run
evidence only; it does not establish missing original mini-game behavior or
justify skipping the command. New binary-derived implementation will require
the project's hash-verified export workflow.

## Verification and priorities

Fresh F0015 public replay: same runtime_blocker/morris/unresolved_random,
player_changed=false, prefix_validation=true. Full `make -C port -j2 all test`
passed with no code changes in this review. Evidence: `triage.json`,
`morris-replay/report.json`, `morris-replay.log`, `build-test.log` in review output.

Next: stabilize save path and test isolation; correct transient-field/RNG
comparison; investigate examine return and remaining action divergences; then
implement MORRIS's missing behavior from verified evidence. Preserve the raw
campaign as the baseline and replay its bundles after corrections. No engine
or explorer behavior was changed as part of this log-review request.

## Subsequent resolution

All20 findings were addressed at the user's request. See
`CAMPAIGN_2000_FIXES.md` for corrections, verified original-code evidence and
20/20 replay results. This review remains the original diagnostic baseline.
