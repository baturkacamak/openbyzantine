# Mixed long routes, outcomes and real-media campaign (30 October2026)

User authorized all four recommendations in SEARCH_POLICY_AUDIT.md. This step
changes Python exploration/test tooling, shell entry points and documentation;
it introduces no new binary-derived engine behavior and no C++ player change.
The user's existing run `random-20261030T144608Z-2014224` and both LATEST pointers
were preserved throughout isolated validation.

## Implemented

1. `--strategy mixed`: initialize all roots, then schedule50% depth-biased,
25% less-visited-seed and25% uniform seed-balanced jobs. Deep selection discounts
inventory/camera-only tails after three consecutive view/selection actions;
weights discount repeated action IDs/commands from the same progress context.
The discount is a search heuristic, never a game rule or state rewrite. All
weights stay positive, all queued routes remain available, and detailed-state
deduplication remains unchanged. Distinct item IDs at the same screen position
are not merged. BFS and legacy random remain available. The long logic launcher
now defaults to mixed/depth24; the small direct CLI default remains BFS.

2. Reports retain lane/depth/category counters, distinct progress/context counts
on individual paths, and replay links for actual observed progress routes.
A long camera/inventory sequence is not presented as solved-puzzle progress.
Plan scheduling metadata is excluded from job identity and exact replay inputs.

3. Real media profile: actual video/audio enabled with dummy SDL, using existing
FFmpeg decode/playback. Completion/resume and positive audio-queue evidence are
required by directed media cases. Decoder initialization or an enabling flag
alone cannot pass that requirement. Missing requested assets are reviews;
decode/audio/frame failures are errors. No clip is substituted or fabricated.
Random media search uses two genuine map saves so its small budget is not
consumed initializing49 roots without triggering any media.

4. Directed outcome contracts:9 fixtures/37 checks for MR arrival, MR departure
from its different saved state, AM arrival, examined-key image/parent return,
changing the examined item, actual Morris response/return and all three Nuray
answers. Fixtures contain ordinary SDL inputs/genuine saves; Nuray declares its
assisted warmup. Checks read scene/background, selection, return context, active
objects, board ownership and actual answer lists. Explorer also checks shared
examine-parent return invariants on discovered paths. Assertions and required
media coverage are included in replay capsules and checked by public replay.
They are explicit known outcomes, not a claim to understand every puzzle or an
independent original-runtime oracle.

MR arrival from MAP45 and departure from MR30 are deliberately separate
contracts: the same scene name does not imply the same story state. The first
prototype mistakenly combined those starting states; the corrected fixtures
use the exact separately evidenced input chains. Prototype outputs remain
separate from final results and are not counted as game defects.

## One-command workflow

`./port/explore_campaign.sh 2000` builds and runs:

- Directed behavior cases.
- Mixed logic exploration across all authentic numbered saves, capped at2000
  total logic-phase launches (including its save/confirmation checks).
- The two real-media directed routes.
- Smaller mixed media exploration, default12 launches.

Thus2000 is the logic-phase cap; the extra checks are additional launches. The
campaign reports the actual total and per-profile results. HTML links each
phase's JSON/logs and shows depth, outcome checks and completed media flows.
The public wrapper publishes its campaign root in EXPLORATION_LATEST; the root
JSON kind is `multi-profile-campaign`. Direct Python without --publish-latest
preserves an ongoing user's pointer. Game logs/LATEST stays separate.

Per-phase wall budgets are enforced; SIGINT lets an inner explorer clean up its
owned player groups before escalation. Errors/reviews are preserved and do not
prevent independent subsequent profiles from running. Unit checks verify error
propagation, review semantics, timeout cleanup and all four phase invocations.

## Final evidence

All outputs are under `port/output/search-upgrade/`.

| Matched comparison | Legacy random | Final mixed |
| --- | --- | --- |
| Seed |1222407329 |1222407329 |
| Starts | same small3-fixture preset | same small3-fixture preset |
| Run/depth limits |90 launches /24 steps |90 launches /24 steps |
| Deepest observed input chain |5 |24 |
| Distinct detailed states |69 |66 |
| Progress/context signatures |23 |19 |
| Findings |0 |0 |

This single-seed comparison establishes that the new policy reaches deeper
routes under the same limit; it does not establish universally greater coverage
or more bugs. The24-step retained route contains5 distinct conservative
progress/context signatures. Fewer global signatures in this sample are the
expected depth/coverage tradeoff; the other scheduling lanes preserve breadth.
Reports: `random-control/report.json`, `mixed-final/report.json`.

Full final matrix: `campaign-final/report.json` and `index.html`:

-9 behavior runs,37 assertions, PASS.
-80 logic launches across all49 saved starts,0 findings.
-2 directed media runs, both observable-outcome contracts pass;3 completed
 media flows/3 positive audio queues. MR has one asset-review signature.
-4 mixed media runs,2 completed media flows,0 findings.
-Total95 launches; no motor/runtime errors. The overall status honestly remains
 FINDINGS because of the unavailable-asset review.

MR requests `mm1car01.avi`, `mm1meh00.avi`, `mm1car02.avi`, absent by exact
basename from **full read-only listings of all six ISOs**. Other requested MR
media does play and the scene continues. `full-iso-absence-check.json` records
all six listing results; `missing-media.json` records media-index hashes.
This establishes unavailable files in the supplied set, not that their absence
is necessarily a defect in the original game. They remain review findings.

Negative control: `negative-behavior/` deliberately expects the wrong scene.
The player exits normally, but the outcome contract fails. Public
`negative-replay/report.json` reproduces `behavior_contract` with an error
exit, proving that non-crash observable failures are detected and replayable.
It is clearly a synthetic wrong-expectation fixture, not a game defect.

Validation:

- `make -C port -j2 all test` passed (`final-build-test.log`).
- Python discovery50 checks pass (`python-discovery.log`): existing regressions,
 mixed scheduling, non-pruning/discounts, depth24 in a branched synthetic search,
 bad/missing outcomes, real-media evidence parsing and campaign lifecycle.
- CMake-selected exploration/policy/matrix3/3 pass (`ctest.log`).
- Existing actual-asset explorer contract passes (`asset-contracts.log`),
 including branching, saved-start restoration, finding bundles and replay.
- Shell syntax and git diff whitespace checks pass.
- `validation-summary.json` consolidates comparison and campaign results.

No full new2000-run campaign was automatically started. These checks do not
validate physical speakers, human input or all story outcomes. Runtime/save
inputs and original ISOs were preserved, and no commit/push was performed.
