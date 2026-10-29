# 4:3 normal windows and desktop-only fullscreen

29 October 2026. User requested that normal windows resize in the game's
aspect ratio and that the picture follow the window. They also reported one
physical monitor losing signal while cycling display options on a dual-monitor
machine.

## Evidence, inference and unknown

Pinned user run `run-20261029T221445.427733Z-7ohjwdtp` finished exit0. Its log
records borderless2560x1440 at +11.162s, then exclusive1280x960 at +13.180s.
This establishes a real display-mode-changing path in the application. SDL
[documents](https://wiki.libsdl.org/SDL2/SDL_SetWindowFullscreen) exclusive
fullscreen as changing the video mode, unlike desktop fullscreen. It is a
plausible trigger for the reported monitor problem, not proof of a specific
GPU driver, cable, monitor or hotplug failure. The narrow kernel journal slice
covering that run had no entries (`kernel-window.log`); hardware cause remains
unknown. The physical failure was not reproduced or re-triggered.

## Implementation

Normal window client areas now retain exact4:3 dimensions. The resize geometry
helper follows the proportionally larger width/height change, rounds to4x3
units, enforces a small interactive minimum and fits the current display's
usable bounds including window decoration sizes. Existing saved non4:3 sizes
are normalized on opening/restoring.

On X11 an optional native WM_NORMAL_HINTS PAspect constraint supplies4/3 as
both min/max ratio, allowing the WM to constrain interactive drags. SDL's own
size/resizable setters overwrite these hints, so they are reapplied afterward.
The common SDL resize/restore path also corrects client geometry, providing
a portable fallback for builds/backends without that native facility. New
window_host code is C++17; X11 is optional in both Make and CMake. No X11
headers or link dependency are required for the fallback build.

In normal windowed mode the image always fits proportionally, even with an
old integer profile. Its settings row shows Oranlı and explains this fixed
normal-window policy; clicking it does not select a misleading inactive value.
The stored scaling preference remains usable in maximized/desktop-fullscreen
modes. Existing shared repaint and logical input mapping remain in charge.

Exclusive fullscreen has been removed from the mode enum and selection cycle.
The remaining modes are normal window, desktop borderless fullscreen and
maximized window. Both F11 and Alt+Enter use desktop borderless fullscreen.
Legacy config values exclusive/2 and the old --exclusive switch migrate to
borderless; the CLI logs that migration. The numeric maximized value3 is
retained for compatibility. No SDL_SetWindowDisplayMode call or exclusive
SDL_SetWindowFullscreen call remains. Source help and current README controls
are updated. Original game video preferences and assets are unchanged.

These are host/window rules, with no new binary behavior derived, no scene
branches, VM state changes or additional presenter. Cached frame presentation
continues through the one Display::present_image call site.

## Validation

Separate evidence: `port/output/window-aspect/`.

- `build-test.log`: `make -C port -j2 all test`, exit0. Settings tests cover
  dominant resize edges, exact ratio, rounding, minimum/usable bounds and
  legacy exclusive migration, including both numeric and string forms.
- `cmake-config.log`, `cmake-build.log`, `cmake-no-x11/`: clean CMake player
  build with X11 discovery explicitly disabled succeeds.
- `verify.py`, `checks.log`: existing real-asset deterministic menu/help,
  slider/toggle/key-capture/navigation and final stdout contract checks pass.
- `wm_probe.py`, `wm-checks.log`, `wm.log`, native-hints.log and PNG/XWD files:
  actual GNOME WM within isolated Xephyr/private D-Bus/XDG directories,
  in-memory desktop settings and private copies of MR/30.btb and GAMEPREF.000.
  An old integer profile opens as a filled normal900x675 client. Programmatic
  width, height and corner resizing, an actual Alt+F8 WM resize grab, external
  maximize/restore, the three-mode cycle, F11 and Alt+Enter pass. Every checked
  normal client satisfies3*width ==4*height. Native hints show4/3 min/max.
  Restoring returns to900x675. XRandR --current output is identical to its
  baseline after each resize, each selected mode and each shortcut.
- `fallback/wm_probe.py` and its logs/captures repeat the same actual-WM checks
  with the CMake no-X11 binary. SDL geometry correction, fitting, manual WM
  resize and all desktop-mode checks pass without native aspect hints.
- `audit-checks.log`: all recorded menu frames retain overlay=0 and neither
  runtime log contains an exclusive transition. Kernel evidence is bounded
  to the reported time, not a claim of a diagnosed driver failure.

Window captures use xwd-root and delivered xdotool input. Audio/video are
disabled for these UI probes; no physical-input, speaker or physical
multi-monitor hardware validation is claimed. Only the isolated virtual
output's unchanged mode was measured. Original user run/LATEST and active
physical displays were not modified by the tests.
