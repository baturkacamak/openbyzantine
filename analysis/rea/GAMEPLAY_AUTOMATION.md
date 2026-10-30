# Four-part gameplay regression automation — 30 October 2026

The original CD/setup blocker reported in this initial investigation is now
resolved; see [automatic disc access](AUTOMATIC_DISC_ACCESS.md). The native EXE
reaches its menu with six CD-ROM volumes. Same-action reference comparison
remains UNVERIFIED; the earlier run results below are retained as history.

## Scope and result

Requested automated action chains, save/load equivalence, repeated interactions
and comparison with the original. The user confirmed there is no separate
working original installation; use the existing EXE/media.

Implemented an asset-backed SDL input runner, declarative cases and an
independent-original-capture comparator. Final run:
`port/output/automation-investigation/final-suite/report.json`:

| Category | Result | Evidence |
| --- | --- | --- |
| Action chains | PASS | EA close-up/exit, inventory camera examine/use/return, MAP→MR conversation→MAP |
| Save/load | PASS | Production F5/F9, durable-state comparison, replayed click and usable exit |
| Soak | PASS | Twenty close-up/return cycles in one process, both views checked every cycle |
| Original comparison | UNVERIFIED | Original runs under Wine but requests a Byzantine CD; no independent gameplay checkpoints |

Five scenarios, 56 JSON/PPM checkpoints. The overall strict command exits 1
because original comparison is UNVERIFIED. `--allow-unverified-original` lets
logic-only development runs return success while preserving that status in
the report. No original equivalence claim is made.

## Implementation and test contract

`port/gameplay_tests.py` extracts fresh per-scene tables into each run, copies
the save directory/preferences per case, records player/engine/save/action
hashes, retains commands and logs, checks committed actions and expected state,
and writes machine-readable category results. It never invokes scene.sh and
therefore never replaces the user's LATEST bundle. Dialogue history, config,
saves and cache writes are private. Normal assets/ISOs are read inputs.

`--automation` scripts enter through SDL_PushEvent in the normal event loop.
There is no duplicate click executor or test-only VM label mutation. Logical
coordinates are transformed back to window coordinates before SDL's event
watch applies its usual mapping. SDL_PushEvent rewrites the event in place:
mouse-up must restore the original window coordinates after mouse-down, or the
renderer scales the same event a second time. The initial harness exposed
this and it was corrected before accepting the suite. Each click is followed
by the normal handlers before a subsequent snapshot.

Snapshots read scene/layer/background, active slots, variable pool/locals,
inventory/channels/selection, objects, sound state, conversation memory
(including overrides, pending reply and the three slot tables), panorama
position, UI state and media profile. PPMs copy the last presented image;
no second SDL_RenderPresent was introduced. Snapshots are diagnostics, not
replacement serialization. The save contract compares the declared durable
subset and the post-load action result, not every transient field in memory.

Automation uses declared conversation choices and bypasses fixed caption dwell
times. Actual enabled media still uses the existing playback path. These
checked-in cases deliberately disable audio/video and use dummy SDL: they
validate script/input/state and media *requests*, not clips, speakers, physical
input, monitor behavior or WM resize. Missing media diagnostics are preserved
in each report. The MR case includes missing clip diagnostics in this data
root; its PASS describes conversation script completion with media disabled.

The initial soak assertion only checked the returned view and could pass on
missed input. Final assertions require the inner close-up and returned slots
on **every** cycle. Required log markers additionally prove inventory media
request, close-up return, both travel commits and completed conversation.
A click stopped by an unresolved command fails the case.

## Original runtime observation and limits

`port/original_runtime_probe.py` launches an unmodified copy in a private
32-bit Wine prefix and an isolated Xephyr server. The existing play-disc ISO
is offered through a private D: directory (authentic CD.BYZ) and D:: raw ISO
link, with cdrom drive type. Only private game saves/preferences are copied;
original files are unchanged. Processes are cleaned up after bounded startup
capture. The final suite automatically ran this probe and retained XWD/logs
under `final-suite/original-startup/`.

Observed screenshot (independent shell/Wine evidence, **not REA provider
Evidence**): `original-probe-3/startup.png`, the Turkish "Herhangi bir Byzantine
CD'sini yerleştirin" dialog. First directory-only and then raw-ISO attempts
both reached that dialog. This does not establish why Wine's media setup is
rejected and is not proof of an engine defect. Startup is never an oracle PASS.

The REA skill was consulted for shipped-runtime evidence. No REA MCP provider
was available; the existing rea.sh doctor could not launch its stale CLI path.
No configuration repair, whole-program decompilation or executable patch was
performed. Existing saved CD-check exports were read; hashes verified against
`pseudocode/decompile-index.json`:

* Target BETRAYAL.EXE:
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
* 0x40c911: `b811903f7ee7b31034e74d8dfdd2359b57fa819c6c241777c1e9c230fe9da1a4`
* 0x40c9ce: `92129e7865232432b040fd322cf018d57c0bf826e373f7014ce7a769c72d35cd`
* 0x40c70e: `775757b8f089c7230ba60ae8daafe895aa32bd8ab9401398ec0bdfe00e85bacf`

Observation from those saved exports: drive enumeration requires type 5;
0x40c70e opens the marker and checks its first character for a requested disc.
No new engine behavior was derived or changed from this inspection.

The comparator requires independent original-runtime manifests matching target,
initial save and action hashes and covering every script checkpoint. It checks
image hashes, exact 640x480 P6 layout, bounds reference paths, and applies a
stated full-image RGB MAE tolerance (default 0, bounded 0..16). It refuses port
self-generated references and empty/partial manifests. The producer string is
an explicit provenance declaration, not authentication of arbitrary captures.
Missing references remain UNVERIFIED. See `port/tests/gameplay/README.md` for
capture schema and remaining original media prerequisite.

## Validation

* `make -C port -j2 all test`: PASS, including strict script parser/escaping
  tests and nine harness tests (missed action, ended session, missing/different
  state, missing/forged original, original pixel/coverage checks, incomplete
  process, nonzero exit and timeout).
* Fresh CMake automation target build and CTest `automation` /
  `gameplay-runner`: 2/2 PASS (`cmake-tests.log`).
* Asset-backed final suite: all five port scenarios PASS, original UNVERIFIED,
  strict exit 1 as designed. Earlier explicit partial mode exited 0 with the
  original status still UNVERIFIED (`suite-4/report.json`).
* User run pinned before tests and preserved:
  `run-20261029T225825.927020Z-rq6cvhmb`; LATEST still names that run.

This is a bounded regression suite, not proof every scene can be completed or
that no bugs remain. Add action chains and independently captured references
as evidence grows; never update expectations merely to bless a failing run.
