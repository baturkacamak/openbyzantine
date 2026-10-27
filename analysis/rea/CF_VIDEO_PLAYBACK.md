# CF video playback (`0xbd`) — the blocking clip, its destination and its logs

27 October 2026. Fix for the detection in `CF_VIDEO_GAP.md`.
**Library boundary:** FFmpeg decodes (Indeo 4.1 + PCM, exactly like the
conversation clips); the port's existing JPEG/WAV readers and SDL2 mixer render
and play; the engine decides when the clip starts, blocks and ends. No new codec
was written.

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`. The four
exports used here were re-hashed against `decompile-index.json` (all match):

| export | sha256 (index) | role |
| --- | --- | --- |
| `0x42db22` | `8df50fc5339a585b…` | dispatcher `case 0xbd` (raw operands 0/1) |
| `0x40d031` | `3c03f813d14edff7…` | path, mode folder, `DAT_004859cd` |
| `0x40d3af` | `0441419c9da98e3e…` | mode → destination/scale, `0x413bdd` call |
| `0x413bdd` | `e159355056f45abf…` | the blocking playback loop |

## What the original does (evidence)

* `0x42db22 case 0xbd` reads operands 0 (CF record) and 1 (mode) with
  `FUN_00436652`, i.e. **raw**; operands 2/3 are read only when their type byte
  is `0x40` and are the destination **x/y** (`0x40d3af`'s `param_3`/`param_4`).
* `0x40d031` tries `avi\<name>.avi`; when it does not open, `FUN_0040c624`
  builds `video\<mode folder>\<name>.avi` (0x45db28 slide, 0x45db34 site,
  0x45db40 char, 0x45db4c intro, 0x45db58 transit, 0x45db68 anim, 0x45db74 the
  bare `video` root). The shipped CDs carry no `avi/` tree at all, so the
  fallback is the only path that exists.
* `0x40d3af` adds `0x14` to y for every mode and picks the scale mode:
  `0/3 → 1`, `1/2/4 → 2`, `5/default → 0`; for scale mode 2 it asks
  `FUN_0041d440(1)` (= `gamepref[1]`) and, when that is 0, degrades to scale
  mode 0 with `x += 0xa0`, `y += 100`.
* `0x43a248` doubles the destination only for an **IV41** stream (`0x31345649`)
  and scale mode 1/2; every other stream is drawn 1:1. All shipped clips are
  IV41, so a mode 2/4 clip with `gamepref[1] = 1` (the shipped `GAMEPREF.000`)
  is a 2x 640x400 frame at (0, 0x14).
* `0x413bdd` blocks in `while (DAT_0045f060 == 0) { FUN_0041624a(); … }` until the
  clip finishes, then `0x40d031` sets **`DAT_004859cd = 1`** for every mode except
  5 (and skips it for the `mcaft1a1` special case, which calls `0x40da1f`).

## Port implementation

* `port/include/byzantine/video.hpp`, `port/src/video.cpp` — testable rules:
  `video_mode_folder`, `video_candidates` (primary `avi/<name>.avi`, then the
  mode folder), `video_sets_4859cd`, and `find_video_file` (case-insensitive
  multi-component lookup; the CD trees really do mix `MCANB1A.avi` with
  `mbgbcxa.avi`).
* `SceneVideoRequest` gained `step` (1-based executed-instruction index) and its
  operands are named `position_x`/`position_y`; `cc_main` prints them.
* `play_decoded_clip` (`src/play_main.cpp`) is the shared 0x413bdd loop: frame
  pacing at the source fps, window events (`phase=conversation_video` for a
  conversation, `phase=video` for a CF clip), audio queued on the SDL2 mixer and
  drained before returning, last frame kept. The conversation path now calls it
  too, so one loop renders both clip families (its log lines are unchanged).
* `play_cf_video` resolves the file, reports it, decodes through the same
  FFmpeg→JPEG/WAV cache (`<dialog-cache>/cf/<scene>/<name>`), places it with
  `video_placement` and applies `DAT_004859cd` only when the clip actually
  played.
* The runners call it where the original blocks:
  `run_label_gated` adds a step budget per request (so the clip plays *before*
  the following instructions commit), and `run_shot`, the interactive
  `!audio_ready` fast path, the travel/restart entry runner and the `--shot`
  startup path call `play_trace_videos`.
* `scene.sh` writes `media_isos.txt` with the **saved disc first** (the same
  rule the animation overlay uses) and the player extracts only the requested
  clip. 7z matches ISO members case-sensitively, so the exact member name comes
  from a cached `7z l -slt` index (`<data>/video-index/<iso>-<key>.tsv`).

### Logs (always on, not only `verbose`)

```
video_request scene=NB instruction=412 step=13 record=1 name=MCaNB1a mode=4 folder=char
video_media name=MCaNB1a file=/tmp/byz-scene/video/char/MCANB1A.avi
video_media extracted=… video/site/MREST1.AVI source=Byzantine_2.iso name=MREst1
video_media clip=MCaNB1a source=MCANB1A.avi frames=53 fps=15 size=320x200 audio=pcm
video name=MCaNB1a mode=4 placement=2x640x400 at=0,0 frames=53 fps=15
video_audio clip=MCaNB1a wav=audio.wav duration_ms=4200 … device=SDL2 mixer 22050Hz 2ch
video_shot <file.ppm> clip=MCaNB1a
 elapsed_ms=4161 written=53 events=17 (script continues)
video skipped scene=NB name=MCaNB1a mode=4 reason=missing root=<data>
video skipped scene=NB name=MCaNB1a mode=4 reason=--no-video
video skipped scene=NB name=MCaNB1a mode=4 reason=decode_failed|not_shown
```

`run_logged.py` now counts `video_request`, `video_media`, `video name=`,
`video skipped`, `video_audio` and keeps the request/skip lines in
`important_lines`. New option `--video-shot <ppm>` writes the first CF frame
(the conversation keeps `--conversation-video-shot`).

## Verification

**NB label 337 (the reported stuck click).** `python3 analysis/rea/…` style
headless run of the shipped NB script with `--run-label 337`:

```
video_request scene=NB instruction=412 step=13 record=1 name=MCaNB1a mode=4 folder=char
video_media extracted=/tmp/byz-scene/video/char/MCANB1A.avi source=Byzantine_2.iso
video_media clip=MCaNB1a source=MCANB1A.avi frames=53 fps=15 size=320x200 audio=pcm
video name=MCaNB1a mode=4 placement=2x640x400 at=0,0 frames=53 fps=15
 elapsed_ms=4157 written=53 events=17 (script continues)
run_label 337 click_result steps=15 sounds=6 …
```

The first frame occupies exactly rows 20..419 (x 0..639) of the 640x480 screen,
i.e. the 2x playfield placement; the frame is the bartender clip
(`port/output/cf-video/nb-337-mcanb1a-dummy.png`). A real window on `DISPLAY=:1`
shows the same frame (`…-window.png`, 1280x960 X11 grab of the player window;
that is a window-content capture, not a speaker/pixel-equivalence claim).

**MAP label 257 (a travel establishing shot).** `MAP 87 / 27.btb
--run-label 257` now plays `MREst1` (160 frames, 10.7 s) *before*
`travel index=27 scene=mr`, where it used to cut straight to MR
(`…/map-257-mrest1-frame40.png` is a decoded street shot).

**Audio-gated path.** With the sound table and mixer present, the same NB run
shows the budgets reaching step 13 through the six `0xbc` stops, then the clip,
then the commit at step 15 — identical fingerprints to the `--shot` run.

**Degradations.** `--no-video` and a data root without `video/` both report
`video skipped … reason=…` and let the script continue (the original's
`0x40d031` returns before `DAT_004859cd`).

**Conversations unchanged.** The refactor kept every conversation log string:
`conversation_media … frames=89`, `conversation_audio …`,
`conversation_video_shot`, `conversation_end … media=1` still appear on the NB
`Ibrnb2a` line 2 (`bn2bar02.avi`). `verify_ea_inline_voice`, `verify_tk_media`,
`verify_mr_arrival`, `verify_ea_layer/photo/single_execution`,
`verify_examine_controls` (which now really plays `exCam`) and the panorama
verifiers all pass.

**Suites.** `make -C port -j2 all test`: 15 native suites (new `video-test`:
mode folders, both candidates, the `DAT_004859cd` rule incl. `mcaft1a1` and
mode 5, and the mixed-case lookup) plus `tests/logging.py`.

## Boundaries

* The palette/format selector `0x43d9c7` and mode 1's intro-buffer clear
  (`0x419d28`/`0x42b280`) are not modelled; the port always draws RGB565 through
  the same surface.
* Mode 3's transition value still depends on `DAT_00464878`/`DAT_00460668` and
  stays unset; `DAT_00460664` consumption by `0xb9` is unchanged.
* Mode 5 and the `mcaft1a1` special case skip `DAT_004859cd` but the one-time
  `0x40da1f` renderer init is not reproduced.
* Playback is real-time; `--shot` runs therefore take as long as the clip. Use
  `--no-video` for body-only checks and say so.
* The saved disc is preferred for extraction; a clip whose copy on that disc is
  wrong or absent falls through to the other discs in `media_isos.txt` order.
* No claim about the original's acoustics; audio goes through the SDL2 mixer and
  the PCM capture is the content evidence.

## Reproduce

```sh
# NB stairs clip (no save needed: --run-label works on the entry state)
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy port/byzantine-play \
  port/output/cc_all/NB port/cb_all/NB 0 --engine /tmp/byz-scene/copy2hd/engine \
  --lcx /tmp/byz-scene/copy2hd/engine/SHOW.LCX --cf /tmp/byz-scene/cf/NB \
  --cs /tmp/byz-scene/cs/NB --ca /tmp/byz-scene/ca/NB --co port/output/co_all/NB \
  --gameprefs /tmp/byz-scene/copy2hd/goldsave/GAMEPREF.000 --anims /tmp/byz-scene \
  --sounds /tmp/byz-scene --backgrounds /tmp/byz-scene \
  --entry --run-label 337 --video-shot /tmp/nb.ppm --shot /tmp/nb-after.ppm

# MAP -> MR establishing shot
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy port/scene.sh . MAP 87 27.btb \
  --no-audio --run-label 257 --conversation-choices 0,0,0 --selftest-events \
  --video-shot /tmp/mr-est1.ppm
```
