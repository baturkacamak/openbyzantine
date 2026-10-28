# Presented-frame trace and the post-video MAP flash — 28 October 2026

## Symptom

User report (28 October 2026): travelling from the map to a site plays the site
video first, then "a picture appears for an instant and closes" and the user
cannot tell what it is. Pinned user run:
`port/output/logs/run-20261028T103348.882238Z-mvh2o62q` (MR → MAP → SM,
`30.btb`, `--entry`, 17.2 s, exit 0).

## What was added (host diagnostics, no game state)

The player had composition lines (`composed background=…`, `composed
panorama=…`) but no record of **what was actually presented** between them, so
a frame drawn by a silent compose or held during a scene load could not be
identified. `port/src/play_main.cpp` now records every `SDL_RenderPresent`:

- `Display::present_image(..., const char* tag)` is the single presentation
  choke point (`port/src/play_main.cpp:1717`). Tags: `scene`, `conversation`,
  `menu`, `menu_movie`, `credits`.
- `Display::log_present` (`:1776`) samples every 4th pixel into an FNV-1a
  checksum and writes a detail-trace line when the picture changes, when a
  capture window is open, or as a 5 s heartbeat. Fields: `index`, `tag`,
  `detail`, `scene`, `label`, `layer` (background JPEG or panorama node with
  yaw/pitch), `dialogue`, `bar`, `size`, `hash`, `prev_hash`, `changed`,
  `prev_held_ms` (how long the previous picture was on screen),
  `since_log_ms`, `captured`, `dump`.
- `play_decoded_clip` sets `present_detail` per frame
  (`clip:index/total:file.jpg`, `:4096`), so each AVI frame is named.
- `travel_to` writes `travel_begin`/`travel_loaded` markers and arms an 800 ms
  capture window (`:2983`, `:3066`, `:1867`); the end of a `0xbd` clip arms the
  same window (`reason=video_end`).
- `--present-dump <dir>` (or `BYZ_PRESENT_DUMP`) writes each distinct presented
  640x480 screen as PPM while a capture window is open or the layer/state
  changed. Bounded by `--present-dump-max` / `BYZ_PRESENT_DUMP_MAX` (default
  40); video frames are not dumped because their cached `f%04u.jpg` files are
  already on disk. Each dump also prints one `present_dump file=…` line to
  stdout; `present_summary frames=… logged=… dumps=…` closes the run.

The lines go to the detail trace (`$BYZ_TRACE_LOG`, i.e. the run's
`.trace.log`) because they can run per frame. `scene.sh` launches already
produce that file.

## Reproduction and logged evidence

Reproduction (dummy SDL devices, same save and click chain as the user):

```
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
BYZ_PRESENT_DUMP=/tmp/opencode/present-test \
bash port/scene.sh . MR 0 30.btb --entry \
  --click-at 584,368 --click-at 154,203 --click-at 139,203
```

Run `run-20261028T105522.508703Z-z2r8s8ur`; the transition slice is
`port/output/present-flash/trace-transition-slice.txt`:

```
present_capture reason=video_end ms=800
present index=71 tag=scene scene=MAP layer=background:MAPFINM1.jpg dialogue=1 \
  hash=0x72d4192a494a1029 prev_hash=0xf1f4a46a639b5837 changed=1 \
  prev_held_ms=66 captured=1 dump=…/p00071-scene-0x72d4192a494a1029.ppm
travel_begin index=30 from=MAP target=sm
travel_loaded index=30 scene=SM
present index=72 tag=scene scene=SM layer=panorama:SM00.JPG yaw=106 \
  prev_hash=0x72d4192a494a1029 changed=1 prev_held_ms=176 captured=1 \
  dump=…/p00072-scene-0xb4a454b925dc30ae.ppm
```

- The 65 clip frames are each logged, e.g. `detail=SMEst1:65/65:f0065.jpg`
  with `prev_held_ms≈66`, i.e. 15 fps.
- After the clip the **departing MAP frame** is presented once and stays on
  screen until the SM panorama is ready. `prev_held_ms=176` on the SM line is
  the map frame's displayed time in this reproduction (another run of the same
  build measured 179 ms).
- The user's own run shows the same sequence with a 112 ms hold
  (`+10.377s` `composed background=MAPFINM1.jpg` → `+10.489s` first SM
  compose), so the hold is shorter on that machine but the same picture.

The dumped frame `port/output/present-flash/flash-after-video-map.png` is
`MAPFINM1.jpg` with only records 0 and 15 (the action ran `0x90(0)`, which closed
the other map records: `drawn=2 skipped_snap=17`) plus the inventory strip,
because `0xb5` (instruction 521) set `dialogue_visible` after the clip. The map
therefore appears without its clickable markers; that is why it does not read as
"the map I was just using". `destination-sm.png` is the following SM00 frame.

## Cause (observation + inference)

Observation (before the fix): `handle_click` unconditionally composed and
presented the action's result, and only then called `follow_switches` →
`travel_to`. When the action ends in a `0x73`, that
presentation is the *departing* scene. The next scene is only presented after
its scripts/JPEG/panorama are read and decoded, so the old frame stays up for
the load time (38 ms for a small JPEG, 100–180 ms for a 2496x768 panorama
node).

Inference (not proven from the binary): the original runs `0xbd` → `0xb5` →
`0x73` inside one script call, and its renderer only draws between calls, so it
would keep the clip's last frame until the new scene's first frame. The same
class of intervening composition was documented for a scene *entry* in
`SCENE_ENTRY_FLASH.md`; this report does not change that fix.

Unknown: the user's physical display was not observed; the dump is the composed
640x480 screen handed to `SDL_RenderPresent`, not a window capture.

## Fix — a 0x73 click no longer presents the departing scene (28 October 2026)

User run `run-20261028T110009.000355Z-_r_fgi3o` (MR → MAP → Topkapı, `TKEst1`
78 frames) confirmed the same picture at the reported entry: the last clip frame
was presented at `elapsed_ms=10160` (`TKEst1:78/78`), then **the MAP frame**
(`MAPFINM1.jpg`, `dialogue=1`, dump `/tmp/byz-present/p00084-…`) at `10234`, and
the first TK frame (`TKXA.jpg`) at `10321` — the map held 87 ms (the user: "the
map appeared very quickly and closed").

`port/src/play_main.cpp:3427-3450`:
`action_switches_scene` / `present_action_result` / `present_refused_switch`
(next to `follow_switches`). The interactive click paths (`handle_click` and the
`--click` path) now skip the post-action compose/present when the committed
trace requests a `0x73`; `travel_to` presents the target scene itself. If the
switch is refused (missing scene index/script), `present_refused_switch` still
shows the click's own committed frame so the screen does not keep the clip's last
frame. Headless `--shot` paths are unchanged: they never presented before
`follow_switches`.

Validation, same save and clicks as the user (`MR` door `599,342` → MAP `455,261`
→ site `450,262`; playfield coordinates for `--click-at` are the screen y minus
0x14): `run-20261028T110317.841986Z-t4soyybp`:
`present … detail=TKEst1:78/78:f0078.jpg` at 5611 → `travel_begin` 5668 →
`travel_loaded` 5686 → `present … layer=background:TKXA.jpg prev_held_ms=96` at
5714. **No MAP frame between the clip and TK.** The same check on the SM site
(`SMEst1`): last clip frame → `present … layer=panorama:SM00.JPG
prev_held_ms=227`, no MAP frame. `make -C port -j2 all test` passes.

Before/after evidence: `port/output/present-flash/topkapi-user-before-map.png`
(the map the user saw), `topkapi-after-fix-first-frame.png` (now the first frame
after the clip), `trace-topkapi-user-before.txt`,
`trace-topkapi-after-fix.txt`.

Note: the pre-clip gate frame at the action's video request still composes the map
(`composed background=MAPFINM1 …` before `video_request`). It is the scene the
player is looking at and the clip's first frame covers it within milliseconds; it
was not changed.

## Details, limitations

- Log volume: 73 detail-trace lines for ~9 s of play (861 presents); animations log
  only when the picture or state changes, plus a 5 s heartbeat.
- Checksum is a 4-pixel-stride sample, a diagnostic identity, not a pixel-exact
  hash.
- Dumps are the port's composed screen, so they show what the renderer was
  given; the window itself, cursor and SDL scaling are not in the image.
- The reproduced runs used SDL dummy video/audio (`--present-dump` under
  `run_logged.py`); the user's run used a real window on `:1`. Both are
  identity/duration checks of the presented screen, not captures of the window.
- Not proven: the original engine's exact inter-call draw points; the fix removes
  a port-only presentation, it does not add a transition effect.
