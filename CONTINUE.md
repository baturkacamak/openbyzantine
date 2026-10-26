# Compact handoff — 26 October 2026

## Current user direction — use libraries, integrate playable behavior

The durable policy is in `AGENTS.md`, "Library strategy and playable milestones".
Use SDL2's existing audio/window/input facilities; no new audio library or standard
codec reconstruction. Keep working JPEG code for now, use established libraries
for missing standard media support, and focus custom work on game rules and
playable integration. Do not search for original engine sources. The full audit
and decision are in `analysis/rea/LIBRARY_STRATEGY.md` (final section).
Audio output is implemented, but this does not complete audio behavior: session
timing, mixing/loops/parameters and original-game fidelity require their own
evidence and validation.

**Repository contents:** git holds `port/`, `analysis/` and the docs only. The
game ISOs/archives, `analysis/rea/BETRAYAL.EXE`, `tools/rea`, `upscale_ornek` and
`port/output/` are ignored on purpose (copyright and size). `make -C port test`
needs `port/output/`: recreate it from your own copy with `port/byzantine-all`
(and `port/scene.sh` for the extracted media) before running the suites.

## Latest step — panorama scenes are playable (look around and click them)

A `0xb9` node scene is no longer a background it cannot click. The player samples
the 2496x768 `nodes/` panorama through the engine's own camera map, turns the
view, and maps a mouse click back to the **source** coordinate the hotspot table
uses, so hotspots at `x > 640` are reachable.

- `0x4240ae` recovered as the inverse of `0x4242bd`'s sampling:
  `linear = map[y*640+x] + yaw_word_offset`, `row = linear / stride`,
  `column = linear % stride - padding` wrapped into `[0, source_width)`.
  `0x401ca6` is the caller: it rejects `y < 0x14`/`y > 0x1a3`, subtracts `0x14`,
  calls `0x4240ae` and adds `0x14` back. `0x40c02c` tests the object rects with
  **inclusive** bounds, ids ascending, first hit wins. So a panorama hotspot rect
  is in source space (`x` up to 2495, `y` = source row + 0x14).
- The renderer's node size is fixed at startup: `0x401256` loads
  `nodes/sm/sm00.jpg` through `0x40f135` with a NULL buffer, which *writes*
  `DAT_0046487c`/`DAT_004654d8`; every later `0x401d29` passes them as the
  expected size and `0x40f135` returns NULL on a mismatch. The port takes the
  size from the first loaded node and reports a later mismatch instead of
  rescaling.
- `0x432295`'s `vertical` operand only scales `_DAT_0047a8dc`, which `0x417491`
  reads as a **message text** offset — it is not a camera parameter. The port
  reports it (`panorama_text_offset=`) and leaves the camera alone.
- `0xcb` is now a real rotation: `0x402285` inverse-maps the viewport centre
  `(0x140,0xf0)`, walks toward the operand's source column in `step`-pixel slices
  (`step` 10 unless operand 1 is type `0x40`), wrapping at half the source width,
  and advances `0x401ee6` per slice. The port applies the same slices in order.
- A staged pixel position (`0xba` -> `0xb9`) is **absolute**: `0x401ddf` calls
  `0x4241e2` (yaw = +pi, pitch 0, zoom 1, map rebuild) before exposing the degree
  delta, so the same staged value lands in the same view from any camera state
  (`panorama-view-test` locks this).
- Port: `port/{include/byzantine,src}/panorama_view.{hpp,cpp}` (`PanoramaView` =
  `0x4236b5` seam rows + camera + `0x42423d` delta + `0x4242bd` sampling +
  `0x4240ae` inverse). `byzantine-cc --panorama` now uses the same class and its
  output line is unchanged (`padding=313 yaw_degrees=101 sampled=256000`).
- Player: `apply_panorama_events` applies the committed trace's loads/rotations
  exactly once (never twice across the audio-gated replays); the scene layer is
  whichever of `0x69`/`0xb9` ran last, and a node from an earlier label stays
  current. Left-drag or `←`/`→` (10°) / `↑`/`↓` (5°) turns the view; a drag does
  not count as a click. New `--staged-x <0..2495>` (headless camera position) and
  `--click-at <x>,<y>` (the real click path in `--shot` mode).
- Real results: `EA` 602 `26.btb` -> `panorama_load instruction=317 name=ea00
  staged_x=none yaw=180 loaded`, `composed panorama=nodes/ea/EA00.jpg
  size=2496x768 padding=313 yaw=180 drawn=1 pixels=13570`; `--staged-x 700` ->
  `yaw_degrees=101 yaw=-79`; click 246,294 -> `click at 2423,496 (panorama)
  slot 4 action 586 click_result steps=11` (rect `2422,496 74x52`, x > 640), the
  same pixel at yaw -79 -> `627,306 slot 2 action 653 stopped: unresolved random
  seed at instruction 328` (honest stop, random seed is not in the save). `NB` 90
  `57.btb` -> `name=NB00`, `--staged-x 600` -> `yaw=-93`, click 192,272 ->
  `480,473 slot 5 action 233 steps=23`. Both frames were written as PPM and
  inspected (no seam, continuous ceiling/wall lines).
- Tests: new `panorama-view-test` proves the round trip: a synthetic 256x96
  source encodes `(row,column)` in each pixel, and for every 64x40 viewport pixel
  the sampled value must decode to exactly what `canvas_point` returns — after
  reset, a yaw-only advance (no map rebuild), a pitch advance (map rebuilt), a
  staged position and 24 further rotations. Rejections: out-of-viewport point,
  mismatched destination, short source, staged position outside 0..2495.
- Validation: `make -C port -j2 all test` passes all nine suites; the three
  panorama verifiers pass. Non-panorama acceptance unchanged: MAP 87 ->
  `drawn=27 pixels=163079`, `--click 34` -> `action 269 steps=6`; VT 72.btb 497
  `--click 14` -> `click_result steps=12 dialogue_visible text=1 sounds=1
  fingerprint=0x849a49bff1a28319`, `wait_selftest clicks_ignored=1
  overlay_toggled=true`.
- Report: `analysis/rea/PANORAMA_VIEW.md` (20 verified hashes). Port coverage
  audit: 120 -> **127** of 840 game functions (14.3% -> 15.1% of functions,
  75,075 -> 76,910 B = 30.9% of game code).
- Still not modelled (reported, never invented): transition effects
  (`0x418bcf`, `0x42bce1`, `0x42be07`, `0x419cf6`, `0x418eb2`, `0x419a51`) and
  transition codes `0x65..0x6a`; `0x4242bd`'s `param_6 != 0` `_rand` noise branch
  and `+0x30`; the kind-2 object branch of `0x40c02c` (`0x40d84d`); the
  `_DAT_0047a8dc` text offset; the snap pass, the `0x42b5b2` blend and the trail
  list of the record pass; text (`0x417a17`), video (`0xbd`), cross-scene state
  and `0x73`. The original's on-screen cursor-zone buttons (`0x41e289`,
  `DAT_0045f548`) are not modelled as input; arrow keys/drag are a port choice.
  Hotspot outlines are not drawn on panorama scenes (their rects are in source
  space).

## Next step candidates (observations only — nothing implemented)

Recorded while looking for the text layer, so the next step does not have to
re-derive it. All of this is export-reading plus shipped-file parsing; no port
code exists for it yet.

- **Font assets exist and are simple.** `copy2hd/fonts/`: `DIALOG.SFN` +
  `GR_DLG.TGA` + `BK_DLG.TGA`, `klio.SFN` + `GR_KLIO`/`BK_KLIO`, `setup.sfn` +
  `setup.tga`, `CAPTIONS.SFN` + `CAPTIONS.TGA`, `White.sfn`, `Yellow.sfn`.
- **SFN layout** (parsed from all six files, sizes add up exactly): header 24
  bytes — `u16` version 16, `u16` 0, `u32` 0, `u16` glyph count, `u16` atlas
  width, `u16` atlas height, `u16` 0, `u32` metrics offset (24), `u32` pixel
  offset; then `count * 8` bytes of `(x, y, advance, height)`, then a
  bottom-up 16-bit atlas of `width*height` (435..2496 px wide, 85..93 tall).
  Glyph index = `char - 0x20` (`0x4061e7` measures, `0x4069a4` draws).
- **The TGA companion is an 8-bit alpha atlas** (`0x406041` reads an 18-byte TGA
  header, flips rows top-down, `0x405c19` converts 16-bit/555 when the BPP byte
  says so). `0x4067e2` blends the fill colour (`DAT_00487554/0x487556`, per font
  from `0x406cfe`/`0x406ef5`) with the `gr` atlas over the `bk` atlas through
  `0x4066c6`'s `(c1*(a+1) + c2*(255-a)) >> 8` and the `0x405b90` multiply table.
- **Font registry** `0x406cfe`/`0x406ef5`, seven 0x48-byte slots:
  0 dialog (0x72C0), 1 klio, 2 setup, 3 klio variant, 4 captions, 5 white,
  6 yellow. `0x4063e0` is the text drawer (alignment 1/2/4, `\n`, max lines) and
  `0x417a17` draws a right-aligned 3-line message at `(0x26e,0x2d)` with font 3.
- **Text content**: `engine/CE.ALL` (16 entries) holds the Turkish examine texts
  (`$`-separated blocks, CRLF lines, CP1254-ish high bytes — e.g. HSTMED's Medusa
  base description). `copy2hd/setupart/MESSAGES.TXT` holds UI/save/character
  names. No dialogue-subtitle asset was found: the conversational text appears to
  be voice + `dialog/*.avi` only, so a text layer mainly serves the examine/
  message window and the UI.
- Verified SHA-256 (first 16) for the font functions, from
  `pseudocode/decompile-index.json`: `0x405b90` `ec676cb6ae3ea311`,
  `0x405c19` `1ec23faffc24579a`, `0x405db2` `e9719b48cd646221`,
  `0x405e7e` `74e5b2941030af57`, `0x406041` `f0955510ba79a92d`,
  `0x4061e7` `53d847dfc7e31e96`, `0x4062bc` `6b7e8f71d8177b7b`,
  `0x4063e0` `a321c3e91c9b99e1`, `0x4066c6` `fbcc5640b962cc1c`,
  `0x4067e2` `99488c31c78b9447`, `0x4069a4` `ab6b7bd02064ab5d`,
  `0x406cfe` `df4c09b307ceca77`, `0x406ef5` `7411bee3de54d03a`.
- The other open playability gap is **cross-scene travel**: `0x42c8a0` clears the
  3000-word pool once, then reloads `engine/<scene>` per `DAT_00482428`; `0x73`
  only writes that index and returns 2. `DAT_0047a238` (the instruction list
  head, linked through `+0x2c`) is the scene entry, i.e. execution starts at the
  first instruction, not at a label.

## Previous step — the SDL2 player draws a real scene (first playable delivery)

`byzantine-play` is no longer a background+hotspot prototype: it loads a real
savegame, composes the background JPEG and the animation records, and updates the
window from a verified click. Sound playback and the script's wait/continue
behaviour are wired with SDL2's audio device.

- Shared loader: the state setup moved out of `cc_main.cpp` into
  `port/src/scene_setup.cpp` + `include/byzantine/scene_setup.hpp`
  (`read_metadata_file`, `read_variable_metadata`, `apply_engine_globals`,
  `require_matching_scene`, `read_name_list`, `apply_dialogue_inputs`,
  `load_scene_setup`) and `byzantine-cc` now calls the same functions, so the
  player and `--trace-save` cannot drift. `apply_dialogue_inputs` also accepts
  `--cs` (the per-scene sound table), which the trace path previously could not
  supply at all — that is why every `0xc4` trace printed `unresolved_sound_name`.
- VM: `trace_scene_label(..., step_budget)` + `SceneTrace::paused_by_budget`.
  A caller can pause a label after N instructions and replay it from the same
  entry state with a larger budget; `scene-state` locks "paused state is exactly
  after N instructions" and "budgeted replay == unbudgeted run".
- Player (`port/src/play_main.cpp`, rewritten): `--save/--engine/--lcx/--lci/
  --ci/--lcy/--cy/--co/--cf/--ca/--anims/--cs/--gameprefs/--inventor/--hotitems`
  plus `--backgrounds <root>` (finds `backs/<2ch>/<name>.jpg` case-insensitively),
  `--sounds <root>` (`sounds/<name>.wav`, `sounds/<scene>/<name>.wav`),
  `--shot <file.ppm>` (writes the composed 640x400 frame), `--click <slot>`,
  `--print-audio`, `--no-hotspots`, `--no-audio`. ffmpeg and `fork` are gone;
  the JPEG decoder is the internal one.
- Click semantics: `0x72` rectangles live in the original **640x480 surface**
  space, so a window click at playfield `y` is tested at `y + 0x14`; the action
  label runs on a copy of the state and is committed only when the trace did not
  stop on `unresolved_*`, `missing_*` or `unsupported_opcode`. A `0x73` scene
  switch is reported but **not** committed (no carried cross-scene state).
- Audio: SDL2 audio device + strict RIFF/WAVE (PCM16) parser, `SDL_AudioStream`
  conversion to 22050 Hz stereo. Each `0xd1`/`0xd3` request resolves its name
  through `DAT_0047a8e0` (`--cs`) and plays `<sounds>/<name>.wav`; the player
  queues the clip and waits until the device drains it, so the label is stepped
  (budget 1..N) and the script does not run past a sound. `--print-audio` lists
  what would play without opening the device.
- Real results: `MAP tempsave.btb` 87 -> `composed background=MAPFINM1.jpg
  drawn=27 skipped_no_frame=0 pixels=163079`; `--click 34` -> `action 269
  steps=6`, active slots 34,35,36,39,40,41,42,46,47,48,49 -> 12 slots
  (1,18,35,36,39,40,41,42,46,47,48,49) and the written frame differs (12,328
  bytes / ~4,109 pixels). `VT 72.btb` 497 -> `sound_start instruction=560
  id=35 name=voTins1 duration_ms=4709 waited_ms=4696 (script continues)`
  (SDL_AUDIODRIVER=dummy, 4.9 s wall clock).
- Launcher: `port/scene.sh <game-dir> [scene] [label] [save]` extracts the six
  ISOs once into `$BYZ_DATA`, splits CS/CI/CY/CF/CA with `byzantine-all`, and
  starts the player. First run verified from an empty directory.
- **Correction after the first player delivery (26 October 2026):** the wait was
  a blocking `SDL_Delay` loop, so the window froze for the whole clip, and a
  click-started label ran to completion first and played its sounds afterwards.
  Now: `SceneSoundStart::step`/`SceneSoundSession::step` carry the executed
  instruction index; the player replays the label with a step budget up to that
  step, presents the frame, plays the clip while the event loop keeps running
  (`SDL_PollEvent` + `SDL_RenderPresent` each iteration, `H`/`Esc` live, clicks
  ignored), then continues with the next budget. The last budget is the label's
  own step count, so the final state equals an unbudgeted run. Measured on
  `VT 72.btb` 497 with `--click 14`: startup label `gate stopped_after_step=4
  of=15`, then `audio step=4 id=35 waited_ms=4669 frames=383 events=19 (script
  continues)`; the click label `gate stopped_after_step=1 of=12`, `waited_ms=4689
  frames=767`, then `click_result steps=12 dialogue_visible text=1 sounds=1
  active_slots=2`. `--selftest-events` (injects H + one click during the first
  wait) reports `clicks_ignored=1 overlay_toggled=true`, i.e. the window stays
  interactive while the script waits. Equivalence proof: the gated click run and
  an unbudgeted single run of the same action label both report
  `step=12 fingerprint=0x849a49bff1a28319` (`describe_trace` hashes variables,
  record kind/position/frame/flags, dialogue visibility and channel stack
  sizes), so the audio-gated run really continues from the queued instruction
  and ends in the same state. `MAP` click 34 (no sounds) is unchanged:
  `click_result steps=6 sounds=0 fingerprint=… active_slots=12`.
- Not modelled by the player (reported, never invented): text/message rendering
  (`0x417a17`), video (`0xbd`), cross-scene state and `0x73` travel, the
  `+0x09 & 0x20` snap pass and the `0x42b5b2` blend. Panorama scenes and their
  screen->canvas mapping ARE implemented as of the step above
  (`analysis/rea/PANORAMA_VIEW.md`).
- Tests: `scene-state` gained the budget cases; `make -C port -j2 all test`
  passes all eight suites and the three panorama verifiers pass. The CLI
  acceptance traces are byte-identical to the previous step (MAP 87: `steps=70
  stopped_at=137`, `variables_restored=300 declared=27 scene=map`).

## Previous step — JPEG decode and the background/panorama layers

`backs/<2ch>/<name>.jpg` and `nodes/<2ch>/<name>.jpg` now decode, so a composed
frame is a real scene: a background photograph with the record sprites over it.

- Loader chain: `0x41045b` (background) builds `"backs\\"` + `"%c%c\\%s"` from the
  CB entry's first two characters and calls
  `0x40f135(path, [640,400], &height, surface+0x6400, 640)`; the panorama path
  `0x401d29` calls the same loader against the renderer's stored size and then
  `0x42382b` rebuilds the seam columns. `0x40f135` fails unless the decoded size
  equals the caller's expectation, so a background must be exactly 640x400.
- The decoder is statically linked **IJG libjpeg 6a**: `0x43de70` rejects any
  version other than `0x3d` (61) and any struct size other than `0x1a8` (424);
  the exports show libjpeg's 200/201/202/205/206 state machine
  (`jpeg_read_header`/`start_decompress`/`read_scanlines`/`finish_decompress`).
  Its defaults therefore apply: JDCT_ISLOW, "fancy" upsampling, JCS_RGB.
- `0x40f080` mode 6 (`_DAT_00485c80 == 6`, set by `0x4162a0` from the green mask
  `0x7e0`) is `r>>3, g>>2, b>>3` over libjpeg's RGB byte order.
- Shipped corpus: **283 JPEGs** (215 `backs/` 640x400, 68 `nodes/` 2496x768), all
  SOF0 baseline 8-bit, 4:2:0 `2x2 1x1 1x1`, one interleaved scan, no DRI, no
  APP14. **283/283 decode**, 11.3 s total.
- Port: `port/{include/byzantine,src}/jpeg.{hpp,cpp}` — strict marker walk,
  canonical Huffman with 0xFF stuffing, DQT/DHT/DRI, baseline + progressive
  (spectral selection and successive approximation), libjpeg 6a's `jidctint`
  IDCT, 3:1 triangle upsampling for 2:1 factors, `jdcolor` 16.16 fixed point,
  `rgb_to_rgb565`. Everything unsupported (SOF3/SOF5..SOF15, DAC, 12-bit, 2/4
  components, truncation, a marker inside entropy data) is an error.
- `compose.cpp` gained `blit_image`; CLI `--background <jpg>` (strict 640x400)
  and `--panorama <jpg> [--panorama-x N]` (decode -> `panorama_geometry` ->
  `pad_panorama_rows` -> `PanoramaProjection`/`consume_panorama_position` ->
  `sample_panorama_rgb16`) both run before `compose_scene`.
- **Correction** to the previous step: the record pass composes into a 640x480
  surface (`0x42b280` clears `0x4b000` words; `0x41045b` writes the background at
  `+0x6400` = 20 rows; `0x40241c` tests `y-0x14` in `0..399`). The port's surface
  is the visible band, so `compose_scene` now draws at `y - 0x14` and rejects a
  top-left outside the window. MAP 87: pixels 156022 -> **163079**, clipped
  records 4 -> **0**, `draw id=33 y=178 -> 158`.
- Real results: MAP `tempsave.btb` 87 with `backs/ma/MAPFINM1.jpg` -> background
  `written=256000`, `drawn=27 pixels=163079` (id 33 at 284,158; id 0 at 143,43);
  NB 57.btb 556 with `backs/nb/NBDESK.jpg` -> `written=256000` plus
  `id=10 kind=1 x=0 y=0 written=20299`; a 2496x768 `nodes/*.jpg` with
  `--panorama-x 700` -> `padding=313 yaw_degrees=101 sampled=256000`.
- Validation: the luma plane against an independent decoder (`ffmpeg`) differs by
  mean |Δ| 0.20 (max 5.1, clamping); a 4:4:4 stream agrees to max 3 / mean 0.009;
  shipped 4:2:0 differs only on chroma edges (mean 0.79, max 15, 0.3 % of bytes
  > 4) where the upsampler's rounding and edge handling are an inference.
- Tests: new `jpeg-test` (synthetic baseline + progressive streams, nine
  rejections, `rgb_to_rgb565`, the two fixture hashes, `--scan` census mode);
  `compose-test` covers `blit_image`; `scene-state` locks the playfield origin and
  the top-left rejection. `make -C port -j2 all test` and the three panorama
  verifiers pass. Census unchanged: completed **5,935** (5,913 `return` + 22
  `script_end`), blocking 0, `unresolved_variable` 1,296, `records_created` 733,
  `anims_load_failures` 8, `frame_overflow_logs` 1, 1,925 CA-type skips.

- Report: `analysis/rea/JPEG_DECODE.md` (15 verified hashes), correction appended
  to `SCENE_COMPOSE.md` §9, fixtures in `analysis/rea/jpeg_sample/`.
- Not modelled: the transitions and blends (`0x418bcf`, `0x42bce1`, `0x42be07`,
  `0x419cf6`, `0x418eb2`, `0x419a51`), the `+0x09 & 0x20` snap pass, the
  `+0x09 & 0x80` destination-read blend (`0x42b5b2`), `0x41045b`'s
  `inventor\examine` variant, audio, AVI and text rendering.

## Previous step — scene composition: records drawn onto an RGB16 surface

A decoded frame is now drawn at its record's position, in the renderer's order,
into an explicit surface and written as PPM.

- Draw loop (`0x43b216`, `0x43b216`-`0x43bb76`): scans `DAT_004649e8` for the
  lowest/highest slot, then (1) a "snap" pass gated by `DAT_004859cd == 0` for
  records with `+0x09 & 0x20` (drawn through the `+0x20` rect, then `+0x20`/
  `+0x22`/`+0x29` cleared) and (2) `for kind = 0..4`, ids from the highest down,
  drawing `0x43c114` rect -> `0x40241c` clip -> `0x42b679` copy.
- `0x43c114`: rect = record `x/y` (mode 0) or `+0x24/+0x26` (mode 1), size from
  the frame node's `+0xc`/`+0x10` (the animation header width/height).
  `0x40241c` clips to 640x400 with a `-0x14` y bias that cancels in the RECT;
  `0x42b679` is a row `memcpy` (no transparency), so the sprite must already be
  composed by `0x40bbfd` — which is exactly what the port does.
- Port: `port/{include/byzantine,src}/compose.{hpp,cpp}` (`Rgb16Surface`,
  `make_surface`, `blit_frame` with coverage + clipping, `write_ppm`) and
  `compose_scene(state, surface)` reproducing the kind 0..4 descending-id pass.
  CLI: `--compose-out <file.ppm>|-` and `--surface <WxH>` (default 640x400),
  printing `composed …` + one `draw id=…` per record + a coarse luminance map.
- Real results: `MAP` `tempsave.btb` 87 -> `drawn=27 pixels=156022`, descending
  ids 33..0, bottom-edge icons report `clipped` (2900/2037/1163/957); `VT`
  `73.btb` 1638 -> `drawn=2 pixels=40756` (ids 26 and 24). Tests: new
  `compose-test` (surface/bounds/blit/transparency/clipping/PPM) plus a
  `scene_state` ordering test.
- Census unchanged: completed **5,935**, blocking 0, `unresolved_variable`
  1,296, `records_created` 733, `anims_load_failures` 8,
  `frame_overflow_logs` 1, 1,925 CA-type skips.
- Report: `analysis/rea/SCENE_COMPOSE.md` (5 verified hashes).
- Not modelled: the `+0x09 & 0x20` snap pass, the `+0x09 & 0x80`
  destination-read blend (`0x42b5b2`), the mode-1 head-to-current sequence, the
  `+0x3c` trail list, and the background/panorama layers (both JPEG,
  `0x401d29`). The composed surface starts black.

## Previous step — frame run stream decoded (0x40bbfd) and RGB555 -> RGB565

The animation frame payload is now decoded to pixels, so a record's current
frame is real image data instead of a request.

- **Format**: the payload is a run stream decoded by `0x40bbfd` (renderer entry
  `0x43b130`, UI loaders `0x417054`/`0x417314`/`0x417491`/`0x417e9c`/`0x41f80c`/
  `0x4090c4`, and `0x4200bb`). `0x40bbb0(x, y, height, stride)` sets the state
  (`stride` at `0x46c42c`, `row_base = (y-1)*stride + x`, `row = -1`).
  | Word | Meaning |
  | --- | --- |
  | `0x0000｜n` | literal run: the next `n` words are pixels |
  | `0x4000｜x` | row start: `row++`, `x = row_base + n` |
  | `0x8000｜n` | colour run: one pixel word follows, repeated `n` times |
  | `0xC000｜n` | transparent gap: `x += n`, nothing written |
  `n` is the low 14 bits read as a signed 16-bit value. Each row opens with a
  command-1 marker and the stream ends with a `0x4000` closing marker, so
  `height+1` markers leave `row == height`; the original exits on that marker.
- **Colour**: `0x40be80` is the load-time pass, not the decoder: it walks the
  same stream and applies `0x4164a0` (`(p & 0x1f) | ((p & 0xffe0) << 1)`,
  **RGB555 -> RGB565**) to the pixel words of commands 0 and 2 only, for
  `height` rows. Skipped only when `DAT_00485c78 == 10` (unrecovered mode).
- Validation: **2,846/2,846 shipped frames decode cleanly** (exact word
  consumption, no negative count, final row == height, no row past the width);
  64 frames are legitimately blank (e.g. `BBBM00A` frame 20). `MAPHL` frame 0
  renders as a coherent icon (3,373 covered of 5,742).
- Port: `anims_frame_words`, `decode_anims_frame(frame, words, rgb565 = true)`
  -> `DecodedFrame` (pixels + coverage mask, since command 3 writes nothing) and
  `AnimsLibrary::decode(name, index)`. Strict: negative count, short literal/
  colour run, row count != height and word count != header are errors. The
  library now accepts either the extraction root or the `anims` directory.
- `--trace` gains `record_frame id=… index=… size=… covered=…` for created
  records: VT 73.btb 1638 -> `id=24 index=5 size=640x400 covered=16023/256000`
  and `id=26 index=4 covered=24733/256000`.
- `anims-test <tree>` now decodes every frame; `anims-test <tree> --render
  <name> <frame>` prints `covered/runs/skipped` and a coarse luminance map
  (e.g. `maphl 0` -> `99x58 covered=3373/5742 runs=3373 skipped=2`).
- Census unchanged (decode is a trace/CLI path): completed **5,935**, blocking
  0, `unresolved_variable` 1,296, `records_created` 733, `anims_load_failures`
  8, `frame_overflow_logs` 1, 1,925 CA-type skips.
- Tests: `anims-test` covers all four commands, clipping, the closing marker,
  five malformed-stream rejections, the 555->565 values, `anims_frame_words`
  bounds, the library lookup and the real-data pass (922 assets / 2,846 frames).
  `make -C port -j2 all test` + the three panorama verifiers pass.
- Report: `analysis/rea/ANIMATION_ASSET.md` (now 15 verified hashes).
- Still not modelled: drawing (`0x43b130`'s clip rect, the `+9 & 0x80`
  destination-read blend `0x42b5b2`, the GDI blit `0x42b679`), the AVI decoder,
  and `DAT_00485c78 == 10`.

## Previous step — animation assets: frame count, geometry, first-frame offsets

`0x64`/`0x91` are no longer request-only: a record created from the CA table now
carries the real frame count, per-frame geometry and the first frame's x/y
offset, because the `anims` container is decoded.

- Path: `0x40fe70` builds `anims\<name[0]><name[1]>\<name>.rle` (strings
  `0x45e800`/`0x45e810`/`0x45e808`); on failure it rewrites the suffix to
  `.avi` and `0x41cb13` builds an AVIFile object instead.
- `.RLE` header (`0x40cb26`): `u16` signature (-> record `+0x28`), `u32`+`u16`
  unused, `u16` **frame count**. Per frame a 26-byte header: `+0x02` width,
  `+0x04` height, `+0x06` x offset, `+0x08` y offset, `+0x16` u32 word count,
  then `words*2` bytes. `0x410078` calls `0x40ffee(record, count)` (count at
  `+0x14`), adds the frame-0 offsets to the record's x/y and fills the 0x18-byte
  frame nodes that `0x40cf73` walks for `0x91`.
- `.AVI` fallback (`0x41cb13`): frame count = video stream `dwLength`, every
  frame node gets the stream width/height, **no** position offset, decoder at
  `+0x38`. 8 of the 10 CA entries without `.RLE` have one; `BC` `bckill1` and
  `bckill2` have neither.
- Data validation: **922/922 `.RLE` parse exactly** (2,846 frames; 179 frames
  with non-zero offsets). Against the 77 CD1 saves: 448 saved node slots belong
  to CA type <=3 assets, **447 have `save.+0x14 == asset frame count`** (one
  outlier: `28.btb` `hl` slot 4 `HLDc00b`, save 21 vs asset 39 — unexplained,
  likely a transition-installed record). **429/429 slots whose asset has a
  non-zero offset satisfy `save.x == x_offset` and `save.y == 0x14 +
  y_offset`** (e.g. `map` slot 0 `maphl` save `143,63`, asset `143,43`).
- Port: `port/src/anims.cpp` + `include/byzantine/anims.hpp`
  (`parse_anims`, `parse_avi_anims`, `AnimsLibrary` with a case-insensitive
  index and the `.rle` -> `.avi` order). `SceneRecordState` gained
  `frame_count`, `anims_signature`, `frames`, `frames_known`, `current_frame`.
  `resolve_record` loads the asset when `--anims <root>` is supplied; a missing
  file is `0x40fe70` failing, so the command body is skipped
  (`anims_load_failures`). `0x91` now models `0x40cf73` (0xffff clears, a walk
  past the end logs `frame_overflow_logs` and stays on the last frame); `0x64`
  restarts the list at frame 0. `load_save` restores `+0x14` and the current
  index.
- Real results: `VOICE` 531 (no save) creates `vtroxeUp` -> `frames=5
  current=4`; `VT` 73.btb 1638 -> `records_created=2 frame_overflow_logs=1`,
  `id=24 frames=6 current=5`, `id=26 frames=5 current=4`; `NB` 556 ->
  `id=10 frames=3`; `HSTMED` 259 keeps the save's `frames=1/8` and
  `current=0/4/2` with `frames_known=false` (no asset reload for marker 0).
- Census: completed unchanged at **5,935** (`return` 5,913 + `script_end` 22),
  unsupported opcode 0, `unresolved_variable` 1,296. Diagnostics: **733**
  records created with frame data, **8** `anims` load failures, **1** frame
  overflow, 1,925 CA-type skips.
- Tests: new `anims-test` (synthetic container, truncation/trailing rejection,
  case-insensitive library, AVI fallback, plus an optional real-data pass);
  scene-state covers the frame walk, the overflow clamp, `0x64` restart and
  creation from an asset (offsets, `+0x28`); save-test covers `+0x14`/current.
  `make -C port -j2 all test` + the three panorama verifiers pass.
- Report: `analysis/rea/ANIMATION_ASSET.md` (10 verified hashes).
- Still not modelled: the packed pixel stream (`0x40be80`'s two-bit commands,
  `0x4164a0`'s 555->565 remap) and therefore sprite rendering; the AVI decoder;
  the 2-byte signature semantics; marker-1 record re-initialisation.

## Previous step — one record table, existence from the CA asset table

Merged the actor and node halves of `DAT_004649e8` and made record existence
data-driven from the shipped `engine/<scene>ca` table. This resolves the
limitation the previous step left open.

- **Correction**: `0x64` is *not* unguarded. The dispatcher case is `case 100:`
  (Ghidra prints decimal) and calls `0x431b0f` before `0x4324b0`
  (`0x42dc4b`..`0x42dc61`). Every record opcode (`0x64`, `0x84`..`0x8e`,
  `0x91`, `0x95`) runs the same guard; when it returns 1 the body is skipped and
  the script **continues** — the original never faults and never stops.
- **Existence**: `0x431b0f` reads `DAT_0047dd70[id]+0x08` (no NULL check) and
  loads a record only for type 0/1 through `0x40fe70`. `0x40fd7b` (scene loader
  `0x42c8a0`, save loader `0x425ab4`) frees the table via `0x4102ae` and
  pre-loads types 2/3 through the same `0x40fe70`. `DAT_0047dd70` is
  `engine/<scene>ca` (suffix `ca` at `0x45eaf0`). Any other type (`0xff` in the
  shipped data) leaves the slot NULL: body skipped, logged via `0x40ac43`.
- Shipped CA data: 44 scenes, 970 entries, types 255 x692, 0 x193, 3 x66, 2 x16,
  1 x3 — only 278 entries (28.7%) can create a record.
- Port: `SceneRecordState` + `SceneState::records` replace
  `SceneActorState`/`actors` and `SceneNodeState`/`nodes`; `load_save` restores
  the whole `0x426b9d` copy list (`+0`,`+2`,`+4`,`+6`,`+8`,`+9`, frame indices,
  `+0x2c`,`+0x30`, extra list). `SceneState::actor_assets` +
  `actor_assets_known` take `--ca`; a trace-local `resolve_record` reproduces
  `0x431b0f` with four outcomes: use/save record, `unresolved_node_asset` stop
  (no `--ca`), `missing_node_asset` stop (id beyond the CA table = NULL+8 read
  in the original), create for type 0..3, skip otherwise. `0x95`/`0x84`/`0x91`
  now guard **before** resolving operands, matching the original's order.
- New counters: `records_created`, `skipped_node_commands` per trace and
  aggregated by `--census` (printed as `skipped_node_commands=N
  records_created=N`).
- Real results: NB 57.btb 556 (save `nodes=0`) ran 9 steps, created the slot-10
  record (CA `NBBdSld` type 1) and wrote `node_write instruction=699 id=10
  value04=5 flags=4` after skipping slot 9 (`NBHandr` type 0xff); it used to
  stop at `unresolved_node_asset=9`. STAR 314 skipped both 0xff records and
  continued. HSTMED 259 still writes 48/50 from the save (0 created, 0 skipped).
- Census (CA.ALL supplied): completed **5,851 -> 5,935** (`return` 5,913 +
  `script_end` 22), `unresolved_node_asset` **76 -> 0**, `missing_node_asset` 0,
  `unresolved_variable` 1,306 -> 1,296, `missing_object` 78 -> 80,
  `scene_switch` 119, RNG 245, `step_limit` 5, disc 9, media 2, chains 2,
  unsupported opcode 0. Diagnostics: 738 records created, 1,925 record commands
  skipped across all label traces. The +84 completed labels are the merge itself
  (a `0x64`-created record used to be invisible to `0x8a`/`0x8b`/`0x86`).
- Tests: scene-state covers the CA type 0..3 creation, the 0xff skip
  (two skips, script continues to the `0x6f` return), the id-beyond-table stop,
  and "a save-restored record wins over a 0xff CA type"; save-test asserts the
  merged table size and the extended field restore.
  `make -C port -j2 all test` + the three panorama verifiers pass.
- Report: `analysis/rea/RECORD_TABLE.md` (21 verified hashes).
- Still not modelled: the animation asset itself (frames, per-frame x/y, frame
  count), `0x40fe70`'s failure path for types 0/1, transition-installed records
  (`0x439349`/`0x438de4`), and `0x8c`/`0x8e` (no shipped label reaches them).

## Previous step — the last seven opcodes, empty blocking census

`0x79`, `0x7f`, `0x86`, `0x8d`, `0xcf`, `0xd0` and `0xd3` now have real
branches, so **every opcode that a shipped scene label reaches is handled** and
the census blocking ranking is empty (58 -> 0). Also fixed a real `0x64` defect.

- `0x79`: writes `DAT_0045f490 = 1` only when it is 0 and sets the dispatcher
  result to 1; `0x42d594` turns 1 into "`DAT_00484e28 = pc`, stop". New terminal
  outcome `script_end` (22 labels; `0x41e289` resumes the saved address, so this
  is a return to the caller script).
- `0x7f`: `variables[raw(1)] = u16(dword+0x00 - dword+0x04)` of
  `DAT_00464f60[raw(0)]`. Objdump `0x430d05`/`0x430d17`/`0x430d1a` settle the
  32-bit subtraction and the 16-bit truncation (Ghidra's `(short)` casts are
  wrong). The record is loaded **before** the NULL check, so an absent slot is
  the explicit `missing_media_record` stop.
- `0x86`: node guard `0x431b0f`, then `0x43299d` writes `+0x04` from the raw
  operand 1 and `0x432904(id, instr, 2)` masks `+0x09` for type-2 operands 2..7.
  `SceneNodeState` gained `value04`/`flags` (0x40ffee defaults, restored from the
  save record's `raw[4..5]`/`raw[9]`), writes are `node_writes`, absent nodes
  stay `unresolved_node_asset`.
- `0x8d`: `0x431a69` -> `0x40c7b4` makes CD `disc` current (drive enumeration,
  volume check, MessageBox); disc 7 means "1, else 2". Success continues,
  failure ends the script and sets `DAT_0045f490` 3 (or 1). Host state, so the
  port stops with `disc_request`. `0x409513` is the debug logger only.
- `0xcf`/`0xd0`: `0x427a01` release requests (`sound_releases`, with the
  `+0x14` gate and record presence; the `0x428143`/`DAT_00485c90` device gate is
  not evaluated) and `0x4278ab` stop-all (`sound_stop_all_requests`).
- `0xd3`: the nine-argument `0x428cbd` form -> `SceneSoundSession` with three
  (timeout ms, `0x431ec2(record, 0x66, 0)`) slots and the shared
  `+0x18`/gamepref[2] gate.
- **Correction**: `0x4324b0` writes byte `+0x09`, not `+0x08`
  (`0x43251d mov BYTE PTR [ecx+0x9],al`; guard reads `[eax+0x9]`). `0x64` now
  writes `flags`, so `0x84`'s `+0x09 & 0x20` trail guard really clears and
  `0x85`'s masks start from the kind bit. `ANIMATION_RECORD.md` updated.
- Real results: AM 66.btb label 1010 -> `script_end` at 1249; AM 1183 ->
  `sound_session instruction=1484 id=24 allowed=true timeout0=15000/18
  timeout1=18000/19`; HSTMED 55.btb 259 -> `node_write instruction=279 id=48
  value04=1 flags=36` and id 50; MAP 134 -> `disc_request=1`; VC 133 ->
  `sound_release id=1 field14=1`; CLOCK 504 -> `sound_stop_all_requests=1`;
  EA 757 -> `missing_media_record=5`; STAR 314 -> `unresolved_node_asset=10`.
- Census: completed **5,818 -> 5,851** (`return` 5,829 + `script_end` 22),
  unsupported opcode **58 -> 0**, `unresolved_variable` 1,306,
  `unresolved_node_asset` 76, `scene_switch` 119, `unresolved_random` 245,
  `missing_object` 78, `step_limit` 5, `unresolved_disc_request` 9,
  `missing_media_record` 2, `missing_dialogue_chain` 2. Opcodes with a port
  branch 70 -> 77 of 155.
- Tests: scene-state covers all seven commands plus the 0x64/0x84 correction;
  `make -C port -j2 all test` and the three panorama verifiers pass. Report:
  `analysis/rea/FINAL_OPCODES.md` (20 verified hashes).
- Known limitation kept: the original uses ONE `DAT_004649e8` record table for
  actors and nodes; the port still keeps `SceneState::actors` and
  `SceneState::nodes` apart. Unifying them is the next structural step.

## Previous step — chains, node lists, save requests and `0xc8`

Implemented `0x80`, `0x8a`/`0x8b`, `0xbe`/`0xd2`, `0xc6`/`0xc7`, `0xc8`,
`0xcb`, `0xd4`/`0xd5` and restored the dialogue-chain table.

- `0x80`: `DAT_00464f60[raw(0)]->+0x0c = raw(1)`; missing uses the 0x40abb4
  path, never an allocation.
- `0x8a`/`0x8b`: `0x431b0f` node guard then `0x432b09` extra-list append
  (`entry+0 = raw(2)`, `entry+4 = resolved(1)`, matching the save's
  `(first, second)` pair) or `0x432c65` `node+0x2c = raw(1)`. A node the save did
  not provide needs `DAT_0047dd70`/`0x40fe70`; it is reported as
  `unresolved_node_asset` instead of inventing the animation.
- `0xbe`/`0xd2`: **save/autosave requests**, not audio. Gate:
  `DAT_00460670 != -1` (an `examine` scene exists) and `gamepref[0] == 1`;
  `0x42515f` writes tempsave / copies it to autosave. The port records
  `tempsave_requests`/`autosave_requests` and continues; no file is written.
  Without the engine/scene metadata it stops with `unresolved_save_setting`.
- `0xc6`/`0xc7`: `DAT_00486a10` chains (25 ids at +0x00, count at +0x64) are now
  restored from save block 24 (`SceneState::dialogue_chains`). `0xc6` appends
  resolved CI name ids; `0xc7` removes one and compacts. An absent chain is
  `missing_dialogue_chain` (the original dereferences NULL).
- `0xc8`: located via the dispatcher jump table
  (`0x431758 + (opcode-0x64)*4` → `0x42e643`) and the decimal `case 200:` in
  the export. Resolved operand 0 is the sub-command: `0x1e` reseeds `_rand`
  from local time → `unresolved_random` stop; `0x1f` needs the `0x42a579` input
  triplet → `unresolved_system_input` (no-op unless operands 2/3/4 are type 2);
  `0x20` sets `dialogue_input_active`; `0x32`/`0x33` write `field_460668`;
  `0x36..0x3a` request a `status.000` write; `0x21..0x31/0x34/0x35` are no-ops.
- `0xcb`: `0x402285(resolved(0), step)` is a panorama rotation/scroll; `step`
  is 10 unless operand 1's type is `0x40`. Retained as a request.
- `0xd4`/`0xd5`: `DAT_004859cd` 1/0 (`flag_4859cd`, set by a successful load).
- `load_save` additions: `dialogue_chains`, `field_460668`, `flag_4859cd`;
  `--save-dump` prints/validates each chain.
- Real results: NB 57.btb label 202 → `dialogue_chain id=0 count=1 ids=20`
  (save chain 2 stays `43,123`); NB 257 → `missing_dialogue_chain=0`; VT 1818 →
  `system_command instruction=2272 sub=0x36 status_write_requested=54`.
- Census: completed **5,702 -> 5,818**, unsupported opcode **289 -> 58**;
  new named outcomes `unresolved_node_asset` (70) and `missing_dialogue_chain`
  (2); unresolved variable 1,278 -> 1,302, RNG 238 -> 245, `missing_object` 78,
  `scene_switch` 115, `step_limit` 5, `error` 0. New ranking: `0x79` (22),
  `0x8d` (9), `0xd0` (8), `0x86` (7), `0xcf` (7), `0xd3` (3), `0x7f` (2).
- Tests: scene-state covers all nine commands and the guard/missing paths;
  save-test covers chain/field/flag restore; `make -C port -j2 all test` and the
  three panorama verifiers pass. Report: `analysis/rea/CHAIN_SAVE_STATE.md`
  (18 verified hashes).
- Non-goals kept: writing a save/status file, audio output, rendering, asset
  loading beyond `unresolved_node_asset`, and guessing the `0xc8` input/timer
  subsystems.

## Previous step — `0xd1`, `0x6a`, `0xb4`/`0xb5`, `0xbb`, `0x73`

Implemented the five top blockers after the variable step. Audio is still
request-only and a scene switch is now an explicit stop.

- `0x6a` media definition: four operands — raw slot, resolved `+0x00`, raw
  `+0x0c`, and operand 3's **low byte** as `+0x11` when its type is 2. Writes
  `+0x10=1`, `+0x04=0`; `timeGetTime()` is not reproducible, so `+0x08 = 0`
  with `media_timestamps_unavailable`. Reuses an existing slot.
- `0xd1`: `0x428cbd(raw(0), raw(1), op2-if-type-2, 0, -1, 0, 0xffffffff, 0,
  0xffffffff)` is retained as `SceneSoundStart` (id, mode, text,
  `record_present`, the `+0x18`/gamepref[2] gate). No playback and no blocking
  dialogue loop.
- `0xb4`/`0xb5`: `DAT_0045d1c0 = 0/1` plus a redraw request. New
  `SceneState::dialogue_visible` (BSS default false), restored from
  `field_45d1c0` in `load_save`.
- `0xbb`: `0x4073f0(raw(0))` selects the CY channel through `0x408c81`, resets
  the scroll and redraws; implemented with the existing
  `resolve_name_index`/`SHOW.LCY` path.
- `0x73`: `DAT_00482460[raw(0)] -> DAT_00482428`. `DAT_00482460` is the
  50-entry array filled from `engine/<chapter>.lcx` (suffix `lcx`, `0x438173`);
  shipped `SHOW.LCX` has 44 entries (`1 -> 25 map`, `9 -> 21 iu`,
  `10 -> 42 vt`). Entries past the file count are the initialised `0xffff`
  ("no entry"); index >= 50 is rejected. The port records
  `scene_switch_requested` and stops instead of continuing with the old
  scene's variable row. `DAT_004603d0`'s override is not modelled.
- New `parse_scene_switch_table` (records.cpp) and CLI `--lcx <SHOW.LCX>`
  (also supplied automatically by the census).
- Real results: VT save 73.btb label 541 runs 41 steps, executes `0xb4` at 611
  and stops at 620 with `scene_switch_requested=21` (raw 9 -> LCX 21 = `iu`).
  VT label 497 shows `sound_start instruction=560 id=35 mode=0 text=2
  allowed=true`; EA label 389 shows `media_definition instruction=421 id=1
  value=50000 media_id=401 type=0` with one unavailable timestamp.
  EA save 26.btb label 602 now **completes** (return at 319) instead of
  stopping on `0xbb`.
- Census: completed **5,438 -> 5,702**, unsupported opcode **686 -> 289**;
  `0xd1` (164), `0x6a` (98), `0xb4` (31), `0xb5` (24), `0xbb` (22) and `0x73`
  (70) are gone from the blocking ranking (73 becomes the explicit
  `scene_switch`, 107 labels). unresolved variable 1,267 -> 1,278, RNG 223 ->
  238 (released labels reach the next blocker), `missing_object` 74,
  `step_limit` 5, `error` 0. New ranking: `0x8b` (63), `0x8a` (37), `0xbe`
  (28), `0xc8` (22), `0xcb` (20), `0xc6` (19), `0xd2` (19), `0x80` (17).
- Tests: scene-state covers the four-opcode media write, the sound-start gate
  (present/absent/disabled), visibility/channel/scroll, and the scene-switch
  target/no-entry/missing-table; records-test covers the LCX parser;
  save-test checks `dialogue_visible` restore.
- `make -C port -j2 all test` and the three panorama verifiers pass. Report:
  `analysis/rea/SOUND_MEDIA_STATE.md` (12 verified hashes).
- Non-goals kept: audio output, rendering, AVI/JPEG decode, save writing,
  cross-scene execution and any guess for `timeGetTime()`.

## Previous step — carried variable pool and dialogue-state restore

Restored the save's 3000-word `DAT_0047e2f0` through the original
`0x46ee00` mapping and the `0x408adc` dialogue window state. This is the
variable-pool step the previous brief asked for.

- `load_save_variables(SceneState&, const SaveGame&, const
  std::vector<std::uint16_t>&)`: `0x42c8a0` initialises 50 rows of 300
  **identity** words and `0x438173` overwrites the first `count` with LCV global
  ids. The port keeps `SceneState::variables` keyed by the raw local operand and
  fills all 300 entries with `node_values[local < declared ? declared[local] :
  local]`. It is a real projection, not a raw 3000-word copy: EA local 5 reads
  global 8 (`lastNode`), local 26 global 74 (`flowChk`), local 133 identity.
- `load_save` now also restores `0x408adc`: `field_45d1c4` channel,
  `field_45d1bc` scroll, the ten depths + 10x350 stacks (top-first, matching
  `0x4077b2`'s shift-up push), the current record (`-1` or an unallocated slot =
  none) and the `0x4089b0` record states. The state byte is the **second** byte
  of each saved pair; the first is only the `0xff` terminator. Record capacity
  comes from the file's own pair count (126 shipped), not a hard-coded constant.
  Stacked ids beyond that count and depths above 350 are rejected.
- `0x75` underflow is corrected: the original calls `0x40ac43`/`0x40ac38`, skips
  the write and continues. Restored values made this branch reachable in 11
  labels (EA/GC) that previously threw; now `subtraction_underflows` counts it
  and the trace continues. Census `error` 11 -> 0.
- CLI: `--engine <dir>` (SHOW.LCC/LCV/LVM) drives the variable projection for
  `--trace-save`, `--trace --save` and `--census --save`. The save's
  `DAT_00482428` scene is checked against the traced CC scene; mismatch is a
  hard error. `--save-dump` now validates the whole restore as well.
- Shipped data: all 77 CD1 save images restore (126 record bytes each, deepest
  channel 20/350, ids < 126, selector 0..7, scroll 0). Every one of the 18
  saved scenes has 0 unresolved variables.
- Real result: EA save 26.btb label 602 now runs 23 steps, loads
  `nodes/ea/ea00.jpg` and stops at the next unimplemented opcode `0xbb` (318);
  475 and 595 complete. The five `step_limit` labels stay step_limit: they are
  self-referential `0x70` returns (IU 459 object 5 -> label 375 -> its own start
  456; same pattern for IU 398, NB 240/242/243) and the original's `0x432d6e`
  waits for input, which is not modelled — not a missing variable.
- Census: completed **4,169 -> 5,438**, unresolved variable **2,939 -> 1,267**,
  RNG 25 -> 223 (released labels now reach `0x7d`; the CRT seed is not in the
  save), unsupported opcode 481 -> 686 (labels moved to the next blocker),
  `missing_object` 74, `step_limit` 5, `error` 0. All 1,267 unresolved labels
  are in the 26 scenes with **no shipped save**; all 18 saved scenes resolve
  every variable. New ranking: `0xd1` (164), `0x6a` (98), `0x73` (70), `0x8b`
  (63), `0x8a` (37), `0xb4` (31), `0xb5` (24), `0xbe` (24), `0xbb` (22).
- Tests: `save-test` covers the mapping (declared + identity), dialogue stack
  order/records/current-line clearing, and rejects bad depth/ids;
  `variable-metadata-test` checks EA 5->8, 26->74 and the identity tail with a
  synthetic 3000-word pool; `scene-state-test` locks the `0x75` counter.
- `make -C port -j2 all test` and the three panorama verifiers pass. Report:
  `analysis/rea/SAVE_VARIABLES.md` (17 verified hashes).
- Non-goals kept: rendering, audio/video, save writing, `engine\<ad>.ccb`,
  inventing values for scenes without a save, and the RNG seed.

## Previous step — save restore, `0xcc` and `0x70` CONVERSE

Restored the four save-carried tables and implemented the two object opcodes.
The carried variable pool is still deliberately not restored.

- `load_save(SceneState&, const SaveGame&)` in `port/src/cc.cpp` replaces
  `objects` (`DAT_00464300`), `media` (`DAT_00464f60`), `sounds`
  (`DAT_00486220`) and `nodes` (`DAT_004649e8`), matching `0x425ab4`'s free-then-
  read order. Object `+0x0a` and media `+0x12` are not copied: neither has a
  recovered writer and both are stale heap bytes in the shipped images.
  `media_objects` was replaced by the field-carrying `SceneMediaState`/`media`.
- `0xcc`: `objects[raw(0)]->+0x14 = raw(1)`. Both operands come from `0x436652`
  (encoded word), not the type-2 resolver `0x4319b3`. The original has no NULL
  check, so an absent slot is `missing_object` and stops; it is never allocated.
  The `short → uint` assignment sign-extends (`0x8000` → `0xffff8000`), tested.
- `0x70`: object `+0x0c` label → `resolve_control_flow` (the port's
  `DAT_004654e0`) replaces `pc`, recorded as `converse_jumps`. The nested
  runner `0x432d6e`, `DAT_0046e870`/`DAT_00485a10` and `DAT_00482ad0` are not
  rebuilt, so operand 0 stands in for the conversation result — the same lookup
  the original performs once the runner returns that object. Absent object →
  `missing_object`; absent label → `unresolved_object_label`.
- A 0x70 jump can return to its own label, so the 10,000-instruction bound is now
  `step_limit_exceeded` (5 shipped labels: IU 375/398, NB 240/242/243) instead
  of a thrown error. They need the dialogue/window state the original runner
  carries.
- CLI: `byzantine-cc <cc> --trace-save <save> <label> <cb>`, plus
  `--save <save>` for `--trace` and `--census`. `--census` now accepts the save
  and the Python census extracts `copy2hd/savegame/*.btb` from CD1 and picks one
  save per scene by `DAT_00482428` (18 scenes).
- Real acceptance: save 26.btb/EA label 1779 reaches
  `converse_jump instruction=2246 id=3 label=544 target=628`; save 73.btb/VT
  label 356 performs six `0xcc` writes (ids 19,18,13,5,1,2). `save-test` also
  covers table replacement, every restored field, jump/write/missing-object and
  the sign extension on a synthetic `SaveGame`; 26.btb was added to
  `analysis/rea/save_sample/`.
- Census: completed **4,045 → 4,169**, unsupported opcode **732 → 481**,
  `0xcc` 194 → 0, `0x70` 61 → 0; unresolved variable 2,891 → 2,939, RNG 25,
  `missing_object` 74, `unresolved_object_label` 0, `step_limit` 5, error 0.
  New ranking: `0xd1` (93), `0x6a` (74), `0x73` (49), `0x8b` (47), `0x8a` (25),
  `0xb4` (24), `0xb5` (24), `0xc8` (20).
- `make -C port -j2 all test` passes; the three panorama verifiers are unchanged
  by this step. Report: `analysis/rea/SAVE_RESTORE.md` (22 verified hashes).
- Non-goals kept: carried variable pool, dialogue depths/stacks, scene records,
  rendering, audio/video, save writing, `engine\<ad>.ccb`.

## Previous step — `savegame/NN.BTB` format and reader

Documented the save image and added a tested C++17 parser. No restore, no
`0xcc`, no `0x70`, no variable pool — this step only reads the file.

- Order of the 25 blocks comes from reader `0x425ab4` and is confirmed by the
  writer `0x424dc0`, which emits the same blocks in the same order, and by all
  77 CD1 images parsing with every block in bounds and their tail a valid
  notebook text region: `u32` CD number, chapter name, save
  name, 168×168 RGB16 thumbnail (`0x420888`/`0x4209d2`, `0xdc80` bytes),
  `u16` scene index, 0x15e per-scene records (`0x426261`/`0x425218`), 3000
  words `DAT_0047e2f0`, dialogue channel depths (10×u32 `DAT_00468cc0`) and
  stacks (10×350×u32 `DAT_00468d78`), current dialogue record, 0x4089b0
  flag block, three mid-block dwords + optional string + display mode +
  panorama id/x, 0x15e nodes (`0x426b9d`), 0x15e objects (`DAT_00464300`,
  0x18), sound table (0x3c) + chain (0x5c), 0x15e media records
  (`DAT_00464f60`, 0x14), current scene, 0x15e dialogue chains (0x68) and the
  notebook text tail.
- The first `u32` is the **disc** number, not a format version: `0x40c7b4` →
  `0x40c9ce`/`0x41d6df` checks/asks for the CD (`0x40c911` enumerates CD-ROM
  drives), and `0x424dc0` forces `1 → 2` before writing. Shipped spread
  2/3/4/5/6 = 22/15/13/15/12. The `strcmp(name,"new.can") == 0` case skips the
  check and does not store the value, so `new.can` loads without its disc.
- `0x408adc` recovered as the dialogue block: 3 selectors, 10 depths
  (`0x28 = 10×4`), 10×350 ids (`14000 = 10×0x578`), the current record index,
  then `0x4089b0`'s per-record flags. All 77 images carry exactly 126 flags,
  independently matching the 126 `DAT_00486f90` records from
  `DIALOGUE_RECORDS.md`; every stored dialogue id is < 126 and the deepest
  channel is 20 (limit 350).
- Object record layout from `0x42db22` case `0x72`: rect at `+0x00..+0x07`,
  `+0x08` active, `+0x09` kind, **`+0x0c` = CC label** (consumed by `0x70`
  through `DAT_004654e0`, built by `0x42d383`), `+0x10` name id, `+0x14` the
  value `0xcc` overwrites. `+0x0a` has **no writer** (un-zeroed `malloc(0x18)`),
  so saved bytes there are stale heap data; exposed as `uninitialized_0a`.
  Media `+0x12` is stale for the same reason (`0x6a` writes `+0x00/0x04/0x08/
  0x0c/0x10/0x11` only).
- Self-validating check: every `+0x0c` of 4,716 object records across the 77
  images is a label of the scene selected by the index, using
  `parse_chapter_scene_list` (0x438173 semantics: lowercase, max 8 chars) over
  `engine/SHOW.LCC` (44 names = the 44 `port/output/cc_all` scenes).
- Anomalies recorded for the next step: `0x4269ec` writes a node extra-list
  **count** while `0x426b9d` reads **control bytes** (assembly `0x426e28`);
  every shipped list is empty so it never fires. `0x425ab4` frees the four
  0x15e tables through `0x40f3f0` *before* reading, and only sets
  `_DAT_00485c70`/`DAT_004859cd` after a fully successful load.

New: `analysis/rea/SAVE_FORMAT.md` (block table, observation/inference/unknown
split, 102 verified export hashes), `port/include/byzantine/save.hpp`,
`port/src/save.cpp`, `port/tests/save.cpp` + `save-test`,
`analysis/rea/save_sample/{00.BTB,12.BTB,73.btb}`,
`byzantine-cc --save-dump [--lcc <SHOW.LCC>] [--cc <scene.cc>]`.
`make -C port -j2 all test` passes with the three fixtures and the three
panorama verifiers pass. The VM census is unchanged in this step (no opcode was
implemented): still 51/155 port branches, 4,045/7,693 labels completed, 732
opcode-blocked, 2,891 carried-variable, 25 RNG.

## Previous step — 0xb9 panorama/node loader

Implemented `0xb9` as an explicit load request instead of an unsupported stop.
Raw operand 0 indexes the CO.ALL record (`DAT_00483d90`) and the name builds
`nodes/<first 2>/<name>.jpg` (`"%.2s\\%s"` + `"nodes\\%s.jpg"`), operands 1/2 are
resolved through the variable table, and operand 3 is a transition id mapped by
`0x432133` (`1 -> 0x67`, `2 -> 0x65`, `3 -> 0x66`, else `100`). When operand 3 is
not a variable, `DAT_00460664` is consumed — the code `0xbd` modes 0/2/4 write,
now correctly described as a **transition code**, not a panorama id.
`ScenePanoramaLoad` carries record/name/path/horizontal/force/vertical/
transition, `pending_panorama_x` is consumed as `0x401ddf` does, and the
`DAT_0047e2f0` flag clear is counted (its table meaning is unrecovered). CO.ALL
comes from `byzantine-cc --co <record>`; without it the trace stops with
`unresolved_panorama_node`. Census: opcode-blocked 876 -> 732, unknown panorama
node 0, but completed only 4,038 -> 4,045 because the released labels now stop on
carried variable state (2,601 -> 2,891): `0xb9` resolves operands 1/2 through the
scene variable table. Opcodes with a port branch 51/155. New ranking: `0xcc`
(194), `0xd1` (93), `0x6a` (74), `0x70` (61), `0x73` (49), `0x8b` (47), `0x8a`
(25), `0xb4` (24). Real trace: EA label 254 under the new-game zero pool now runs
57 steps to 2081 on `unresolved_sound_name=27`, having loaded `ea02` at
`nodes/ea/ea02.jpg` with horizontal 747 and transition 100.
Report: `analysis/rea/PANORAMA_LOAD.md`.

## Previous step — 0xb2/0xb3 calls, 0xbd video, 0xbf/0xc0/0xc2 audio parameters

Recovered `0x412762`'s instruction-node layout (opcode, eight type bytes, operand
low/high words at `+10+4i`/`+12+4i`), which explains the dispatcher's
`*(char *)(*param_1 + 3)` / `+ 4` type tests. Implemented:

- `0xb2`/`0xb3`: the dialogue comparison as calls (`0x43194f` return address);
- `0xbd`: video command. Operand 0 indexes the per-scene `engine/<scene>cf`
  table (`DAT_00484330`), operand 1 is the mode (0 slide, 1 intro, 2 site,
  3 transit, 4 char, 5 anim), operands 2/3 are read only for type `0x40`.
  Primary path `avi/<name>.avi`, fallback `video\<mode>\<name>.avi`. Modes 2/3
  only run when `gamepref[5]`/`gamepref[4]` are nonzero (`0x41d440`, loaded by
  `0x41f753` from `savegame/gamepref_%03d`). Modes 0/2/4 also set
  `DAT_00460664` (`0x65`/`0x6a`/`0x67`), the id `0xb9` consumes;
- `0xbf`/`0xc0`/`0xc2`: `DAT_00486220` audio parameters `+0x1c/+0x20/+0x24/
  +0x28/+0x2c` from resolved operands (operand 3 only for type 2), with the
  `0x427d1a` refresh counted instead of played.

`0xcc` (now the top blocker, 194) is **not** a script-only command: it writes
`DAT_00464300[id]->+0x14`, and that 0x15e-entry object table is allocated only by
the save loader `0x425ab4`. It belongs to the save/load step. Data: 347 video
commands (mode 5: 110, 3: 89, 4: 38, 0: 30, 2: 24, 1: 2); `GAMEPREF.000` is
`1 1 1 1 1 1 0`, so the shipped defaults enable both gated modes. Census:
completed 3,899 -> 4,038, opcode-blocked 1,168 -> 876, unknown-variable
2,601 -> 2,754, RNG 25, unknown video/dialogue tables 0; opcodes with a port
branch 50/155. New ranking: `0xcc` (194), `0xb9` (144), `0xd1` (93), `0x6a`
(74), `0x70` (61), `0x73` (49), `0x8b` (47), `0x8a` (25).
Report: `analysis/rea/VIDEO_AND_AUDIO.md`.

## Previous step — dialogue records (0xab/0xb0/0xb1/0xb6)

Recovered the whole dialogue model from the saved exports: `0x408662` allocates
one `DAT_00486f90` record per global `engine/SHOW.LCI` name with `+0x00 = 0`
(126 records on CD1); `0x408bb2`/`0x408c81` map a per-scene `CI`/`CY` name id to
that list and to the 8-entry `engine/SHOW.LCY` channel list. A data check
confirmed all 366 CI names of the 44 scenes are in SHOW.LCI and every CY name is
in SHOW.LCY. Implemented `SceneState::dialogue_records`, the ten channel stacks,
`0xab` (mode 1 start / 0 or 2 stop plus the trailing mode-byte write), `0xb0`/
`0xb1` as real branches, and `0xb6` (start + `0x407ad5` promote). The stack push
depends on `0x407254`'s `inventor/<name>.tga` + `inventor/hotitems/<h-name>.tga`
load, so the media name lists are explicit inputs; without them the trace stops
with `unresolved_dialogue_inventory`. `byzantine-cc` gained `--lci/--ci/--lcy/
--cy/--inventor/--hotitems`. Census: completed 3,510 -> 3,899, opcode-blocked
1,754 -> 1,168, unknown-variable 2,404 -> 2,601, unknown dialogue table 0;
opcodes with a port branch 44/155. New ranking: `0xcc` (188), `0xbd` (171),
`0xb9` (108), `0xbf` (105), `0xc0` (92), `0xd1` (91), `0xb2` (52), `0x70` (47).
Report: `analysis/rea/DIALOGUE_RECORDS.md`.

## Previous step — 0x6c media objects

`0x6c` clears `DAT_00464f60[id]+0x10`; a missing object records the original
`0x40abb4` error path (`missing_media_requests`) instead of writing. Added
`SceneState::media_objects` and a test for the write and the missing-object
path; the EA label-464 test now expects both branches to complete. Census:
completed 3,510, opcode-blocked 1,754, unknown-variable 2,404, RNG 16;
opcodes with a port branch 40/155. New ranking: `0xab` (313), `0xcc` (178),
`0xb0` (154), `0xb1` (141), `0xbd` (121), `0xbf` (104), `0xb9` (94), `0xb6`
(91).

## Previous step — sound objects (0x65/0xc4/0xbc) and 0xc1

`0xc1` writes `DAT_00486220[id]+0x30` from a resolved operand and maps the
`3000` sentinel to `-1` (initial value from `0x65`). Added `field30` to
`SceneSoundState` and tests for the value and sentinel paths. The EA label-464
test now expects the skipped path to complete instead of stopping at `0xc1`.
Census: completed 3,439, opcode-blocked 1,909, unknown-variable 2,329, RNG 16;
opcodes with a port branch 39/155. New ranking: `0xab` (310), `0x6c` (236),
`0xcc` (178), `0xb0` (153), `0xb1` (140), `0xbd` (110), `0xb6` (91), `0xb9`
(91).

## Previous step — sound objects

Implemented `SceneState::sounds` from `0x65` (`+0x04/0x08/0x0c/0x10/0x14/0x18`,
resolved operands 1/2, raw 3/4/5/6), `0xc4` (activate type-2 operands, `endgame`
name flag, `unresolved_sound_name` when the per-scene CS entry is absent) and
`0xbc` (deactivate). `byzantine-cc --census <cb> [<scene-cs-entry>]` accepts the
CS entry; `build_vm_census.py` extracts `CS.ALL` from the CD1 ISO, splits it
with `byzantine-all` and supplies each scene. Tests cover definition fields,
activation+endgame, the missing-table stop and deactivation. Census: completed
3,149 -> 3,418, opcode-blocked 2,431 -> 2,044; new top blockers `0xab` (310),
`0x6c` (235), `0xcc` (178), `0xc1` (154), `0xb0` (153), `0xb1` (140). Report:
`analysis/rea/SOUND_TABLE.md`.

## Previous step — per-scene engine tables and the sound table

Recovered the eleven per-id engine tables loaded by `0x412f77`/`0x411dfc`:
`0x412d35` builds `engine/<2-char prefix><suffix>` names from
`ca,cb,cd,ch,cm,cs,ct,cv,co,ci,cy,cf,ce,ccs`; each file is a sequence of
`<name>\0<source>\0<type byte>` records. `0x47a8e0` is the **sound table**:
`CS.ALL` (CD1) holds one entry per scene and `VH` contains a record named
`endgame`, which is exactly the string `0x42db22` case `0xc4` compares against.
Record order is the script id. Added `port/src/records.cpp` +
`port/include/byzantine/records.hpp` (`parse_named_records`, `0x15e` limit) with
`records-test`. Report: `analysis/rea/SOUND_TABLE.md`.
Next: implement `0x65`/`0xc4`/`0xbc` on an explicit `SceneState::sounds` map with
the CS entry supplied by the caller.


Goal: reconstruct a portable Windows/macOS/Debian engine from the Turkish
game media. Original C++ source/PDB is absent; `port/` is a reconstruction.
Debian Make builds are tested; other platforms and full gameplay remain unverified.

## Progress assessment (static, 26 October 2026)

Three different numbers, deliberately not merged:

- **Extraction:** 1,237 / 1,397 recovered functions have pseudocode (the other
  160 are imports); 325,382 body bytes, 249,250 of them game code (840
  functions at `0x401000`–`0x43ffff`). Complete for what the saved profile
  recovered.
- **Port behaviour:** **127 of 840** game functions have a documented `port/`
  counterpart, i.e. **15.1% of functions / 30.9% of game-code bytes**
  (76,910 / 249,250 B) at `partial`/`bounded` scope — not runtime equivalence.
  The count is mechanical: it only reflects which function addresses are
  mentioned under `port/`, so it moves when a comment moves; the last step added
  the panorama inverse mapping/view functions.
  The audit is mechanical (every `0x4xxxxx` address mentioned under `port/`
  matched against the 840 function starts); run
  `python3 analysis/rea/audit_port_coverage.py`. The 713 remaining functions
  split into 74 that call GDI/DirectDraw/DirectSound/AVI/mmio/dialog APIs
  (24,312 B) and 639 others (148,028 B), including the MFC/dialog/editor mass
  the EXE links in.
  Bounded counterparts added by the late steps: `0x425ab4`/`0x424dc0`/`0x426261`/
  `0x425218`/`0x426b9d`/`0x42722b`/`0x408adc`/`0x4089b0`/`0x408dbd`/`0x420888`/
  `0x4209d2`/`0x438173` (save read), `0xcc`/`0x70` support (`0x436652`,
  `0x42d383`), `0x431a69`/`0x40c7b4` (CD request), `0x43299d`/`0x432904`
  (record write), `0x427a01`/`0x4278ab` (sound release), `0x431b0f`/`0x40fd7b`/
  `0x40fe70` (record creation), `0x410078`/`0x40cb26`/`0x40bbfd`/`0x40bbb0`/
  `0x4164a0`/`0x40cf73` (animation), `0x43b216`/`0x43b130`/`0x43c114`/
  `0x40241c`/`0x42b679` (composition), `0x401d29`/`0x41045b`/`0x40f135`/
  `0x40f080`/`0x42b280` (JPEG + background/panorama layers) and
  `0x4240ae`/`0x401ca6`/`0x40c02c`/`0x402285`/`0x401256`/`0x42423d`/`0x4241e2`
  (panorama view, inverse click mapping and scripted rotation), with libjpeg 6a's
  `0x43de70`/`0x43e900`/`0x43e7b0`/`0x43dfc0`/`0x43e3d0`/`0x43e560`/`0x43e320`/
  `0x43df40` entry points documented. `0x428cbd`/`0x431ec2` are only requested.
- **VM coverage:** 77 of 155 recovered opcodes have a branch, and those 77 are
  **exactly the opcodes the shipped scripts use** (77 distinct opcodes in 39,356
  instructions). 5,935 of 7,693 scene entry labels reach a supported end
  (`return` 5,913 + `script_end` 22) from a fresh `SceneState` plus the scene's
  shipped save tables, `engine/<scene>ca` asset table and variable pool when a
  save exists. **No label stops on an unsupported opcode.** 1,296 stop on
  carried variable state, 245 on an unknown RNG seed, 80 on a `missing_object`,
  2 on a `missing_dialogue_chain`, 2 on a `missing_media_record`, 9 on a
  `disc_request` (host CD state), 119 on an explicit `scene_switch` and 5 on the
  instruction bound after a 0x70 loop. No label stops on a node asset: 733
  records are created from the CA table with real frame data and 1,925 record
  commands are provably skipped (`0x431b0f` returns 1 for a type outside 0..3).
  **All unresolved-variable labels are in the scenes without a shipped CD1
  save; the 18 saved scenes resolve every variable.**

## Previous step — actor records (0x64/0x85/0x95/0x84/0x91) and text (0xcd)

Recovered the record initializer `0x40ffee` (y=0x14, flags=0x20, +0x04=10,
frame list at +0x18/+0x1c) and the two byte tables at `0x460418`/`0x460428`
(byte-indexed, objsize confirms `mov 0x460418(%edx),%al`). Added
`SceneState::actors` and implemented:

- `0x64` kind/+0x04/x/y update with the `kind != 0x20 || !(flags & 0x20)` guard;
- `0x85` mask-and/or over type-2 operands 1..7 (index >= 16 rejected);
- `0x95` kind write; `0x84` deduplicated `+0x3c` trail append + x/y write with
  the +0x14 y bias; `0x91` frame restart/advance requests (frame count is asset
  state and is not invented).
- `0xcd` records a text-draw request (0x417a17) instead of dropping it.

The animation loader's per-frame x/y offsets and frame count are documented as
not modelled. Tests cover all six commands, the guard no-op, trail dedupe, mask
rejection and the frame requests. Census: completed 2,357 -> 3,149,
opcode-blocked 3,522 -> 2,431, unknown-variable 1,799 -> 2,098, opcodes with a
port branch 29 -> 35. Report: `analysis/rea/ANIMATION_RECORD.md`.

## Previous step — VM opcode census and RANDOM (0x7d)

Added `byzantine-cc --census <cb>` and `analysis/rea/build_vm_census.py`: every
label of all 44 extracted scenes is traced from a fresh `SceneState` (static
coverage, not runtime). Baseline 7,693 labels: 2,357 completed, 1,730 blocked by
carried variable state, 3,606 by an unsupported opcode. The ranking is now in
`reconstruction/vm-census.md`; top blockers are `0x64` (503), `0x85` (411),
`0xcd` (268), `0xc4` (237), `0xbc` (235), `0xab` (202), `0x6c` (193).

First command implemented from that ranking: **0x7d RANDOM**. `0x431e89` calls
the CRT `_rand` (`0x44d070`: `state*0x343fd+0x269ec3`, result
`(state&0x7fff0000)>>16`) only for a nonzero range, then stores
`rand() % range`. Seeds are real state (`0x415a4e` → `0xbeef`; `0x42a6aa` →
local time), so `SceneState::random_state` is explicit and unset by default;
without it the trace stops with `unresolved_random` instead of inventing a
stream. Range operand: type 2 → scene variable, type 0x40 → constant, else
0xffff; zero range stores 0 without advancing. Tests lock the `0xbeef`
sequence (84, 31, 11, 57). Census after: 1,799 unresolved variable, 15
unresolved RNG, 3,522 unsupported opcode. Report: `analysis/rea/VM_RANDOM.md`.
`make -C port -j2 all test` and the three panorama verifiers pass.

## Previous step — full source corpus and VM opcode map

`analysis/rea/build_source_corpus.py` generated `analysis/rea/reconstruction/`
from the saved exports: 1,237 pseudocode bodies (every export hash re-checked),
1,237 per-function files with address/range/call/coverage headers, a complete
249,250-byte game-code inventory (840 functions, `0x401000-0x43ffff`) next to
397 static-CRT functions (`0x440000+`), 3,406 call edges, 206 roots, a call-graph
cluster map, the imported API list by DLL, and a 40-function roadmap of the
largest unimplemented bodies. `build_vm_command_map.py` recovered the two
opcode dispatchers (`0x43340a`: 99 cases, `0x42db22`: 108 cases; 155 opcodes).
39 mnemonics were recovered from the strings each debug-trace branch passes to
its logger (GOTO, REPLY, HOOK, THOOK, ON, OFF, SET, IFGOTO, INVGOTO, CURSGOTO,
BACKGRND, NODE, CONVERSE, …). The portable VM handles 28 of 155; 127 remain.
Unnamed cases stay `—` instead of being guessed. Report:
`analysis/rea/SOURCE_CORPUS.md`.

## Previous step — load-time projection state (0x4236b5)

Recovered `0x4236b5` from saved exports plus focused objdump: decoded dimensions
are compared with `+0x24`/`+0x20`; only on a change are scale `+0x28` =
binary64(width/6.28318530718), padding `+0x38` = extent+1 and stride `+0x1c`
= width+2*padding recomputed and the buffer reallocated. The row copies and
seam refresh run for every successful load, and `0x4241e2` (camera reset +
map rebuild) is called **unconditionally** afterwards, so a same-size reload
still resets yaw to pi. Implemented as `PanoramaProjection::set_source`
(returns true iff geometry was recomputed) plus `padding()`/`stride_words()`
and `PanoramaGeometry::stride_words`. `0x423cdf` uses renderer `+0x4`/`+0x8`
as the viewport, so padding is size/viewport-only. Tests cover 2496x768 ->
P=304/stride 3104, same-size reset, 2496x480 -> P=270, and rejected width=0.
Report: `analysis/rea/PANORAMA_LOAD_STATE.md`. No allocation/buffer ownership
modelled; `0xb9`/`0xbb` still unsupported. All Make tests and the three
panorama verifiers pass.

## Previous step — panorama precision fix (test suite green again)

The earlier `projection reset camera` failure was a real port defect, not a
stale test: `panorama_yaw_offset` widened `S*yaw` to `long double`, but
`0x4245a5..0x4245b1` is `fld QWORD/+0x28`, `fmul QWORD/+0x50`, `__ftol` under
the startup PC53 control word. `64/(2*pi)*pi` is 32 under PC53 and
31.9999999999999992 under PC64, so the widening returned 31 instead of 32.
Fixed with `rounded_mul`. The same PC53 model implies `-180` degrees lands one
ulp BELOW `-pi`, so `advance()`'s strict `< -pi` wrap moves it above `+pi`;
the test now asserts those exact bits instead of an unreachable exact `-pi`.
`make -C port -j2 all test` passes; all three panorama verifiers pass.
Report: `analysis/rea/PANORAMA_PRECISION_FIX.md`.
No binary session opened; no cleanup pending. Next bounded step unchanged:
projection reset/load-time callers and an explicit state/cache helper.

## Reuse before analyzing again

- Target `analysis/rea/BETRAYAL.EXE`, SHA-256
  `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
- Saved REA/Ghidra profile:
  `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.
- `analysis/rea/pseudocode/functions/<address>.c`: 1,237 saved functions.
  Select by address/`rg`; avoid loading the entire 2.4 MB index into chat.
  Parse the index locally to verify selected file hashes.
- REA exports: `analysis/rea/mcp-session.json`, `session.json` and the
  decompile journal/index. `rea.sh` supplies the CLI environment if needed.
- Library/dependency strategy (what is hand-written, what is linked, what runs
  as an external tool, and what to adopt next): `analysis/rea/LIBRARY_STRATEGY.md`.
- Previous pass recorded MCP session closed, no cleanup pending. This pass
  opened no binary session; existing saved REA evidence sufficed.

## Current implementation

`port/` reads RLE, ALL, CC, CB and variable metadata, decodes the shipped
`.RLE` animation frames and the shipped JPEG backgrounds/panoramas, and composes
them into an RGB16 surface; it includes an SDL2 scene player and a strict partial
VM. `port/README.md` lists supported commands.
Global variable metadata: 44 scenes, 1,622 mappings, 1,049 names. EA local 5
= global 8 `lastNode`; local 26 = global 74 `flowChk`.
Normal traces preserve unknowns. `--trace-new-game` explicitly seeds declared
variables from the zero pool but does NOT execute startup or restore a save.

Panorama helper status: recovered the normal `0x4242bd` RGB16 sampling loop and added
standalone `sample_panorama_rgb16`: explicit pre-yaw map, signed yaw word offset,
positive even destination byte pitch, pixel rectangle and effect argument.
Copies full words; validates final yaw addresses and all extents before writing.
Rejects nonzero effects, odd/negative pitch, aliasing and unsupported signed
32-bit offset arithmetic. Mapping regenerates iff vertical delta !=0 or zoom
factor !=1; clamp equality sets +0x68, neutral inputs leave it untouched. Rules
are documented, not yet a state/cache helper. Six synthetic full-viewport draws
(1,843,200 pixels) and hand-checked rectangle/yaw/rejection cases pass.
Target/profile, 20 exports, 18 assembly ranges, seven data ranges verified;
`make -C port -j2 all test` passes. Report: `analysis/rea/PANORAMA_SAMPLING.md`.
No original execution, loader integration or binary session.

Previous step: recovered `0x423a37` mapping entries and implemented standalone
`panorama_mapping` with explicit geometry/pitch/zoom. It returns signed RGB16
word offsets into the padded source BEFORE yaw. Separate binary64 operation
stores model normal-range PC53 basic arithmetic; host trig/tables remain
approximations. Requires nearest host rounding; rejects invalid depth,
unwritten table cells, source bounds and unsupported 32-bit arithmetic.
Small hand-checked maps, odd dimensions, zoom/pitch direction, rejection paths
and 921,600 synthetic full-viewport entries pass. No original runtime capture
or loader integration. Report: `analysis/rea/PANORAMA_MAPPING.md`.
Projection (20 exports/16 assembly/7 data ranges) and camera verifiers pass;
`make -C port -j2 all test` passes. No binary session opened.

Previous step: recovered CRT startup precision (PC53, rounding mode preserved),
lookup-table allocation/initialized bounds and conditional bias quantization.
Standalone `PanoramaLookupTables` exposes only initialized cosine (-1569..1569)
and atan (-1999..1999) cells; unwritten allocated cells are explicitly rejected.
`panorama_table_coordinate(double)` supplies the bounded nearest/even model.
A controlled local x87 probe passes 140,007 samples and demonstrates why PC64
can differ: 0.5001 => 0 with PC64, 1 with PC53. This is NOT original execution.
Host CRT/libm values and effective runtime rounding remain unverified.
Focused report: `analysis/rea/PANORAMA_TABLES.md`; probe:
`analysis/rea/probe_panorama_bias.cpp`. Expanded projection verifier checks
20 exports, 16 assembly ranges and seven data ranges. All Make tests pass.

Previous step: recovered the missing x87 angle getters, yaw pixel conversion,
pitch-limit and seam-padding formulas using saved exports plus focused objdump.
`panorama_geometry`, `panorama_pitch_limit`, `panorama_yaw_offset` and
`panorama_angle_degrees` are standalone C++17 helpers. Explicit synthetic
2496x768 source / 640x480 viewport => padding 304; no game viewport assumed.
Host libm/long-double behavior is NOT claimed pixel-exact. Bias conversion
constant is exactly 0x4338000000000100; table conversion was recovered in the
subsequent lookup-table step above.
`python3 analysis/rea/verify_panorama_projection.py` verifies target/profile,
12 exports, 10 retained assembly ranges and five PE data ranges. Focused report:
`analysis/rea/PANORAMA_PROJECTION.md`. All Make tests pass.

Previous camera/seam step: standalone `port/src/panorama.cpp` now reconstructs known staged
camera consumption (0x401ddf) and RGB16 seam rows (0x4236b5/0x42382b).
For EA width=2496, x=747 => 108 degrees; x=2495 => 360. Explicit x=-1
means no request; VM unknown optional state is NOT converted to this sentinel.
Alternate-mode vertical delta is -6, subtracted by the projection renderer.
Helpers are not wired into the unsupported loader. New panorama-test passes.
Target/profile and 17 saved export hashes verified with
`python3 analysis/rea/verify_panorama_camera.py`; focused report:
`analysis/rea/PANORAMA_CAMERA.md`, source manifest `panorama-camera-evidence.json`.

Previous step: `0xba` stages raw unsigned horizontal position clamped to 2495,
stored in `SceneState::pending_panorama_x`. Static `--panoramas <CO record>`
joins `0xb9` IDs to CO.ALL entries and `nodes/<first2>/<name>.jpg`.
44 scenes / 182 load commands: 165 match JPEG paths on six ISOs; 17 MZ2 names
unmatched. EA return instruction 317 => ea00, instruction 2074 => ea02.
EA00.jpg is 2496×768. Loader `0xb9` remains explicitly unsupported.

## Verified commands

```sh
make -C port -j2 all test
./port/anims-test <extracted anims tree>      # 922 assets, 2846 frames decoded
./port/anims-test <tree> --render maphl 0     # 99x58 covered=3373/5742
./port/jpeg-test analysis/rea/jpeg_sample/BCLINTEL.jpg analysis/rea/jpeg_sample/vc05.jpg
# scene player (extracts once, then opens a window)
./port/scene.sh "/media/.../Byzantine - The Betrayal" MAP 87 tempsave.btb
./port/scene.sh "/media/.../Byzantine - The Betrayal" VT 497 72.btb   # audio: waits 4.7 s
# headless checks of the same paths
SDL_VIDEODRIVER=dummy ./port/scene.sh "<game>" MAP 87 tempsave.btb --click 34 --shot /tmp/map.ppm
SDL_VIDEODRIVER=dummy ./port/scene.sh "<game>" VT 497 72.btb --print-audio
find <extracted> -iname '*.jpg' -print0 | xargs -0 ./port/jpeg-test --scan | tail -1
                                              # jpeg scan: 283 files decoded
python3 analysis/rea/build_source_corpus.py
python3 analysis/rea/build_vm_command_map.py
python3 analysis/rea/build_vm_census.py
port/byzantine-cc port/output/cc_all/EA --census port/output/cb_all/EA
python3 analysis/rea/audit_port_coverage.py
python3 analysis/rea/verify_panorama_bindings.py
python3 analysis/rea/verify_panorama_camera.py
python3 analysis/rea/verify_panorama_projection.py
port/byzantine-cc port/output/cc_all/EA --panoramas port/output/co_all/EA
port/byzantine-cc port/output/cc_all/EA --trace-new-game 254 port/output/cb_all/EA analysis/rea/engine_sample/copy2hd/engine
port/byzantine-cc analysis/rea/save_sample/00.BTB --save-dump --lcc analysis/rea/engine_sample/copy2hd/engine/SHOW.LCC
port/byzantine-cc analysis/rea/save_sample/00.BTB --save-dump --lcc analysis/rea/engine_sample/copy2hd/engine/SHOW.LCC --cc port/output/cc_all/MAP
port/byzantine-cc port/output/cc_all/EA --trace-save analysis/rea/save_sample/26.btb 1779 port/output/cb_all/EA
port/byzantine-cc port/output/cc_all/VT --trace-save analysis/rea/save_sample/73.btb 356 port/output/cb_all/VT
port/byzantine-cc port/output/cc_all/EA --trace-save analysis/rea/save_sample/26.btb 602 port/output/cb_all/EA \
  --engine <engine-copy2hd/engine> --co port/output/co_all/EA --ci <CI/EA> --cy <CY/EA> \
  --lci <SHOW.LCI> --lcy <SHOW.LCY> --cf <CF/EA> --gameprefs <GAMEPREF.000> \
  --inventor <names> --hotitems <names>
port/byzantine-cc port/output/cc_all/VT --trace-save analysis/rea/save_sample/73.btb 541 port/output/cb_all/VT \
  --engine <engine-copy2hd/engine> --lcx <engine-copy2hd/engine/SHOW.LCX> \
  --co port/output/co_all/VT --ci <CI/VT> --cy <CY/VT> --lci <SHOW.LCI> --lcy <SHOW.LCY> \
  --cf <CF/VT> --gameprefs <GAMEPREF.000> --inventor <names> --hotitems <names>
port/byzantine-cc port/output/cc_all/NB --trace-save <CD1:copy2hd/savegame/57.btb> 556 port/output/cb_all/NB \
  --engine <engine-copy2hd/engine> --lcx <engine-copy2hd/engine/SHOW.LCX> \
  --ci <CI/NB> --cy <CY/NB> --lci <SHOW.LCI> --lcy <SHOW.LCY> --co port/output/co_all/NB \
  --ca <CA/NB> --cf <CF/NB> --gameprefs <GAMEPREF.000> --inventor <names> --hotitems <names>
# steps=9 stopped_at=691 unresolved_sound_name=11 records_created=1 skipped_node_commands=2
# node_write instruction=699 id=10 value04=5 flags=4   (save has nodes=0; CA[9]=0xff, CA[10]=1)
port/byzantine-cc port/output/cc_all/NB --trace-save <CD1:copy2hd/savegame/57.btb> 202 port/output/cb_all/NB \
  --engine <engine-copy2hd/engine> --lcx <engine-copy2hd/engine/SHOW.LCX> \
  --ci <CI/NB> --cy <CY/NB> --lci <SHOW.LCI> --lcy <SHOW.LCY> --co port/output/co_all/NB \
  --ca <CA/NB> --cf <CF/NB> --gameprefs <GAMEPREF.000> --inventor <names> --hotitems <names>
port/byzantine-cc port/output/cc_all/AM --trace-save <CD1:copy2hd/savegame/66.btb> 1010 port/output/cb_all/AM \
  --engine <engine-copy2hd/engine> --lcx <SHOW.LCX> --lci <SHOW.LCI> --lcy <SHOW.LCY> \
  --co port/output/co_all/AM --ci <CI/AM> --cy <CY/AM> --cf <CF/AM> \
  --gameprefs <GAMEPREF.000> --inventor <names> --hotitems <names>
# steps=2 stopped_at=1249 script_end_requested=true  (0x79 at 1249)
port/byzantine-cc port/output/cc_all/HSTMED --trace-save <CD1:copy2hd/savegame/55.btb> 259 port/output/cb_all/HSTMED \
  ...same tables...
# node_write instruction=279 id=48 value04=1 flags=36  (0x86)
# record id=48 frames=8 current=4 x=0 y=20 frames_known=false  (from the save)
port/byzantine-cc port/output/cc_all/VT --trace-save <CD1:copy2hd/savegame/73.btb> 1638 port/output/cb_all/VT \
  --engine <engine-copy2hd/engine> --lcx <SHOW.LCX> --lci <SHOW.LCI> --lcy <SHOW.LCY> \
  --co port/output/co_all/VT --ci <CI/VT> --cy <CY/VT> --cf <CF/VT> --ca <CA/VT> \
  --anims <anims root> --gameprefs <GAMEPREF.000> --inventor <names> --hotitems <names>
# records_created=2 frame_overflow_logs=1
# record id=24 frames=6 current=5 x=0 y=20 frames_known=true
# record_frame id=24 index=5 size=640x400 covered=16023/256000
# record id=26 frames=5 current=4 x=0 y=20 frames_known=true
port/byzantine-cc port/output/cc_all/MAP --trace-save <CD1:copy2hd/savegame/tempsave.btb> 87 port/output/cb_all/MAP \
  --engine <engine-copy2hd/engine> --lcx <SHOW.LCX> --lci <SHOW.LCI> --lcy <SHOW.LCY> \
  --co port/output/co_all/MAP --ci <CI/MAP> --cy <CY/MAP> --cf <CF/MAP> --ca <CA/MAP> \
  --anims <anims root> --gameprefs <GAMEPREF.000> --inventor <names> --hotitems <names> \
  --compose-out /tmp/map87.ppm --background <backs/ma/MAPFINM1.jpg>
# background=... size=640x400 written=256000 clipped=0
# composed size=640x400 drawn=27 skipped_no_frame=0 skipped_other_kind=0 pixels=163079
# draw id=33 kind=0 x=284 y=158 written=7272 clipped=0
# ... draw id=0 kind=0 x=143 y=43 written=3373 clipped=0
port/byzantine-cc port/output/cc_all/MAP --trace-save <CD1:copy2hd/savegame/tempsave.btb> 87 port/output/cb_all/MAP \
  ...same tables... --compose-out /tmp/pan.ppm --panorama <nodes/vc/vc05.jpg> --panorama-x 700
# panorama=... size=2496x768 padding=313 yaw_degrees=101 pitch_degrees=0 sampled=256000
```

`--trace-save` also needs the dialogue/video/panorama tables; see the acceptance
commands in `analysis/rea/SAVE_RESTORE.md` §5, `SAVE_VARIABLES.md` §6,
`SOUND_MEDIA_STATE.md` §6 and `RECORD_TABLE.md` §9. `--ca <engine/<scene>ca>`
supplies the animation asset table: without it an absent record stops as
`unresolved_node_asset`; with it the `0x431b0f` decision is exact. Real results: EA 1779 makes one `converse_jump`
(id 3 → label 544) at instruction 2246; VT 356 makes six `object_write`s and
returns.

With the save-backed variable pool and the new opcodes, EA 602 runs 24 steps and
**returns at 319** (the `0xbb` at 318 now selects channel 0 instead of stopping);
EA 475 and 595 complete. VT 541 runs 41 steps, executes `0xb4` at 611 and stops
at 620 with `scene_switch_requested=21` (raw 9 → LCX 21 = `iu`).

Chain/system evidence: NB 57.btb label 202 creates `dialogue_chain id=0
count=1 ids=20` (the save's chain 2 stays `count=2 ids=43,123`); VT 73.btb label
1818 runs `system_command instruction=2272 sub=0x36 status_write_requested=54`.

Final-opcode evidence: AM 66.btb 1010 → `script_end` at 1249; AM 1183 →
`sound_session instruction=1484 id=24 allowed=true timeout0=15000/18
timeout1=18000/19`; HSTMED 55.btb 259 → `node_write instruction=279 id=48
value04=1 flags=36`; MAP `tempsave.btb` 134 → `disc_request=1`; VC 60.btb 133 →
`sound_release id=1 record=true field14=1`; CLOCK 71.btb 504 →
`sound_stop_all_requests=1`; EA 26.btb 757 → `missing_media_record=5`.
The census now ends every label at a named state boundary
(`blocking_total: 0`, `return` 5,829 + `script_end` 22 = 5,851 completed).

Dialogue trace (tables extracted per run by `build_vm_census.py`):

```sh
port/byzantine-cc port/output/cc_all/EA --trace 458 port/output/cb_all/EA \
  --ci <CI.ALL/EA> --cy <CY.ALL/EA> --lci <SHOW.LCI> --lcy <SHOW.LCY> \
  --inventor <names> --hotitems <names> --ca <CA.ALL/EA>
# steps=6 stopped_at=516 records_created=2; dialogue_id=28 state=1
# inventory_loaded=true; dialogue_channel=0 stack=28; dialogue_refresh_requests=1
# (without --ca the 0x85 at 511 stops with unresolved_node_asset=1)
```

Panorama loader trace (staged `0xba` position consumed by `0xb9`):

```sh
port/byzantine-cc port/output/cc_all/EA --trace-new-game 254 port/output/cb_all/EA \
  analysis/rea/engine_sample/copy2hd/engine --co port/output/co_all/EA
# steps=57 stopped_at=2081 unresolved_sound_name=27
# panorama_record=3 name=ea02 jpeg=nodes/ea/ea02.jpg transition=100 horizontal=747
```

Video/audio trace (CF.ALL entry and the shipped `GAMEPREF.000`):

```sh
port/byzantine-cc port/output/cc_all/EXAMINE --trace 288 port/output/cb_all/EXAMINE \
  --cf <CF.ALL/EXAMINE> --gameprefs <goldsave/GAMEPREF.000> ...
# steps=3 stopped_at=298, background "examine",
# video_record=0 name=exCam mode=0 panorama_id=101
```

## Next bounded step

The dispatcher, the record table, the animation container, the frame pixels, the
record composition and the background/panorama image layers are closed. What
remains is the draw-pass details, transitions and the interaction layer.

1. ~~JPEG decode (`0x401d29`) + background/panorama layers.~~ **Done**
   (`JPEG_DECODE.md`); the port decodes all 283 shipped JPEGs and composes a
   real background, and `--panorama` samples a 2496x768 node image.
2. **The rest of the record draw pass**: the `+0x09 & 0x20` "snap" pass (draw
   through the `+0x20` rect, then zero `+0x20`/`+0x22` and clear `+0x29`), the
   `+0x09 & 0x80` destination-read blend (`0x42b5b2`) and `0x43b130`'s mode-1
   head→current frame sequence. The saves carry `+0x20`/`+0x22`/`+0x29`, so this
   is verifiable without inventing state.
3. **Transitions and blends** (`0x418bcf`, `0x42bce1`, `0x42be07`, `0x419cf6`,
   `0x418eb2`, `0x419a51`) that `0xbd`/`0xb9`/`0xd3`/`0x73` request — two decoded
   images plus a progress argument; the port currently renders neither.
4. **Carried scene state (1,296 labels).** All of it is in the 26 scenes with no
   shipped CD1 save; `0x439349` (transition install from the `0x42d915`/
   `0x43c566` payload) is the other real carrier and is not modelled. This is a
   session runner, not a single-scene trace.
5. **RNG seed (245).** `0x415a4e` seeds `_rand` with `0xbeef`, `0x42a6aa` with
   local time, `0xc8` sub `0x1e` reseeds from the same.
6. **Request-only subsystems**: audio `0x427d1a`/DirectSound sessions, AVI
   `0x40d3af`, the text/message renderer (`0x417a17` + the 0x407xxx window),
   inventory/hotspot UI. `0x8c`/`0x8e` stay unimplemented: no shipped label
   reaches them.

### Next step brief (self-contained — "devam et" is enough)

Goal: finish the record draw pass (item 2) so every draw the original performs is
modelled.

- Reuse, do not re-derive: `SCENE_COMPOSE.md` (draw loop `0x43b216`, `0x43c114`
  rect, `0x40241c` clip, `0x42b679` row copy, `compose_scene`, `--compose-out`)
  and its §9 playfield-origin correction, `JPEG_DECODE.md` §5-§7 (background and
  panorama composition), `ANIMATION_RECORD.md` (`+0x09` flag bits, `0x84`/`0x85`/
  `0x86` writers), `SAVE_FORMAT.md` (`+0x20`/`+0x22`/`+0x29` in the node block).
- Deliverables:
  1. `0x42b5b2` recovered: the `+0x09 & 0x80` per-pixel destination-read blend
     (source/target arithmetic, rounding, clipping) as a standalone helper next
     to `blit_frame`, with explicit inputs and rejection of anything unread.
  2. The `+0x09 & 0x20` snap pass in `compose_scene`: draw through the `+0x20`
     rect (`0x43c114(..., 1)`), then zero `+0x20`/`+0x22` and reset `+0x29`, in
     the same order as `0x43b216` (before the kind 0..4 loop, and again after a
     record is drawn in that loop).
  3. `0x43b130`'s mode-1 sequence: the frame list walk from `+0x18` (head) to
     `+0x1c` (current) when `record+0x28 != 0`, matching the circular list's
     termination.
  4. Tests: the blend against hand-computed 565 arithmetic, the snap pass
     ordering/clears, and a real composition that changes once the blend is
     applied.
- Acceptance: `make -C port -j2 all test` + the three panorama verifiers pass;
  MAP 87 and VT 1638 compose with the blend/snap rules enabled; the census stays
  classified (drawing is a CLI/trace path).
- Non-goals: windowed rendering, GDI/DirectDraw emulation, transitions (item 3),
  audio, AVI.

## Extraction for manual traces (not committed)

`--ca` and `--anims` need shipped files. `build_vm_census.py` extracts them
automatically into a temp tree; for a manual trace:

```sh
E=/tmp/opencode/byz-engine && mkdir -p $E
7z x -y "Byzantine-The-Betrayal_Win_TR_CD1/Byzantine_1.iso" \
  "copy2hd/engine/*.ALL" "copy2hd/engine/SHOW.L*" "copy2hd/goldsave/GAMEPREF.000" \
  "copy2hd/savegame/*.btb" -o$E
for i in 1 2 3 4 5 6; do
  7z x -y "Byzantine-The-Betrayal_Win_TR_CD$i/Byzantine_$i.iso" "anims/*" -o$E
done
for a in CS CI CY CF CA; do l=$(echo $a | tr A-Z a-z); mkdir -p $E/$l; \
  port/byzantine-all $E/copy2hd/engine/$a.ALL $E/$l; done
# inventor/hotitems name lists: 7z l ... | grep -i '\.tga$' | sed ... (see README §--inventor)
```

## Tooling note — REA MCP registration

`rea doctor` previously reported the OpenCode client registration as missing;
only the `rea.sh` CLI wrapper was wired. This pass ran
`rea setup --client opencode --yes` (backup:
`~/.config/opencode/opencode.jsonc.rea.backup`) and verified a live MCP
`initialize` handshake (`rea 6.8.0`, Ghidra 12.1.4). The `rea` MCP tools are now
available in-session. No target was opened through MCP; behavior in this pass
still came from the saved exports.


## Status-only review — 25 October 2026

`make -C port -j2 all test` on the current files: scene-state and variable-metadata
tests pass, but panorama-test fails at `projection reset camera` (exit 2).
Panorama source/tests have newer mtimes than the previous handoff. Cause not
investigated in this review; previous all-pass validation is historical.
See `analysis/rea/PROJECT_REVIEW_2026-10-25.md`. No implementation changes or
new binary analysis; no binary session opened.

RESOLVED in the panorama precision fix above: the failure was a PC64 widening
in `panorama_yaw_offset`, plus an unreachable `-pi` expectation. Current build
and tests pass.

Quick source/report audit: `analysis/rea/IMPLEMENTATION_QUICK_AUDIT.md` lists
31 unique original addresses with bounded or partial port counterparts. This
is not 31 completed functions or a game completion percentage. No new binary
behavior derived; the panorama test failure above remains unresolved.

## REA provenance review — 25 October 2026

Target and all 1,237 written export hashes match the decompile index. Saved
REA/Ghidra records support real analysis use. handoff.md reports subsequent
direct MCP use, but saved exports do not independently establish transport;
no historical REA MCP call transcript was identified in the bounded local
log search. Initial CLI and later saved-export use are documented. See
`analysis/rea/REA_PROVENANCE_REVIEW.md`. Build/tests rerun: same panorama
`projection reset camera` failure, exit 2. No implementation changes or
new binary session.
