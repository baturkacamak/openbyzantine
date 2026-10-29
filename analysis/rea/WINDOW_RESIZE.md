# Repaint the current image when the SDL window changes size

29 October 2026. User screenshot showed the menu retaining its old large image
inside a smaller window, cutting off the right page.

## Observation and cause

Pinned user run: `run-20261029T215303.333604Z-siozchcw`, finished exit 0,
MR entry label 0 with save 30.btb and /tmp/byz-scene assets. Main log ends with
mode switches back to windowed; trace lines 1442–1464 include repeated size,
resize and expose events ending at 1428x784. The settings input branch consumes
these events without presenting. Settled menus only redraw on relevant input,
so SDL's refreshed logical viewport alone does not paint a resized window.

An isolated real-asset MAP menu reproduction kept the pointer stationary after
resizing 1280x960 to 800x600. `before-fit-800x600.png` shows the old 2x notebook
clipped at the new window boundaries. Against the expected complete 800x600
frame, 1,321,710 RGB bytes differ. This establishes missing repaint, rather than
requiring a new scene or mouse-coordinate rule.

SDL documents size/expose events and automatic logical-coordinate input
mapping: [window events](https://wiki.libsdl.org/SDL2/SDL_WindowEventID),
[logical sizing](https://wiki.libsdl.org/SDL2/SDL_RenderSetLogicalSize),
[integer scaling](https://wiki.libsdl.org/SDL2/SDL_RenderSetIntegerScale).
No new binary behavior was derived; this is host SDL/window lifecycle work.

## Correction

Display retains the last actually presented RGB565 image, its content owner,
hotspot offset and tag. The common logged-event poll handles resize,
size-changed, expose and restore events before an individual input owner can
ignore them. It refreshes the logical scale and re-presents the retained frame
through `present_image` under its retained ScreenContentScope. This works for
idle menus as well as scene/media event owners without executing a game action,
restarting a help fade or guessing which background to compose.

The previous raw texture `repaint_window()` shortcut was removed, restoring
`present_image` as the single SDL_RenderPresent point. Window-mode callers
already present their current result; window events now use the same route.
Preserving the content owner keeps hotspot diagnostics off menu/clip/transition
frames. Repainting a transition retains its original tag, so its departing
playfield cache is not overwritten.

Requested integer upscaling remains stepped. Below the logical image's 1x
size, the effective renderer scale temporarily becomes proportional fit so
small windows show the whole image instead of cropping. Returning to a larger
window restores integer scaling. The requested preference is not rewritten.
Both renderer drawing and SDL logical mouse mapping use the same viewport.
`window_repaint` logs reason, window/output/logical sizes, requested scale,
effective integer flag and content owner.

## Verification

`port/output/window-resize/` retains separate evidence:

- `build-test.log`: `make -C port -j2 all test`, exit 0.
- `window_probe.py`, `before*`, `after*`: real Xephyr window, xdotool delivered
  events and xwd-root screenshots. Before/after MAP menu harness is a label
  shortcut. `after-mr*` repeats the corrected sequence from the user's actual
  MR/30.btb save and asset root, with its actual GAMEPREF.000. Audio/video are
  disabled for this UI probe; no speaker or media-playback claim is made.
- Fit at 800x600, 1000x600, 480x360 and 1280x960; integer at the same sizes;
  hide/re-expose; original preferences; returning to scene and resizing it to
  960x720. Resize captures are taken without moving the pointer to trigger a
  redraw. Both final runs also toggle VSync by a correctly mapped logical
  click through a centered integer viewport (1000x600, viewport origin180,60).
- `check_pixels.py`, `pixel-checks.json`: larger fit frames match full-frame
  nearest-scaled/letterboxed references exactly (0 wrong bytes). Integer
  paper/border/letterbox outside dynamic help/control text match their expected
  placement. For 3/4 downscaling, SDL and FFmpeg can differ at half-texel ties;
  unambiguous 3px anchor grids match, and complete small frames are visually
  verified. Expose restores the exact pre-hide picture in both final runs.
  Below-1x requested-integer logs confirm effective integer=0. All recorded
  menu/settings presentation lines have overlay=0 (63 in the MR run).

No physical-input, pixel equivalence to the original Windows engine or actual
resized clip playback is claimed. The resize fix is verified on host menus,
input and scene return with actual extracted assets. Cached clip/transition
ownership follows the same shared path but was not separately replay-tested.
Private INIs/logs keep the user run, LATEST and original assets untouched.
