# Notebook explanations fade over the paper

29 October 2026. User requested gentle, theme-appropriate transitions when
showing, hiding and changing menu explanations.

## Design and implementation

A 220ms smoothstep crossfade blends only the left notebook help patch
(x48..243, y54..353). Existing gold/cream text appears from and dissolves back
into the paper texture. Right-page hover art, settings, sliders and buttons
respond immediately. Both native preference explanations and advanced menu
category/setting explanations use the same `MenuHelpFade` compositor.

This is a host UI enhancement, not recovered original animation behavior. It
introduces no binary-derived rules, new art, fonts, dependencies or game state.
The compositor runs after the desired menu is drawn and before its existing
`present_image` call. It has no presenter and makes no SDL_RenderPresent call;
scene transitions still belong exclusively to Display::present_transition.
All menu frames keep ScreenContent::menu and therefore overlay=0.

Animations advance on the existing paced event loop, with no sleeps/waits or
nested event pumping in the compositor. Identical targets do not restart the
clock. New input retargets from the last displayed pixels, so fast hovering
has no queue and no jump back to an old explanation. Finishing restores exact
target pixels. Closing/opening menus resets the cache; reopening the original
preferences also clears stale advanced-entry hover. Confirmation/edit/other
native screens bypass and reset the helper. Only an active animation requests
extra menu frames; a settled menu retains event-driven redraw.

Headless rendering remains immediate and deterministic. `--no-transitions`
also makes help changes immediate. Tests supply a synthetic clock to check
actual timing, continuity on reversal, exact endpoints, RGB565 channel math,
control pixels outside the patch and reset/disabled behavior.

## Validation

Evidence is separate in `port/output/menu-help-fade/`. User run resolved before
testing: `run-20261029T211454.070293Z-jvwka4dm` (finished, exit 0). Its bundle
and LATEST were preserved; checks use private configurations and logs. Existing
object-kind naming work is untouched. No new binary behavior was derived.

- `build-test.log`: `make -C port -j2 all test` including the new meaningful
  menu timing/retargeting checks.
- `verify.py`, `checks.log`: existing real-asset/headless menu controls,
  help, toggles, sliders, key capture, return/resume/quit and stdout contract.
- `window_probe.py`, `window-checks.log`, `*.xwd`, `*.png`, `window.trace.log`:
  real Xephyr window with delivered xdotool events. xwd-root snapshots sample
  fade-in, fade-out, interrupted hover, advanced help and click during fade,
  menu return and resume. Capture times are approximate wall times after
  delivered input; synthetic-clock tests cover exact progress. Audio uses SDL
  dummy; no physical-input or speaker validation claim.
- `window_cut.py`, `cut-checks.log`: separate real-window captures with
  --no-transitions check the immediate fallback.

Final measured pixel checks are in `fade-checks.json`.

Final result: full build/tests and deterministic/window checks pass (exit 0).
Measured native fade-in pixel distance from bare paper increases monotonically
0 → 820080 → 3586088 → 6911008, and fade-out decreases to exactly zero.
Advanced help also increases monotonically and returns to exact blank paper.
All pixels outside the help patch are identical between the input-handled
60ms sample and settled native frame. Rapid hover settles to exactly the
independent reference for its last preference, and returning to preferences
restores blank help. The --no-transitions early and settled help crops match
exactly, with no animation logged. All 177 captured menu/settings presentation
trace lines have overlay=0. Check script: `check_fades.py`.
