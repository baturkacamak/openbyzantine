# Preference typography and direct aç / kapat choices

28 October 2026. The user pointed to the original gold “YER TANITIM
GÖRÜNTÜLERİ / aç / kapat” row and asked why that typography and structure
were not used in the new pages.

## Observation and scope

The original preference labels are pixels in PREFFS.JPG; the hover art is in
ROLL.RLE. MenuUi draws those images, rather than rendering the header string
with an SFN face. The previously added notebook pages used a reduced DIALOG
coverage atlas; visual comparison shows a heavier, ornamental face. The saved
TOP_MENU report and current image/UI code suffice for this observation: no new
binary behavior, export derivation or REA session was involved.

The exact original design font has not been established. The new host face is
a visual approximation, **not** an extracted original typeface: regular
Liberation Serif rasterized at 14 and 16 logical pixels. Its bundled derivative
is called Byzantine Menu Serif. Source font SHA256:
`705903ae1382f0150f115d6f10a30f5b2f3d6ade649c474afe6366c39174fb65`.
The OFL/copyright notice is retained at `port/licenses/menu-typeface-OFL.txt`.

## Change

- `port/include/byzantine/menu_typeface.hpp` embeds the grayscale masks and
  proportional advances. Normal C++17 builds need no system font, font path,
  Pillow, FreeType or new runtime library. Gameplay SFN fonts are untouched.
- `port/generate_menu_typeface.py <LiberationSerif-Regular.ttf>` regenerates the
  header using Pillow (generation only). Masks cover ASCII and all twelve
  Turkish letters in the existing game-text encoding. Every glyph is cropped
  to its own cell so serif overhangs cannot overwrite adjacent glyphs.
- Host pages and the new main preference entry use the regular face instead of
  resizing DIALOG. Labels use black when idle and gold when selected. The main
  entry gains its existing frame only on hover, keeping idle black readable on
  the gray paper. Left descriptions remain light on the leather.
- Boolean rows show a centred `> HEADING` and the lower `aç / kapat` line.
  The active choice is gold and the other black, matching the original row's
  visual rule. Each word chooses its value directly: clicking “kapat” twice
  stays off, rather than toggling back on. Original preference rows remain
  read-only in the advanced pages. The original main preference artwork and
  its real switch handlers are unchanged.
- Categories, sliders, key bindings, menu return/resume/quit and presentation
  ownership remain the shared implementation from NOTEBOOK_MENU_DESIGN.md.

## Evidence and checks

`port/output/menu-typeface/` contains separate evidence; it does not overwrite
the previous menu-design captures. The user's starting LATEST was
`run-20261028T222544.429386Z-pik1_mbn.log` and remains unchanged.

- `build-test.log`: `make -C port -j2 all test`.
- `text-test` now checks nonempty coverage for every Turkish letter in both
  embedded sizes, and verifies that the advanced title fits the native banner.
- `verify.py`, `checks.log`: the previous six-category, back/resume/quit,
  slider persistence, key capture and stdout checks; plus repeated explicit
  on/off choices and a default (non-hovered) preference entry capture.
- `window_probe.py`, `window-checks.log`, `window*.png`, `window.trace.log`:
  isolated Xephyr, xdotool delivered input, xwd captures. Validates slider drag
  to 50%, direct subtitle off/on, category return, resume and F10. Host window
  unmapped to exclude outside mouse motion. Audio uses a dummy device; there
  is no speaker-validation claim.
- `preferences.png`, `preferences-idle.png`, `language.png`,
  `subtitles-off.png`, `subtitles-on.png`, and `menu-preview.png` show the result.

No new game state, VM behavior, per-scene branch or presentation path was added.
The font resemblance is a visual choice; original pixel equivalence is not
claimed.

Results: full build/test passed without compiler warnings; deterministic UI
checks and the isolated window probe passed. Direct choices preserve their
requested value on repeat clicks, slider drag persists 50%, and all 16
recorded settings frames have `overlay=0`. Regeneration reproduced the
embedded header byte-for-byte.
