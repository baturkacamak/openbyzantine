# Examine actions rejected by a stale held item

27 October 2026. Scope: fix the shared inventory selection state, not change
the item compatibility rules or add scene-specific exceptions. Original media
was not changed and no new decompilation session was opened.

## Observed failure

EA 26.btb, label 602, select ICAMERA (17), examine, click playfield (210,280):

```
closeup_enter scene=EXAMINE parent=EA item=17
item_use slot=91 item=17 object_kind=0xe object_name=4294967295 compatible=no
slot 91 action 288 item_use rejected item=17 kind=0xe
```

The action was never executed. The original item-use gate correctly rejects a
held item on a nameless kind 0x0d/0x0e control. The port had incorrectly kept
the examined item held; its previous inventory tests even asserted that state.

## Saved evidence and change

`0x408076`'s examine branch sets DAT_0045d1cc = DAT_0045d1c8, sets state 4,
then calls `0x407b9c(0)`. That helper assigns DAT_0045d1c8 = 0. The port's
`current_use_item` is the examined identity and `current_dialogue_record` is the
held selection: they must not both remain set by `inventory_examine`.

The helper now clears the held selection while retaining the examined id/state.
Switching examined items still resets the previous state-4 item and reloads the
close-up, rebuilding its object table. The compatibility gate itself is intact.
The inside-close-up log now reports `selected=no examined=yes`.

Hashes checked against `pseudocode/decompile-index.json`:

| Input | SHA-256 |
| --- | --- |
| BETRAYAL.EXE | 3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226 |
| 0x408076 | 0a9377f19d5045b63c409cfd6e64fb308ceff484f0bd9a8ad80e37fe0e9fd887 |
| 0x407b9c | a73a0edaa295e576e3599fa267ea9d861ce1567a6ff49e29a1177857a2bc389a |
| 0x407c5a | 862c7fc71bc4f5da8ec6b020cc36e83583d5a8d5f867129ce5cd0addc078bb26 |
| 0x40c329 | 5a9e9344f5661475134cfcfc8f708404831951a0bc006244f506ddeabcfa7491 |

## Validation

- `make -C port -j2 all test`: all 14 suites pass. Inventory regression tests
  check kind 0x0d and 0x0e controls after examine/re-examine, preserving the
  examined identity and keeping ordinary held-item rejection intact.
- `python3 analysis/rea/verify_examine_controls.py /tmp/byz-conv`: real player,
  shipped scripts/assets/save, SDL dummy video/audio. Camera action runs for
  3 steps; switch to note (30), switch back to camera (17), action runs again;
  exit returns to EA with 9 hotspots. Temporary artifacts are outside the repo.
- Window path: Xephyr :98, XTEST mouse events at screen (210,300), strip
  (190,450), strip (310,450), (210,300), exit (589,369). Logs show actions,
  both switches and EA return without an item-use rejection. Extra host input
  events were present; this is not a claim of human mouse or acoustic testing.
  Local log: `/tmp/byz-examine-fix/mouse.log`.
- Also checked HSTMED crowbar/wooden-piece and AM ring close-ups: the examined
  item remains known while `selected=none`. No claim of every item action.

## Limits

This repairs the common action-dispatch regression. Camera action 288 is a
three-instruction path that includes a `0xbd exCam` video request. The player
does not yet execute generic CF video requests (conversation videos use another
path), so the camera clip itself is not fixed or claimed to play. Other missing
action implementations must be distinguished from this compatibility rejection.

Earlier EXAMINE_SWITCH.md statements about preserving selection/highlight are
superseded here: clearing the held selection is the observed original behavior.
The old assertions were corrected rather than bypassing the item-use gate.
