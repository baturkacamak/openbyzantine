# Advanced settings follow the native preference layout

29 October 2026. User requested applying the suggested native-style layout
throughout advanced settings, matching the original preference writing as
closely as practical.

## Evidence and limits

The supplied screenshots and SettingsUi::draw show two incompatible layouts:
boolean settings were centered with a second choice line; other controls kept
a narrow left label and a right value. The original preference labels are
baked into PREFFS.JPG / ROLL.RLE, as recorded in MENU_TYPEFACE.md. The original
font remains unidentified. This change retains the existing embedded regular
Liberation Serif approximation (14px menu text, 16px prose/title); it does not
claim extraction of the original font or pixel equivalence. No new binary
behavior was derived, so no new export/hash derivation was required.

## Implementation

All setting headings are now uppercase and centered on x493, with their value
or control centered beneath. This applies to display, audio, language,
session, keys and the read-only original preference view. Category links are
centered too. Normal headings are black; hover/keyboard selection and active
choices use the existing pale gold 0xF6AC with the existing subtle dark shadow.
Large shaded rectangular row frames were removed. The back button and notebook
decorations remain.

Boolean rows measure and center the whole aç / kapat line, fixing its former
offset from its heading. Hotspot now offers kapalı / imleçte / tümü underneath
its heading. Each text choice has a hit rectangle derived from the drawn
width; repeated clicks select the same value without cycling. Clicking the
heading only selects the explanatory row. Enum/language values and their
arrows move below the heading. Sliders and their percentages also move below,
with a shared track origin/width for drawing, clicking and dragging. Long
values use the wider centered area and an ellipsis if required.

Session headings now say PORT OTOMATİK KAYIT, GÖRÜLENİ ATLA, HOTSPOT GÖSTERİMİ
and OYUN KUMANDASI. Regular rows retain 42px spacing; eight key assignments
use 32px spacing with a 15px control offset. Key-capture guidance is on the
left page, avoiding the last key row. Original preference values remain
read-only. Presentation uses the existing menu ScreenContentScope and
present_image path; no gameplay/VM/asset changes were introduced.

## Validation

Evidence is separate in `port/output/menu-native-layout/`:

- `build-test.log`: `make -C port -j2 all test`, exit 0.
- `verify.py`, `checks.log`: real extracted MAP/menu assets with SDL dummy
  devices; six categories, back/resume/quit, key capture/persistence, 50% audio,
  repeated explicit on/off choices, all three repeated hotspot choices, and
  the required last stdout `scene=…` line pass.
- `window_probe.py`, `window-checks.log`, `window*.png`, `window.trace.log`:
  isolated Xephyr window, delivered xdotool events, xwd-root captures. Audio
  dragging persists 50%; direct subtitle and three hotspot choices, menu
  return, scene resume and F10 work. This is window/input validation with
  dummy audio, not physical-input or speaker validation.
- All six pages were visually reviewed. `layout-checks.log` confirms no long
  rectangular hover border in the setting area. Last key gold ink ends at
  y399 and original preference gold ink at y400, above GERİ at y412. All 24
  settings presentation trace lines carry overlay=0.

The original user's LATEST was resolved before testing:
`run-20261029T204747.852586Z-my9ylec5` (running). Its bundle and live player
were preserved. Tests use private INIs/logs and LATEST is unchanged. Existing
uncommitted object-kind naming work was left in place.
