# Automatic ISO access — 30 October 2026

## User request and result

The user authorized a one-time ISO-directory configuration and automatic disc
access, including investigation of the original EXE's Wine CD prompt.

The port now discovers all six volumes by their actual `CD.BYZ` marker, retains
the directory, completes the VM's `0x8d` host request and resumes the running
script. Names and letter case of the ISO files are irrelevant. Missing or
ambiguous volumes fail explicitly; an incomplete discovery cannot overwrite
the previous working configuration/manifest. No ISO mounting or original file
modification is required.

The original EXE now reaches its native menu without asking for a CD. Its
private Wine prefix exposes all six authentic extracted volumes as D:..I:.
Windows API probes report type 5 and logical-drive presence for all six.
Original gameplay equivalence remains UNVERIFIED: startup readiness is not a
same-save/action comparison or a complete playthrough.

## Port implementation

`port/media_sources.py` scans the selected directory and one child level,
checks authentic volume markers using 7z, detects duplicates and validates a
complete set before publishing configuration atomically. `port/scene.sh` uses
this catalog rather than guessing disc identity from filenames. Explicit ISO
roots remain compatible with existing calls; an omitted/empty root recalls
`port/media_sources.json` (overridable with `BYZ_MEDIA_CONFIG`). The repository's
actual ISO directory has been configured locally; this per-user file is
ignored by git. Source manifests are replaced atomically, never truncated while
a player might read them.

The VM still records `0x8d` as a request. It does not assume a physical CD
exists. The host's single `run_media_segment` executes the segment from its
**pre-probe** state, verifies the requested ISO, prepares the appropriate
animation library, updates media priority and resumes after the instruction.
It is used by initial entries, scene arrivals, restarts, real clicks, CLI
clicks/labels and the headless startup path. No scene-name branches and no new
presenter or click-commit implementation were introduced.

Disc 7 uses the recovered "1, otherwise 2" choice. The exact VM return stack
is retained at a disc pause and supplied to the resumed evaluation, including
budgeted media runs. The nested-call regression proves the caller continues
and a variable is incremented exactly twice (not lost or replayed from an
already-mutated probe result). Other unresolved commands remain explicit
blockers; no unsupported VM command is silently skipped.

The active disc takes priority over both other ISOs and old flattened media.
Candidate paths are tried **within one disc before another disc**, preserving
the primary AVI / mode-folder order. This avoids choosing another disc's
primary path before the active disc's fallback folder. Exact ISO member names
come from a cached case-insensitive index, including dialogue. Extraction and
decoded frame caches include source path, size and modification time. Old
same-name decoded frames cannot mask a different disc's clip. Explicit voice
packs retain priority. Loading a save changes the animation source and source
priority before restoring the scene; a missing required source is an error,
not permission to keep the old disc's art. New saves record the active disc.

Cache identity uses stat metadata; it is not a cryptographic content-integrity
scheme. The source ISO files are treated as immutable inputs. Tests retain
source/target evidence separately.

## Original EXE / Wine investigation

The previous startup probe wrote the cdrom type under **HKCU**. Wine 9.0's
mount manager reads **HKLM** `Software\Wine\Drives` when its devices are
created. This is directly visible in the
[Wine 9.0 source](https://github.com/wine-mirror/wine/blob/wine-9.0/dlls/mountmgr.sys/device.c#L987).
A small independent Windows API probe observed `GetDriveTypeA(D:\\) == 3`
before correction, and 5 after writing HKLM and restarting the *private*
Wine server. This corrects our test-environment setup, not the game EXE.

After the CD check passed, an incomplete copied installation crashed. The
port's prepared data intentionally omits files such as `drives.btb`, cursors
and `Title.avi`; it is not a full native installation. Using the authentic
CD1 `copy2hd/` tree removed that startup failure. No claim is made that a
specific omitted file alone caused the fault. Native installation data,
engine tables, fonts, saves and setup art are copied into the private C: tree;
mutable save/config data never shares the source game's files.

`port/original_runtime_probe.py` now discovers the complete volume set, caches
separate authentic volume trees and maps contiguous CD drives D:..I: with
HKLM type cdrom. This needs no root permission, host mount or physical drive.
Its private server is restarted to apply the types. The executable remains
unmodified. The probe saves XWD plus a PNG decoded with the actual XWD pixel
size/pitch (24-bit capture with padded rows is covered by a unit test).

Observed native menu: `port/output/disc-access/final-original-startup/startup.png`.
API evidence: `drives_verified.txt` (D..I all `type=5 logical=1`). Independent
shell/Wine observations are not represented as REA-provider Evidence. The REA
connection remains unavailable as recorded in GAMEPLAY_AUTOMATION.md; no
configuration repair or whole-program decompilation was performed.

## Binary evidence used

Verified the target and selected saved exports against decompile-index.json
before deriving the host handling:

* BETRAYAL.EXE: `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`
* 0x431a69: `c2404c4cc8c7f3eb3556cd21d6f8acd96a66e49ff2a32ac5ff1e743895d032ad`
* 0x40c7b4: `a6ef5837f169597feb83c4ecf7a86e351115b3720cbde12abd1ad280e34f641d`
* 0x40c624: `fc4f2517444a68c72c590697ad57079639d8ecdcbc4fa7ff97f3fceaf4b2eeb8`

Observation: 0x431a69 implements disc-7 fallback and continues on successful
availability. 0x40c7b4 performs host drive/media checks and otherwise asks for
media. The existing dispatcher already records this as `disc_request`.
Inference/port policy: authentic, validated ISO access satisfies that host
availability request without showing a physical-media prompt. This is not a
claim that ISO extraction reproduces Windows CD-ROM internals.

## Validation and artifacts

All artifacts below are under `port/output/disc-access/`:

* `make -C port -j2 all test`: PASS (`final-build-test.log`), including five
  source/capture tests and the nine gameplay-harness checks.
* Fresh CMake automation build and selected CTest: 3/3 PASS
  (`cmake-tests.log`).
* `release-suite/report.json`: PASS. Four synthetic host cases with real
  renamed ISOs cover nested return, disc 7→1, fallback 7→2, and explicit missing
  media. A separate synthetic archive/AVI fixture proves red/blue same-name
  videos from different discs/folders both decode correctly, despite a stale
  flattened video and shared logical clip name. These fixtures are explicitly
  synthetic, not original-runtime evidence.
* The same suite executes all six **shipped MAP disc labels** from private
  copies of original save 27 with the actual asset root. Each request selects
  the correct disc and leaves clickable objects. Disc 3 is quicksaved and
  quickloaded; its variable pool survives and the actual written save dump
  reports `cd=3`. These direct-label checks are diagnostic shortcuts, not a
  natural playthrough or an assertion that every destination is complete.
* Real launcher recalls the configured root with an empty first argument and
  runs MAP label140 through disc3 without a prompt. The final stdout line stays
  `scene=… slots=…`. Evidence: `launcher-disc3-verified.log`.
* Existing five-scenario/56-checkpoint gameplay regression remains PASS for
  action chains, save/load and soak (`final-gameplay/report.json`). Its
  original-image comparison remains honestly UNVERIFIED.
* Native original startup with all six volumes: authentic menu captured,
  no CD dialog; native same-action comparisons are not yet captured.

Pinned user bundle/LATEST remains
`run-20261029T225825.927020Z-rq6cvhmb`. Launcher checks use a separate
BYZ_LOG_DIR; investigation does not replace the user's bundle. No original
ISO, executable or source game save is patched.
