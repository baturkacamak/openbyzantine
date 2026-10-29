# Centered page titles and native preference explanations

29 October 2026. User requested centering page headings, removing the redundant
ORİJİNAL category, and adding more detailed explanations to the game's own
preference screen.

## Changes

Advanced settings retains the approved left-label/right-value rows, with only
the page title centered at x493. The hub now has five categories; the original
preference category, action enumeration, read-only value cases and related
helpers were removed. The shipped preference screen and its real controls
remain available.

On that screen, hovering any of the six visible native preference rows now
writes a heading and wrapped explanation on the left page. Hovering the
advanced entry uses the same help renderer. Moving off the relevant controls
redraws the original art and clears the explanation. Native art, hover
highlights, setting markers, toggle/save handlers and presentation ownership
are unchanged. Help does not introduce a presenter or a scene-specific rule.

Descriptions distinguish the original automatic-save preference from the
port's scene-travel save, movie size from window/fullscreen mode, and the
original sound gate from the port mixer levels. Tutorial and location clip
help describes their distinct gates. Conversation-note help explicitly says
that its original behavior is not implemented and points to port subtitles
and conversation history. No original behavior has been invented to make the
help more complete.

These are host UI explanations based on the current implementation and prior
TOP_MENU.md / PORT_SETTINGS.md evidence, not newly derived binary behavior.
Current source references include cc.cpp's original autosave request gate,
script sound gates and mode-2/3 video preference gates; play_main.cpp's movie
placement and port autosave path. No new decompilation/export derivation was
needed. The unexplained seventh stored preference has no visible row in the
supplied native artwork and was not given an invented explanation.

## Verification and evidence

Evidence is separate in `port/output/menu-preference-help/`. User run resolved
before tests: `run-20261029T210746.613812Z-pw5tfz9e` (finished, exit 0). Its
bundle and LATEST are preserved. Checks use private configurations. Existing
object-kind naming changes remain untouched.

- `build-test.log`: final `make -C port -j2 all test` exits 0.
- `verify.py`, `checks.log`: five categories, heading/value behavior, sliders,
  subtitle/hotspot toggles, key capture, last rows, back/resume/quit and final
  stdout contract pass. All six visible native control hover IDs and the
  advanced-entry explanation are exercised.
- `window_probe.py`, `window-checks.log`, `window-native-help-*.png`: isolated
  Xephyr window with delivered xdotool events and xwd-root captures confirms
  native help/clear and existing menu interaction. Dummy SDL audio; no speaker
  validation claim. `window_capture_final.py` updates the final location-help
  image after replacing a word containing an unsupported circumflex glyph.
- `text-fit.log`: all seven explanations use covered glyphs and need 5–9
  lines, within the ten-line space. Full final sentences remain visible.
- `help-checks.log`: six distinct help panels; leaving native controls
  restores the blank left-page crop exactly. All 35 menu/settings trace
  presentations have overlay=0.

The final hub and native preference panels were visually reviewed. Existing
left/right setting controls and original preference toggling are retained;
this change adds help rather than claiming completion of conversation notes.
