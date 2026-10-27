# Dialogue media: `0x40ced0` is case-insensitive

Date: 27 October 2026. Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`, profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

## Reported symptom (pinned user run)

`run-20261027T140201.207358Z-cuxn9p2q` (EA 602 / `26.btb` -> MAP -> SM, clicked
Ima, chose answers): the character sprite and both conversations work, but no
actor video plays:

```
conversation_line index=5 clip=IIaIma05 speaker=Ima text=yes media=no
conversation_line index=6 clip=IIaIma06 speaker=Ima text=yes media=no
  choice=0 line=21 clip=IIapro01 text=yes media=no
conversation_end ... lines=7 answer_lists=5 media=0
```

No `conversation_media` line appears at all: the port never even looked for the
AVI. For contrast, an older Nur run logged `clip=Nt1nur02 speaker=nur ...
media=yes` and reached `conversation_media skipped (--no-video)`.

## Evidence

`FUN_0040ced0` (the speaker table PTR_DAT_0045f4a0, 16 entries) compares with
**`__strcmpi`**, i.e. case-insensitively, and after the table tests two extra
strings:

```
for (i = 0; i < 0x10; i++) if (!__strcmpi(table[i], speaker)) return i;
if (!__strcmpi("hnt", speaker)) return -2;      // DAT_0045dadc = "hnt"
if (!__strcmpi("pro", speaker)) return -1;      // DAT_0045dae0 = "pro"
return -3;
```

`FUN_00424780` (display/play a line) then plays the clip through `0x40d2a1`
when **the id is not `-1` and the text is not `null`** (`0x45fdc0 = "null"`,
`0x45fdc8 = "#"`):

```
id = FUN_0040ced0(clip + 3);
literal = __strcmpi(text, "null") != 0;        // local_8
if (id != -1 && literal) FUN_0040d2a1(clip, state + 0x1e94);
```

So every speaker except the player's own `pro` lines reaches the AVI lookup;
`hnt` (-2) and unknown codes (-3) included (`0x40d2a1` logs and returns 0 when
the file is missing).

The shipped `.ccf` clip names mix cases for the same actor
(`PTR_DAT_0045f4a0` itself is lowercase):

| speaker code | clips | speaker code | clips |
| --- | --- | --- | --- |
| `pro` | 491 | `Nur` | 22 |
| `hnt` | 123 | `Ima` | 20 |
| `sha` | 94 | `Coi` | 20 |
| `aka` | 66 | `Prf` | 15 |
| `cur` | 45 | `Boo` | 15 |
| `nur` | 42 | `Car` | 10 |
| `meh` | 41 | `Esk` | 8 |
| `esk` | 34 | `Cur` | 8 |
| `ima` | 13 | `Bas` | 7 |
| `prf` | 19 | `Jer` | 6 |

A case-sensitive comparison therefore dropped the media from every capitalised
line (~130 clips) plus all `hnt` narration lines.

## Port bug

`src/conversation.cpp` had:

* `known_speaker()` comparing `speaker == name` (case-sensitive) against the
  lowercase table, and
* `media_allowed = text != "null" && known_speaker(speaker)` — a 16-name
  membership test instead of the original's "not `pro`" test.

`IIaIma05` gives speaker `Ima`, which matched nothing, so `media_allowed` was
false and `0x40d2a1` was never reached. `hnt` lines were rejected the same way.

## Fix

* `speaker_id()` reproduces `0x40ced0`: case-insensitive match against the 16
  names, then `hnt` -> -2, `pro` -> -1, else -3.
* `media_allowed = speaker_id(speaker) != -1 && text != "null"` (case-insensitive
  `null`), i.e. `0x424780`'s condition. `show_text` is unchanged (`null` and `#`
  carry no text; `#` is media-only).
* `tests/conversation.cpp`: new `test_media_speaker_rules` covers `Ima`/`ima`/
  `Nur` (media yes), `pro` (no), `hnt` (yes), unknown (yes), `null` (no text, no
  media) and `#` (media only, no text).

## Verification on real data

Full journey EA -> MAP -> SM with the camera on Ima, then her click
(`--panorama-yaw SM:19.3173 --click-at 280,157`):

```
conversation_line ... clip=IIaIma05 speaker=Ima text=yes media=yes
conversation_media extracted=/tmp/byz-scene/dialog/iimsm1a/Iiaima05.avi source=Byzantine_1.iso
conversation_media clip=IIaIma05 source=Iiaima05.avi frames=84 fps=15 size=320x200 audio=pcm
conversation_audio clip=IIaIma05 wav=audio.wav duration_ms=5600 queued_bytes=493920 device=SDL2 mixer 22050Hz 2ch
conversation_end ... media=1 audio_failures=0
```

The frame is the shipped FMV of Ima's actor with the subtitle
(`port/output/dialogue-media/sm-iima05-video-frame.png`); FFmpeg decodes the
`indeo4`/IV41 video and PCM16 22050 Hz stereo audio. The clip was not extracted
in the analysis copy (scene.sh only extracts the *starting* scene's dialog
folders), so the player's on-demand `7z` extraction from `Byzantine_1.iso` found
and unpacked `dialog/iimsm1a/Iiaima05.avi` (case-insensitive match) before the
decode; a second run reports `cached=yes`.

Regression: `make -C port -j2 all test` passes, and
`verify_ea_layer`, `verify_ea_photo`, `verify_ea_inline_voice`,
`verify_ea_single_execution`, `verify_examine_controls`,
`verify_panorama_bindings/camera/projection`, `verify_tk_animation`,
`verify_tk_hotspots` and `verify_tk_media` still pass.

A lowercase-speaker control (EA label 1599 -> `MCaEA3b`, media enabled) still
works, and now also attempts the previously blocked `thu` line:

```
conversation_line index=2 clip=Mctthu00 speaker=thu text=yes media=yes
conversation_media clip=Mctthu00 missing (0x40d2a1 logs and returns 0; text stays)
conversation_line index=3 clip=Mctcar02 speaker=car text=yes media=yes
conversation_media clip=Mctcar02 source=mctcar02.avi frames=146 fps=15 size=320x200 audio=pcm
conversation_audio clip=Mctcar02 wav=audio.wav duration_ms=9733 queued_bytes=858452
conversation_end ... media=1 audio_failures=0
```

`mctthu00.avi` is on none of the six CDs (listing search), so the missing clip
is shipped data, not a lookup failure.

## Still open

* Answer lines: the port shows the chosen answer's text but does not attempt
  its AVI. `0x436b01` -> `0x424780` would play it for a non-`pro` answer line.
  Every shipped answer observed so far is `pro` (`Nt1pro0x`, `IIaprox`), which
  the original also excludes, so the path is unexercised rather than known-wrong.
* The `0x424780` speaker-change display reset (`DAT_0045fdbc` + `0x424c4e`) is
  not modelled; it only clears the text area.
* `--dialog` must point at the extracted `dialog/` root (scene.sh does). Without
  it the port reports `not configured (--dialog); text stays` instead of guessing
  a path.
