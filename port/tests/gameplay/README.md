# Asset-backed gameplay regression

Run from the repository root after `port/scene.sh` has prepared the extracted
media root (it is only a prerequisite; the tests never invoke scene.sh):

```sh
make -C port -j2 all test
make -C port test-gameplay DATA=/tmp/byz-scene
```

The second command runs all four checks and writes a unique
`port/output/gameplay-<timestamp>/report.json`. It returns nonzero for failures
**or an unverified original comparison**. For development, explicitly allow
missing original checkpoints while retaining UNVERIFIED in the report:

```sh
make -C port test-gameplay DATA=/tmp/byz-scene GAMEPLAY_ARGS='--allow-unverified-original'
```

Use `GAMEPLAY_ARGS='--soak-multiplier 10 --allow-unverified-original'` for 200
close-up cycles in one process. A player timeout (default 120 seconds per case)
is a failure; increase `--timeout` for longer runs. Choose `--output` to retain
a named run; an existing output directory is rejected. Existing runtime logs
and `port/output/logs/LATEST` are untouched.

## Checks

* **Action chains:** EA panorama camera turn, close-up and exit; inventory
  camera selection, examine, media request and return; MAP hover/travel to MR,
  conversation completion and travel back. Starting scenes/labels and saves
  are explicit shipped fixtures, not proof of a complete new-game playthrough.
* **Save/load:** actual F5/F9 shortcuts, change view, reload, compare the scene,
  3000-variable pool, inventory records/channels and conversation memory, then
  repeat the click and verify the same background and active exits. This is a
  defined durable-state contract, not equality of every transient renderer,
  timer, sound or VM field. Additional save contexts need additional cases.
* **Soak:** twenty close-up/return cycles in one live SDL event loop. Both the
  inner view and the usable return slots must match on every cycle. A series
  of missed clicks cannot pass merely by remaining in the starting scene.
* **Original comparison:** compare every PPM checkpoint against independently
  recorded original-runtime captures. Verify executable/save/action hashes,
  image hashes, dimensions and an explicit pixel tolerance. Missing references
  are UNVERIFIED, never automatically blessed from the port's output.

The player uses SDL's normal mouse/key queue and production click/save/load
handlers. Snapshots only read state and the last presented image. The script
uses logical 640x480 coordinates (including the top bar), SDL key names,
`wait <ms>`, `snapshot <unique_name>`, and a final `quit`. It rejects malformed
or incomplete scripts. Each action is processed before the next checkpoint.
Automation takes declared conversation choices and bypasses fixed text dwell
only; it does not directly execute labels or edit state.

These are **logic/input checks with dummy SDL and audio/video disabled**.
They do not validate speakers, clip pixels/completion, real input hardware,
physical displays, live WM resizing or original-runtime equivalence. Media
requests and missing media diagnostics remain visible in reports and logs.
Save directories, preferences, settings, dialogue history and caches are
private per case. Original ISOs, extracted saves and EXE remain unchanged.

## Original-runtime evidence

With no reference set, a present repository CD2 ISO starts an unmodified EXE
probe in a private 32-bit Wine prefix and nested Xephyr server.
`--original-iso /path/to/disc.iso` overrides it. When the complete ISO set is
found, the probe prepares CD1's authentic installation and maps six separate
volume caches to CD-ROM drives D:..I:. Drive type is set in HKLM and the private
server restarted. No host mount or executable patch is required. It retains
XWD/PNG/log/provenance and cleans up its own processes. The original now reaches
its native menu without a CD dialog. Startup success still cannot stand in for
same-action original checkpoints; those remain independently required. Initial
volume preparation needs disk space for extracted media; later probes reuse
immutable source caches. Details: `analysis/rea/AUTOMATIC_DISC_ACCESS.md`.

Supply `--original-references /absolute/reference/directory` to compare actual
original captures. For each scenario, `<scenario>.json` must contain:

```json
{
  "producer": "original-runtime",
  "exe_sha256": "<authentic target SHA256>",
  "save_sha256": "<initial save SHA256 from report>",
  "script_sha256": "<actions.txt SHA256 from report>",
  "checkpoints": {
    "start": {
      "image": "start.ppm",
      "sha256": "<capture SHA256>",
      "mean_absolute_error": 0
    }
  }
}
```

Include **all** checkpoint names for that scenario; this abbreviated example
only illustrates the schema. Captures must be binary P6 RGB 640x480 with the
header `P6\n640 480\n255\n`, without WM decorations. MAE is averaged over every
RGB channel (0..255), default zero, maximum 16; declare tolerances based on
observed original-renderer differences. The manifest is provenance supplied
by the reference recorder, not cryptographic proof of its producer. No tool
can authenticate arbitrary images merely from that string. Keep capture
method, action mapping and timing evidence alongside the reference set.
