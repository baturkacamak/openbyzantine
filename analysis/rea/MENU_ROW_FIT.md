# Two-line settings rows must contain their choice line

28 October 2026. User screenshot: selected “PORT OTO KAYDI” on OTURUM shows
“aç / kapat” crossing the bottom selection border.

## Cause and correction

Source observation: SettingsUi kept the old one-line rectangle `{x, y-5,
width, 35}` after adding a second line at `y+17`. The regular face has 17px
glyph cells and a one-pixel shadow. Its full two-line draw extends to `y+35`,
while the frame ended at `y+30`. This is a shared geometry error, not a scene
or mouse-coordinate problem.

SettingsUi now identifies pages with boolean choices and gives them:

- a frame at `y-4`, with height `17 + face.line_height + 1 + 6` (41px),
- 42px between headings and the first heading at y158,
- the very same row rectangle for border, hover and click routing.

The sixth original preference ends at y405, above the GERİ button at y412.
Single-line category, sound and key pages retain their previous geometry.
Choice positions and direct on/off semantics remain unchanged relative to
each heading. No new binary behavior, scene state or VM command was derived;
this is a host-only layout correction.

## Evidence

User's LATEST resolved before testing:
`run-20261028T223508.979736Z-kwr_312s` (summary still running). The screenshot
and current layout code explain this defect; the user's live game was not
interrupted. Tests/captures use a separate private config and direct commands,
leaving the original run and LATEST intact.

`port/output/menu-row-fit/` contains `build-test.log`, `verify.py`, `checks.log`
and real-asset captures. The integration probe exercises the existing six
categories, direct repeated on/off choices, slider persistence, keyboard
capture, back/resume/quit and last-stdout-line contract. Extra captures select
each OTURUM row, the off variant of its first row and the last ORİJİNAL row.
The image check locates the gold borders and verifies that the gold text fits
inside them with lower padding. SDL dummy devices; no physical-input or speaker
claim is made for these captures.

Results: `make -C port -j2 all test` passed without compiler warnings; the
existing integration checks passed. `check_frames.py` / `frame-checks.log`
measure four pixels of bottom padding under the choice text on every selected
boolean row, including the off variant and the sixth original row. The last
frame's bottom border is y404, before GERİ starts at y412. Screenshots were
visually reviewed. LATEST still names the user's original run.
