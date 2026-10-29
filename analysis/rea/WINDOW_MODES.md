# Window mode state and explanations of selected display values

29 October 2026. User supplied two same-sized windows labeled Pencere and
Büyük, both with a small centered picture, and asked why resizing still seemed
wrong and what these modes mean.

## Observation

Pinned run `run-20261029T220300.559400Z-h0xs10v0` was running at investigation
start. Its log confirms integer scaling and a 1260x806 client. A 640x480 image
cannot scale to 1280x960 within that client, so the retained 1x picture is the
intended integer preference, rather than the previous missing-redraw bug.
Fit (Oranlı) already enlarges continuously to the available 4:3 viewport.

The run also shows MAXIMIZED and RESTORED events (including restore at trace
seq3167), but the old settings mode did not observe those events. A manually
restored window could retain the Büyük label. Switching back to ordinary
windowed mode did not explicitly restore a maximized SDL window either.

## Correction

- MAXIMIZED/RESTORED events synchronize the window mode and host setting from
  actual SDL flags in the shared event poll. The settings page redraws its
  value after these events, including its current-value explanation.
- Selecting Pencere explicitly calls SDL_RestoreWindow before applying the
  saved normal-window geometry. Geometry is remembered during manual normal
  resizes, but maximized/fullscreen geometry never replaces the restore size.
- Büyük is now Büyütülmüş. It maximizes the decorated window, retaining the
  title bar; Pencere is the normal manually resizable decorated window.
- Display explanations now describe the selected window, scaling, VSync and
  cursor value, including why integer scaling leaves large bars and how to
  choose proportional fit. The existing gentle help compositor handles their
  changes. All complete static choice paragraphs fit in 6–8 lines.
- The fullscreen diagnostic now reports the actual SDL fullscreen flag instead
  of incorrectly treating maximized as fullscreen.

Integer preference semantics and the user's current INI were not changed.
The user's running player was not stopped. The prior resize repaint correction
and single present_image presentation point remain intact. This is host SDL
lifecycle/UI work, with no new game/binary behavior derived.

API references: [SDL_RestoreWindow](https://wiki.libsdl.org/SDL2/SDL_RestoreWindow),
[SDL_MaximizeWindow](https://wiki.libsdl.org/SDL2/SDL_MaximizeWindow),
[SDL_GetWindowFlags](https://wiki.libsdl.org/SDL2/SDL_GetWindowFlags).

## Verification

`port/output/window-modes/` contains separate evidence:

- `build-test.log`: `make -C port -j2 all test`, exit 0.
- `verify.py`, `checks.log`: existing real-asset deterministic UI/help/input
  checks pass, including final stdout behavior.
- `wm_probe.py`, `wm-checks.log`, `wm.log`, `wm.trace.log`, PNG/XWD captures:
  actual GNOME Shell window manager inside an isolated Xephyr display and
  private D-Bus/session/XDG directories with in-memory desktop settings. The
  final probe uses a private copy of MR/30.btb and its original preference
  input, plus a private host INI; actual game assets are read from /tmp/byz-scene.
  Delivered xdotool input and xwd-root captures compare fit/integer at 900x650,
  externally maximize to 1600x1131, externally restore to 900x650, select all
  four window modes and return to exactly 900x650. Mode-sync log records both
  actual transitions. Audio/video are disabled; no speaker validation claim.
- `text-fit.log`: complete static selected-choice explanations fit the left
  page with covered glyphs; screenshots visually checked.
- `audit-checks.log`: recorded menu frames retain overlay=0; shared preference
  copy content verified restored.

### Initial test correction

GNOME's startup overview intercepted an initial activation click. A subsequent
script click hit native preference row1 and wrote the shared extracted runtime
`savegame/gamepref_000` with video size0 (sound toggled twice back to1). The
pinned user run logs its only native changes as row1 ->0 ->1, starting from
GAMEPREF.000, establishing the prior seven values (1,1,1,1,1,1,0). That exact
content was restored and checked against the uppercase original input. ISO and
original uppercase inputs were unchanged; no false claim of untouched runtime
copy metadata is made. The mistaken content is retained in
`first-probe-pref.bin`. Further tests use a private save/pref directory and
assert settings entry before interacting. The final GNOME probe handles the
startup activation through guarded Escape/F10 input and passes.
