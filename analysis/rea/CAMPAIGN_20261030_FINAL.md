# Completed campaign review — 30 October 2026

Pinned user run: `campaign-20261030T182903-1437175`. This was the six-phase campaign launched before
story was added. No new process was launched and no user's bundle/pointer was
modified. The campaign and child processes have exited.

Aggregate:2427 player starts, status FINDINGS; one review, zero error-severity
findings. All child exit codes0. Logic/media exploration status BOUNDED means
requested max_runs reached, not failure or exhaustive search completion.

| Phase | Starts | Outcome |
|---|---:|---|
| behavior |9|9 cases/37 assertions PASS|
| settings |400|200 distinct configurations/103808 checks PASS; all32 operations|
| logic |2000|0 findings;1411 detailed/757 progress contexts;58/58 save checks|
| media-behavior |2|8 assertions,3 playback completions;1 known missing-media review|
| media-settings |4|2 cases/941 checks,2 playback completions PASS|
| media-exploration |12|0 findings;10 playback completions;bounded at requested12|

Logic reaches32 scene IDs including examine/puzzle screens and observed map
travel to15 destination IDs.49 deep starting contexts each received19–20
selections;243 distinct hotspot targets attempted. Actual longest executed input
chain has22 actions (last checkpoint exists); retained detailed-state maxdepth21,
progress-context maxdepth20. Parent table reports retained-state depth21. These
are different metrics, not solved-puzzle counts.3868 shared behavior checks ran.
Remaining frontier14186 confirms coverage is sampled, not exhausted.

The single aggregate review is MR arrival missing `mm1car01`; the actual log
also names `mm1car01, mm1meh00, mm1car02` as absent requested clips. Previous full six-ISO
listing checks found these files unavailable (see EXPLORATION_CAMPAIGN_UPGRADE.md
and SETTINGS_FUZZING.md). This is a media availability limitation, not newly
proved engine corruption. Existing clips still complete and continuation runs.
The15 playback completions are events, not15 unique files or a speaker check.

Separately, `port/output/story-chains/final-suite/report.json` has9 cases,
8 PASS,1 clock contract FAIL;7 saved continuations PASS,23 starts total. Public
replay reproduces failure after first clock gate: hand hotspot0 stays disabled,
preventing later gates. See STORY_MILESTONE_TESTS.md for observed state versus
candidate callback-gap inference. That unresolved functional contract does not
appear in this older campaign's aggregate because story was not in its loaded
phase list. No bug was fixed or new2000-run campaign initiated during this review.

Reports use dummy SDL; no claim of original-runtime equivalence or complete
new-game-to-ending validation. A clean sampled search does not cancel the known
clock failure.
