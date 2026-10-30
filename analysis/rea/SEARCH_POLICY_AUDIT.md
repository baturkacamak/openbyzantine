# Search policy audit (2026-10-30T15:20:42.934760+00:00)

User asked whether more steps are needed after another2000-run launch.
Pinned run: `random-20261030T144608Z-2014224`. It is still RUNNING at observation time, with
1521/2000 launches, 1250 detailed states,
368 progress/context signatures and no findings.
Depth distribution of discovery trials: {0: 49, 1: 325, 2: 572, 3: 324, 4: 156, 5: 35, 6: 7, 7: 2}.
Configured max_depth12; deepest observed7; depth_frontier is
0. Therefore the depth cap has not been the limiting
factor. Increasing max_depth alone would not change decisions before that cap.
49 roots share the budget and 13664 jobs remain queued. Seed-balanced
random selection is broad rather than a guarantee of long action chains.

An earlier nearby snapshot's final-action categories were622 inventory,
353 hotspot,279 camera,115 wait,14 examine,49 roots. This measures the last
action of each discovery trial, not unique routes or all replayed inputs.
Useful interaction/view paths must remain available; do not prune them merely
because the progress projection is unchanged.

Recommended next implementation: mix explicitly depth-biased jobs with current
breadth/seed balancing; reserve exploration draws; prefer untried interactions
and new progress outcomes while reducing repeated selection/view cycles; keep
checkpoint/provenance replays for genuinely reached progress states. Raising
an actual long-chain ceiling to20–30 helps only together with scheduling.
Also run a smaller --with-media profile (current run has media disabled) and
add evidence-derived behavioral assertions for puzzle/interaction outcomes.
0 findings establishes no detected monitored failure, not full story correctness.

No player/explorer code changed, no new sweep started, current run left
untouched. Counts are an intermediate observation, not its final result.
