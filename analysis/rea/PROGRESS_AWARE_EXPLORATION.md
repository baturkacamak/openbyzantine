# Progress-aware exploration (30 October 2026)

## Scope and source evidence

User requested that adventure-game state coverage distinguish progress from
camera/selection combinations. This change is confined to the Python explorer,
launcher presentation, tests and documentation. No new executable-derived
behavior or engine/player modification was introduced.

Source observations: `port/src/inventory.cpp` selection marks state 3 as 1;
mark/examine sets state 4 and changing examined item restores state 1.
`port/src/variables.cpp::export_variable_pool` provides the effective carried
pool with actual local writes applied. Existing read-only automation snapshots
provide channel membership, conversation memory and scene-local object gates.

## Implemented policy

Detailed fingerprint v4 remains unchanged, including replay prefix validation.
It alone deduplicates search states, per seed. A separate projection v1 hashes:

- Scene and ended-session status, effective variable pool.
- Inventory status with 1/3/4 normalized, and per-channel sorted unique members.
  Lazy inventory loading, selection, scrolling and item-use UI are excluded.
- Full observed conversation memory, object fields after x/y/width/height
  (activation, kind, handler, name and argument), and enabled flags.

Camera, background, layer, coordinates, selected item, current channel,
random-generator state and presentation metadata are excluded from this second
key. Any relevant differences they contain still survive in detailed search.
Progress/context signatures are globally counted across seed contexts, but
never merge their queues or prune descendants. Each detailed state links to
its projection. Each edge records novelty and changed projection fields.

`progress_changes` and `interaction_only_changes` classify the final input of
each successful discovery trial. They include repeated observations, not just
unique changes; roots are excluded. Existing run/detailed-state budgets retain
their meaning. HTML, JSON, live output and the long launcher distinguish counts.

Random scheduling first runs all roots and then selects a starting seed fairly.
Within that seed, 75% of draws prefer jobs whose parent produced a globally
novel projection, if available. The other 25% draw uniformly from all its jobs.
No candidates are removed by this preference. BFS remains FIFO. Existing caps
can still defer work; a finite run does not guarantee all queued jobs execute.
Priority is scheduling metadata, excluded from job identity and replay inputs.

## Interpretation and unknowns

This projection is a conservative progress/context signal, not proof of story
advancement or a count of solved puzzles. Unknown VM variables/dialogue fields
are retained; transient script changes, hover activation and scene changes can
increase it. Background-only puzzle changes may not increase it, but remain in
the detailed key. Inventing a semantic whitelist would risk hiding real state.
Tests cannot prove original-runtime equivalence or whole-game completion.

## Validation

- `make -C port -j2 all test`: passed; explorer suite now 24 tests.
- New behavioral checks: selection/examine/new-item status and camera/coordinates
  do not inflate projection; ownership, unknown variables, dialogue and gates do;
  four equal-progress camera states all expand; seeded soft preference keeps
  lower-priority paths and does not alter replay/job identity.
- Actual asset contract passed: MAP discovery/random search, all three actual
  answer branches, authentic saved-start/F9 equivalence, immutable synthetic
  fault bundle and replay. Discovery: 4 detailed / 4 progress-context states;
  random: 5 launches, 4 detailed / 4 progress-context states. Those particular
  paths changed scene/gates, so equal counts are expected, not evidence that
  every interaction advances the story.
- Evidence: `port/output/progress-investigation/build-test.log`, `assets.log`
  and `assets/*/report.json`. Synthetic unit coverage separately establishes
  equal-progress states are retained; it is not presented as real-game evidence.
- Normal game `logs/LATEST` preserved at
  `run-20261029T225825.927020Z-rq6cvhmb.log`; original game assets/save inputs
  untouched. No 2000-trial run was started.

Actual long-launcher smoke (`./port/explore_random.sh 12 20261030` with the
MAP seed fixture, depth3/save-every0 overrides) additionally produced 10
detailed states but only 5 progress/context signatures over MAP/SM/TK.
Its final-input counters were 4 progress/context changes and 6 interaction-only
changes, with no findings; 82 jobs remained at max_runs. This is direct
runtime evidence that the new projection does not suppress expansion of
same-progress interaction variants. Report and runner log:
`port/output/random-20261030T130439Z-664157/`. Wrapper summary showed both counts.
