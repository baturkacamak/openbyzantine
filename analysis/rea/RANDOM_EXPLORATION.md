# One-command seeded random exploration — 30 October 2026

User requested a button-like way to run 1,000–2,000 random automatic trials and
then inspect the logs. The old explorer was breadth-first, with small caps;
changing only max-runs would stop on its 40-state bound. Added a genuinely
random, seeded search and a separate long-run launcher.

The starting-pool and scheduling policy was subsequently broadened; see
[MULTI_START_EXPLORATION.md](MULTI_START_EXPLORATION.md). The launcher now uses
all valid numbered saves and balances random selection by starting context.

## Interface

From the repository root:

```
./port/explore_random.sh 2000
```

Omitting the count also means 2,000. Each launch generates and records a new
32-bit random seed. `./port/explore_random.sh 2000 12345` selects a known seed.
The script builds the port, uses prepared BYZ_DATA (default /tmp/byz-scene),
and runs with these explicit bounds:

* Total player launches: supplied count, default 2000.
* State cap: same count, so the old small-state limit cannot stop the run.
* Path depth12, frontier20000, up to32 candidate actions per state, answer
  branch prefix limit64.
* Save/replay check every20 newly discovered states.
* Trial timeout60 seconds, overall exploration budget14400 seconds (4 hours).
* Actual game data, private mutable inputs/caches, dummy SDL, audio/video off.
* Keep screenshots for findings; keep JSON checkpoints and logs for all trials.

Additional explorer budget/profile flags may follow the numeric count and
numeric seed. Wrapper-owned output/replay/strategy/seed flags are rejected;
use the direct Python CLI for those modes. Missing prepared data is an explicit
setup error; the launcher does not start the normal game to prepare it.

A trial is one fresh player process replaying an action path. Different item
choices, camera directions, answers and scene transitions can share the same
scene. The count is **not** a number of different scenes or guaranteed distinct
states. Save checks and finding confirmations also consume the total launch
budget. The report separately exposes discovery_trials, distinct states,
scenes, duplicate states and requested_run_limit_reached. A finite frontier,
time limit or other bound can stop before the requested count; no repeated
trials are fabricated merely to reach a quota.

## Search policy

`--strategy bfs` preserves the existing behavior. `--strategy random` uses an
independent Python Random instance with the recorded `--random-seed`. Every
initial authentic seed runs once before random descendant selection. Pending
paths are selected uniformly from the current frontier; observed candidates
are shuffled before applying the per-state action limit. Thus the profile
randomizes paths/actions and the order of available answer branches; it is
not a renamed sequential sweep. Every executed path still has explicit input
and answer-prefix replay files. No hidden labels or invented game state are
introduced.

The same PRNG seed reproduces scheduling when observations/frontier are the
same. Engine timing, resource interruptions or changed data/player can alter
observations; no bit-identical full-run guarantee is made. Individual recorded
paths retain the existing immutable-save/preferences and core-table hash
checks for reproduction.

## Logs and footprint

Each launcher run gets `port/output/random-<UTC>-<pid>/` containing index.html,
report.json, per-trial artifacts, findings and runner.log. While running,
startup/console output is initially its sibling `<run>.log`; normal completion
moves it into the run directory. `port/output/EXPLORATION_LATEST` atomically
records the most recently started exploration attempt. A failed setup may have
only the sibling log. This pointer is independent of `port/output/logs/LATEST`;
the user's normal game bundle is never replaced.

`--screenshots findings` deletes healthy trials' PPM files after classification.
It retains their JSON/input/log evidence and keeps finding screenshots and
confirmation/bundle evidence. It does not change the state key or actions.
Direct explorer/replay defaults still keep all screenshots. The HTML remains
available without healthy-trial images; finding before/after previews remain.

## Validation

Artifacts: `port/output/random-investigation/` and
`port/output/exploration-investigation/random-launcher-smoke.log`.

* Actual launcher smoke, count8/seed314159, authentic MAP/27 start: **8
  launches, 8 distinct states, 4 scenes (EA/MAP/MR/TK), max_runs reached**, no
  findings. It retains runner.log, JSON/HTML and the separate latest pointer;
  healthy PPMs are pruned. This is a small launcher validation, **not a claim
  that 2,000 trials were executed**. Smoke path:
  `port/output/random-20261030T121324Z-552213/`.
* Added three unit checks for deterministic random frontier order, all initial
  seeds preceding descendants, and seeded non-mutating candidate selection.
  Explorer contract suite now has17 checks.
* Asset contract now exercises random mode/report metadata and healthy-image
  pruning in addition to previous discovery/replay/actual-dialogue checks.
* Replayed the existing HL review candidate with finding-only retention:
  its same review difference is retained together with screenshots
  (`random-investigation/finding-retention/`). This validates retention, not a
  new original-game correctness verdict.
* `make -C port -j2 all test` and selected CTest results are retained as
  `build-test.log` and `ctest.log`.
* User game LATEST remains `run-20261029T225825.927020Z-rq6cvhmb`.

No C++ game behavior changed in this step; only the Python exploration policy,
artifact retention, report metadata and launcher changed.
