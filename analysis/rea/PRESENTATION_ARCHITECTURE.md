# Presentation and click architecture — one policy instead of 40 call sites (28 October 2026)

## Report

After the departing-scene flash fix the user reported the same family again:
entering a site, "the map areas are visible again after the video". Pinned run
`run-20261028T112743.479241Z-rhubkx1k` (MR → MAP → EA, `EAEst1`, 125 frames):

```
present index=129 detail=EAEst1:125/125:f0125.jpg       10660 ms   last clip frame
present_capture reason=video_end                        10734 ms
travel_begin index=7 from=MAP target=ea                 10736 ms
travel_loaded  index=7 scene=EA                         10764 ms
present index=132 layer=panorama:EA02.jpg yaw=115 …     10944 ms   first EA frame
```

Indices **130 and 131 are missing from the trace**: they re-presented a picture
identical to index 129, and the detail line is written only when the picture
changes. They are the clip's **audio drain**: `EaEst1`'s audio is 8333 ms and the
frames take 8344 ms, so `play_decoded_clip` calls `pump_window` for the last
~11 ms, and `pump_window` presented with `session.options.overlay` — the
`H` hotspot-outline diagnostic. The MAP's active regions
(`active_slots=4,32,34,36,39,40,41,46,47`) were drawn over the clip's last frame
and stayed there until the target scene's first frame (~214 ms). The same shape
appears in the 26 October session (`run-20261028T112623.622921Z-zl5skrcy`,
`video_end` at 15038 ms, unlogged 187/188).

## Why this kept coming back (audit)

The overlay decision was repeated at every presenter: 26 `present(view,
options.overlay)`, 6 `present(view, false)`, 5 `ui.present(session)` plus 1
`ui.present(session, false)`, 2 `present_image(..., overlay, …)` — **40 decision
points**. The 27 October fix changed the clip's frame callback to
`present(view, false)` but not `pump_window` (used by the audio drain and by
`0xd1` sound waits), so the same outline returned through another path.

The same duplication existed for a committed click: the sequence
`show_object_text → show_message_text → apply_panorama_events →
refresh_background → compose/present → click_result → follow_switches` was copied
in four places (window `handle_click`, `--click` interactive, `--click` shot,
`--click-at` shot) plus a fifth variant in `--run-label`, with slightly different
ordering. The flash fix had to edit two of them.

## Is this how the shipped engine works? No

The original draws everything into **one surface** and presents it once; it has
no hotspot outline at all. Recovered exports (`analysis/rea/pseudocode/functions`):

| Address | Role |
|---|---|
| `0x43b216` | record pass onto the scene surface |
| `0x4242bd` | panorama sampling into the playfield |
| `0x413bdd` | blocking AVI loop; frames blitted over the same surface |
| `0x4146a7` | DirectDraw `Blt` wrapper (maps `DDERR_*`), the present primitive |
| `0x41b251` | GDI `BitBlt`/`StretchBlt` present for frame data |
| `0x41e289` | menu screen; draws into the same surface |
| `0x40c329` | hover query; only selects the cursor (`0x40de7b`), draws nothing |

So the port's several presenters (SDL scene, clip media, conversation, menu,
headless) and the diagnostic overlay are **host additions**, not inherited from
the binary. The binary is a 1997 engine with globals and several blitters, but it
does not have a per-screen "should I draw the debug outline" policy. Our
duplication is ours, and it is what made the same bug return.

## Change

`port/src/play_main.cpp`:

1. `enum class ScreenContent { scene, conversation, clip, menu }` and
   `ScreenContentScope` (RAII). The scope records `screen_content now=…
   previous=…` in the detail trace when a clip starts/ends.
2. `Display::present_image` lost its `overlay` parameter. It is the only
   `SDL_RenderPresent` and now calls `overlay_allowed()`, which is
   `ui_options->overlay && (content == scene || content == conversation)`.
   The outline is now **off by default** (`--hotspots` or `H` enables it;
   `--no-hotspots` is still accepted); a clip (frames **and** audio drain) and a
   menu screen can never be outlined, whichever function presented them.
3. Every present line now carries `overlay=0|1`, so a leak is visible in the
   trace instead of being inferred from index gaps.
4. `commit_click(Session&, Options&, const ClickCommit&, bool)` is the single
   click-commit path; `ClickCommit` flags (`object_slot`, `result_first`,
   `refresh`, `present_frame`, `active_slots`, `panorama_verbose`, `prefix`)
   reproduce each path's output exactly. The window, `--click`, `--click-at` and
   `--run-label` calls are now one call each; `present_action_result` /
   `present_refused_switch` stay the only `0x73` presentation policy.
5. `Display::log_present_summary()` fixes a latent regression from the previous
   commit: `present_summary` used to print from the destructor *after*
   `scene=… slots=…`, which broke the "last stdout line" contract used by seven
   verification scripts.

## Validation

- Real chain (dummy devices, the user's clicks): `run-20261028T115014.820707Z-ihsxl6j2`.
  All 125 clip presents have `overlay=0`; the three MAP scene presents and the MR
  frame have `overlay=1`; the EA panorama is `overlay=0` (panorama rule).
  `screen_content now=clip previous=scene` at the clip start and
  `now=scene previous=clip` after the drain.
- `make -C port -j2 all test` passes (audio, menu, video, conversation, save,
  logging …).
- All 12 `analysis/rea/verify_*.py` scripts pass, including
  `verify_examine_controls.py` (the one that exposed the summary ordering),
  `verify_ea_layer/photo/single_execution`, `verify_mr_arrival`,
  `verify_ea_inline_voice`, `verify_tk_hotspots/media/animation` and the three
  panorama verifiers.
- The clip audio guard from `AUDIO_TAIL_GUARD.md` still reports
  `tail_guard …` when the Nargile clip plays.

## Follow-up — the outline follows the pointer (28 October 2026)

After the outline was made opt-in the user reported that hovering no longer
"activated" a hotspot: the map looked bare and the pointer gave no feedback.
The pinned live run `run-20261028T115911.039934Z-fm5xb4ev` (MR → MAP, `30.btb`)
shows the facts:

- the map's entry active set is `34,36,39,40,41,46,47,49` — **identical in every
  saved run since 27 October**, including before these changes;
- the hovered point `(159,221)` is inside slot 41 (active, label 433); `(86,218)`
  and `(103,220)` are in a gap; the click at `(309,216)` legitimately reports
  `hit no hotspot`;
- on the map the pointer is `map` (id 0x1d) everywhere: `0x40de7b` maps both the
  default id 0 and the map-hotspot kind `0x17` to the same cursor, so the
  original gives no hover feedback on the map either. The user's "hotspot was
  there and activated on hover" refers to the port's outline, not to the game.

"Mehmet'in Halı Dükkanı" (the other circled label) is object 8, kind 0x8,
`active=0`; the script has not enabled it in this state, so it is neither
outlined nor clickable. That is script/save state, not the presentation change.

First implementation attempt: the diagnostic was enabled but **hover-only**
(`Options::hotspots_all = false`). It worked — a real X11 check drew the outline
exactly over the hovered object's rect (1560 outline pixels, bbox
`208,360..503,455` = logical 104..252 × 180..228) and nothing in a gap — but the
user rejected it: the always-visible rectangles are the port's **navigation
aid**, and hiding them removed "the working part". The hover-only mode is kept
as `--hotspots-hover`.

Final configuration:

- `Options::overlay = true`, `Options::hotspots_all = true`: the scene outlines
  **every active hotspot** again (the pre-change look the user navigates by).
- `--hotspots-hover` draws only the active hotspot under the pointer
  (`Display::hover_overlay_slot`, updated in the motion handler, re-present on
  change); `--no-hotspots` / `H` turn the diagnostic off; `--hotspots`/
  `--hotspots-all` stay as explicit aliases.
- `ScreenContent` still forbids the outline on a clip (frames **and** audio
  drain) and on a menu — that was the reported bug, not the scene rectangles.

Real window checks (Xephyr `:9`, 1280x960, window at 0,0 so logical `(x,y)` is
window `(2x,2y)`; yellow-ish = `r>235, 180<g<215, b<60`):

| Frame | Outline pixels |
|---|---|
| MAP with all active hotspots (default) | **10160** (8 rectangles, `port/output/hover-outline/default-all-hotspots.png`) |
| `SMEst1` clip playing | **3** (video content only, `clip-no-outline.png`) |
| hover-only over slot 41 / gap (previous mode) | 1560 / 8 |

The click path itself was also re-checked in the same window: MR door → MAP →
click slot 41 (label 433, selects + highlights) → click again (slot 7, label 444)
→ travel. The user's last run had only the first of those two clicks
(`run-20261028T121121.739401Z-2uwroqc4`: three button-downs, the map one at
`128,204` → slot 41; no second click after it), so the "broken click" was the
missing second click, not a regression.

`make -C port -j2 all test` passes; the verify scripts still pass (the outline is
SDL-only and never in a `--shot` PPM).

## Follow-up 2 — hover highlights the destination and one click travels (28 October 2026)

The user's requirement (28 October 2026): "no frame; while moving the mouse on
the map the reachable places highlight, and clicking goes there". A later report
(28 October 2026) added: the highlight stayed after mouse-out and different
destinations accumulated different colours.

What the data says: the map's destinations are kind-0x17 objects whose *select*
labels (433 for Süleymaniye, 450 for Emre's House, 269 for Topkapı, …) draw the
destination's `*Hi` record (0x64/0x95) and swap the destination for its travel
object (0x6e disables 41, 0x6d enables 7 and the full-playfield cancel 24). The
shipped engine's own hover (0x40c329) only sets the cursor and DAT_004603d4, so
the *original* has no hover highlight; running the select label on hover is the
port's map UX built from the game's own data.

Implementation — `hover_select` (play_main.cpp, called from the motion handler):

- `hover_select_pick` considers only an **active kind-0x17 destination
  rectangle**, never a full-playfield "click anywhere" handler (24/32), and
  reads the pre-preview snapshot while a preview is active (see below).
- On entering a destination the select label is traced and committed: the `*Hi`
  art appears and the travel object is enabled, so **one click travels**.
- The preview is **transactional**: `hover_select` snapshots the whole
  `SceneState` and `SceneTrace` before committing the label, and
  `restore_hover_preview` puts the snapshot back exactly when the pointer
  leaves or changes destination — and on `SDL_WINDOWEVENT_LEAVE` /
  `FOCUS_LOST`, where no motion event would arrive. Logs:
  `hover_highlight slot=N label=… steps=… at=x,y` and
  `hover_highlight slot=-1 restored`.
- The pick must use the snapshot, not the live trace: after the label runs, the
  destination is gone (41 disabled) and only its travel object (7) and the
  cancel handler (24) remain, so a live pick re-enabled and re-ran the label on
  every motion event. That friction produced the reported "the highlight never
  goes away" and the flicker; staying on the same destination is now a no-op.
- A hover is refused (`skipped reason=not_a_highlight`) when the label would
  request a scene switch, open a conversation, play a clip, queue a sound or
  stop on a blocker.

Rejected attempts (recorded so they are not repeated):

- Running every kind-0x17 hit, including the full-playfield handlers, with a
  fallback/retry: one sweep ran 8 labels (`41→433, 24→439, 46→229, 29→235,
  34→269, 18→275, 47→246, 30→252`) and churned the object set.
- Keeping the committed label and only restoring `records`: the object/hotspot
  swap survived, so the next hover started from a selected map and the
  highlights accumulated.

Real window checks (Xephyr `:11`, window at 0,0, MR → MAP):

```
hover_highlight slot=41 label=433 steps=6 at=178,204
click at 178,204 slot 7 action 444 scene_switch=30      → SM, one click
hover_highlight slot=34 label=269 steps=6 at=440,265
click at 440,265 slot 1 action 263 scene_switch=33      → TK, one click
```

Reversibility evidence (frame captures with `xwd -root`, converted with
ffmpeg; pixel diffs against the plain map):

| capture | differing pixels vs. plain map |
|---|---|
| hover Süleymaniye (`base` → `sm`) | 26,780 (blue `mapsmHi` ellipse) |
| pointer moved to empty map (`gap1`) | **0** (pixel-identical) |
| hover Topkapı (`tk`) | 24,740 (cyan `*Hi` ellipse) |
| pointer moved to empty map (`gap2`) | **0** (pixel-identical) |
| sweep over 5 destinations, then empty map | **0** |
| hover, click empty map, move away | **0** |
| hover, pointer leaves the window, returns | **0** |

The frames are in `port/output/hover-outline/` (`hover-highlight-on.png`,
`hover-highlight-off.png`, `hover-topkapi-highlight.png`,
`hover-sweep-restored.png`).

Test-method caveat (cost an hour here): `ffmpeg -f x11grab` recentred the
pointer in this environment, so the captures did not correspond to the hover
under test and the game kept receiving motion events. `xwd -root` is the safe
capture path. `make -C port -j2 all test` and the `verify_*.py` scripts pass;
the `--click-at` scripts keep their documented two-click chain because they do
not hover.

Known limitation: the snapshot restores record frame counters too, so a preview
that is left open while animated records advance (e.g. the window loses focus)
rewinds those counters on restore. The map's `*Hi` art is static, so this is not
visible there; a scene with long hover periods over animated records would need
a narrower merge.

## Follow-up 3 — scene changes go through one transition presenter (28 October 2026)

The policy above decided *what* is on screen; the arrival itself was still a
cut. Two frames of the same run prove it: `present index=105` (the last
`HLEst1` clip frame, 12840 ms) and `present index=115` (`HL00`, 13129 ms) with
`prev_held_ms=282` and nothing between them.

The shipped engine blends there (`0x40186e`'s `switch(param_3)`, see
`TRANSITIONS.md`), so `Display::present_transition` is now the only way a scene
change is shown:

- `present_arrival` composes and presents the target for `travel_to` (entry and
  save paths), the close-up return and `restart_scene`;
  `present_action_result` uses it when the clicked label ran a `0xb9`.
- The intermediate frames are ordinary `present_image` calls, so the single
  `SDL_RenderPresent` rule and the `overlay=` trace field still hold. They run
  under `ScreenContent::transition`, so the outline is never drawn on them, and
  `screen_content` records the ownership change.
- `present_image` keeps `last_playfield` (the 640x400 rectangle the original's
  blends cover) for the next transition; a presentation that is not a full
  screen invalidates it instead of guessing, which is why a headless
  `--click-at` run whose first action is a travel reports
  `skipped reason=no_departing_frame`.

Evidence: `port/output/transition-demo/` (the port's own frames per code),
`TRANSITIONS.md` §4.

## Boundaries

- The outline is drawn by SDL on top of the composed image, so the
  `--present-dump` PPMs cannot show it. This audit's evidence is the single
  policy plus the `overlay=`/`screen_content` trace fields; the runs are
  headless (SDL dummy), not a window-pixel capture. The earlier 27 October
  window-pixel check (`port/output/cf-video/map-travel-no-outline.png`) remains
  the visual reference for the clip-frame part.
- `ScreenContent` says *what* is on screen; transitions are a separate
  presenter (`present_transition`) that composes its steps through
  `present_image`. Menus keep their own screens and the diagnostic stays off
  there and on transition frames.
- Default-off means the player now looks like the shipped game by default: no
  outlines at all. `--hotspots`/`H` restore the diagnostic for investigation; the
  `present … overlay=` field records which state was active.
- The original's per-screen policy is not claimed: the table above is what the
  recovered exports show (one surface, one present, no outline).
