# Stop aspect correction from fighting a live WM resize

29 October 2026. User reported that a slight drag started a repeated,
ticker-like resizing motion. This is a regression in the previous host
aspect-ratio change, not original game behavior.

## Evidence and cause

Pinned user run: `run-20261029T223058.162357Z-3o9rwdk9`, finished exit0.
The main log contains 74 aspect actions, including 73 live resize corrections. Near the end of a drag it alternates
between requested1086x814 and1135x851, issuing corrected1084x813 and1136x852
sizes every few dozen milliseconds. Both requested sizes already approximate
4:3 within device-pixel rounding. The repeated SDL_SetWindowSize calls change
the WM's active resize geometry/anchor and trigger another size event: an
application-versus-WM feedback loop. Focused source log is retained separately
as `reported-aspect-events.log`; the original bundle is unchanged.

The previous brief resize test released the grab too soon to establish
stability during a stationary held-pointer period. That validation was
insufficient for this bug; the new probe holds the actual corner grab and
samples window geometry over time.

## Correction

- When native aspect hints are supported, size events never request a new
  window size. The WM owns the live resize, including harmless pixel rounding.
- Native X11 aspect property updates are idempotent. Renderer setup and size
  operations can overwrite SDL's WM hints, so missing hints are repaired
  without changing window geometry or repeatedly rewriting an unchanged hint.
- The portable fallback collects resize events, retaining the drag-start
  dimensions. It waits180ms after the last size event and until all mouse
  buttons are released before considering one correction. Mode changes cancel
  pending work. Near4:3 sizes (roughly one-pixel rounding) are accepted without
  snapping to exact4x3 units. Processing occurs on the existing poll path,
  without blocking sleeps or a new event loop.
- Explicit opening/mode changes can normalize initial non4:3 dimensions once.
  Normal image fitting, shared redraw/input mapping, gentle help animation,
  the sole present_image presenter and desktop-only fullscreen remain intact.

New synthetic-clock settings tests cover native ownership, reported rounded
sizes, debounce extension, held-pointer suppression, drag-start preservation,
one-shot completion and mode-change cancellation. No new game state, VM
behavior, scene branches, binary evidence or external/runtime dependency was
introduced.

## Verification

Evidence is separate in `port/output/window-resize-feedback/`. Both native and
no-X11 fallback player builds use private MR/30.btb/preference copies, private
host configuration and an isolated GNOME WM inside Xephyr/private D-Bus/XDG
with in-memory desktop settings. Input is delivered with xdotool; screenshots
use xwd-root. Audio/video are disabled: no speaker or physical-monitor claim.

The final held-pointer/geometry results, build logs and UI checks are recorded
in this directory. Original user run and LATEST remain pinned and preserved.

Final results:

- `build-test.log`: final `make -C port -j2 all test` exits0, including the
  reported rounded-size and resize-ownership/debounce/held-pointer tests.
- `cmake-build.log`: no-X11 fallback player rebuild succeeds.
- `checks.log`: deterministic real-asset menu/help/input checks pass.
- `wm-checks.log`, `held-resize.json`: actual GNOME/Xephyr held-button corner
  grab receives tiny pointer moves, two1s stationary held periods, another
  move and release, then20 further samples. Geometry is constant in both
  held-pointer sampling windows and all released samples. Actual corner
  resizing is asserted, so this is not a pointer-only or label shortcut test.
- `fallback/` repeats the actual WM/held-pointer sequence with the no-X11
  player. Its held/released geometry is stable too. Extra keyboard WM resize,
  mode cycle, external maximize/restore, fullscreen shortcuts and unchanged
  XRandR mode checks pass in both builds. These are virtual display tests,
  not physical dual-monitor/speaker validation.
- `stability-checks.json`: zero live resize corrections in both new runs,
  zero settled corrections in the native run, and zero measured stationary
  width/height jitter. All recorded menu presents keep overlay=0.

Native WM rounding is retained: examples in the stable new sequence include
485x364 and510x383. Forcing these onto exact multiples would recreate the
reported regression. The previous report's exact-integer-client promise is
superseded by WM-owned4:3 with normal device-pixel rounding; fallback fitting
happens only after a drag settles. Reported user run and LATEST are unchanged.
