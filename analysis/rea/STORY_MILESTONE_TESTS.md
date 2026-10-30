# Story/puzzle milestone contracts — 30 October 2026

**Resolution update:** the clock blocker below describes the initial failure.
It is now fixed by shared animation callbacks and saved-preload asset restoration;
all nine cases/eight saved continuations pass. Current evidence and the stronger
clock goals are in `CLOCK_CONTINUATION_FIX.md`. The initial bundles are preserved.

## Scope

User requested more end-to-end story and puzzle tests. Added a dedicated
`port/test_stories.py` runner and `port/tests/exploration/story-contracts.json`.
This is source-level testing against already recovered engine rules and shipped
script data; no new binary behavior is inferred, no engine logic is changed,
and original ISO/save/EXE bytes are preserved. It is not an independently recorded
original-runtime oracle or a complete new-game-to-credits playthrough.

Each case declares its authentic starting save, ordinary SDL actions, intermediate
and final assertions, named milestones, evidence and optional saved continuation.
Missing or wrong fields/checkpoints fail closed. A process exiting0 is insufficient.
Manifest validation requires final assertions and observable milestone checks.
The report explicitly lists uncovered early-game, cross-save chapter continuity,
optional dialogue branches and ending/credits coverage. No expected state is
synthesized, no labels are run directly and no failing goal is blessed from a
port-generated screenshot. CC hashes below pin the shipped script data used as
evidence. Existing Runner replay capsules retain input and source hashes.

## Implemented goals

| Case | Authentic checkpoint | Observable goal |
|---|---|---|
| Omphalos orb/diadem | VH25 | Orb state1->2; diadem0->1; global953=1; usable VH return |
| Forum clues/fen | HSTMED55 | Records19/30/47 consumed, fen74 awarded, flags464/465 set; VB return |
| Completed lintel handoff | LINTEL59 | Exit supplied completed checkpoint, year96/truth97 recovered into channel4; usable VC return |
| Emre cassette | MAP48 | Travel/approach player, operate controls, cassette27 state2 and flag315=1; leave |
| Morris loss | IU61 | Human/computer turns through terminal code1; release board, usable VC exit |
| Morris win | IU61 | Nine-human-move route through terminal code2; release board, usable VC exit |
| Morris draw | IU61 | Nine-turn terminal code3, fresh initialized zero-turn board; usable VC exit |
| Clock first gate | CLOCK70 | Hour6->11, minute11; gate9=1, other gates unset |
| Clock three gates | CLOCK70 | Insert star, ordered11/11 ->5/5 ->2/8 gates, complete flag; currently fails |

Lintel59 is already completed: the test explicitly checks handoff, not an invented
solution. State2 for the cassette is not called possession. The cassette profile
has media disabled, so it verifies controls/script state rather than audible sound.
Morris inputs were planned using the existing recovered board model with the
existing deterministic automation seed48879, then executed as actual token/board
clicks; terminal result assertions and script exits are checked separately. No
board result is injected into the runtime.

Seven cases also run a prefix, quicksave, execute the next ordinary action,
quickload and repeat it using Runner's persistence comparison. A fresh OS process
then loads the genuine saved BTB plus its byte-bound companion and executes the
remaining suffix. Both restored boundary and final durable state are compared
against the uninterrupted route, and suffix goals are re-evaluated. Its own replay
capsule preserves the actual intermediate save. Cases with failed baseline goals
cannot claim saved-continuation success.

## Actual verification

`port/output/story-chains/final-suite/`:9 cases,23 player starts,115 explicit
assertions,24 named milestones (21 pass). Eight cases PASS; seven saved
continuations PASS with no durable-state differences. One clock case FAIL;
its four failing assertions are one earliest continuation blocker plus dependent
later goals, not four independently identified game defects.

`clock-public-replay/report.json`: the same player and public explorer replay
reproduce `behavior_contract`, expected `active_slots`, with prefix validation
still enabled. Player exits0; the harness returns1 because the goal is unmet.
The story report retains screenshots, input/logs, expected/actual checks and a
replay link. `final-suite/index.html` summarizes goals, status and unmet milestones.

`build-test.log`: make -C port -j2 all test PASS. `python-tests.log`:68 Python
unit tests PASS. `ctest.log`:5 relevant CMake suites PASS. git diff --check PASS.
The asset suite intentionally returns1 for the observed clock contract failure;
it must not be described as an entirely passing gameplay suite.

## Clock observation, inference and unknowns

Observation: source CLOCK instructions593-619 define ordered hand-pair gates
11/11,5/5,2/8. The test inserts the star via normal slot26 (record21 becomes2),
selects the hour hand at(300,192), places it at(320,132), then presses the first
gate at(220,376). After a1000ms wait, both hand variables are11 and gate9=1,
but active_slots are[25,27,30,31]; hand slot0 remains disabled. Clicking the
hand next reaches full-playfield slot31 (action576), then further hand clicks
miss. Gates11/10 and complete3 remain0. Public replay reproduces this.

Inference: the frame-followup/hand reactivation path needs investigation. The
shipped script attaches extra record entries with0x8a and its hand-update labels
call the hotspot-enabling labels. Existing compose/host code currently does not
consume record.extra/field_2c as a runtime callback dispatcher. That is a candidate
shared-rule gap, not permission to insert a CLOCK-name workaround. This task
adds detection/provenance; it does not claim to have implemented that missing
runtime behavior. Original runtime timing/input and exact dispatcher semantics
remain unverified and require focused evidence before an engine correction.

## Campaign/use

The full campaign now has seven phases, inserting `story` after `behavior`.
`./port/explore_campaign.sh 2000` picks it up on the next invocation. Logic2000
remains its own cap; the story profile adds up to23 starts at current manifest.
Story errors propagate to the aggregate findings while later phases still run.
Standalone command:

    make -C port test-stories DATA=/tmp/byz-scene OUTPUT=/tmp/byz-stories-new

User's active campaign `campaign-20261030T182903-1437175` was not interrupted.
It retains its loaded six-phase list; separate inspection/test output does not
publish EXPLORATION_LATEST or normal logs/LATEST. Tests use dummy SDL devices,
not real monitors/controllers/speakers. No commit or push was requested.

## Script hashes

- VH: `0d9ab127415be2acb849c0b49671a5e53349fa76e93f269534f77d4f76264463`
- HSTMED: `48f776e86de41bd424006bdae886fc6e6604317a35d7b1dbb41ba8071b2d2d22`
- LINTEL: `1d0a2b1f8f025e44dbaf5c5f6a862441acc11fcdad6fd74442eaf819357f10b5`
- EA: `66264c55c1ac206e0d3a784f148387bd5980162c32f473ef7164a1cf42ca0b89`
- MORRIS: `d0b7d7d565636ecb64f3a75e33206b44d7d7e786ff503678f805946e66d9d67c`
- CLOCK: `e9966656c82f413cb3fb1461be3cc2bc6ef89f4ba4de98d2712efe1dd274c4b4`
