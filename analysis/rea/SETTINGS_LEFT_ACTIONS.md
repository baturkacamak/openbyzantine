# Advanced settings: live left-page resume and quit actions

29 October 2026. User reported that the baked OYUNDAN ÇIK / OYUNA DEVAM
buttons do not work inside advanced settings.

## Observations

Pinned user run `run-20261029T224358.574755Z-whgo99fq` finished exit0. A click
at logical156,429 was delivered while settings were open; the old resume hit
rectangle ended at y418. It produced no settings action log. Later the user
returned through GERİ to native preferences, where162,402 closed that screen
and178,383 opened quit confirmation normally. These observations establish a
miss and an awkward advanced-menu affordance, not a global SDL coordinate
mapping failure or proof that every old center click was broken.

The host drew only the baked black lettering: it had no matching left-button
hover state, native gold hover art or pointer feedback. Its initial capturing
guard also prevented all left buttons from working during key assignment.
Previous deterministic center-coordinate tests did not cover these cases.

## Correction

One host `left_action_at` now owns hover and click geometry for both left
rows. The quit row covers x70..257/y365..391; resume covers x70..257/y392..434,
including comfortable padding below the visible lettering and the reported
y429 click. These are explicitly host accessibility hit rows; the recovered
native menu control table is unchanged.

Hover uses the game's existing ROLL frames4/5 and menu pointer. Row changes,
window leave and focus loss clear the hover. Both actions remain available
while a key assignment is pending; exiting cancels that pending capture.

Resume closes settings directly into the scene. Quit closes into the native
preference/confirmation path; confirmation, cancel and actual quit still use
the existing original menu handler. `settle_settings_exit` shares that handoff
between window and headless navigation, deriving its native quit point from
control4 instead of maintaining two magic-coordinate sequences. Settings
closure has an explicit return target; returning to preferences no longer
presents a transient game scene first. No new presenter, VM state, scene rule
or native quit behavior was introduced. `settings_click` and
`settings_left_action` logs make delivered clicks and their selected action
visible for future diagnosis.

This is host UI reuse of existing menu/asset behavior from TOP_MENU.md; no new
binary behavior or export derivation was required.

## Validation

Separate evidence: `port/output/settings-left-actions/`.

- `build-test.log`: final `make -C port -j2 all test`, exit0.
- `verify.py`, `checks.log`: existing deterministic real-asset checks plus
  both actions on the hub and all five categories, the reported lower-padding
  click, right end of each label, hover screenshots, capture-mode resume/quit,
  quit cancel and confirmed quit. Last stdout scene contract passes.
- `wm_probe.py`, `wm-checks.log`, `wm.log`, `wm.trace.log`, PNG/XWD files:
  actual isolated GNOME/Xephyr with delivered xdotool input, xwd-root captures,
  private MR/30.btb/preference copies and private configuration. F10 and native
  entry, hub/all-five-page quit/cancel and resume, capture-mode exits,
  borderless/integer viewport click mapping and final confirmed process exit0
  pass. F10 can reopen settings after each resume, establishing a usable exit.
  Audio/video are disabled; no physical-input/speaker claim.
- `audit-checks.json`: native hover pixels change on the correct row; all
  recorded menu frames retain overlay=0 even with --hotspots-all enabled.

The first WM harness attempt could not reach the top strip reliably through
its simulated window coordinates; it stopped on an entry assertion. The final
probe enters original preferences through the guarded F10/quit/cancel path
before using its advanced banner, and asserts entry at each stage. No original
preferences were toggled. User run/LATEST and actual assets were preserved.
