# Real mouse coordinates in the SDL2 renderer

Date: 26 October 2026.

Status: implemented and committed separately from the conversation/ESC work.
This report covers the common input path only; it is not scene-specific.

## Why the old mapping was wrong

`SDL_RenderSetLogicalSize` installs `SDL_RendererEventWatch`. For
`SDL_MOUSEMOTION`, `SDL_MOUSEBUTTONDOWN` and `SDL_MOUSEBUTTONUP` it rewrites the
queued event coordinates in place:

```
x = (x - viewport.x * dpi_scale.x) / (scale.x * dpi_scale.x)
y = (y - viewport.y * dpi_scale.y) / (scale.y * dpi_scale.y)
```

with `dpi_scale = window_size / renderer_output_size`. The event fields a
player polls are therefore **logical** coordinates, not window points.

`SDL_GetMouseState` is the other case: it returns window points.
`SDL_RenderWindowToLogical` is the inverse and divides by `dpi_scale` itself
before subtracting the viewport and dividing by the renderer scale.

The old `Display::window_to_logical` multiplied the window point by
`output/window` *before* calling `SDL_RenderWindowToLogical`, applying the DPI
factor twice. The bug is invisible on a 1:1 X11 display (`output == window`) and
appears on HiDPI outputs.

## Common fix

`port/src/play_main.cpp`:

- motion/button event fields are used directly (they are already logical);
- `SDL_GetMouseState` + `SDL_RenderWindowToLogical` remains only for hover
  (`Display::mouse_logical`);
- every real press/release logs
  `mouse_click phase=… logical=… window=WxH output=OxH logical_size=…`.

No scene name, character name or label is consulted anywhere in this path.

## Physical evidence at different window sizes

An isolated X11 server (`Xephyr :9`, 2000x1200) was driven with real X11 pointer
events (`xdotool`/XTEST). The host window was minimized so the host pointer
could not inject extra events. Target: the MR door, logical playfield
`(352,201)` (hotspot 42, action 165). The window click point was computed from
the actual client size with SDL's aspect-preserving viewport rule.

| requested window | client used | window point | logged logical | result |
| --- | --- | --- | --- | --- |
| 1280x960 | 1280x960 | 704,482 | 352,201 | slot 42 action 165 |
| 1920x1080 | 1920x1080 | 1046,543 | 351,201 | slot 42 action 165 |
| 1024x768 | 1024x768 | 563,386 | 351,201 | slot 42 action 165 |
| 900x700 | 900x700 | 495,352 | 352,201 | slot 42 action 165 |
| 640x480 | 640x480 | 352,241 | 352,201 | slot 42 action 165 |
| 1500x900 | 1500x900 | 822,452 | 352,200 | slot 42 action 165 |
| 960x600 | 960x600 | 528,302 | 352,201 | slot 42 action 165 |

All seven clicks produced the same hotspot with the same logical coordinate
(±1 px from SDL's integer truncation at non-integer scales). Logs:
`port/output/recordings/size_check/summary.txt` (ignored build evidence).

The same mapping is exercised by the recording
`port/output/recordings/step_mouse_esc_retalk.mp4` (resize to 1024x768 + door
click, then 1280x960 + seller click).

## Regression test

`port/tests/input.cpp` (`input-test`, part of `make test`) creates a renderer
with `SDL_RenderSetLogicalSize`, pushes mouse events in window coordinates
through `SDL_PushEvent` at 1280x960, 1024x768, 900x700, 960x600 and 640x480
(640x400 playfield) plus 1280x960/1024x768 (640x480 conversation screen), and
asserts the queued event arrives at the intended logical point. It also checks
`SDL_WarpMouseInWindow` + `SDL_GetMouseState` + `SDL_RenderWindowToLogical`.

`make -C port -j2 all test` passes all suites, including `input-test`.

## Boundaries

- The test server is X11 (dpi_scale 1). The code no longer applies any manual
  DPI factor, so the HiDPI branch is delegated to SDL's own event watch and
  inverse function; the unit test pins the logical event contract at several
  window sizes.
- Touch events are outside this change.
