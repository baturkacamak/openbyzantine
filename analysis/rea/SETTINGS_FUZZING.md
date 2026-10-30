# Random settings/UI fault search — 30 October 2026

## Scope and implementation

User requested randomized on/off settings and control checks interleaved with
play, attempting to break the port. This is source-level automation work; no new
binary behavior is inferred and no original media is modified. Existing recovered
native preference rules are exercised, not replaced. Engine scene logic is unchanged.

`port/settings_fuzz.py` adds seeded private configurations and genuine SDL menu
input between ordinary gameplay actions from20 existing regression routes. Its32
primary operations cover23 port settings, seven native preferences, resize and
focus; default32 cases rotate every primary operation and add random operations.
Configurations include all-off/all-on endpoints, volume extremes, three window
modes, cursor sizes, unique rebound function keys and installed language choices.
Each case has private INI, preferences and saves. No VM labels or invented story
variables are injected. Gameplay routes are retained provenance, not proof every
puzzle or reachable state is covered. Random settings are not deduplicated away.

`--automation-inspect-settings` adds optional read-only host/runtime observations
to snapshots. Normal automation snapshots remain unchanged. Resize calls the
normal SDL window API; focus is delivered through the normal SDL event loop.
Presentation and click commit remain single points. Player builds publish the
finished executable atomically so rebuilding does not truncate the executable
being launched by an existing campaign.

`settings_checks.py` checks requested values against applied viewport geometry,
renderer flags where VSync is supported, mixer gains, mute, overlay flags,
controller closure when disabled, and language resolver state. It checks native
preferences, menu dismissal, actual quicksave/load with rebound keys, persistence
in a fresh OS process, eligible-travel autosave logs and native video gates.
Media cases require actual playback completion/audio queueing and native half/full
video placement. Other settings have state/persistence checks; this is not complete
behavioral proof of every shortcut, gamepad control or native preference.

The six-phase campaign now includes32 settings cases and two media-settings
cases alongside behavior, logic, media-behavior and media-exploration. Each
settings case normally uses two player starts (original plus fresh-process check).
Thus `./port/explore_campaign.sh 2000` still caps the logic phase at2000 and adds
up to91 default profile starts, subject to time budgets. Increase with
`--settings-cases N --settings-seconds S`. The standalone settings CLI and public
explorer replay preserve exact inputs, initial profiles, expectations and save keys.
Failed contracts retain images even if the player exits successfully.

## Observed evidence

All evidence is separate under `port/output/settings-fuzz/`:

- `final-controls/report.json`: canonical player, seed1337,32 cases/64 starts,
  all32 operations covered,16050 settings assertions, PASS, no findings.
- `final-controls/flow-verification.json`: retrospective application of the final
  flow oracle to the retained genuine snapshots/logs,339 checks, no failures.
- `all-controls/`: earlier independent seed2029269672,32 cases/64 starts,
  all32 operations,15015 checks PASS;358 final flow checks also PASS.
- `flow-final/report.json`: final oracle integrated in execution, seed991,
  two cases/four starts,852 checks PASS.
- `full-matrix/report.json`: all six phases executed,31 total starts. Settings
  two cases/four starts and media-settings two cases/four starts PASS; latter
  has two completed video playbacks. Existing MR unavailable `mm1car01` is
  reported by both media phases as review, not a new settings/engine failure.
- `media-controls/report.json`: two additional native video-size cases/four
  starts, two actual playbacks, PASS.
- `replay-positive/report.json`: public replay including fresh restart succeeds,
  two starts, no issue (explorer status BOUNDED is normal replay completion).
- `replay-negative/report.json`: synthetic incorrect expected master volume
  yields `settings_contract` error `host.master_volume` with player exit0.
  This deliberately invalid oracle is evidence of detection, not a game bug.
- `final-build-test.log`: `make -C port -j2 all test` PASS.
- `python-final.log`:64 Python tests PASS; `ctest-final.log`: four relevant
  CMake exploration/policy/matrix/settings suites PASS. `git diff --check` PASS.

The first prototype smoke had native menu opening delay failures; automation
was corrected to wait for the existing100ms topbar activation. These were harness
errors and are retained separately in `smoke/`; subsequent smoke-fixed and full
controls pass. No game workaround was introduced.

## Limits and retained user run

SDL dummy devices validate delivered input/runtime state and genuine media
pipeline completion, not physical monitor disconnection, speaker output or real
controller input. Only the base auto language is installed in this asset root;
no fabricated language packs are used. Combinations are sampled, not exhaustive,
and no clean run proves absence of bugs.

User campaign `campaign-20261030T173323-131904` and normal log run
`run-20261029T225825.927020Z-rq6cvhmb` were pinned before testing. Investigation
runs do not publish LATEST or interrupt the user's processes. An already running
campaign retains its loaded phase list; new phases apply on its next launch.

## User live campaign inspection

Pinned `campaign-20261030T182903-1437175`. User launched200 settings cases/1800s budget. All200
configurations/400 starts completed with103808 assertions and0 findings; all32
operations covered. Behavior9/37 PASS. At this read logic694/2000,
522 detailed/323 progress contexts,
30 scene IDs (includes examine/puzzle screens), depth9,
23 save checks all PASS,0 findings. All49 deep starting
contexts received6–7 selections, so no single-root monopoly.
Observed map travel has13 destination IDs. Media phases
are pending. Parent aggregate currently records409 starts at the last phase
boundary; the live child report is authoritative for current logic progress.
Processes and pointers left untouched. Snapshot: `port/output/settings-fuzz/
live-campaign-inspection.json`. This is sampled coverage, not a complete playthrough.
