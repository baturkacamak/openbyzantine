# Compact handoff — 26 October 2026

## Latest step — a scene entered with zero hotspots because its entry dialogue was skipped (27 October 2026)

Pinned user run `run-20261027T142409.559687Z-qtcd5rmd` (EA 602 / `26.btb` ->
MAP -> SM conversations -> MAP -> `slot 13 action 257 scene_switch=27`, MR).
MR arrived as `entry scene=MR steps=103 stopped_at=169 hotspots=0` on
`MRDOORCL.jpg` and 16 clicks in a row reported `hit no hotspot`; the scene had
no exit. Not a mis-click: MR's entry head runs 44 `0x72` objects, `0xb8`
(0x438cf9) suspends them for the arrival dialogue, then `0x70(0,0)`
(CONVERSE record 0 = `MMhmr1a`) at instruction 169. The port's travel path ran
the script but ignored `trace.conversation_request`, so the dialogue and its
continuation (instruction 170, which calls `0x6d(42,43)`, opens the door and
re-enables the view's hotspots) never ran.

Fix: `travel_to` and `restart_scene` now run an entry `0x70` through the same
`execute_plan` flow as the startup path (pre-dialogue segment, real
conversation, `apply_conversation_result`, resumed segment; `0xfffe` still ends
the session, an incomplete conversation prints `entry_conversation not
committed`). The entry probe also runs on a copy of `entry_state` now - it used
to mutate the member and replay the entry from its own output. New diagnostics
`entry_no_hotspots`/`restart_no_hotspots` make a scene with nothing clickable
visible instead of silent, and `--panorama-yaw` became repeatable per scene
(`EA:-44.4519`, `SM:166.721`) so a multi-drag journey can be replayed headlessly.

Real data: MAP 87/`27.btb` -> click 47 twice -> MR gives `entry_conversation
record=0 instruction=169 object_context=0`, conversation `MMhmr1a` (4 lines,
`media=1`; `Mm1meh01.avi` extracted from CD1, 19.4 s PCM; the other three clips
are on no CD), `entry scene=MR steps=9 stopped_at=172 hotspots=2`,
`background=MRDoorOp`, hotspots 42 (door) and 43 (exit); the exit travels back
to MAP (index 25, 6 hotspots). The old binary printed `hotspots=0` for the same
command. Screenshot `port/output/mr-arrival/`. `make -C port -j2 all test` and
all `analysis/rea/verify_*.py` pass. Report:
`analysis/rea/SCENE_ENTRY_CONVERSATION.md`.

## Previous step — dialogue FMVs were skipped by a case-sensitive speaker test (27 October 2026)

Pinned user run `run-20261027T140201.207358Z-cuxn9p2q` (EA 602 / `26.btb` ->
MAP -> SM, clicked Ima, chose answers). The sprite and both conversations work,
but every line logged `media=no` and `conversation_end ... media=0`: no actor
video, only subtitles. A `0x40d2a1` attempt was never made.

Cause: `FUN_0040ced0` compares the clip's 3-character speaker code with
**`__strcmpi`** (case-insensitive) and returns `-2` for `"hnt"`, `-1` for
`"pro"`; `FUN_00424780` calls `0x40d2a1` whenever the id is not `-1` and the
text is not `null`. The port compared case-sensitively against a lowercase
16-name table **and** required membership in it, so `IIaIma05` (speaker `Ima`)
and every `hnt` line were dropped. The shipped `.ccf` mixes cases for the same
actor: `Nur` 22, `Ima` 20, `Coi` 20, `Prf` 15, `Boo` 15, `Car` 10, `Esk` 8,
`Cur` 8, `Bas` 7, `Jer` 6 vs their lowercase forms, plus `hnt` 123; `pro` 491 is
the player's own voice and stays excluded.

New: `speaker_id()` (0x40ced0 including `hnt`/`pro`) and
`media_allowed = id != -1 && text != "null"` in `src/conversation.cpp`;
`tests/conversation.cpp::test_media_speaker_rules` covers `Ima`/`ima`/`Nur`
(media), `pro` (none), `hnt`, unknown, `null` (text+media none) and `#`
(media only). Real run: SM at yaw 19.3173 -> `conversation_media
extracted=/tmp/byz-scene/dialog/iimsm1a/Iiaima05.avi source=Byzantine_1.iso`,
`frames=84 fps=15 size=320x200 audio=pcm`, `conversation_audio ... duration_ms=5600`,
`conversation_end ... media=1`; the FMV frame + subtitle is
`port/output/dialogue-media/sm-iima05-video-frame.png` (dummy video, not a
display claim). `make -C port -j2 all test` and all `analysis/rea/verify_*.py`
still pass. Report: `analysis/rea/DIALOGUE_MEDIA_SPEAKER_CASE.md`.
Still open: answer-line media is not attempted (every shipped answer observed is
`pro`, which the original also excludes).

## Previous step — panorama scene sprites were never drawn (27 October 2026)

Pinned user run `run-20261027T132308.815408Z-eox_w1hb` (EA 602, `26.btb` -> MAP
-> SM, then a click on the woman at screen `280,256` -> `slot 7 action 375`
conversation `IImSM1a`). In SM the character was never composited:
`composed panorama=nodes/sm/SM00.JPG ... drawn=0 pixels=0` while
`record 3 x=1302 y=353 asset=smim00b` (150x150, Ima+Emre) existed. Every
panorama compose in the saved logs had `drawn=0`.

Cause: in a panorama scene a record's `+0x00/+0x02` are panorama **source**
coordinates (SM record 3 x=1302 = object slot 7 x=1302; VA records sit at
x=65..2250 on a 2496-wide node), but `compose_scene` always drew them as screen
pixels, so `0x40241c` rejected the top-left as off-window.

`0x40241c`'s panorama branch (objdump, not the Ghidra text) projects five rect
anchors through `0x423e03` (source -> viewport, +/-60 degree window) in the order
centre, top-left, bottom-right, top-right, bottom-left, and the first success
gives the sprite's top-left. The shipped branch computes that anchor but returns
at `0x4025bd` without storing it (the non-panorama path stores at `0x402626`) —
an inference marker in the report.

New: `SceneProjector` (`compose.hpp`), `PanoramaView::project_source` (the exact
inverse of the port's `panorama_mapping`: `depth = S*cos(pitch)*cos(angle) +
dy*sin(pitch)`, `dx = F*S*sin(angle)/depth`, `dy_screen = F*(dy*cos(pitch) -
S*cos(angle)*sin(pitch))/depth`), `project_record_position` (the five anchors),
`compose_scene(..., projector)` + `SceneCompose::projected` + `projected=` in the
panorama log line, `scene_animates(view)`, and `--panorama-yaw [SCENE:]deg`
(headless absolute camera yaw; `--staged-x` is consumed by the first panorama
scene). Real run: SM at yaw 17.5865 -> `drawn=2 projected=2 pixels=27465
drawn_ids=4,3`, screenshot `port/output/panorama-sprites/sm-entry-projected.png`;
the same camera's click on her still reaches `slot 7`. VA `--entry` went from
`drawn=0` to `drawn=2 projected=2`; background scenes (MAP/CLOCK/HSTMED/IU) and
EA/NB/AM/GC/VH are unchanged. `make -C port -j2 all test` and all
`analysis/rea/verify_*.py` pass. Report: `analysis/rea/PANORAMA_SPRITES.md`.

## Previous step — the game's own mouse cursors (27 October 2026)

The player now installs the shipped pointer art and the engine's cursor rules
instead of a host arrow. `0x40dd7f`/`0x40dae5` load two 32-entry tables —
`cursors/<name>*.tga` and `cursors/klio/<name>*.tga` (alternate mode, `0x68` ->
`DAT_00464878`) — whose names/hotspots are static data at `0x45dbe8` and
`0x45e090`. Selection: `0x40c329` uses the hovered object's kind byte (+0x09) as
the cursor id; `0x407bd7` gives the held item's `inventor/<name>.tga` and, over
kind `0x12/0x13/0x14`, its `inventor/hotitems/<h-name>.tga`; `0x4083d4` does the
inventory band (slots, arrows, examine button, channel area); `0x40de7b` maps
`0`/`0x17` -> `0x1d` ("map") and `0x1c` -> `0x1e` ("hotsmall") in the normal set
outside a panorama. Real SDL colour cursors keep screenshots pixel-identical.

New: `port/{include/byzantine,src}/cursors.*` (tables + rules + wildcard file
lookup), `port/tests/cursors.cpp` (`cursor-test`, in `make test`), `--cursors`
and `--cursor-at <x>,<y>` diagnostics, and `scene.sh` extraction of
`copy2hd/cursors/*` with a `.cursors_v1` backfill. `make -C port -j2 all test`
passes. Verified on `/tmp/byz-scene`: magnify over EA's hotspot, default/hotbig
on the strip, `map`/`hotsmall` in the flat close-up, ISTAR item + `hSTAR`
hotitem in CLOCK, klio set in CLOCK/HSTMED, and a live Xephyr pointer transition
(not a physical-device claim). EA layer/photo/single-execution/examine
regressions still pass. Open: `0x40ebe1`'s 250 ms multi-frame cursors,
`0x4013ae`'s edge-band arrows/rotation, top-menu pointer, strip channel tabs.
Report: `analysis/rea/CURSORS.md`.

**Recovery note.** While editing `port/src/play_main.cpp` this step, a careless
scripted deletion removed a ~1.7k-line region of the working file (uncommitted
animation-pacing work included). It was restored byte-exactly from the OpenCode
snapshot object `64d5886d…` (the pre-deletion working tree), the duplicate
`SceneHit`/`hotspot_at` copy from the interrupted edit was removed, and the full
suite + real-data regressions were re-run. No other file was affected.

## Current user direction — use libraries, integrate playable behavior

The durable policy is in `AGENTS.md`, "Library strategy and playable milestones".
Use SDL2's existing audio/window/input facilities; no new audio library or standard
codec reconstruction. Keep working JPEG code for now, use established libraries
for missing standard media support, and focus custom work on game rules and
playable integration. Do not search for original engine sources. The full audit
and decision are in `analysis/rea/LIBRARY_STRATEGY.md` (final section).
Audio output is implemented, and the script sound records now run through a
common SDL2 mixer with the original start/stop/loop/transition rules
(`analysis/rea/AMBIENT_AUDIO.md`); volume/pan parameters, the saved loop chain
and the `0xd3` follow-up records are still open. Audio fidelity beyond those
points requires its own evidence and validation.

**Repository contents:** git holds `port/`, `analysis/` and the docs only. The
game ISOs/archives, `analysis/rea/BETRAYAL.EXE`, `tools/rea`, `upscale_ornek` and
`port/output/` are ignored on purpose (copyright and size). `make -C port test`
needs `port/output/`: recreate it from your own copy with `port/byzantine-all`
(and `port/scene.sh` for the extracted media) before running the suites.

## Latest fix — animation pass rate (TK character too slow, 27 October 2026)

Pinned user run `run-20261027T121108.021855Z-_6z7t4zt` (EA, 602, 26.btb ->
EA02 -> MAP -> TK). In TK the woman's animation (`TKNUXB`, action 61 /
instruction 87 `0x64(1,1,20,244,150)` -> `+0x04 = 20`, 18 ping-pong delta
frames) ran at ~390 ms per animation frame, i.e. ~2.5 frames/s.

`0x43b216`'s `0x10 < elapsed` gate is **not** a 16 ms cap: the target stores the
elapsed *delta* back into `_DAT_0046187c` (`0x43b334 mov [0x46187c],eax`, eax =
delta), so the uncapped `0x42d594` loop stops throttling after the first passes.
The shipped `+0x04` values are divisors of that pass rate. The earlier 16 ms
model (one pass per tick) read the gate wrongly: it made delay 20 ~3 frames/s
and was still too slow on screen; the user identified the *unpaced* host loop
(the `--compose-frames` runs, 200-345 passes/s here) as the original's speed.

New model: render on the 16 ms `FramePacer` tick, but keep the record-pass clock
separate (`byzantine::PassClock`, `--anim-pass-hz`, default 250). The passes
owed between two host frames are applied to the records the compose drew via
`advance_record_passes` -> `0x43bb77`; a backlog over one second (scene load,
conversation, media) is dropped, and a scene without animation drains the
clock. Nothing in `+0x04`/`0x43bb77`/frame lists changed. Measured live:
`passes` grows 250/s and the TK index advances every 80 ms (was 336 ms).
`make -C port -j2 all test` passes (new `PassClock`/`advance_record_passes`
cases); `BYZ_ANIM_TRACE=1` prints `frame/count/delay/passes/applied` every
100 ms. Report: `analysis/rea/ANIMATION_FRAME_PACING.md`. 250 Hz is a host
reference, not a recovered 1997 frame rate.

## Previous fix — background lost when an action loads no 0x69 (27 October 2026)

Pinned user run `run-20261027T115813.469736Z-x3d7vvnq` (EA, 602, 26.btb):
the player looked at the photo close-up `EAHKEY.JPG` (slot 68) and the frame
switched to `EAEmShMs.jpg`, the shelf with framed photographs. Action 1442 runs
six instructions (1810..1815: `0x6e`, `0x85`, `0x64`, `0x6e`, `0x6d`, `0x6f`)
and contains no background load, so the original keeps the loaded picture.
The port adopted each action's trace and `pick_background()` fell back to the
scene's FIRST static 0x69 entry (instruction 532 = `EAEMShMs`) whenever that
trace had no background. That is the wrong-scene jump; the same trace shape
appears on the rug close-up (`EARugDr` -> `EAEmShMs`) in
`run-20261027T114033.089234Z-6r6h69iv`.

New `resolve_frame_background()` (`src/cc.cpp`) resolves trace 0x69 -> the name
remembered from the last committed 0x69 -> first static entry (startup only).
`SceneView::committed_background` is filled in `update_layer`, so scene travel,
reload and examine return start empty and no name leaks between scenes. Layer
selection (0x69 vs 0xb9 order) is unchanged; no state or opcode was skipped.
`tests/scene_state.cpp` covers the three steps, and
`analysis/rea/verify_ea_photo.py` drives the reported click chain on real data
and asserts `EAHKEY.JPG` + record 16 (`EATexflp`) with no `EAEmShMs` compose.
`make -C port -j2 all test` passes; EA layer/single-execution/inline-voice and
examine verifications still pass. Report:
`analysis/rea/BACKGROUND_RETENTION_FIX.md`; frame:
`port/output/ea-photo-fix/after-1442.png` (dummy video, not a display check).
The rug instance was not replayed end to end (its click depends on the run's
camera state); only the photo chain is proven interactively.

## Previous fix — TK delta frames and wrong-disc animation collisions (27 October 2026)

Pinned user run `run-20261027T105116.045293Z-vk3qe1h9` reaches TK naturally
from EA -> MAP. Two distinct visual problems were reproduced:

1. TKNUXB has signature0 but later frames contain transparent unchanged pixels.
   The player repaints the scene every frame, so drawing only the current delta
   caused holes. compose_scene now reconstructs head..current for all signatures,
   not only nonzero ones. Packed RLE files are cached to avoid reopening them
   repeatedly; no new standard codec was introduced.
2. The all-CD flattened extraction overwrote CD2 TKNUXA with the different CD3/5
   version. CD1/2 has 16 frames,120x120; CD3/5 has 2 frames,90x107, with a visibly
   different background baked into it. save26.btb says cd=2. scene.sh now reads
   the save disc and prepares a separate immutable animation overlay from that
   ISO, which takes precedence over the union. The original files and old union
   are preserved. All scenes in that session use the selected disc overlay;
   existing unsupported disc-change requests are not silently resolved.

New helper: `port/prepare_disc_anims.py`; option `--anims-overlay` in player/CLI.
Cache path `$BYZ_DATA/disc-media/cdN-<source-identity>/`, selected once per launch.
The CD2 animation tree added about21MB in this environment, extracted only once.
Default launcher now handles old prepared roots automatically.

`make -C port -j2 all test` passes native14 suites plus logging checks. New tests
cover signature-zero base/intermediate pixel preservation and backward seek,
and preferred-root collision precedence. `verify_tk_animation.py <data-root>
[disc-overlay]` compares four time samples against independent accumulated RLE
pixels; legacy CD3 variant passed frames1/3/9/17 with zero bad pixels, and CD2
was checked separately. TK travel/conversation and examine regressions pass.
Generated visual checks: `port/output/tk-animation-fix/`. Report:
`analysis/rea/TK_ANIMATION_VISUAL_FIX.md`. Pixel tests use dummy SDL devices.

## Previous fix — inline conversation VOICE commands (27 October 2026)

Pinned user run `run-20261027T104224.941655Z-_wyjx80u` shows EA rug/drawer
slot34/action1307 -> slot81/action1316 -> slot83/action1325 -> MCaEA3b record2
at instruction2018. It failed twice on unsupported conversation opcode0x7c
and rolled back the click. This is distinct from the previous probe replay bug.

Hash-verified saved `0x43340a` -> `0x4096f9`: runtime VOICE 0x7c/0x92/0x93/0x94
all play literal operand2 through the usual conversation line/media path;
operands0/1 and the selector are for original debug presentation. Implemented
all four with resumable instruction-list work: pause at the inline line, wait
for `line_shown`, then execute following writes/branches. Prelude, line/sub-list
and answer continuations preserve their existing ordering; start clears pending
work. No new codec or scene-specific branch was introduced.

Real MCaEA3b check: 3 lines, no answer list, local0 becomes1, completion0xffff,
6 conversation instructions, resume at2019 -> EALOOKIN with exit160. Actual
Mctcar02 AVI decoded146 frames/15fps, 320x200, PCM9733ms; audio_failures=0
on SDL dummy devices. Screen(589,349) hits160/action1611 and returns to the
EA03 panorama with slots157/158. Acceptance uses explicit label1599 to reach
the failing conversation, not a claim of natural end-to-end drawer traversal.

`make -C port -j2 all test`: 14 native suites plus logging tests pass, with new
inline-VOICE tests for all variants, write sequencing, prelude and answer lists.
`verify_ea_inline_voice.py /tmp/byz-scene` covers dialogue completion and exit;
existing TK conversation, EA desk single-execution and examine tests pass.
Evidence and bounds: `analysis/rea/CONVERSATION_INLINE_VOICE.md`.

## Previous fix — action execution reused its probe output (27 October 2026)

Pinned user run: `run-20261027T102315.015177Z-m1mx8hhh` (EA 602 / 26.btb,
normal audio/window launch). EA01 -> desk slot 28/action 1356 -> slot 41/action
1377 left EASHOMER with zero hotspots. The probe had reached the correct branch
and enabled exit 37; execute_plan then ran from `plan.entry` (the already-mutated
probe result). It saw local 43 already set and skipped the exit setup.

Non-conversation actions now execute from `plan.entry_before`, just as the
conversation path already does. This is the common executor for both windowed
and --shot actions and forced diagnostic labels; no exit hotspot was invented.
The earlier missing-video hypothesis was incomplete: CF homer1 remains request-
only and timer callbacks remain absent, but the demonstrated exit loss was a
probe-state replay bug. No capability guard or scene-specific bypass was kept.

`verify_ea_single_execution.py /tmp/byz-scene`: desk/local43=0 -> object/local43=1,
local51=1, exit 37 -> slot37/action1392 -> EA01 with slots27..35. The initial
label653 room shortcut is diagnostic. Existing EA layer, examine, TK view-switch
and TK dialogue checks pass; all 14 native suites plus logging tests pass with
`make -C port -j2 all test`. Interactive-path validation uses dummy devices;
physical mouse/speaker validation is not implied.

Log workflow was expanded in AGENTS.md and `analysis/rea/RUNTIME_LOGGING.md`:
pin the user's basename before tests move LATEST, inspect action/probe/execution
separately, and verify a real exit. Summaries now retain `last_actions` and
`possible_stalls` independently of ordinary output tails. Focused report:
`analysis/rea/EA_SINGLE_EXECUTION_FIX.md`.

## Previous fix — EA panorama/close-up navigation (27 October 2026)

User's normal EA 602 / 26.btb log `run-20261027T101756.588677Z-_4qu0xq7`
shows slot 3/action 544 completing at 4.85 s with active slots 18/19, but all
later clicks still mapped through the old panorama. `update_layer` ignored an
explicit `0x69` when the current layer was already panorama. The loaded close-up
was EAPupAll, but the player kept showing EA00 and its panorama hit mapping.
This condition is present in HEAD b01cd37 too; logging exposed it, rather than
introducing it. No unknown script commands were bypassed.

An explicit background load now selects the flat layer even after a panorama;
a trace without either load still preserves the current layer. Removed the
obsolete fresh-scene parameter. This fixes both displayed image and coordinate
mapping through their existing shared `panorama_layer` decision.

Real-data regression `python3 analysis/rea/verify_ea_layer.py /tmp/byz-scene`:
slot 3 -> EAPupAll/slots 18,19 -> screen (589,349) hits slot 18/action 551 ->
EA00 panorama/slots 1..7. Before the fix the same point mapped to (236,527),
missed, and left the user stuck. Interactive branch with dummy SDL devices and
audio enabled passes the same sequence using --selftest-events. Existing real
examine and EA -> MAP -> TK dialogue regressions still pass. All 14 native
suites plus logging acceptance pass via `make -C port -j2 all test`.
Report: `analysis/rea/EA_LAYER_NAVIGATION_FIX.md`. No new binary analysis.

## Previous step — automatic per-launch diagnostics (27 October 2026)

Every normal `port/scene.sh` launch now goes through `port/run_logged.py`,
including preparation/extraction errors. Each launch creates three files in
`port/output/logs/` (override with `BYZ_LOG_DIR`): timestamped stdout/stderr
`.log`, detailed `.trace.log`, and compact `.summary.json`. `LATEST` points to
the last STARTED run's log, not necessarily the last completed run. Start from
its summary, then search relevant log sections; do not load whole VM traces
into model context. If status is `running`, inspect the live log tail.

The summary records command/cwd, Git HEAD/dirty file list, player SHA-256,
selected device environment, elapsed time, actual child exit/signal status,
counts and selected important/last lines. Native detail records every SDL event
delivered through the scene, sound-wait, conversation-video and answer loops
(including motion), VM steps/arguments, variable writes, conversation commands,
and adopted state (locals, inventory, records and object rectangles). No pixel
or every-frame dump. VM logs explicitly include probes/budgeted re-evaluations,
NOT just committed actions. Input events do not by themselves imply acceptance.

Logging leaves terminal gameplay output intact; verbose input/VM records go
only to the detail file. Trace records flush on each line. SIGINT/SIGTERM are
forwarded and logged; SIGKILL/power failure can leave a running summary. Old
runs are retained. Direct commands use `python3 port/run_logged.py -- <command>`;
unwrapped direct binaries do not automatically create run bundles.

Validation: `make -C port -j2 all test` passes the 14 existing native suites plus
`tests/logging.py` (concurrent runs, stderr/partial lines, failed spawn, nonzero
exit and SIGTERM forwarding). Actual EA dummy launch captured 16 input events;
an isolated Xephyr/XTEST run recorded motion/buttons/wheel/keys and normal exit.
Real examine and EA -> MAP -> TK conversation regressions still pass. No REA or
new binary behavior derivation. See `analysis/rea/RUNTIME_LOGGING.md` and the
logging workflow in AGENTS.md / port/README.md.

## Previous fix — TK missing character / wrong conversation (27 October 2026)

The user's TK slot 3/action 77 was playing EA's `VOemphn2` because `travel_to`
rebound the other scene tables but retained `--cd`. It now rebinds CD too:
TK record 0 is `Inutk1a`, using `Nt1nur*` AVIs, not missing `ep2emr01`.
The launcher provides a manifest of user-owned ISOs; a conversation reached
outside the starting scene extracts only a missing exact-name AVI on demand
through 7z. No fabricated WAV substitution and no new codec.

**Important correction:** raw CA type 255 is NOT dead data. Hash-verified
`0x42d52f`, called by the scene loader, normalizes it to type 0. CA parsing now
performs that normalization (other named tables keep their raw bytes). This
restores the real TKNUXA/TKNUXB actors. `0x90` now also implements the saved
`0x432303` type-0/1 close and `0x432401` hide/trail cleanup, so the old view's
small actor does not remain over the new view's large actor. TKXB draws only
record 1, 37,500 pixels. This is shared behavior, not a TK-only patch.
The old CONTENT_COVERAGE report has a superseded banner; its dead-data claims
and runtime counts must not be used as current evidence. Its generator wording
has been corrected, but the whole-game runtime census has not been rerun.

The real TK dialogue exposed missing conditional TOFF/TON (`0x4c/0x4d`);
these now apply the variable comparison and operand-2 line/answer selection.
A real TK run completes 6 lines / 2 answer lists / 5 decoded videos, with
`audio_failures=0` and script continuation. Audio used the SDL dummy device,
not a human speaker check. A separate EA -> MAP -> TK regression completes
4 lines / 1 list from that save's actual variable pool. The initial EA label
1790 shortcut is diagnostic, not proof of a natural EA exit.

`make -C port -j2 all test`: all 14 suites pass. Optional real-data checks:
`verify_tk_media.py`, `verify_tk_hotspots.py`, `verify_examine_controls.py`
(each under `analysis/rea/`, argument `/tmp/byz-conv`). A fresh empty dialogue
root also verified exact `Nt1nur02` extraction from CD1. Screenshots are in
`port/output/tk-character-fix/`. Evidence/hashes and limitations:
`analysis/rea/TK_CHARACTER_MEDIA_FIX.md`.

## Previous fix — stale hitboxes after a view switch (TK, 27 October 2026)

Reproduced the user's TKXA -> TKXB trace: slot 2/action 61 stayed active and
won the hit test repeatedly; TKXB incorrectly exposed 1/2/3/4/5. `0xb8` and
`0xb7` updated only `hotspot_enabled`, while the hit test also read the real
object +8 bytes. They now snapshot/clear/restore `objects[].active` and keep
the compatibility mirror synchronized. Active-set construction falls back to
the mirror only when the object is absent, not when an existing object is off.
Hash-verified saved evidence: `0x438cf9` / `0x438d78`; no new decompilation.

TK real-script regression: TKXA slots 1/2 -> TKXB slots 3/4/5 -> TKXC slot 6
-> TKXB -> TKXA slots 1/2. `python3 analysis/rea/verify_tk_hotspots.py
/tmp/byz-conv` passes. `make -C port -j2 all test` passes all 14 suites.
The examine regression now compares parent return to its actual arrival set
(EA 602 now 7, not the old 9 which included stale mirror-only enabled bits).
This is a shared VM fix, not removal of all overlapping rectangles: legitimate
overlaps retain the existing ascending-id priority.
Report: `analysis/rea/HOTSPOT_ACTIVE_REGRESSION.md`.

## Previous fix — examined items must not remain held (27 October 2026)

User reported every item's examine-specific action stopped working. Reproduced
on EA `26.btb`, label 602, ICAMERA (17): EXAMINE slot 91/action 288 was rejected
as kind 0x0e because `inventory_examine` retained `current_dialogue_record=17`.
The saved, hash-verified `0x408076` sets DAT_0045d1cc/state 4 then calls
`0x407b9c(0)`, clearing DAT_0045d1c8. The shared helper now does exactly that:
the examined identity stays, the held selection clears. The ordinary item-use
gate is unchanged; no scene-specific bypass was added.

`make -C port -j2 all test` passes all 14 suites. Regression tests cover both
kind 0x0d/0x0e controls and continued rejection of a genuinely held item.
`python3 analysis/rea/verify_examine_controls.py /tmp/byz-conv` passes the real
camera action -> note -> camera -> action -> EA return chain. Window/XTEST
events exercised the same chain (not a manual physical-mouse claim).
Report: `analysis/rea/EXAMINE_ACTION_REGRESSION.md`.

Boundary: camera action 288 now executes its script, including a `0xbd exCam`
request; generic CF videos are still request-only in this player. This fix does
not claim that clip plays. Conversation AVI playback is a separate existing path.

## Previous step — record animation playback (`0x43bb77` + the head-walk redraw)

Records now step their frame list after every drawn frame and draw the whole
animation (head frame plus its delta frames), so a saved frame index that points
at a delta or blank frame no longer freezes or shreds the character. Report:
`analysis/rea/ANIMATION_PLAYBACK.md`.

- **Rules.** `0x43bb77` (after every record `0x43b216` draws) uses `+0x04` as the
  frame delay (`+0x06` countdown, `_DAT_004859fc = 1`) and walks the circular
  node list (`0x410078`: `node[1]` next, `node[2]` prev, `node[5]` frame index);
  flag `0x04` freezes, `0x08` stops at the tail, `0x01` enables ping-pong and
  `0x02` marks the reverse leg. `0x43b130`'s `param_7 == 1` branch draws from the
  head node to the current one; the animation frames are deltas (`0x40bbfd`
  command 3 leaves the destination), so that walk is what completes the picture.
- **Port.** `advance_record_animation` implements the step; `compose_scene` uses
  the head walk when the animation signature (`+0x28`) is non-zero (the port
  repaints the layer every compose, unlike the original's persistent surface);
  `--compose-frames <n>` replays frames for headless checks; the window loop
  composes at its 16 ms cadence while a record can still animate.
- **Verified.** Unit tests cover forward/countdown/wrap/frozen/ping-pong/stop.
  HL `28.btb` entry went from `drawn=1 pixels=4000 ids=0` to `drawn=2
  pixels=32800 ids=4,0` (slot 4 stores the blank frame 1; the receptionist now
  appears and animates). IU `61.btb`, HSTMED `43.btb`, MAP `27.btb` and CLOCK
  `70.btb` keep their documented drawn/pixel values. Real window on Xephyr `:8`:
  both HL and IU animate and the patches stay complete.

## Previous step — ambient sound records through the SDL2 mixer (`08809c5`, restart follow-up `5a06d8c`)

The script's `0x65` sound records now run through one common SDL2 callback
mixer, with the original start/stop/loop rules and without waiting on every
sound. Report: `analysis/rea/AMBIENT_AUDIO.md`.

- **Rules.** `0x427de1` uses `Play(...,1)` when `+0x14` is set (loop) and
  `Play(...,0)` otherwise; it does not block. `0xc4` is the ambient start
  (`0x427de1(id,1)`, `+0x34` for loops); `0xd1` waits only in interactive mode 0
  (`0x428cbd`'s `param_2 == 0` loop, `0x428143` poll); `0xd3` waits a timed
  session; `0xbc`/`0xcf`/`0xd0` stop, release and stop all. A scene load frees
  the per-scene sound table, so a `0x73` does not keep the old ambience; the
  saved loop chain `DAT_0046019c` (block 21) is not reconstructed.
- **Port.** `AudioMixer` (`port/{include/byzantine,src}/ambient_audio.*`) opens
  one S16 stereo device with a callback and mixes tagged voices at the device
  rate (loops wrap, one-shots finish, clamped). Conversation AVI audio keeps the
  `queue`/`queued` drain contract. `0xc4` now emits a `SceneSoundStart` with
  `opcode/loop/allowed`; `0xbc` emits `SceneSoundStop`; `0xcf`/`0xd0` keep
  per-step release/stop-all records. `play_events` starts every event and waits
  only for `blocking` (0xd1 mode 0 or a session). `resume_ambient` starts the
  arrival state's active `+0x14` loops; a normal travel stops all voices first
  (`ambient_scene_stop`); the close-up return re-arms the parent's loops.
- **Verification (AM `62.btb`).** Entry: `audio step=259 opcode=0xc4 id=10
  name=amotambr duration_ms=4795 loop=1 blocking=0 (ambient started)` and the
  same for id 9 `amotambl` 5132 ms; `ambient_resume loops=2 started=0`; the
  entry finishes without waiting. Real window (Xephyr `:8`, 1280x960) with
  `SDL_AUDIODRIVER=disk`: 10.40 s PCM, RMS 1427/peak 7351, non-silent through
  t=10 (past both clip lengths) -> the loop repeats; screenshot
  `am_ambient_window.png`. Click object 85 (label 339) -> `0xbc 11/10/9`
  (`ambient_stop … voices=2/1/0`) -> `0x73 [1]` -> MAP; the capture is silent
  after the stop. Examine `ibeering` -> `ambient_scene_stop from=AM voices=2`,
  `closeup_return … ambient=2`, `ambient_resume id=9/10`.
- **Blocking dialogue.** VT `72.btb` label 497: `opcode=0xd1 id=35
  name=voTins1 duration_ms=4709 blocking=1 waited_ms=4706 frames=323
  events=17 (script continues)`; VT's `0xc4 0 tkamb` loop does not block.
  `--print-audio` now lists `kind/opcode/loop/blocking`.
- **Tests.** `audio-test` (SDL dummy): WAV strictness + 5 rejections, loop
  start/restart, two ambient voices, stop one, stop_all, queue drain.
  `scene-state-test`: 0xc4 start trace (opcode/loop), 0xbc stop, 0xcf release
  step, 0xd0 stop-all. `make -C port -j2 all test` passes.
- **Extraction.** `port/scene.sh` now extracts `copy2hd/sounds/*`
  (`endgame`/`Emre2`/`Pxstart`) into `sounds/`; the lookup also accepts the
  original `copy2hd/sounds` layout.
- **Open.** Volume/pan fields `+0x1c/+0x20/+0x24/+0x28` and
  `0xbf/0xc0/0xc2` are not applied (full volume); `0xd3` follow-up records are
  not executed; the saved loop chain is not resumed; `endgame` stays a flag.
  The real-device check proves the device opens and receives mixes, not an
  acoustic measurement; the PCM capture is the content evidence.

## Previous step — shared record draw paths (`49faebb`)

The census's "snap pass" gap was completed and the census reading corrected.
Report: `analysis/rea/DRAW_PATHS.md`.

- **Snap pass.** `0x43b216` phase 1 runs only when `DAT_004859cd == 0`: a
  record with `+0x09 & 0x20` (or CA type 1/2 with bit 4 clear) and a non-NULL
  `+0x20` frame is drawn once at `+0x24/+0x26`, the matching `+0x3c` trail node
  is removed (`0x43264d`), then `+0x20` is cleared. `SceneRecordState` gained
  `snap_x/snap_y`; `0x64` writes them as x/y, `load_save` restores them, and
  `frame_index_20` defaults to `0xffff` (NULL) like `0x40ffee`. `compose_scene`
  counts `snap_drawn`.
- **Correction.** The 103 `skipped_snap` records are **not** missing art: every
  shipped save record with the flag has `+0x20 = 0xffff` (the original's phase 1
  rejects it) and a save load sets `DAT_004859cd = 1` (phase 1 off). No shipped
  script sets a `+0x20` frame, so the path has no visible arrival effect; it is
  implemented and unit-tested, not claimed as a visual fix.
- **Save record assets.** `0x426b9d` marker 1 also loads the CA animation
  payload through `0x40fe70`; for an existing record `0x410078` fills the frame
  list/count/signature **without** adding the frame-0 offsets again (the save
  already has them). `load_save_record_assets` runs after the CA/anims inputs in
  the player and `--trace-save`. VH `25.btb` now reports `record_assets=3` for
  `vh04helm`/`vh04chal`/`vh04divd`.
- **Verification.** `scene-state-test` snap unit test (draw/clear; no draw when
  `flag_4859cd`). Real entries keep their exact counts: HSTMED 10 rec/14237 px,
  MAP 7/37885, AM 0, VT 0, CLOCK 3/9949, VH 0; `record_assets` 37/6/1/0/2/3.
  Logs `port/output/recordings/draw_paths/<scene>_entry.log`.
- **Open (unexercised, not guessed).** The `+0x09 & 0x10` offset pair
  (`+0x0a/+0x0c = 0` in every shipped save), the `+0x3c` trail **draw**
  (`0x43b216` is called with `param_3 = 0` by the normal renderer), and the
  `+0x09 & 0x80` destination blend.

## Previous step — per-scene content coverage census (`ab02a84`)

`analysis/rea/CONTENT_COVERAGE.md` (generated by
`analysis/rea/build_content_coverage.py` + `render_content_coverage.py`) covers
all 44 scenes with static tables + one player entry each. It answers what object,
character and media assets a scene needs, what loads, what is not drawn and why;
a background that opens is not counted as scene completion.

- **Aggregate.** 970 CA sprite entries: 278 createable, 692 type 255 (`0x431b0f`
  leaves the slot NULL). At arrival only 7 scenes draw records (CLOCK, HL,
  HSTMED, IU, MAP, STAR, VOICE; 34 records total). `skipped_hidden`=0,
  `skipped_no_frame`=0; the 103 `+0x09 & 0x20` records are explained above.
  Background-only arrivals: `start`, `hints`, `iuover`, `vbdisc`, `vcdisc`,
  `vhdisc`, `vtdisc`.
- **Missing media.** 3 backgrounds are genuinely absent (ALGDoor2, BCCountr,
  EAPhne2); the 7 close-up CB names are expected placeholders. MZ2's
  mz18/mz19/mz21..mz29/mz31..mz36 panorama nodes are not on the CDs (only
  MZ01..MZ10/mz30 are). BC `bckill1`/`bckill2` are type 0 with no anims file.
  3 CD records have no dialog clip folder (EA `VOemphn1/2`, HINTS `Hints1`).
  5 CS sounds are not shipped. `endgame`/`Emre2`/`Pxstart` live only under
  `copy2hd/sounds` (now extracted).
- **Common fixes exposed by the census.** A no-save entry now applies
  `0x42c8a0`'s 3000-zero pool (`apply_variable_pool`), so identity locals
  resolve to 0 instead of "unresolved". The click/active set uses the object
  table's own `+0x08` byte like `0x40c02c` (the `hotspot_enabled` list alone
  lost save-active objects); arrival hotspot counts for AM/MAP/NB/VH/VT rise
  accordingly.
- **Legitimate hidden content** is labelled `script-gated (hidden)` (declared
  `0x72` slot not enabled at arrival) and is not forced visible.
- **Real-window check of the active set.** MAP `27.btb` entry now exposes 8
  hotspot rectangles including slot 0 (`185,71 action 342`, a save-active map
  record). A real X11 click in Xephyr hit it and ran `click_result steps=9 …
  scene_switch=17` -> HL; screenshot `port/output/recordings/map27_real.png`.
- **Tools.** `--print-objects`, `--print-variables`, `--print-records`,
  `--save-dump` channel item ids and the `0x72`/`0x2c`/`0x24` fields.

## Previous step — shared item use on scene objects (commits `5d2687c`, `ee7a3d8`, `011cf4a`)

The inventory strip is now also a real item-use system: the selected item,
the clicked object's kind/name id, the shipped script and the item's own
consume path are all common code. The examine close-up's item switch was
reproduced and fixed in the same layer. Reports:
`analysis/rea/ITEM_USE.md`, `analysis/rea/EXAMINE_SWITCH.md`.

### Item use (`5d2687c`)

- **Gate.** `0x407c5a` (`862c7fc71bc4f5da`) is the compatibility switch:
  kinds `0x00/0x01/0x02/0x05/0x08/0x09/0x0c` accept anything when `+0x10 == -1`
  (this is how exits keep working with an item selected), `0x03/0x04/0x06/0x07`
  and `0x0f/0x11/0x18..0x1b/0x1d/0x1e` never accept, `0x0a/0x0b/0x0d/0x0e/0x10`
  accept only "nothing selected", `0x13/0x14` search a dialogue chain, and every
  other kind must name the selected record. `0x40c329` returns `0xfffe` for a
  rejected use, so `0x43c8d2` never runs the object label. The port applies the
  same gate in `plan_hotspot` before tracing; a wrong item cannot write
  variables, objects, records or the strip. `0x407bd7` is the item cursor id
  only, not a mutation; consuming is the script's own `0xab` mode 0/2.
- **Object name ids.** `0x42db22` case `0x72` resolves operand 7 through
  `0x408bb2` (per-scene CI -> SHOW.LCI record id) for every kind except
  `0x13/0x14`, which store a dialogue-chain id. Without the CI table the field
  stays `-1`; it is never guessed.
- **Real puzzle (CLOCK `70.btb`).** Page 2 is `vTk` with one item, record 21
  `ISTAR`; object 26 is active with kind `0x12`, label 202, `+0x10 = 21`.
  Selecting ISTAR and clicking (388,369) runs label 202: hotspot 26 off, ISTAR
  removed from `vTk`, hotspot 27 on. `bar_state page=2 count=0` and
  `slots=0,7,24,25,27,30,31`.
- **Persistence.** Use ISTAR, click object 30 (label 166) -> `0x73 [10]` -> VT,
  then VT slot 114 (label 1374) -> `0x73 [8]` -> CLOCK. The return re-runs the
  entry and rebuilds object 27 (object 26 inactive) from the persisted record-21
  state byte (2): label 137's `0xb0 CI 0 == 2` -> 145 -> 155. The control run
  without the use returns with 26 active / 27 inactive. Logs:
  `port/output/recordings/item_use/clock_roundtrip{,_without_use}.log`.
- **Wrong item.** With ISTAR selected, object 24 (kind `0x0e`) is rejected
  (`compatible=no`, no `click_result`); the HSTMED control (select `ihstd` and
  click a crowbar tile) leaves a byte-identical frame. The examine window alone
  is cleared, exactly like `0x4177c7(0)` before the gate.
- **Second scene.** HSTMED `43.btb`: the crowbar (record 72) on object 8
  (label 1107) changes `VBHSTSCU.jpg` -> `VBMEDDWN.jpg`, 10 -> 9 drawn records
  and 12 -> 11 hotspots. The tool is not consumed (the script does not author
  that).
- **Real window.** Xephyr `:8` 1280x960, real clicks: `bar_item … ISTAR`,
  `item_use slot=26 … compatible=yes`, `click_result steps=9 … active_slots=7`;
  screenshot `port/output/recordings/item_use/clock_after_use.png` shows the
  empty page-2 strip. A real wrong-item click on object 24 was rejected.
- **Item cursor (follow-up `a6613e8`).** While a strip record is selected the
  pointer is the item's `inventor/<name>.tga` (`0x407bd7`/`0x40df05`), via a
  real SDL colour cursor; deselection restores the arrow. A rejected item use
  now logs `(item in hand; click its strip slot again to cancel)`. The
  hotitem (`0x407bd7(1)`) hover image is still not claimed.
- **Tools/tests.** `--click-at` with `y >= 0x1a4` now uses the real strip
  handler; `--print-objects` (objects + strip state) and `--print-variables`
  (local/global/name/value) are diagnostics; `--save-dump` prints channel item
  ids. `inventory-test` covers every `0x407c5a` branch and the chain rule;
  `scene-state-test` covers `0x72` CI resolution. `make -C port -j2 all test`
  passes.

### Examine switch (`ee7a3d8`)

- **Bug reproduced (HSTMED).** In `EXAMVB` for `icrwbar`, clicking slot 0
  (`ihstd`) only moved the strip selection: image `ECRWBAR`, hitboxes and
  `DAT_0045d1cc` stayed on 72, and the parent returned with selection 66 but the
  examine pointer/state still 72 (state 4 left behind).
- **Fix.** `inventory_examine` updates selection (`DAT_0045d1c8`), examined item
  (`DAT_0045d1cc`), state 4 and returns the previous examined record to 1 in one
  step. Inside a close-up it reloads the *same* close-up scene
  (`0x408076`+`0x438c80`+`0x73` self reload) while holding the parent snapshot,
  so the `0x69` transition-4 image and the rebuilt hitboxes match the new item.
  The return branch mirrors `0x4085e7`: examined record -> state 1 and
  `DAT_0045d1cc` cleared; the strip selection stays process-global.
- **Evidence.** `closeup_enter … image=ecrwbar` -> `closeup_item slot=0 id=66
  … examined=yes` -> `closeup_switch … image=ehstd` -> `closeup_return …
  hotspots=12`. The parent playfield is **byte-identical** (0/768000 bytes);
  only the strip band differs (2056 bytes). Real Xephyr click run + screenshots
  `examine_before.png` / `examine_switched.png` / `examine_returned.png`.
- **Description.** No close-up text asset exists (`engine/exam* ce` absent, no
  `0xcd` in the EXAM* scripts), so the "description" is the same item id/name/
  stem tuple the image uses; `examine_state_mismatch` reports any drift instead
  of pairing one item's name with another's image.
- Still open: item combining, the cursor atlas, `va`'s out-of-range examine
  group, and the exact original snapshot table order.

## Previous step — inventory strip, examine close-up and mouse pitch

The AM conversation now returns to the original 640x480 view with the shipped
bottom inventory strip, and the player can continue with the real mouse. This is
the first common inventory/close-up layer, not a scene-specific patch. Report:
`analysis/rea/INVENTORY_AND_CLOSEUP.md`; recording:
`port/output/recordings/am_bar_examine_exit.mp4` (68.0 s, H.264+AAC).

- **Data correction.** Save blocks 8–12 had been called dialogue channels; they
  are the inventory strip (`0x408adc`, `0x4083d4`, `0x407420`, `0x408076`):
  visible, page, scroll, ten page counts, 10×350 item ids, selected item and the
  item state bytes. `SHOW.LCI` names are the item names. `SAVE_FORMAT.md` and
  the `SceneState` comments now say this.
- **640x480 surface.** `Display` keeps the 640x400 playfield and composes a
  640x480 screen with the playfield at row 0x14 and the strip at 0x1a4.
  `inventor/INSTRIP.TGA`, the six real item TGAs and page arrows are drawn;
  missing icons are reported, not invented. Clicks in rows >=0x1a4 are consumed
  before the scene hit test. `--shot` now writes 640x480.
- **Strip input.** Six slots at `x=0xa0+0x3c*i`, examine button `x=2..0x3a`,
  page arrows `0x23b..0x259`/`0x25d..0x27b`; selection sets
  `current_dialogue_record`, examine sets item state 4 and `current_use_item`
  (DAT_0045d1cc), page scroll is clamped like `0x407527`.
- **Close-up.** `0x69` with transition code 4 now requests
  `inventor/examine/<8-char stem>.jpg` (0x40860e/0x41045b) instead of a CB
  background. `enter_examine` uses the original scene-group table
  (0x438afa/0x438173: AM→EXAMINE, VT→EXAMVT, hstmed→EXAMVB, lintel→EXAMVC,
  vh→EXAMVH); `travel_to` carries the inventory and the exit `0x73` restores
  the captured parent view (the DAT_004603d0 override). `va` is left unresolved
  because the original reads index 5 of a five-entry table.
- **Mouse pitch.** Left-drag now sends yaw and pitch (0.15°/pixel, a port input
  decision); the wheel sends ±5°. Arrow keys still work. The original
  cursor-edge bands of `0x4013ae` are not claimed.
- **Physical acceptance (AM 62.btb).** Conversation `Inuam3a` ends with
  `active_slots=0`; the strip is visible. Vertical drag changed pitch to −1.5,
  −3, −4.5, −6, −7.5, −9. Click item slot 1 (`id=11 ibeering`), examine:
  `closeup_enter scene=EXAMINE parent=AM item=11 index=8`, background
  `EBEERING.jpg`; click exit playfield `589,370`: `closeup_return scene=AM …
  bar=visible`; click another item (`slot=2 id=6 IHOMER`). Normal input
  continues.
- **Different scene (physical).** EA `26.btb` item 30 (`iridnote`) on page 0:
  click slot 0, examine -> `closeup_enter scene=EXAMINE parent=EA item=30`,
  `ERIDNOTE.JPG`; exit playfield `589,349` -> `closeup_return scene=EA …
  hotspots=2, bar=visible`. Recording `ea_bar_examine_exit.mp4` (12 s). VT
  `73.btb` item 11 -> `EXAMVT`/`EBEERING.jpg` is a third headless check.
- **Still open / prioritised by subsystem.** Inventory: top menu
  (`0x408e3f`/`0x408f32`/`0x408fe7`/`0x4090c4`), item-on-object compatibility
  (`0x407c5a`) and consume (`0x407bd7`), remaining page tabs. Close-up: exact
  original snapshot tables (port snapshots the whole `SceneView`), `va`, and
  transition effects. Camera: exact cursor-edge bands/acceleration. Conversation:
  immediate view restore is done; out-of-script close-up transitions remain.
  Item use: selection/examine done, object use/combine missing. Scene
  transitions: LCX/pool/inventory/parent return done; exact object/media/dialogue
  snapshot contents and map UI remain.
- **Assets.** `port/scene.sh` now extracts `copy2hd/setupart/*` to `setupart/`,
  `inventor/*.tga`, `inventor/examine/*` and `inventor/hotitems/*` from every
  CD, with a `.ui_v1` backfill marker. The test dir was manually backfilled.

## Previous step — one AM conversation end to end; map destinations and RNG

The four-record AM interpreter sweep is no longer the end-to-end claim. One
conversation is played from a real AM save with normal panorama input and the
shipped AVI/PCM, and the conversation's script continuation runs to its return.
Map destinations now get their backgrounds/panorama nodes and start with the
original startup RNG seed; the exact old stream from a save is explicitly not
claimed. Commits: `829d380` (AM/RNG/map media) and `d3e682f` (0x72
objects, map click); reports: `analysis/rea/AM_NURAY.md`,
`analysis/rea/VM_RANDOM.md`, `analysis/rea/HOTSPOT_TRANSITIONS.md`.

- **Start state and normal clicks.** Save `62.btb` at AM (`AM03`, entry yaw
  180) enables slots 82..88,90. Rotate toward source x1470 until slot 88
  (`action 324`) is visible; click it (tested at playfield `(55,248)`) to run
  `click_result steps=21 … active_slots=2` (slots 91/92). Rotate back toward
  yaw 150; slot 92 (`action 397`, source `50,70 550x250`) appears at screen
  `(630,47)`; click it and reach `0x70` at instruction 466:
  `conversation record=0 name=Inuam3a`. No `--run-label`/`--click`/forced label
  is used in this path.
- **Real media.** Record 0 is `Inuam3a` (`stem=INuAM3a`, speaker `nur`, 11
  clips/texts). The first line `NA1nur01` is textless/media-less; the first
  answer list has 3 choices; choice 0 (real click at `(304,310)` in the 640x480
  screen) leads to line 4 `NA1nur04`, whose shipped
  `dialog/inuam3a/na1nur04.avi` (Indeo 4 + PCM16 22050 Hz stereo, 320x200,
  44.849 s) is decoded by the existing FFmpeg cache and played through SDL2:
  `conversation_audio … duration_ms=44849`, `media=1`, `audio_failures=0`.
- **Continuation.** Result 0xffff resumes at 467; the two instructions are
  `0xb7` (restore) and `0x6f` (return):
  `conversation_resume_probe steps=2 stopped_at=468 switch=none blocker=` and
  `click_result steps=2 … active_slots=0`. There is no random call here. The
  action's own `0x6e`/called labels run before `0xb8`, so the state at the
  conversation has `enabled=0/0 suspended=0/0`; the `0xb7` snapshot is empty
  and the post scene has 0 active hotspots. That is the shipped script's
  outcome, not a skipped continuation. The common 640x480 view and inventory
  strip now return immediately; see the next step.
- **Map destinations.** The black TK window was reproduced from MAP
  `travel index=33 scene=tk`: the entry ran but `composed background=none`
  because the partial data dir lacked `backs/tk/TKXA.jpg`. With the shipped
  asset the same travel composes `TKXA.jpg` and hotspots 1/2. Two generic
  fixes: `port/scene.sh` now extracts `copy2hd/nodes/<scene>` and flattens it
  to `nodes/<scene>/` (the player's `--backgrounds` layout), and a `.media_v2`
  marker backfills all `backs/nodes/anims/sounds` once for older data dirs.
  `refresh_background` now logs
  `background_missing scene=… name=… root=…` instead of leaving a silently
  black window. A 17-destination MAP census
  (`port/output/recordings/map_destinations.txt`) resolves and composes every
  target background/panorama; all 17 entries run clean. VB used to stop on
  `missing_object=4`; the generic `0x72` object creation declares its slot 4
  and the entry now returns at instruction 217 with 4 hotspots. A real
  double-click on the map (`289,63` on `39.btb`: select slot 37/365, then go
  slot 2/376 → `0x73 [15]`) travels to GT (`entry steps=58`, 1 hotspot,
  `GT00.jpg`); recording `port/output/recordings/map_click_to_gt.mp4`. GT is a
  panorama + 1 hotspot (its final `background=` is empty by design).
- **RNG: original startup seed, not a guess.** `0x41535a` calls `0x415a4e`
  (game/render init) before `0x42c8a0`; `0x415a4e` calls
  `FUN_0044d060(0xbeef)` (`_srand`). The player now seeds
  `SceneState::random_state = 0xbeef` at startup and carries it across `0x73`
  travel. Saves do not carry `DAT_00462ef8`; `0x42d594` also draws `_rand()`
  once per instruction-fetch loop, which the static trace does not add. The
  exact old stream from a mid-game save therefore cannot be reproduced; the
  player applies the established startup initialization and reports the
  limitation (`random_seed=0xbeef source=0x415a4e
  note=save_does_not_carry_DAT_00462ef8`). The `0x42a6aa` local-time reseed is
  a separate host-dependent path and remains an explicit unresolved stop.
- **Recording.** `port/output/recordings/am_nuray_end_to_end.mp4` (60.73 s,
  1280x960 H.264 30 fps + AAC 48 kHz stereo, mean -20.3 dB, max -0.7 dB):
  normal camera turns and the two clicks, the answer list, the real Nuray AVI
  and voice, then the post-conversation continuation. Log:
  `port/output/recordings/am_nuray_end_to_end.log`.
- **General vs example.** General: scene travel/load, `copy2hd/nodes`
  flattening and media backfill, `background_missing`, startup RNG seed/carry,
  the conversation media/answer/continuation path. Only this AM example: the
  two action/camera positions, the `Inuam3a` text/choices/media, and the empty
  post-conversation hotspot set (the script disables its view before `0xb8`).
  The previous four-record AM sweep is retained only as an interpreter
  regression (`7e7808c`), not as the end-to-end claim.
- Try: `BYZ_DATA=/tmp/byz-conv ./port/scene.sh . AM 0 62.btb --entry` (or use
  `/tmp/byz-scene` after the media backfill); rotate with Left/Right, click the
  slot 88 view then the slot 92 hotspot, and choose the first answer. For MAP:
  `BYZ_DATA=/tmp/byz-conv ./port/scene.sh . MAP 0 39.btb --entry`, click a
  location; TK/AM etc. now load their real backgrounds/panoramas.
- Still not modelled: the top 20-px menu bar, item-on-object use (`0x407c5a`)
  and the map/other top-menu screens; the exact per-instruction `_rand()` draw
  count from a save; the local-time reseed path.

## Previous step — mouse fix committed, ESC dialog, explicit session end

The mouse-coordinate fix is now its own commit and the ESC path models the
original dialog instead of silently reloading the scene. A different
character's real conversation (AM curator) exercised the shared interpreter and
exposed a missing generic opcode pair. Report:
`analysis/rea/INPUT_AND_CANCEL.md`; mouse report: `analysis/rea/MOUSE_INPUT.md`.

- **Commits:** `197e97e` mouse fix + tests; `7e7808c` shared 0x45/0x46;
  `dad129a` ESC dialog/explicit reload; `cfbb814` reload from the scene's
  arrival state (follow-ups `1aac33d`, `7e0a7d3`).
- **Commit `197e97e` — mouse fix + tests.** SDL_RenderSetLogicalSize's event
  watch already delivers logical mouse events; the old `window_to_logical`
  multiplied by `output/window` and applied dpi_scale twice. Events use their
  fields directly, `SDL_GetMouseState` + `SDL_RenderWindowToLogical` remains for
  hover, and every real click logs `mouse_click … logical=… window=…`.
  `input-test` is part of `make test`; a real X11 check hit the same MR door at
  1280x960 (704,482), 1920x1080 (1046,543), 1024x768 (563,386), 900x700
  (495,352), 640x480 (352,241), 1500x900 (822,452) and 960x600 (528,302), with
  the logged logical point 351/352,200/201 and slot 42 every time.
- **Commit `7e7808c` — shared conversation 0x45/0x46.** AM `icuam1a` stopped at
  `unsupported conversation opcode 0x46`; 0x43340a writes an operand byte plus
  a FUN_004367eb-resolved entry in state+0x2a (0x45) or state+0x5c/0x8e
  (0x46). The port stores `{byte, entry key}` in ConversationMemory and carries
  it across 0x70 calls; a synthetic regression test covers both selectors and
  persistence. No scene/character branch was added.
- **ESC is the original dialog, not a cancel key.** VK_ESCAPE (0x414cd2) sets
  `DAT_0045f058`; 0x436b01 opens `FUN_0041d6df(9)` (`0x41d6df`, dialog proc
  `0x41d523`). `MESSAGES.TXT` is read 0-based by `0x41d45a`: id 9 is
  “Kaydetmeden oyundan çıkmak istiyor musunuz?” and labels 0x4c..0x4f are
  EVET/HAYIR/TAMAM/İPTAL. Only result 6 (EVET) is 0xfffe; 1/2/7 return to the
  **same** answer list. The port draws these real strings (loaded with
  `--messages`, extracted by `scene.sh`); ESC, HAYIR (7), TAMAM (1), İPTAL (2)
  resume without recreating the session, so choices, conversation memory and
  variables are preserved. The vertical layout is the port's; button ids and
  actions are the original's.
- **Session end is explicit.** 0xfffe → 0x42db22 local_8=1 → 0x42d594 stops the
  script → 0x42c8a0 calls 0x41e289(0x41de4b()); 0x41de4b returns 6 and
  0x41e289(6) returns 1 because `DAT_0045f48c`, so the scene session ends
  (0x41535a returns). The original does not restore the 0xb8 snapshot; MR has
  0xb8 at 166/278 and no 0xb7. The port leaves the scene as committed
  (`scene_session_ended … hotspots=0`), draws
  `OTURUM SONLANDI / YENİDEN YÜKLE: R / ÇIKIŞ: ESC` in the message window and
  logs `reload_choice … (explicit user choice; not automatic)`. Only pressing
  R performs the load from the scene's arrival entry_state (save state for the
  start scene, carried pool/state for a 0x73 target;
  `scene_restart reason=conversation_session_exit_user_choice … hotspots=2`).
  `--conversation-cancel N` is the deterministic terminate path: hotspots=0,
  notice printed, no reload. No silent rollback.
- **Verification and recording.** `step_mouse_esc_menu_reload.mp4` (22.3 s,
  1280x960 H.264, silent `--no-video --no-audio`): resize to 1024x768 + door
  click, restore 1280x960 + seller click, ESC dialog, ESC/İPTAL returns to the
  same list, answer 0 picked, next list ESC → EVET (click at logical 110,168),
  scene session ends with the notice, R reloads, door+seller re-talk. Log and
  screenshots: `port/output/recordings/menu_flow/` (`menu_open.png`,
  `menu_resumed.png`, `menu_terminated.png`, `menu_reloaded.png`).
- **Same interpreter, different character.** AM save `62.btb`, real script
  actions 1512/1530/1546/1633: records 6 `icuam1a` (8 lines, 7 lists, 44
  instructions, resume 1898), 7 `ICuAm3a` (8/6/18, 1918), 8 `icuam5a`
  (11/10/30, 1937), 10 `INuTK3a` (7/4/20, 2051), all result 0xffff with no
  unsupported opcode. The AM first answer list also opened and resumed the same
  exit dialog (`am_curator_menu.log`/`.png`). The AM resumed segments stop at
  `unresolved_random`
  (instruction 1777/2005; the CRT seed is not in the save). The AM entry uses
  `--run-label` only because the save's active hotspot set lacks those slots;
  this is an interpreter test, not physical-mouse verification. AM dialog media
  is missing from the prepared data dir, so AM video was not verified.
- **General vs example.** General: mouse mapping/`input-test`, ESC dialog
  paths, session end + explicit R, 0x45/0x46, persistent slot tables. Only MR:
  its 0xb8/no-0xb7 state and first-list choices. Only AM: the four records and
  their counts.
- **Map return (unchanged, still valid).** MAP after the MR conversation:
  slot 47/action 246 opens the region overlay, second click at (192,300) hits
  slot 13/action 257 → `0x73 [31]` → `SHOW.LCX[31]=27` = MR. Verified with
  real mouse earlier; see `analysis/rea/INPUT_AND_CANCEL.md` §5 /
  `port/output/recordings/map_return_physical.log`.
- Try: real window `./port/scene.sh . MR 0 30.btb --entry` (or with
  `BYZ_DATA=/tmp/byz-conv` here), click the door at window (704,482), seller at
  (600,680), then at the first answer list press ESC → the real dialog appears.
  ESC/HAYIR/TAMAM/İPTAL returns to the same list; EVET/6 ends the session with
  `hotspots=0` and the notice; press R to reload explicitly, then door+seller
  again.
- Still not modelled: the original main menu (R is the explicit substitute),
  save/load actions for dialog buttons 1/2/7 (the original caller also only
  resumes the answer loop), AM dialog media, and the CRT random seed needed by
  the AM resumed segments.

## Previous step — natural MR conversation continuation and persistent state

The MR conversation now starts from the evidence save with real hotspot clicks,
returns to the same scene on completion, keeps `DAT_00482ad0`'s asked/disabled
state across a same-scene re-talk, and treats 0xfffe as an answer-list cancel
instead of closing the player.

- **Natural start:** save `30.btb` at MR label 0 -> click hotspot 42 (door,
  action 165) -> click logical `(300,300)` in the mr02 panorama = hotspot 1
  (action 261) -> label 261 loads mr01 and reaches the `0x70` at instruction 523
  (record 2). No `--run-label` is used. The first list is IMhMR2a group 2.
- **Persistent state:** `SceneState::conversation_memory[record]` mirrors the
  persistent part of `DAT_00482ad0` (0x410669): entry `enabled`/`unspeak`, line
  overrides, skip arrays, `state+0xb`/`0xd`, `state+0xe`. `0x432d6e` still resets
  the per-call cursor/mode/chosen slots, as the original does. `travel_to` builds
  a fresh SceneState and logs `conversation_memory reset=scene_load`; 0x42c8a0
  frees every state through 0x41316f/0x40fbf6/0x40f98c.
- **Measured re-talk:** with choices `1,1,1,1` the first run is
  `asked=0 disabled=0 done=no`; the completed run is
  `asked=3 disabled=4 done=yes default_line=5`, and the second 0x70 via hotspot
  17 (action 396) uses the default line, shows 1 line / 0 answer lists. It is a
  real hotspot action, not a forced label.
- **Same-scene completion:** `1,1,1,1` resumes at 524 and stays in MR with
  hotspots 17..21; `0,0,0,0` travels to MAP. The natural chain's camera is
  yaw=151.596, so the player must rotate the panorama to bring the post-scene
  hotspots into view. `--print-hotspots` prints the actual logical rectangles.
- **Cancel (superseded by the top section):** answer-list ESC opens the original
  dialog; only EVET/6 returns 0xfffe, and the other paths resume the same list
  with memory/variables intact. 0xfffe ends the scene session without restoring
  the 0xb8 snapshot; the port leaves `hotspots=0` and offers an explicit R
  reload instead of reloading silently. `--conversation-cancel N` is the
  deterministic terminate path.
- **Blocker (superseded):** in the MR label-261 path the script has disabled the
  hotspots by the time 0x70 runs; after EVET the scene stays at `hotspots=0`.
  The latest step proves the original does not stay in that scene and makes the
  fresh load an explicit user choice (`R`); see the top section.
- **Scene exit/return (corrected in the latest step):** re-entry ends at 524 and
  travels to MAP; the travel log shows the memory reset and a fresh MAP state.
  The return is a two-stage click: MAP slot 47/action 246 opens the region
  overlay (slot 13), and the second click at the same point runs action 257 →
  `0x73 [31]` → `SHOW.LCX[31]=27` → MR. Verified with physical mouse; see the
  top section.
- **Input note (superseded):** the earlier note about host 2x X scaling was
  based on the old `window_to_logical`, which applied the output/window factor
  before `SDL_RenderWindowToLogical` and thus double-applied DPI scaling. SDL's
  renderer event watch already delivers logical event coordinates; the latest
  step uses them directly and verifies real clicks at seven window sizes.
- Real-window clip: `port/output/recordings/mr_natural_start_clean.mp4` (8 s,
  1280x960 H.264 + AAC, mean -23.7 dB, max -1.3 dB), started by the door/NPC
  `--click 42 --click-at 300,300` path; it shows the `Mr2meh03` video and green
  subtitle. The full chain/state evidence is in the logs above.
- Tests: `conversation-test` now covers same-scene memory (asked answer hidden
  after re-entry), cancel returning 0xfffe without setting done, and a cancelled
  list restarting unchanged. `make -C port -j2 all test` passes all eleven
  suites.
- Try:
  `./port/scene.sh . MR 0 30.btb --entry`
  then click the door at logical `(352,201)` and the shopkeeper at `(300,300)`;
  answer lists: click the second row. After the conversation rotate with
  Left/Right and click hotspot 17. Headless chain:
  `… --click 42 --click-at 300,300 --conversation-choices 1,1,1,1 --click 17`.

## Previous step — match the reference conversation screen layout

The reference frames show the 640x480 original layout: the full-playfield video
on black, a green subtitle in the bottom 60-px band, and the answer list over
the video. The port now models that instead of its earlier top orange panel and
bottom answer boxes.

- **Screen/background:** the conversation screen is 640x480. The composed
  640x400 scene sits at rows `0x14..0x413`; the video's mode-2 destination is
  `(0,0x14)..(640,0x414)` (source*2), the whole playfield. The shop JPEG is not
  the conversation background: `0x40d5ff` clears the `0x4b000`-word surface in
  the mode-0 branch, and a mode-2 frame covers the playfield; the last decoded
  frame stays under the answers. `--shot`/`--conversation-shot` images are
  640x480 for the conversation.
- **Subtitle:** `0x424981` clears rows 420..479 (`0x12c00` bytes at `0x83400`)
  every draw and draws `DAT_0045fdb4` at **x=55, y=420**, font slot 4, left,
  max 3 lines; `0x424780` wraps at **530 px**. The fill is green **0x03eb**
  (`0x1f<<5 | 0xb`). The port draws exactly that and keeps the line while the
  answer list is up.
- **Subtitle font blocker:** the Turkish `CAPTIONS.SFN/TGA` has 2-pixel blanks
  for 9 of the 12 Turkish letters and places the remaining accents at odd
  indices; its ASCII glyphs are pixel-identical to `DIALOG` (62/63 compared), so
  the port uses `DIALOG`'s glyph shapes with the green fill and reports the
  missing CAPTIONS glyphs at startup. Coordinates, wrap, max lines and colour
  still come from the original code.
- **Answers:** `0x436b01` x=100, wrap 440, prefix `0x7f` + space (DIALOG glyph
  0x5f star), font slot 0 normal 0xe2c0 / hovered 0xffff, line step 20, block
  bottom `0x19f-0x2d=370`. The hit rect is the drawn block
  (`x=100..100+max line width`, answer lines*20). Hover redraws the option white.
  The first list's rects are `(100,250,234x20)`, `(100,270,440x40)`,
  `(100,310,440x60)`.
- **Text lifetime:** clear at conversation start; replace on each NPC text;
  keep when the answers open; the chosen answer becomes the subtitle; clear at
  the end. Logs: `conversation_caption source=start|npc|answer|cleared`. The
  caption is local to the 640x480 conversation screen, so no text can reach the
  next scene.
- **Conversation end:** the 0xffff path resumes at 524 and travels (`0x73`) to
  scene 25 = **MAP**; the object-click path also travels there. The shop frame
  is the pre-conversation scene. The port does not draw the inventory bar in the
  band outside conversations; the after-exit comparison image pads the 640x400
  scene shot into 640x480.
- **Verification:** `port/output/recordings/shot_talk.png`,
  `shot_answers.png`, `shot_answers_hover.png`, `shot_after_480.png` (all
  640x480) and `mr_new_layout.mp4` (9 s, 1280x960 H.264 + AAC, mean -21.8 dB).
  The recording's envelope matches `Mr2meh03` at **0.999** correlation, offset
  -1.825 s (the recording starts inside the clip). `shot_answers_hover.png`
  shows the third option white under the pointer while the other two stay
  orange; the real-device recording is the talk image's source.
- Tests: `text-test` now covers the single-atlas CAPTIONS loader, the
  zero-advance out-of-atlas sentinel and the green fill override. `make -C port
  -j2 all test` passes all eleven suites.
- Try: `./port/scene.sh . MR 0 30.btb --entry --run-label 413` (real window;
  options at logical y 250/270/310, subtitle at 420; hover an option to see it
  turn white).

## Previous step — conversation screen fixes (text, video, audio)

The IMhMR2a screen had three real defects; this step fixes only those.

- **Turkish glyphs:** the shipped `.byz`/`CE.ALL` bytes are **not CP1254**. The
  text drawer's high-byte path `0x4061e7`/`0x4063e0` calls `FUN_00405bf4`
  (`898ae1b9123f325e`); in the non-English branch (`0x406cbf`, language string
  check) it returns `byte - 0x21`, so the glyph index is **`byte - 0x41`**
  (not `byte - 0x20`). `0xa7 -> ç`, `0xa8 -> ş`, `0xaa -> ı`, `0xab -> ğ`,
  `0xac -> ü`, `0xad -> ö`; capitals `0xa1..0xa6` = Ç Ş İ Ğ Ü Ö. `SfnFont::glyph`
  now applies that index so width and drawing agree; no re-encoding table was
  invented. The DIALOG glyphs are at 0x80..0x8c (Ç Ş İ Ğ Ü Ö ç ş gap ı ğ ü ö).
  The window titles are ASCII now (no em dash). The same fix repairs the
  CE/examine window ("ARAŞTIRMACIYA UYARI…", "AHŞAP").
- **Conversation video:** the old port drew a centred 320x200 rectangle. The
  shipped `GAMEPREF.000` has gamepref[1] = 1; `0x40d3af` type 4 therefore keeps
  mode 2, `0x43a248` calls `0x43d9c7(codec, 2)` and sets the destination to
  **source*2 = 640x400** at x=0, y=0x14 in the 640x480 surface — the whole ported
  playfield. `blit_image_2x` doubles the 320x200 frame exactly (nearest 2x); the
  frame is opaque (no key/mask/background blend in this path). If gamepref[1] is
  0 the original's other branch is mode 0, 320x200 at (160,120), which the port
  also implements. The exact DirectDraw stretch filter is not recovered. The
  subtitle/answer layout was superseded by the latest step (original 640x480
  green band and middle answer list).
- **Audio:** the voice is inside the AVI (PCM16 stereo 22050 Hz; `ffprobe` for
  every IMhMR2a `meh` clip), not a separate file. The cached `audio.wav` is
  parsed and queued through SDL2 before the frame loop and drained after it.
  A real-window/real-device 10 s recording (`/tmp/opencode/
  mr_conversation_record.mp4`, AAC 48 kHz, mean -26.7 dB) shows the fullscreen
  video and Turkish subtitle; the recording's audio envelope correlates **0.98**
  with `Mr2meh03`'s PCM at the video start (correct clip, time-aligned). A
  second recording `/tmp/opencode/mr_answers_record.mp4` shows the three Turkish
  answer rows. Device/`--no-audio`/WAV/queue failures now print
  `conversation_audio … ERROR …` per clip and a summary; nothing is silent.
- Tests: `text-test` now locks the `0x41` high-byte index (0xa7 -> glyph 0x66,
  0xa8 -> 0x67; the direct 0x87 index must not be used); `compose-test` locks
  the exact 2x pixel doubling and rejects a non-doubled surface. `make -C port
  -j2 all test` passes all eleven suites. `analysis/rea/TEXT_WINDOW.md` §3 and
  `analysis/rea/CONVERSATION.md` were corrected.
- Try: `./port/scene.sh . MR 0 30.btb --entry --run-label 413` (real window;
  the current option/subtitle coordinates are in the latest step above). The
  first clip's voice and the fullscreen frame are in the recording above.

## Previous step — one real MR conversation end to end (`0x70`)

MR label 413 now runs the shipped conversation instead of treating operand 0 as
an object. **Correction:** `0x70` operand 0 is the **`engine/<scene>cd` record
index** (`DAT_0046e870`, resolved by `0x410f09`/`DAT_00482ad0`), not an object
slot; operand 1 is the object id the conversation starts with. The old
`operand 0 -> object -> +0x0c label` guess produced `missing_object=2` (MR save
`30.btb`) and, when object 2 existed, the door label 265 — the wrong branch.
`0xffff` means "continue after the 0x70"; any other result is an object the
player clicked during the answer list, whose `+0x0c` selects the pc; `0xfffe` is
ESC/stop.

- Data: `0x410669` loads three **packed** records from a cd entry:
  `engine/<stem>.ccb` (bytecode: two instruction lists + two 50-slot group
  arrays), `.ccf` (voice-clip names) and `.byz` (**the dialogue text**).
  `IMhMR2a` uses `Mr2meh02..09/11..14` AVIs; `mr2meh00/01/10` and all `Mr2pro*`
  files are not shipped (`pro` is not in `0x40ced0`'s speaker list, so the
  original never plays them). This corrects the earlier "conversations are
  voice-only" claim: `Byz.all` was simply not a loose file
  (`analysis/rea/TEXT_WINDOW.md` §5 updated).
- VM slice: `0x432d6e` outer loop, `0x43688b` line branch, `0x436b01` answer
  list/input. Opcodes implemented for IMhMR2a: `0x3a,0x3e,0x32,0x33,0x36,
  0x37,0x38,0x39,0x40,0x42,0x43,0x44,0x49,0x4a,0x4b,0x67,0x74`. Every other
  conversation opcode is an explicit error, never skipped. Text/lines come from
  BYZ; `null`/`#` follow `0x424780`.
- Port: `conversation.{hpp,cpp}` (strict `.ccb`/`.ccf`/`.byz`/`cd` parsers,
  `ConversationSession`), `--cd/--ccb/--ccf/--byz`, `--dialog/
  --dialog-cache`, `--run-label`, `--conversation-choices`, `--conversation-
  object`, `--conversation-shot`, `--conversation-video-shot`. `0x70` now stops
  with `SceneTrace::conversation_request`; `trace_scene_from_instruction` resumes
  and `apply_conversation_result` performs the real branch. An unfinished
  conversation is not committed (the pre-click state is restored).
- Media: Indeo 4 + PCM16 AVIs are decoded once by FFmpeg (`ffprobe` fps,
  `ffmpeg` JPEG frames + WAV) into a per-clip cache; playback uses the port's
  JPEG decoder and SDL2 audio. Missing FFmpeg/AVI keeps the text and reports the
  gap. Video fills the 640x400 playfield at source*2 (the latest fix step;
  the earlier centred 320x200 rectangle was wrong). The original's bottom 60-px
  dialogue band is not in the 640x400 surface, so the 3-line scrollback is drawn
  over the playfield top (real text, changed layout).
- Real MR `30.btb` entry + label 413 result (headless): 8 lines, 4 answer lists,
  7 media clips, 43 instructions, `result=0xffff`, `resume_instruction=524`;
  only locals 14..17 of 300 changed (`0x67/0x74`), objects/hotspots/records
  unchanged. Choices `1,0,0,0,0,0` stop after 3 resumed instructions
  (fingerprint `0x1e6d3b1d25532648`); choices `0,0,0,0` continue through the
  label's `0x73` at 547 and travel to MAP (18 steps, fingerprint
  `0xc8470f2394d7e98a`, `pool_changed=7 (8,79,80,480,713,714,715)`, MAP entry
  `steps=147 stopped_at=133 hotspots=8`). Without `--inventor/--hotitems` the
  resumed `0xab` at 550 stops with `unresolved_dialogue_inventory` and the click
  is not committed (honest data blocker). A door click during the answer list
  returns object 2 -> label 265 -> target 324 (20-step resume, fingerprint
  `0xefcc0d48e65d85ef`) — the real branch, now from a real click.
  The interactive SDL path (answer rect hit test, `--selftest-events`) with the
  same choices/tables gives the same `steps=18` travel and fingerprint; answer
  and first-video PPMs were inspected (real Turkish text + decoded 320x200
  frame).
- Tests: new `conversation-test` (synthetic CCB/CCF/BYZ/CD, both group arrays,
  empty slot, branch/end, 0x37/0x42/0x67/0x74, object result, rejections, real
  `Byz.all` IMHMR2A); `save-test` asserts the new 0x70 request +
  `apply_conversation_result` for the real EA 1779 (record 3, object 3 -> label
  544). `make -C port -j2 all test` passes all eleven suites. Census: 80/7,693
  labels are `conversation_request` (interactive), `blocking_total=0`; completed
  5,899 (5,877 return + 22 script_end), unresolved variable 1,296. Reports:
  `analysis/rea/CONVERSATION.md`, `analysis/rea/reconstruction/vm-census.*`.
- Try: `./port/scene.sh . MR 0 30.btb --entry --run-label 413`; the current
  option/subtitle coordinates are in the latest step above. Headless:
  `… --conversation-choices 1,0,0,0,0,0 --shot /tmp/mr-final.ppm` (~60 s real
  video).
- Not modelled: repeated 0x70 calls on the same record keep `DAT_00482ad0`
  across calls in the original (unspeak/done/group), the port starts a fresh
  session per call; the `DAT_0047a280` gate; conversation opcodes outside the
  table above (`0x34/0x35,0x45/0x46`, inventory/cursor branches, `0x69/0xb9/
  0xbd/0x8d`); `0x436b01`'s ESC/menu result, the 0x4176d8/0x424981 reveal
  tick, and a scene **entry** that
  starts a conversation inside `travel_to` (travel commits the pre-conversation
  pause; MR's entry does not reach one).



## Previous step — the game's text windows (real font + real CE.ALL text)

The port now renders the game's own text: the **examine scroller** (`0x4177c7`,
font 1, x=5, y=0x14a, 3 lines) and the **message block** (`0x417a17`, font 3,
right-aligned to 0x26e/0x2d). The content is the shipped `engine/<scene>ce`
table (`CE.ALL`), the font is `klio.SFN` + `GR_KLIO.TGA`/`BK_KLIO.TGA` with
`0x4067e2`'s gr-over-bk blend.

- **Correction (26 Oct 2026): conversations are not voice-only.** The paragraph
  below was based on the loose-file view; `.byz` (the dialogue text) is packed
  inside `Byz.all` and `0x424780` draws it. `CONVERSATION.md` and the new latest
  step above implement it. The text windows below remain the other real text
  interaction.
- **Conversations are voice-only (superseded).** The `0x70` CONVERSE path is real (MR label
  413 → `converse_jump id=2 label=265`), but `0x410669` loads
  `engine/<name>.ccb` (bytecode) + `.ccf` (voice-clip names: `Mr2meh00…`) and
  the conversation VM `0x43340a` never calls the text drawer. There is no
  dialogue-subtitle asset; the only shipped prose is `CE.ALL` and
  `setupart/MESSAGES.TXT`. A "conversation with text" would have to invent the
  text, so the deliverable is the game's real text-bearing interaction: click an
  object → its `+0x14` indexes `engine/<scene>ce` → `0x4177c7` shows it, and the
  object's action label continues the script (`0x40c329`).
- Text pipeline: `--ce <file>` (per scene, so travel reloads it) and
  `--fonts <dir>` are new; `text.{hpp,cpp}` parses the SFN metrics (223 glyphs,
  `0xcdcd` filler entries), the 8-bit TGA masks (row flip) and draws with the
  exact blend. The CP1254 bytes index the font directly (superseded: the
  shipped bytes are the Turkish font encoding with the high-byte `byte - 0x41`
  index; see the latest fix step). An unterminated entry,
  a wrong version, a glyph outside the atlas or a non-8-bit TGA is rejected.
- Wire-up: after a committed click the object's `+0x14` selects the examine
  text (a text-less object clears it, as `0x40c329`'s `0x4177c7(0)` does); a
  `0xcd` request sets the message text; both windows are drawn in
  `Display::compose` after the record pass; a scene load clears them.
- Demo (`CLOCK`, save `70.btb`): `./port/scene.sh . CLOCK --entry 70.btb` →
  `fonts klio glyphs=223 atlas=375x93 line=22`, `composed background=vtclock.jpg
  drawn=3 pixels=9949`. Click the clock body (logical ~330,250 → slot 31) →
  `text object=31 index=0 lines=4`, `text_draw window=examine x=5 y=330
  width=308` and the frame shows "ARAŞTIRMACIYA UYARI: YERLEŞİK VERİ / MEKANİK
  SAAT / AHŞAP VE ALTIN, 1520" (the 4th line is the one the original's 1-line/s
  scroll would reveal). Clicking the pendulum (slot 0) clears the window
  (`text object=0 cleared`) and runs label 264, which enables the twelve
  mechanism hotspots — the real answer set. `--entry` starts at instruction 0
  (a scene load without needing a label). VT 72.btb 497 shows the message window
  instead: text 1 at x=378..622, y=49..62 ("HÜNKAR SOFASI (TAHT ODASI)"), and
  `--click 19` adds examine entry 3 ("SAZ HEYETİ BALKONU").
- Not modelled: the message panel behind the right block (`0x417a17`'s
  DirectDraw band copy), the 1-line-per-second reveal/scroll and `0x4176d8`'s
  tick (the final 3-line block is drawn), and `0x4069a4`'s SFN-atlas blit.
- Tests: new `text-test` (synthetic SFN + TGAs, blend values, alignment,
  newline/max lines, undefined codes, seven rejections) and `make -C port -j2
  all test` passes all ten suites.
- Report: `analysis/rea/TEXT_WINDOW.md` (19 verified hashes).

## Previous step — two scenes you can walk between (MAP ↔ MR)

A hotspot action that ends in `0x73` now loads the target scene and runs its
entry, and the 3000-word `DAT_0047e2f0` pool crosses the switch. Verified round
trip: **MAP 87 → click 47 twice → MR → click 38 → MAP**, every hop a shipped
hotspot label of the shipped script.

- `0x73` writes `DAT_00482428` and returns **0** (a table `0xffff` returns
  `0xffff` = continue). `0x42d594` turns 0 into "stop the dispatcher" and
  `0x42c8a0`'s outer `while (result == 0)` reloads the scene selected by
  `DAT_00482428`; `DAT_00483bc0` is the SHOW.LCC list (`0x438173`), so the LCX
  value is an LCC index — the mapping the port uses. Execution then starts at
  `DAT_0047a238`, the **instruction-list head**, not at a label (the save resume
  `0x41e289` only runs on the first load).
- Only `DAT_0047e2f0` survives: it is cleared once before the scene loop. Every
  other table is freed and rebuilt per load — `0x40f3f0` frees objects
  (`DAT_00464300`), sounds (`DAT_00486220`), dialogue chains (`DAT_00486a10`)
  and media (`DAT_00464f60`); `0x40fd7b`→`0x4102ae` frees the record table and
  pre-loads CA type 2/3; `0x408662` re-creates the 126 dialogue records;
  `0x4117a6` re-creates the sound rows from the new script's `0x65`s.
- Port: `SceneState::variable_pool` (the carried pool) plus the existing
  `variables` as the current scene's 300-local projection of its `0x46ee00` row;
  `export_variable_pool`/`apply_variable_pool` in `variables.cpp`, and
  `load_save_variables` now keeps the save's whole 3000-word block. A new
  `SceneState::variable_writes` log replays the script's stores in order, because
  one scene row can map a declared local and an identity local onto the same
  global (without the log the untouched identity copy would overwrite the write).
- VM: `trace_scene_entry` runs a script from instruction 0 through the same
  runner as `trace_scene_label`; that needed two real branches: `0x68` (writes
  `DAT_00464878`: 0 only for a type-2 zero operand, else -1, counted in
  `alternate_mode_changes`) and `0x7a` (a dispatcher no-op — the rectangle is
  `0x72`, the enable is `0x6d`/`0x6e`).
- Player: `plan_hotspot` accepts a `0x73` probe as travel; `travel_to` resolves
  the index through SHOW.LCC, derives the target's `cc/cb/co/ci/cy/cf/cs/ca`
  from the current files (same directories, case-insensitive), drops `--save`,
  exports/imports the pool, keeps the renderer's fixed node size and current
  node, runs the entry audio-gated, and prints `travel …`/`entry …`.
  `follow_switches` follows an entry that switches again (8-hop bound, reported).
  `--click-at` is now repeatable so the whole chain is reproducible headlessly.
- Real results: `travel index=27 scene=mr from=MAP pool=3000 locals=300
  pool_changed=1 (2) … locals_digest=0x23569faa90b9e0fb`, `entry scene=MR
  steps=100 stopped_at=89 hotspots=2`, `travel index=25 scene=map from=MR
  pool_changed=4 (10,79,80,718) … locals_digest=0xe6aae5f275869f89`, `entry
  scene=MAP steps=158 stopped_at=137 hotspots=11`. **2996/3000 pool words are
  bit-identical** across the visit; the 4 that changed are MR's own stores
  (global 10 is shared, MAP local 18 = MR local 18), and MAP's own value (global
  2 = 19, local 25) survived. MAP's re-entry then increments its visit counter
  (local 25 19→20) and rewrites local 18, as the shipped script dictates.
  MR shows `composed background=MRDOORCL.jpg`; MAP after return draws the 1
  record its fresh entry creates (the save's 27-record table is per-scene data,
  freed on load — not carry loss). The visible layer after a switch comes from
  the new entry's own trace, so a background replaces a node carried from
  another scene (verified MAP → AM → MAP); a label *inside* one scene that loads
  a background without a node load still keeps the current node, preserving the
  documented EA 602 click frame.
- **A click whose trace stopped on missing data/settings is no longer a
  success** (review fix). `plan_hotspot` used to know eight stop reasons; a probe
  that stopped on `unresolved_dialogue_channel`, `unresolved_save_setting`,
  `unresolved_video_*`, `disc_request`, `step_limit`, … was accepted and its
  partial state committed. `byzantine::trace_blocker` now classifies every
  stop field in one place (0x6f return / 0x79 script end / 0x73 switch are real
  ends; continue-only counters are not blockers) and the player, the entry line
  and the tests share it. Real check: MAP 87 without `--lcx` → second click
  prints `slot 13 action 257 stopped: unresolved_scene_table=31 at instruction
  302`, no `click_result`, no travel, no commit.
- **MAP's frame after a switch now matches the original's table and draw gate**
  (review fix). `0x40fd7b` really pre-loads the CA type 2/3 records: MAP's
  `engine/map ca` has 17 type-3 entries, and all 17 match the save's own
  preloaded records exactly (x/y and +0x14 frame count, 17/17). `0x43b216`'s
  normal pass also skips `+0x09 & 0x20` (snap pass) and `+0x09 & 0x40`, which
  the port ignored. New frames: resume `tempsave.btb` → `drawn=11
  skipped_snap=16 pixels=62263 drawn_ids=33,28,26,24,16,14,12,10,8,2,0`
  (the save's 10 room records carry `flags=0x4`; the 16 inactive `…Hi` preloads
  carry `0x20`), after MR → MAP → `records=17 record_preload=17 drawn=1
  skipped_snap=16 pixels=3373 drawn_ids=0` (`maphl` only, activated by the
  entry's `0x64`), and `--click 34` adds `mapTkHi` (`drawn=12 pixels=70684`).
  The two frames differ in 55,409 pixels = exactly the 10 room records a scene
  switch drops. **Correction:** the earlier `drawn=27 pixels=163079` MAP frame
  and EA `drawn=1 pixels=13570` were over-drawing `0x20` records (EA's
  `EACaset2` is a type-2 preload); EA now reports `drawn=0 skipped_snap=1`.
  The snap pass (`+0x20` rect, `0x43264d` clear) stays unmodelled and is counted
  in `skipped_snap`.
- Regression checks unchanged: MAP 87 `--trace-save` `steps=70 stopped_at=137`
  `tempsave_requests=1`; VT 497 audio `gate stopped_after_step=4 of=15`,
  `audio step=4 id=35 name=voTins1 duration_ms=4709`, `--click 14` →
  `steps=12 … fingerprint=0x849a49bff1a28319 active_slots=2`; EA 602 panorama
  `instruction=317 yaw=180`, `click_at 246,294 -> 2423,496 slot 4 action 586`.
- Tests: `variable-metadata-test` (A/B pool round trip, "no writes = unchanged
  pool", the declared/identity alias, short pool and 301-entry row rejected) and
  `scene-state-test` (`0x68` three operand shapes + counter, `trace_scene_entry`
  starting at instruction 0 through `0x7a`, `trace_blocker` for every stop field
  and for the continuing counters, `preload_scene_records` (CA 2/3 only; 0xff
  not preloaded; a missing asset leaves the slot NULL), and `compose_scene`'s
  `0x20`/`0x40` skip counters). `make -C port -j2 all test` passes all nine
  suites.
- Report: `analysis/rea/SCENE_TRAVEL.md` (17 verified hashes). Port coverage:
  127 → 129 of 840 functions (15.1% → 15.4%, 76,910 → 77,230 B = 31.0%).
- Still not modelled: the `DAT_004603d0`/`DAT_00460660` transition snapshot
  (`0x73`'s override, `0x439349`), `DAT_00477350`/`DAT_0047ae80`/`DAT_00485a10`
  (cleared once before the loop, meaning unrecovered), aliasing **reads** inside
  one scene (exports replay writes, so the carry is exact), scenes whose entry
  needs the CRT `_rand` state (IU, NB, EA, … stop at `unresolved_random`), the
  transition effects, and the snap pass (`+0x09 & 0x20` with a non-zero `+0x20`
  drawn once through the mode-1 rect and cleared by `0x43264d`, plus the `0x40`
  → `0x20` conversion and the `flags & 0x10` offset/blend path). Such records are
  skipped and counted (`skipped_snap`, `skipped_hidden`) rather than guessed.

## Previous step — panorama scenes are playable (look around and click them)

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
- **Cross-scene travel is now implemented** (see the latest step): `0x73` writes
  `DAT_00482428` and returns 0, `0x42c8a0` reloads that LCC-indexed scene and
  runs it from `DAT_0047a238` (the instruction-list head, i.e. the first
  instruction, not a label), and only the `DAT_0047e2f0` pool survives the
  switch. MAP ↔ MR is the verified click round trip (`SCENE_TRAVEL.md`). What
  remains: the `DAT_004603d0`/`DAT_00460660` transition snapshot (`0x73`'s
  override, `0x439349`), the other once-cleared pools (`DAT_00477350`,
  `DAT_0047ae80`, `DAT_00485a10`), and scene entries that need the CRT `_rand`
  state (IU, NB, EA, …).

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
