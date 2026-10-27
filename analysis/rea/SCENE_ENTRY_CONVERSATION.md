# Scene entry conversations: the 0x70 at the instruction head

Date: 27 October 2026. Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`, profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

## Reported symptom (pinned user run)

`run-20261027T142409.559687Z-qtcd5rmd`: EA 602 / `26.btb` -> MAP -> SM (two
`IImSM1a` conversations) -> MAP (`slot 47 action 246`) -> `slot 13 action 257
scene_switch=27`, i.e. MR. Then 16 clicks in a row:

```
entry scene=MR steps=103 stopped_at=169 hotspots=0 records=0
composed background=MRDOORCL.jpg ...
click at 218,285 hit no hotspot
click at 391,220 hit no hotspot
...
```

`hotspots=0`: no object is clickable, including the standard bottom-right exit.
The scene was not frozen (clicks were processed), there was simply nothing to
hit.

## Evidence

The MR arrival trace (evaluations 46..49, `instructions=782`) runs

```
  0x72 x44      create objects 0..43 (positions up to x=2453)
  0x67         var18 = 0, var19 = 1000, var20 = 500
  0xc6         dialogue chain 0
  0x9d(21,1,16) conditional call
  0x90(1)      resource refresh
  0x9b(192)    call
  0x99/0x71    variable-gated branches (vars 1, 3, 22, 30)
  ...
  0xb8         FUN_00438cf9: suspend every active hotspot into the snapshot
  0x90(1)      refresh
  0x9b(137)    call: 0xc1(7,3000), 0xbf(7,100,200), 0xc4(7) -> mrAmb1
  0x70(0,0)    CONVERSE with cd record 0 (MMhmr1a)
```

So the arrival raises the door (the background is still `MRDOORCL`, the closed
door), **suspends the hotspots for the dialogue** and starts `MMhmr1a` at
instruction 169. The continuation from the conversation result (instruction 170
for `0xffff`) is what finishes the view:

```
  170 0x67 var30 = 1
  171 0x9b call label 128
  145 0x6d(42,43)        enable the view's hotspots
  146 0x71 / 147 0x69 / 148 0x99 / 151 0xb0 ...
  172 end
```

The port's scene-entry path for a **travel** (`0x73`) only ran the script and
never ran a `trace.conversation_request` (the interactive *startup* path did).
The whole pre-dialogue state was therefore adopted: every object inactive
(`state_adopted ... objects=0:...:0,...` all with active 0), `suspended` left
non-empty, and no continuation. Nothing was clickable and the exit arrow never
appeared - a real bug, not a missed click.

Contrast with the old binary on the same journey:

```
old: entry scene=MR steps=103 stopped_at=169 hotspots=0  (background mrDoorCl)
new: entry_conversation record=0 instruction=169 object_context=0
     entry scene=MR steps=9 stopped_at=172 hotspots=2    (background MRDoorOp)
```

## Fix

* `travel_to` (and `restart_scene`) now treat an entry `0x70` exactly like the
  startup path: run the pre-dialogue segment (sound-gated when audio is ready),
  run the conversation on the state at the pause, apply the result and run the
  resumed segment - the existing `execute_plan` flow. `0xfffe` still ends the
  scene session; a conversation that cannot complete (missing fonts, exhausted
  `--conversation-choices`) prints `entry_conversation not committed` and leaves
  the scene in the pre-dialogue state instead of pretending it is ready.
* The entry probe now runs on a **copy** of `SceneView::entry_state`. It used to
  mutate the member and then replay the entry from its own output, applying
  every write twice (the "probe must not become the action's entry state" rule).
* New diagnostic `entry_no_hotspots scene=...` (also `restart_no_hotspots`) when
  a scene ends with no active hotspot and no exit: the reported situation was
  completely silent before.
* `--panorama-yaw` is now repeatable and keyed per scene (plus a fallback entry
  for every panorama), which is what made the reported multi-scene journey
  replayable: its drags put EA at -44.45 deg and SM at 166.72 deg at different
  clicks.

## Verification (real data)

Documented MR route (`port/README.md`: MAP 87 -> click 47 twice):

```
bash port/scene.sh . MAP 87 27.btb --click-at 192,300 --click-at 192,300 \
     --conversation-choices 0,0,0 --shot /tmp/mr.ppm
travel index=27 scene=mr from=MAP ...
entry_conversation record=0 instruction=169 object_context=0
conversation record=0 name=MMhmr1a result=65535 lines=4 answer_lists=0 media=1
conversation_resume_probe steps=9 stopped_at=172 switch=none sounds=0 blocker=
entry scene=MR steps=9 stopped_at=172 hotspots=2 records=0
hotspot_screen slot=42 x=307 y=79 w=92 h=245 centre=352,201 action=165
hotspot_screen slot=43 x=539 y=300 w=101 h=100 centre=589,349 action=160
scene=MR background=MRDoorOp hotspots=2 slots=42,43,
```

The arrival dialogue plays (`Mm1meh01.avi` extracted from `Byzantine_1.iso`,
19.4 s of PCM queued; `mm1car01`, `mm1meh00` and `mm1car02` are not on any of the
six CDs, so they report "missing" like `0x40d2a1` returning 0), the door opens
(`MRDOORCL` -> `MRDoorOp`) and the two hotspots appear
(`port/output/mr-arrival/mr-arrival-after-entry-conversation.png`). Clicking the
exit (589,349 -> slot 43 action 160 `scene_switch=25`) travels back to MAP with
6 hotspots:

```
click_at 589,349 -> 589,369 slot 43 action 160 scene_switch=25
travel index=25 scene=map from=MR ...
entry scene=MAP steps=141 stopped_at=133 hotspots=6 records=22
```

`analysis/rea/verify_mr_arrival.py` drives both cases through `scene.sh` (the
arrival view and the exit back to MAP); it fails against the previous binary
(`entry scene=MR ... hotspots=0`) and passes now. `make -C port -j2 all test`
passes and `verify_ea_layer`, `verify_ea_photo`, `verify_ea_inline_voice`,
`verify_ea_single_execution`, `verify_examine_controls`,
`verify_panorama_bindings/camera/projection`, `verify_tk_animation`,
`verify_tk_hotspots` and `verify_tk_media` still pass.

## Limits / open

* Headless runs that enter a scene with an arrival dialogue now need
  `--conversation-choices`; without it the conversation cannot advance and the
  port says so (`entry_conversation not committed`).
* `0x6d` (the view's hotspot enable list) and `0xb7`/`0xb8` are only modelled at
  the level the shipped scripts use (see `HOTSPOT_TRANSITIONS.md`); the
  transition *effects* still are not.
* A scene that legitimately has no hotspot (a static close-up reached other than
  through the examine path) now prints `entry_no_hotspots`; it does not invent an
  exit.
