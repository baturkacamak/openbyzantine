# Examine close-up: atomic item switch and parent return

Date: 26 October 2026.

This step reproduces and fixes the item-switch defect in the examine close-up
that the previous step introduced. The close-up is a real scene travel
(`0x69` transition 4 + the `0x438afa`/`0x438173` scene groups), so the fix is in
the shared inventory/close-up layer, not in a scene.

## Reproduction (before)

HSTMED save `43.btb`, page 5 (`vTk`-style strip page with `66 ihstd`,
`72 icrwbar`, ...). Real X11 clicks in an isolated Xephyr `:8`:

1. click slot 5 (`icrwbar`) -> `bar_item slot=5 id=72 … selected=yes`;
2. click the examine button -> `closeup_enter scene=EXAMVB parent=HSTMED
   item=72 name=icrwbar image=ecrwbar index=9`, background `ECRWBAR.jpg`;
3. click slot 0 (`ihstd`) inside the close-up: the old code only changed the
   strip selection:
   `bar_item slot=0 id=66 name=ihstd selected=yes current=66` — the image stayed
   `ECRWBAR.jpg`, the close-up hitboxes stayed the crowbar's, and the
   `current_use_item` still named 72;
4. click the close-up exit: the parent returned with selection 66 but the
   examine pointer/state still 72 (and item 72 was left in state 4).

The selected item, the visual, the item identity and the active hitboxes
disagreed, and the parent returned with an inconsistent examine state.

## The original path

`0x408076` (digest `0a9377f19d5045b6`) is the strip click. On a slot inside a
close-up:

* a record that is already `DAT_0045d1cc` is ignored;
* otherwise the clicked record becomes the selected one (`0x407ad5`, state 3);
* the examine button (`0x407f6a`) then sets `*DAT_0045d1c8 = 4` and
  `DAT_0045d1cc = DAT_0045d1c8`;
* `0x43c8d2` case 2 maps the close-up through `0x438c80` to the same scene
  index and stops the script, so `0x42c8a0` reloads the close-up scene. Its
  `0x69 transition 4` request therefore reads the new `DAT_0045d1cc` and the
  scene entry rebuilds its hitboxes for that item.

On return, `0x439349` (`66126a8910fc3565`) restores the captured tables and
calls `0x4085e7` (`e20f05040ada55e7`): `*DAT_0045d1cc = 1`, `DAT_0045d1cc = 0`,
redraw. The strip selection `DAT_0045d1c8` is process-global and stays.

## Port fix

* `byzantine::inventory_examine(state, id)` is one atomic update: selection,
  `current_use_item`, record state 4, and the previous examined record back to
  state 1. The image stem (`0x40860e`) therefore always derives from the same
  id as the strip highlight.
* `handle_bar_click` inside an armed close-up uses that function and then
  `enter_examine`, which now:
  * reloads the *same* close-up scene through `travel_to` while holding the
    parent snapshot (`0x408076`+`0x438c80`+`0x73` self reload),
    `closeup_switch scene=… item=… image=…`;
  * keeps `return_view`/`return_armed` unchanged, so the exit still returns to
    the original parent.
* The examine button uses `inventory_examine` in both the normal scene and the
  close-up (the original's `0x408076` examine branch).
* The close-up return branch of `travel_to` mirrors `0x4085e7`: the examined
  record returns to state 1 and `current_use_item` is cleared. The strip's
  selection and all other process-global inventory edits still carry.
* `refresh_background` now takes the examine item from the trace request
  (`0x69`'s `DAT_0045d1cc` read) for both the name and the file. If the live
  examine pointer ever disagrees it reports `examine_state_mismatch` instead of
  silently pairing one item's name with another item's image.

The "description" of the close-up is the item identity the image is built
from. The shipped `EXAMINE`/`EXAMVB`/`EXAMVC`/`EXAMVH`/`EXAMVT` scripts contain
no `0xcd` message and no `engine/exam* ce` exists, so the original close-up has
no text block to keep in sync. The port logs the same id/name/stem tuple
(`closeup_enter`/`closeup_switch`) and no invented text is drawn.

## Verification

Headless (deterministic), HSTMED `43.btb` with real page 5:

```
bar_use id=72 name=icrwbar
closeup_enter scene=EXAMVB parent=HSTMED item=72 name=icrwbar image=ecrwbar index=9
closeup_item slot=0 id=66 name=ihstd selected=yes examined=yes
travel index=9 scene=examvb from=EXAMVB …
closeup_switch scene=EXAMVB item=66 name=ihstd image=ehstd index=9
click at 589,349 slot 0 action 28 scene_switch=9
closeup_return scene=HSTMED from=EXAMVB requested_index=9 hotspots=12 bar=visible
bar_state page=5 scroll=3 count=11 visible=1 selected=66 examined=none ids=66,70,67,64,68,72,
```

The composed parent playfield before the examine and after the switch/return is
**byte-identical** (0 of 768000 RGB bytes differ, 640x400); only the strip band
differs (2056 bytes, the selection moving from slot 5 to slot 0). The parent
still has 12 hotspots and the same background.

Real window: Xephyr `:8` 1280x960, real X11 clicks for select/examine/switch/
exit; logs `closeup_enter … image=ecrwbar`, `closeup_item … examined=yes`,
`closeup_switch … image=ehstd`, `closeup_return … hotspots=12`. Screenshots:

* `examine_before.png` — crowbar close-up (`ECRWBAR`), crowbar highlighted;
* `examine_switched.png` — wooden piece close-up (`EHSTD`), `ihstd`
  highlighted, hitbox outlines rebuilt;
* `examine_returned.png` — HSTMED parent with its 12 hotspots and the strip
  selection on `ihstd`.

Files: `port/output/recordings/item_use/examine_real_window.log` and
`examine_{before,switched,returned}.png`; headless logs
`examine_switch_headless.log`, `parent_return_before.ppm`/`parent_return_after.ppm`.

## Tests

`inventory-test` now locks the atomic switch: selection, examined item, state 4,
the previous record back to 1, the image stem moving `ecrwbar` -> `ehstd`, and
an off-page item rejected. `make -C port -j2 all test` passes all suites.

## Boundaries

* The close-up selection is *immediate*: the original takes two clicks (select,
  then examine). The port performs both because the shipped strip click inside
  a close-up is the only way to change the examined item, and leaving the
  selection and the image apart is the defect being fixed. The log still shows
  the same IDs the original would use.
* The close-up snapshot is the port's whole-`SceneView` copy, not the original
  per-subsystem table order; that difference is unchanged from the previous
  step.
* `va`'s out-of-range examine group stays unresolved rather than guessing a
  sixth scene.
