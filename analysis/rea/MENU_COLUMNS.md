# Advanced settings: left labels and right current values

29 October 2026. User rejected the centered two-line design in their display
settings screenshot and requested left-aligned labels, right-aligned options,
removal of leading > signs and no separate aç / kapat choice line.

SettingsUi now uses a single-line, two-column layout on every settings page.
Page titles and category entries start at x388; option captions end at x599.
Leading > markers and selector triangles are removed. Booleans show only the
current açık / kapalı value, and Hotspot shows its current mode. Clicking the
current value cycles it; clicking the label selects its description.
Key capture and read-only original preference handling use the same route.
Audio tracks and percentages fit the right column; draw and drag share the
same x508 origin and 48px width. Value hit rectangles come from their drawn widths, so long labels do not
activate a neighboring setting. Labels use the space left by the measured
current value, maintaining a
12px gap; unexpected long captions receive an ellipsis. A few session labels were shortened
to fit without shrinking the text. No font, color, game/VM or original asset
changes were required. This supersedes MENU_NATIVE_LAYOUT.md's centered row
and direct-choice design. The existing embedded serif remains an approximation
of the original baked preference lettering; no new binary behavior derived.

Evidence: `port/output/menu-columns/`. The user's LATEST was resolved before
checks: `run-20261029T210135.525167Z-a9h0bkiy`, finished with exit 0. Its bundle
and LATEST are preserved; checks use private configurations and output paths.
Existing object-kind naming edits remain untouched.

Validation: `make -C port -j2 all test` passed (`build-test.log`). Real-asset
`verify.py` / `checks.log` covers all six pages, label clicks preserving state,
current-value clicks toggling subtitles and cycling all three hotspot modes,
50% slider persistence, key capture, crowded final rows, back/resume/quit and
the final stdout contract. `window_probe.py` / `window-checks.log` exercises
slider dragging, subtitle toggling, three hotspot modes, menu returns, resume
and F10 in an isolated Xephyr window. Captures use xwd-root, delivered input
uses xdotool and audio uses SDL dummy: no physical-input or speaker claim.
Screenshots in this directory were visually reviewed.

Final window trace: 48 settings presentations, all overlay=0
(`presentation-checks.log`). Final build and both UI check scripts exit 0.
