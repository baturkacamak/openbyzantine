# Bounded gameplay discovery

The explorer discovers actions from the live player's read-only observations.
It does not execute hidden labels, force inventory, edit VM variables or pretend
to solve the complete game. It uses authentic save fixtures and normal SDL
scene input. Answer lists use the player's existing conversation-choice API.

From the repository root, after preparing media with `port/scene.sh`:

```sh
make -C port -j2 all test
make -C port explore-game DATA=/tmp/byz-scene
```

The command prints a unique output directory with `index.html` and
`report.json`. Open the HTML for the visited scenes, findings, logs and
before/after screenshots. Defaults: 80 player invocations, 40 distinct states,
six action steps, 300 seconds of exploration time, 30 seconds per player,
24 candidates per state and a 512-job frontier. Camera steps group six ordinary
left/right keys (60 degrees) or three up/down keys (15 degrees). They do not
set camera state directly.

For a longer logic run:

```sh
make -C port explore-game DATA=/tmp/byz-scene \
  EXPLORE_ARGS='--max-runs 300 --max-states 120 --max-depth 8 --max-seconds 1800'
```

Audio/video are disabled by default; this is fast script/input exploration.
`--with-media --timeout 180 --max-seconds 1800` enables actual media playback
with dummy SDL devices. Long conversations take real playback time. Neither
profile validates physical speakers, monitors or human input, and neither is
an original-runtime correctness oracle. `--no-isos` explicitly selects an
asset-only diagnostic profile; otherwise an available data-root
`media_isos.txt` supplies disc-correct animation/media sources. These caches
belong to the run's private asset root.

## One-command random runs

```sh
./port/explore_random.sh 2000
```

Run this from the repository root; omit the count for the same 2000 default.
`./port/explore_random.sh 1000` selects 1000 instead. A fresh random seed is
printed and retained. To select the seed explicitly:
`./port/explore_random.sh 2000 12345`.

The launcher automatically selects all distinct valid numbered save contexts
(current data: 49 saves across 18 scenes), restored without rerunning their scene
entries. The catalogue appears in `seed-pool.json`. It increases the
state/frontier/depth caps along with the run count,
allows up to 4 hours (60 seconds per trial), and keeps screenshots only for
findings to reduce retained disk use. It builds before starting and reads
`BYZ_DATA` (default `/tmp/byz-scene`). Each run has its own `runner.log`,
`report.json` and `index.html`. `port/output/EXPLORATION_LATEST` points to the
latest exploration attempt, including while running; normal game logs/LATEST
are untouched. Startup output initially lives in the sibling `<run>.log`.

The number is a **total player-launch upper bound**, including save/replay and
confirmation checks. It is not a count of distinct scenes, unique states or
necessarily 2000 different paths. The same scene can be visited with different
items, answers and action order. The report separates all these counts. Bounds
or an exhausted frontier can stop early; it never fabricates repetitions to
fill the count.

Direct CLI options are `--strategy bfs|random`, `--random-seed N` and
`--screenshots all|findings`. BFS/all-images remain defaults. Random mode
initializes all seeds, then randomly selects an available starting context and one of its queued
paths, and shuffles candidates before capping them. Within the selected seed,
75% of draws prefer children of newly observed progress/context states when
available; 25% remain uniform across its entire frontier. This balances starts even
when one context has many more queued branches. Scheduling is reproducible for the same seed
and observations; individual paths also have exact replay files. If you need
more time, provide numeric count/seed before extra flags, e.g.
`./port/explore_random.sh 2000 12345 --max-seconds 43200`.

## Progress versus detailed interaction states

Reports now show two separate counts. `states` remains the detailed search key
(v5), including camera, selection, close-up return context and clickability.
Only this key deduplicates exploration, independently per starting seed.
`progress_states` is a global, conservative progress/context projection (v2):
scene/end state, effective variable pool, Morris board, inventory record status (1/3/4
normalized), channel membership, conversation memory and scene-local object
activation/handler gates. It ignores camera, coordinates, current selection,
scrolling, inventory display/loading and random generator state. Inventory
ordering within a channel is ignored; channel membership remains meaningful.

Unknown VM variables and dialogue fields are deliberately retained. Scene
changes, hover gates or transient script variables can therefore create a new
progress/context signature without solving a puzzle. This is **not** a solved
puzzle counter or proof of meaningful story advancement. Detailed exploration
still covers differences omitted by that projection.

`progress_changes` counts discovery trials whose last input changes the
projection; `interaction_only_changes` counts last-input detailed changes with
an unchanged projection. These are observations, including repeats, not unique
milestones. Roots have no last input and increment neither. Edges record the
changed projection fields; states link to their progress signature. Cross-seed
progress counts deduplicate matching observations without merging search paths.
Random priority is a soft preference for descendants of novel progress/context
observations, with seed balancing and uniform draws retained. BFS stays FIFO.
All existing budgets still apply, so retained paths are not guaranteed a turn
before the run ends. `--max-states` continues to cap detailed states.

## Search and observations

The direct CLI small preset uses genuine EA/26.btb, MAP/27.btb and MR/30.btb fixtures in
`seeds.json`. Each is a separate starting context, not proof it was reached
from a new game. Override with `--seeds /path/seeds.json`:

```json
[{"id":"map","scene":"MAP","label":87,"save":"27.btb"}]
```

Direct `--all-saves` discovers all valid distinct two-digit saves automatically
and balances the initial catalogue by scene. Automatic seeds use `"restore":true`
and restore the actual saved view/state through the production loader. Invalid
or byte-duplicate saves are listed in the catalogue. An explicit `--seeds` on
the long launcher overrides its broad pool for targeted checks.

An optional `"entry":true` runs the actual entry head; it cannot be combined
with `"restore":true`. The seed's saved scene
must match the chosen scene. For an already-known reachable context, a seed
may declare `warmup` actions (the same `{id,category,commands}` schema as
snapshots). This assisted setup remains visible in the seed, input script and
replay manifest. `dialogue-seed.json` is explicitly an **assisted integration
fixture** for AM's Nuray conversation, using actual camera keys and two scene
clicks; it is not counted as unassisted discovery.

Each branch starts a fresh process with an immutable initial-save copy and
replays its input path. It never restores from a probe's mutated result, never
uses a newly written quicksave as a search-state snapshot, and never overwrites
the user's saves/config/history or LATEST bundle. Runtime save directories,
preferences and configuration are private. Core tables are extracted once
per run. Original assets are read through links; extraction/decoding caches
are private to the explorer run.

The player samples visible hotspot pixels through its real inverse-panorama
and first-hit rule. Candidates also include compatible selected-item use,
visible inventory selection/examine/scrolling, camera turns and a wait. The
item compatibility gate is applied only while holding an item, as in real
input. It does not remove ordinary hover/select targets. Menus, arbitrary
keyboard shortcuts, item combinations not supported by the player and
conversation object-click branches are outside this candidate set.

At answer lists, the diagnostic mode records the actual answer line IDs,
chosen index and input event that triggered the list. The explorer forks at
that input, truncates stale descendant clicks, and supplies a global choice
prefix. The initial policy chooses available answer 0; alternatives are
then replayed independently. Branch prefixes are enumerated up to
`--max-decisions` (default 32); the player additionally stops an automatic
conversation after 128 decisions as a resource limit, not a game-defect verdict.

The state key contains current scene/layer/background, effective variable
pool and locals, active slots/objects, inventory/selection, dialogue memory,
RNG, source disc, camera and close-up return context. Animation countdowns,
current animated frame phase and dialogue *history* are intentionally omitted.
The key is a search abstraction, **not equivalence of every engine byte**.
Replays check prior checkpoint hashes for drift. Remaining frontier and
skipped candidate/depth/frontier counts are reported. Even exhausting this
bounded abstraction never sets `complete_playthrough_verified` to true.

## Findings and replay

Process failures and explicit failed actions/runtime blockers are error
findings. VM probe messages alone are not classified as committed failures.
Timeouts, decision limits, replay drift, missing actions after a real wait and
save/load contract differences are review findings. No-hotspot states alone
are never declared softlocks; terminal/pending-switch states and other
available controls are considered.

Every eight new states (`--save-every`, zero disables), when a hotspot exists,
the runner quicksaves, executes an action, quickloads and repeats that action.
It compares the declared durable fields and the resulting background identity,
active slots and effective variable pool. The pool is exported through the
existing engine local-to-global mapping without mutating live state; a normal
flush of a dirty raw pool is not reported as progress loss. Dialogue-memory
reset or an original-runtime rule may explain a remaining difference: a
repeatable difference is still a **review candidate**, not automatically a bug.

A unique finding is confirmed in another fresh process when budget permits.
Each `findings/F0001/` bundle contains immutable initial save/preferences,
script, exact command, observations, logs, screenshots and `replay.json`:

```sh
python3 port/explore_game.py --replay /path/to/findings/F0001/replay.json
```

The supplied data root must remain available (override with `--data` when
moved). If ISO paths also moved, supply an updated `--media-manifest`. Initial save/preferences and core table hashes are checked. Original
media are not copied into the bundle. A changed player is allowed and reported,
so the same bundle can check a fix. Fingerprint-version differences disable
prefix-hash comparisons explicitly; the input and save/replay checks still
run. A bundle's `actions.txt` also shows any assisted seed warmup.

Exit status is 1 for tool errors, interruptions or error findings. Review-only
findings return 0 but remain `FINDINGS` in JSON/HTML. A clean bounded run is
`BOUNDED`, not an assertion that the game is complete. Timeouts terminate the
owned process group on POSIX, including decoding/extraction children. Runtime
logs and checkpoints remain available when a trial fails. Partial screenshots
cannot prevent a finding from being reported.

## Tests

```sh
make -C port test-explore DATA=/tmp/byz-scene OUTPUT=/tmp/explorer-contract
```

Use a new output directory. This checks the six real MAP candidates and natural
travel discovery, finding bundles/replay with an explicitly synthetic failing
player, changed-player replay, and all three actual AM answer choices from
the assisted fixture. The normal test target includes search-contract unit
tests for state keys, parent context, answer forking, deduplication/budgets,
issue classification, timeout cleanup and escaped/partial-image reports.

## Campaign regression suite

The 20 findings from the completed 2000-run campaign are retained as ordinary
input fixtures (not snapshots of invented VM variables):

```sh
make -C port test-campaign DATA=/tmp/byz-scene OUTPUT=/tmp/byz-campaign-regressions
```

Use a new output directory. It checks every original action/roundtrip, restores
saved states in separate OS processes, and exercises an actual Morris token
pick/placement, computer response, persisted board and usable exit. F5/manual/
autosaves now write a portable `.btb.byzport` companion for the port's exact
camera, RNG, dialogue and examine-return context. Keep it beside the matching
BTB when moving saves; original saves without companions remain supported.
The companion is byte-bound to its BTB, so stale data cannot override another
save. Test comparisons ignore lazy inventory asset-cache flags and dormant
panorama caches, while retaining possession and the active camera state.

## Mixed long-route search and the full campaign

`./port/explore_random.sh 2000` now uses `--strategy mixed --max-depth 24`.
The direct CLI still defaults to BFS, and `--strategy random` retains its
previous seeded policy for controlled comparisons. Mixed search initializes
all selected roots, then reserves50% of discovery choices for depth-biased
routes,25% for underexplored seeds and25% for uniform seed-balanced exploration.
Long inventory/camera-only tails receive a depth discount; repeated input from
the same progress context gets a lower positive weight. Item IDs distinguish
selections even when their screen coordinates match. No route is removed by
these heuristics. Existing detailed deduplication and bounds still apply.

Reports include scheduling lane/depth/category counters and the number of
progress/context signatures on each retained path. `progress_states[].replay`
links to the actual input provenance; these are observed routes, not invented
save states. A24-action path does not imply24 solved puzzles or progress steps.

Run all seven profiles with one command:

```sh
./port/explore_campaign.sh 2000
# Reproducible scheduling, if desired:
./port/explore_campaign.sh 2000 --random-seed 12345
```

The number limits the **logic phase**. Additional directed behavior checks,
32 settings cases (each with a fresh-process restart), two directed media routes,
two media-settings cases and a smaller12-launch media search run separately;
total player starts are reported explicitly. The full command writes one HTML/
JSON campaign report with links to every phase and publishes its root through
EXPLORATION_LATEST. The JSON kind is `multi-profile-campaign`; counts should be
read per profile. Original game logs/LATEST is separate. Direct Python CLI
without --publish-latest is suitable for isolated investigation runs.

Nine behavior fixtures currently check37 expected outcomes: MAP/MR travel,
MR departure from its distinct original save state, AM arrival, examined-key
identity/return, changing examined items, a real Morris turn/return and all
three actual Nuray answers. They use declared ordinary inputs and genuine
numbered saves; the Nuray approach is assisted setup, not unassisted discovery.
They do not constitute an original-runtime oracle or cover every puzzle.
Behavior assertions and media-coverage requirements enter replay capsules, so
an incorrect outcome can be reproduced even when the process exits normally.
Random exploration also checks the shared examine-parent return invariants.

Media profiles enable actual FFmpeg decoding, frame playback and SDL audio
queueing, then require return to the declared observable outcome. Enabling a
flag or merely decoding a clip is not counted as completed playback. Missing
requested assets are review findings; decode/audio failures are errors.
The current ISO set has no mm1car01/mm1meh00/mm1car02 AVI entries, so the MR
arrival profile honestly reports unavailable media while exercising the clips
that exist. No substitute clip is fabricated. Physical speakers are not tested.

Individual profiles are available through `make -C port test-behaviors DATA=...
OUTPUT=<new-dir>` and `BEHAVIOR_ARGS=--with-media`. For the full command, use
--media-runs, --logic-seconds and --media-seconds to bound its phases.

### Coverage balancing correction (30 October2026, policyv3)

The deep lane now chooses the least-used eligible starting context **before**
choosing a long route within it. Its previous global-depth choice could spend
half a run on one mature NB branch. Weights now select an intent/category first,
then a target, normalized by category size;100 inventory jobs do not buy100
chances against one exit. The25% random lane likewise chooses seed, intent,
and target. Travel intent is inferred only from the already observed kind0x17
map destinations; actual scene changes are separately counted from snapshots.

Reports add deep selections per seed, intent counts, distinct hotspot targets
attempted and observed scene-transition counts. These distinguish actual travel
from a planned travel click. The24-step ceiling remains; spreading long-route
work across49 contexts means long chains grow more gradually in small budgets.
No contexts/actions are discarded by these weights. An already running Python
logic process retains its loaded policy; newly started processes usev3.

### Random settings and hostile UI sequences

The campaign runs `settings` and `media-settings` phases alongside behavior,
story, logic and the two other media phases. Each settings case starts from a private randomized INI
and seven native preferences, changes settings through ordinary SDL menu input,
and interleaves changes with existing gameplay routes from genuine saves.
Defaults cover every one of23 port setting rows, seven native preference rows,
resize and focus events across32 cases, with five operations per case. Values,
order, route interleaving and rebound function keys are seeded; endpoints include
all toggles off/on and volume0/1. More cases repeat the coverage cycle with new
combinations. This is sampled adversarial testing, not exhaustive combinations.

Checks compare requested values with independent runtime observations: renderer
viewport/4:3 resizing, integer scaling, supported VSync, mixer gains and focus
mute, hotspot flags, language resolution, controller closure when disabled,
rebound quicksave/load, autosave during eligible travel and native video gates.
Each case closes menus, saves/loads, then starts a fresh process using the final
private preferences to check persistence. Media settings cases decode/play clips
and check native half/full video placement. Some settings are checked for applied
state and persistence only; this is not proof every shortcut or puzzle works.

```sh
# Same full campaign; settings cases are additional to the2000 logic budget:
./port/explore_campaign.sh 2000
# More settings combinations, with a separate time budget:
./port/explore_campaign.sh 2000 --settings-cases 200 --settings-seconds 1800
# Settings only, separate output (does not replace LATEST):
python3 port/settings_fuzz.py --data /tmp/byz-scene --cases 32 \
  --random-seed 1337 --output /tmp/byz-settings-new
```

Private INI/preferences/saves isolate player settings and saves. Reports retain
ordinary inputs, expected/actual checks and replay capsules; failures keep images.
Replay uses `python3 port/explore_game.py --replay <trial>/replay.json --output
<new-dir>`. It rechecks settings contracts as well as crashes. Only installed
language packs are selected. Dummy SDL validates delivered input and runtime
state; physical display disconnects, speaker output and controller hardware need
real-device checks. An already running campaign retains its loaded phase list;
the new settings phases begin on the next invocation.

### Directed story and puzzle milestones

The campaign also runs `story`, using `story-contracts.json` and
`port/test_stories.py`. These are authentic original-save checkpoints to explicit
goals, **not** a certified new-game-to-ending walkthrough. The report lists
uncovered sections. Nine cases currently cover orb/diadem progression, forum
clue exchange/fen award, the completed lintel handoff, Emre cassette controls,
Morris loss/win/draw, first clock gate and the intended three-gate clock solution.
Each goal has intermediate and final assertions for actual item states, script
flags, board outcomes and usable exits. Simply staying alive cannot pass.
The lintel fixture is already completed; it checks return/awards, not its solution.
Cassette state changes do not imply ownership or validate sound in this profile.

Eight cases save at an intermediate checkpoint, exercise the next action, reload,
then start a separate OS process from the saved BTB plus companion and complete
the remaining inputs. Both saved boundary and final durable state must match the
uninterrupted run. No variables are synthesized. Each case retains replay/logs/
checks and a named milestone report. Asset script hashes pin the expected rules.

```sh
make -C port test-stories DATA=/tmp/byz-scene OUTPUT=/tmp/byz-stories-new
# Or directly:
python3 port/test_stories.py --data /tmp/byz-scene --output /tmp/byz-stories-new
```

The clock continuation issue found by the initial suite is now fixed in the
shared engine: frame/end labels execute, and saved marker-0 CA preloads keep
their animation metadata. The clock contract now also retrieves key/glass and
returns to VT. The first-gate case saves while a hand animation is pending,
then verifies completion after reload and in a fresh process. All nine cases
and eight saved continuations pass; the old failing evidence remains unchanged.
Details: `analysis/rea/CLOCK_CONTINUATION_FIX.md`.

Exploration fingerprintv6 includes pending frame/end labels and their active
phase, so delayed script work is not merged with an idle record. Progressv3
retains pending labels/targets but ignores their frame/countdown. Ordinary idle
frame counters remain excluded. Old replay capsules still execute their original
inputs and outcome assertions; their v5 prefix hashes are explicitly not validated
against the changed abstraction. This does not certify full-game completion.
