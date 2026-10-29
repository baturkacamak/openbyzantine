> Typography follow-up: the reduced DIALOG masks below were replaced by a
> bundled regular serif face and native two-line aç/kapat choices. See
> [MENU_TYPEFACE.md](MENU_TYPEFACE.md). Navigation remains as described here.

# Notebook menu design from the user's reference images

28 October 2026. Source-only host UI change in `port/src/play_main.cpp`.
The user supplied three mockups: a gold advanced-settings entry on PREFFS,
a category hub with separate setting pages, and left-page hover descriptions.
No binary behavior was newly derived; no REA/decompiler session was opened.

## Implemented presentation

- The existing preferences page retains its shipped switches and now has a
  single gold advanced-settings entry below them. The old expanded shortcut
  drawer and tiny category tabs are removed.
- Both this entry and F10 open the same notebook hub. Categories have separate
  pages and a bottom **GERİ** button. ESC returns one level; F10 closes the
  settings. The native hub returns to preferences; the shortcut hub returns to
  the scene. The left **OYUNA DEVAM** resumes directly from every subpage;
  **OYUNDAN ÇIK** uses MenuUi's existing confirmation/action path.
- PREFFS supplies the leather, binding, pen, rolled paper and page outline.
  A mirrored clean gray-paper sample from PREFFS (x379..418, y185..228)
  replaces its baked setting labels on the subpages. This is a host composite,
  not a recovered original blank-page asset. No shipped image is modified.
- Gold borders, corner ornaments, triangular arrows and a diamond divider are
  drawn directly into the host surface. Smaller serif faces are area-averaged
  from the game's existing dialogue font masks and retain its Turkish glyph
  mapping. The font caches are refreshed for language changes.
- Hovering a real row shows its explanation on the left and outlines the row.
  Both x and y must be inside the row; hovering the left leather does not select
  a right-page item. Leaving a row clears its preview. Text explains player
  behavior without file names or internal implementation details.
- Sound rows use sliders with numeric levels. Clicking the track sets an
  absolute level; dragging follows the initiating slider and clamps at 0/100.
  Arrows retain incremental adjustment. Live mixer gain and private INI
  persistence reuse the existing settings implementation.
- Keyboard navigation and key capture remain available. Both language labels
  and values now render (the previous implementation omitted their switch
  cases); installed-pack names are converted from UTF-8 for the game font.
- The deterministic `--menu-click-at`, `--menu-key` and `--menu-hover` route
  through the same SettingsUi handlers as SDL, including native back and quit
  handoff. `--menu-shot` can capture an advanced page. The final stdout line
  remains `scene=... slots=...`.

## Scope relative to the mockups

The visual hierarchy follows the references; the shipped 640x480 notebook
geometry and the six existing categories are retained: GÖRÜNTÜ, SES, DİL,
OTURUM, TUŞLAR, ORİJİNAL. Existing host action bindings are configurable;
ORİJİNAL shows the six original preferences read-only. The original switches
are still edited on the main preferences page.

The mockups' resolution/brightness/contrast/gamma rows, a separate music bus,
independent subtitle language and WASD movement bindings were not added.
There are no decorative controls claiming those capabilities. Sound currently
has master, speech, ambient and effects buses. Languages list the base choice
and installed packs; this work does not add new translations.

## Validation and evidence

Evidence directory: `port/output/menu-redesign/` (separate from normal player
run bundles). The user's existing `LATEST` at the start was
`run-20261028T220700.718928Z-sdkzxb_1.log`; this work used direct isolated
commands and did not overwrite that run or the user's INI.

- `build-test.log`: `make -C port -j2 all test`.
- `verify.py`, `checks.log`: real extracted MAP assets, six category captures,
  hover bounds, slider value/persistence (50%), key capture (F2), hub/back,
  resume, quit confirmation, keyboard navigation and stdout contract.
  Private config: `test.ini`; SDL dummy video/audio.
- `preferences.png`, `hub.png`, `display.png`, `audio.png`, `language.png`,
  `session.png`, `keys.png`, `original.png`, and navigation/interaction images.
- `window-*.png`, `window.log`, `window.trace.log`: separate Xephyr window,
  xdotool-delivered mouse/key input and xwd root captures. The Xephyr host
  window was unmapped to exclude outside pointer motion. The drag probe
  asserts the persisted master gain is exactly 0.5. Exercises category
  entry, sound slider drag, language page, back, resume and F10. Audio remains
  a dummy device; there is no speaker validation or original-runtime pixel
  equivalence claim.

Every settings frame still passes through `ScreenContentScope(menu)` and the
single `Display::present_image`. This change adds no scene-name branches,
scene click commit path, transition presenter, game-state field or VM command.

Final results: full build/test suite passed without compiler warnings; the
deterministic notebook checks passed, and the isolated window slider persisted
50%. All recorded settings presentation lines have `overlay=0`.
