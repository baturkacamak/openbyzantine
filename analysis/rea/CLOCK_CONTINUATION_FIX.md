# Shared animation continuation and saved-preload fix — 30 October 2026

## Result

The formerly failing clock chain now solves all three gates (11/11,5/5,2/8),
acquires key25/glass26 and returns to VT with usable controls. Both engine fixes
are shared rules with no scene-name branch. All nine story cases, eight saved
continuations and the earlier20 regression fixtures pass. Missing MR clips remain
an asset-availability limitation, explicitly separate from the corrected engine.

## Verified evidence

Used the reverse-engineer-anything workflow against cached REA/Ghidra exports;
no new whole-program decompilation or engine-source search. The original target
`analysis/rea/BETRAYAL.EXE` SHA256 is
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Target and21 selected export hashes match `pseudocode/decompile-index.json`.
Full selected hash ledger: `port/output/clock-continuation-fix/verified-exports.json`.
Original ISO/EXE/save bytes are unchanged.

Observed static rules:

- `0x43b216` accepts records in kind0..4/highest-ID-first draw order, calls
  `0x43cd2c` before stepping and `0x43bb77` afterward, sharing one output pointer.
- `0x43ccae` supplies the current zero-based frame; `0x43cd2c` finds the first
  matching extra (+0x34 list), writes that entry's label and calls `0x432be2`
  to remove exactly one entry. Frozen records can still carry frame callbacks.
- `0x43bb77` consumes +0x2c at either forward or reverse tail only with a live
  output receiver, clears it to-1, then applies freeze/loop/ping-pong flags.
  Its end event can replace the earlier frame event. Later records can replace
  earlier output; events are not accumulated into an invented queue.
- `0x432b09`/`0x432c65` establish frame-list entries/end labels;
  `0x40ffee` initializes +0x2c/+0x30 to-1, not0.
- `0x43c466`/`0x42d594` pass the live instruction pointer through rendering and
  execute a redirected pointer using the ordinary script interpreter.
- `0x425ab4` calls `0x42d52f` then `0x40fd7b` BEFORE `0x426b9d` reads saved
  records. CA type2/3 assets are already loaded. Marker0 reuses that record;
  marker1 calls `0x40fe70`. The port previously restored marker0's scalars but
  lost its frame metadata. CLOCK minute record5 was the concrete example:
  type2,12 frames but frames_known=0, so the live clock would not step it.

The initial clock report's callback-gap hypothesis is therefore confirmed by
static rules and the corrected runtime chain. Saved-preload metadata loss was a
second independently observed reason the later hand could not complete.

## Implementation

`advance_record_pass` now observes one normal draw pass and returns its final
callback, consuming frame/end entries exactly once with the recovered order.
`advance_record_animation` keeps callbacks pending for null receivers (headless
composition), supports single-frame end events and the reverse stop-at-end flag.
Defaults use the original-1 sentinel. Trail copies are explicitly tagged in draw
reports so they cannot step or dispatch the same record an extra time.

The live host dispatches the returned label through the existing transactional
probe, execute_plan/run_media_segment and commit_click path. It executes from
entry_before, not probe output; unsupported commands are explicit errors. Each
pass completes before dispatch; another pass recomputes the accepted draw list
if the callback changed records/layers. Media, conversation and scene switches
reset animation debt. Presentation, transitions and refused-switch behavior keep
their existing single points. Event/menu/media repaints no longer step animation
outside the live pass clock; deterministic headless composition still can.

Saved marker0 CA type2/3 records now reattach immutable frame metadata, like the
original preload before restoration. Saved x/y/current-frame are retained and
frame-0 offsets are not added a second time. Lazy marker1 restoration remains.

Exploration snapshots expose only script-bearing record phases. Fingerprintv6
includes these fields; progressv3 includes pending labels/targets without the
frame/countdown. Idle animation counters do not create unbounded novelty. Older
prefix hashes are explicitly version-incompatible; original inputs and directed
outcome assertions remain replayable. Native save format is unchanged.

## Runtime verification and test corrections

Evidence root: `port/output/clock-continuation-fix/`.

- `before/report.json`: original failure reproduced from the original capsule,
  missing hand hotspot0 after first gate; user bundle/pointers untouched.
- `verified-stories/report.json`:9/9 PASS,25 player starts,136 explicit outcome
  checks,27/27 milestones,8/8 saved continuations. The first-gate case now saves
  while the hour animation has pending frame callback[170,11]; waiting/reload and
  a new OS process must complete that same callback and permit the first gate.
- The full case saves after gate2, resumes in a fresh process, solves gate3,
  acquires key/glass and returns to VT. Assertions retain the original goals;
  no state/label shortcut is introduced.
- `clock-media/report.json`: full chain PASS with actual FFmpeg/SDL media,
  two completed clip playbacks and two positive audio queues. Dummy devices do
  not validate physical speakers or original-renderer pixel equivalence.
- `verified-regressions.log`:20/20 earlier fixtures PASS; matching portable
  saves restored in separate processes and actual Morris move/exit remain usable.
- `verified-broad/report.json`: final fingerprintv6,all49 starts,80 launches,
  no findings. An earlier160-launch sweep of the corrected engine also had0
  findings (`broad-check/`, before the snapshot abstraction update).
- `settings-regression/report.json`:8 cases/16 launches,3737 checks PASS.
- `verified-replay/report.json`: public replay of the pending-animation goal
  passes with the same player, no issue (BOUNDED is normal replay completion).
- `build-final2.log`: required make -C port -j2 all test PASS; C++ tests cover
  frozen/duplicate/hidden events, end override, reverse/single-frame events,
  null receivers, output order and marker0/1 asset attachment.
- `python-verified.log`:69 tests PASS. `ctest-verified.log`:5 relevant CMake
  Python suites PASS. git diff --check PASS.

The intended clock contract also needed two harness corrections, kept explicit:
hand-placement waits now allow the global60Hz delay10 and nine-frame wrap (about
1.65s; bounded2500ms wait). The solved clock adds a glass hotspot overlapping
old coordinate588,368; the actual exit candidate is596,368. The test collects
both rewards then uses that real exit, rather than mistaking a glass pickup for
travel. Initial fixed1000ms timing and guessed exit are preserved in intermediate
bundles; their failures are not additional engine defects. A JSON separator typo
in the new diagnostic was caught by actual pending snapshots and corrected;
`stories-v6/` retains that harness evidence, not a game-bug verdict.

## Missing media: observation and limit

`missing-media-recheck.json` repeats read-only full7z listings of all six current
ISOs. Exact case-insensitive AVI basename search finds no mm1car01.avi,
mm1meh00.avi or mm1car02.avi. No replacement is fabricated, no diagnostic is
suppressed and no original files are modified. Their absent content cannot be
recovered from code; authentic files are needed to restore those particular
videos. Existing missing-media/text continuation behavior remains explicit.

## Remaining unknowns

The port's60Hz reference is a documented host timing choice; original timings
are machine-dependent. This validates real SDL-delivered inputs/dummy devices,
not hardware input, monitors or speaker output. No complete new-game-to-ending
or original-runtime equivalence claim is made. No new2000-run campaign, commit
or push was performed. EXPLORATION_LATEST remains the user's completed
campaign-20261030T182903-1437175, normal logs/LATEST remains
run-20261029T225825.927020Z-rq6cvhmb.log.
