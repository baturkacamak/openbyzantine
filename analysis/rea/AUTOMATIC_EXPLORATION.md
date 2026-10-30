# Bounded automatic gameplay discovery — 30 October 2026

## Request and delivered behavior

The user asked for automatic route discovery instead of manually replaying the
whole game. Added `port/explore_game.py`, a bounded breadth-first explorer that
uses the actual SDL player, immutable original-save fixtures, observed input
candidates and fresh-process path replay. It produces HTML/JSON reports and
replayable finding bundles. It neither edits game state nor executes hidden VM
labels to manufacture progress. This step is source instrumentation and runtime
port testing; no new original-binary semantics were derived or decompiled.

Run `make -C port explore-game DATA=/tmp/byz-scene`.
Usage and scope: `port/tests/exploration/README.md`.

## Architecture and scope

The existing `--automation` input mechanism remains the only event injector.
`--automation-explore` adds read-only observations and global, recorded answer
prefixes. Normal interactive play and the old per-conversation explicit-choice
mode retain their behavior. Scene actions still pass through normal SDL input,
`plan_hotspot`, `execute_plan` and the single `commit_click`. No extra presenter,
scene-name rule, label executor or inventory mutation was introduced.

Snapshot discovery samples the real first-hit/inverse-panorama function over
the visible playfield. Inventory selection/examine/scroll, camera keys and a
bounded wait are also candidates. Item compatibility is checked only while an
item is actually selected, matching the player (applying it unconditionally
would incorrectly hide map destination selectors). Actual MAP metadata is
covered by an asset test requiring all six visible destination candidates.

Every branch restarts from a private copy of the initial authentic save and
replays its inputs. Intermediate quicksaves and mutated probe results never
become the search's starting state. Seed setup, preferences, config, history,
saves and extraction/decoding caches are isolated from user data. Disc-specific
animation overlays are prepared from actual volume markers. Default seeds
EA/26, MAP/27 and MR/30 are declared diagnostic starting contexts, not proof of
new-game reachability.

Answer observations record the real list's scene/record/group/line IDs, chosen
index and triggering automation event. New branches replay the same prefix,
change one answer and stop stale descendant input at the triggering action.
Default answer zero is a declared exploration policy over available rows.
Already observed explicit/default-zero prefixes are deduplicated. Answer lists
use the existing `conversation.choose` acceptance path rather than synthetic
VM writes; scene clicks and camera/inventory input use SDL events.

The search key includes scene/layer/background, current effective variable
pool and locals, active slots/objects, inventory/held item, dialogue memory,
RNG, source disc, camera, record identity and the close-up's parent context.
Raw dirty pool flushes are normalized through the existing, read-only
`export_variable_pool` mapping. Snapshot files retain both raw and effective
pools. This corrected an initial harness false positive; it did not change
save semantics or bless an observed failure. Key version 4 is explicit in
replay manifests. Animation countdown/current frame phase and some other
transient/unsupported state are intentionally outside this abstraction.
Deduplication therefore does not prove byte-for-byte semantic equivalence or
complete path coverage. Older key versions disable prefix comparison explicitly.

Separate caps bound launches, states, path depth, actions per state, answer
prefixes, frontier size, trial duration and exploration time. Deferred actions,
frontier and depth limits remain visible. The queue being empty means only
that the bounded abstraction was exhausted. `complete_playthrough_verified`
never becomes true. Defaults disable audio/video; `--with-media` uses actual
media with dummy SDL devices and requires realistic playback timeouts.

## Finding policy and artifacts

Process failures and explicit failed committed actions/runtime blockers are
error findings. Probe traces alone are not evidence of a committed failure.
Timeouts/decision limits, replay drift, lack of actions after a real wait and
save/replay differences are review findings. A no-hotspot state alone is not
called a softlock: terminal/pending-switch states and remaining player controls
are considered. No finding in this report is asserted equivalent to an
original-runtime defect.

Periodic checks use actual F5/F9: save, perform an observed hotspot action,
load and repeat that action. The durable/effective state and resulting
background/active slots are compared. A dialogue-memory reset may reflect an
original rule or incomplete save semantics; it is deliberately a review item.
Unique findings are replayed in a fresh process when budget permits, including
the entire save/action/load/action sequence for save differences. Confirmation
is retained separately from severity: a repeatable difference remains a review
candidate. A confirmation timeout cannot erase the original finding.

Bundles contain immutable initial save/preferences, exact inputs/command,
logs, JSON/PPM checkpoints, lossless PNG previews and `replay.json`. Relative
seed paths let the bundle move; copyrighted assets remain external. Save,
preference and core-table hashes are checked. The player may change for fix
validation; that fact is recorded. HTML escapes log text and tolerates partial
screenshots from failed/timed-out processes. POSIX timeouts/interrupts terminate
the owned process group. No user processes or user LATEST bundle are targeted.

Replay example:

```
python3 port/explore_game.py --replay \
  port/output/exploration-investigation/verified-discovery/findings/F0002/replay.json
```

## Observed run

Artifacts: `port/output/exploration-investigation/verified-discovery/`.
This unassisted search starts only from authentic MAP/27 label87, with bounds
55 launches, 30 states, depth4, 180 seconds, save checks every six new states.
Observed: **41 launches, 30 distinct abstract states, 7 scenes**:
EA, HL, MAP, MR, SM, SP and TK. It stops at max_states with **316 queued jobs**,
so this is explicitly incomplete exploration. Five save/replay checks ran.
All three unique review candidates were reproduced in fresh processes:

| Bundle | Scene | Difference | Classification |
| --- | --- | --- | --- |
| F0001 | MR | conversation memory | Reproduced review candidate |
| F0002 | HL | post-replay active slots and effective pool | Reproduced review candidate |
| F0003 | SP | conversation memory | Reproduced review candidate |

These are possible follow-up investigations, not three proven bugs. The HL
bundle was independently replayed through the public CLI:
`verified-replay/report.json` retains the same review signature, same player
hash and successful prefix-hash validation. The HTML and generated PNGs were
inspected; screenshots contain actual presented game frames.

## Validation

* `make -C port -j2 all test`: PASS. Includes 14 explorer contract tests for
  state/clickability/parent keys, malformed inputs, answer-prefix truncation,
  startup answer branching, probe-vs-action classification, budget/dedup,
  timeout cleanup and escaped/partial-image reporting.
* `tests/explorer_assets.py`: PASS. Actual MAP candidates/natural travel;
  an explicitly synthetic failing-player fixture creates a confirmed bundle;
  the public replay reproduces that fixture, and replacing it with the actual
  player clears it while recording player_changed. The synthetic failure is
  never represented as a discovered game defect.
* The same asset contract uses an **assisted** AM/Nuray fixture, with actual
  camera keys and two scene clicks, to reach a known answer list. Three runs
  automatically try choices0/1/2 and retain three result states. This validates
  answer discovery/forking; the assisted approach is not part of the unassisted
  seven-scene coverage claim.
* Fresh selected CTest: automation, media-sources, gameplay-runner and
  exploration — 4/4 PASS.
* Existing five gameplay scenarios /56 checkpoints: PASS for port logic; their
  original-runtime reference comparison remains UNVERIFIED as before.
* An enabled-media EA startup smoke completes using dummy SDL devices. It is
  not physical speaker/video-pixel validation or full media-route coverage.

Evidence logs: `verified-assets.log`, `verified-build-test.log`,
`verified-cmake-tests.log`, `gameplay-regression.log`, `media-mode/`,
`verified-replay/`. Pinned user run/LATEST is unchanged:
`run-20261029T225825.927020Z-rq6cvhmb`. Source ISOs/EXE/game saves are not modified.
