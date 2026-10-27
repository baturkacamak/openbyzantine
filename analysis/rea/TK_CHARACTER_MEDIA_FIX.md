# TK character and conversation media — 27 October 2026

## Observed failures and changes

The reported slot 3/action 77 reaches TK instruction 104, `0x70 record 0`.
The user's log resolved record 0 to `VOemphn2` and tried `ep2emr01`. That is
EA's CD table. `travel_to` copied the previous scene options and rebound CI,
CY, CF, CS, CA, CO and CE but omitted CD. CD now follows the destination too;
TK's actual record 0 is `Inutk1a`, stem `inutk1a`, with `Nt1nur*` media.

Both TK CA entries have raw type 255. The earlier coverage conclusion that
such entries are dead was incorrect: saved scene-load helper `0x42d52f`,
called by `0x42c8a0`, changes CA 255 to 0 before lazy loading. The new CA parser
performs this normalization; generic named-record parsing stays byte-faithful.
The shipped TKNUXA/TKNUXB RLE assets can then create real animation records.

Restoring actors exposed another omitted step: `0x90(0)` previously recorded
only an intent to close resources, leaving TKXA's small actor over TKXB's
large actor. It now erases loaded type-0/1 records (`0x432303` -> `0x4102f7`)
and hides/clears trails on remaining records (`0x432401` -> `0x432602`).
Nonzero selectors retain lazy records but still hide/clear them. Subsequent
script commands activate the view's own records. No scene name is hardcoded.

Finally, TK's real dialogue used unsupported conditional TOFF/TON commands
`0x4c/0x4d`. The runtime now compares the scene variable to operand 1, then
sets operand 2's selected entry enabled/disabled; its type bit 0x10 selects
lines versus answers (`0x4367eb`). Unconditional TOFF/TON uses the same bit.
The renderer already rebuilds answer lists from those entry flags.

## Media availability

The launcher initially extracts conversations for its starting scene only.
Correctly rebinding CD on later travel therefore also needs access to the
destination's AVIs. `scene.sh` now writes an absolute-path manifest of the
available user-owned ISOs. The player first looks in the extracted analysis
copy; when an exact AVI is absent it uses 7z to extract just that clip from
`dialog/<record>/<clip>.avi` or `avi/<clip>.avi`. Names must be safe leaf names;
paths are shell-quoted. Archives remain unchanged. FFmpeg and SDL2 continue
to decode/play the clip. A missing clip remains an explicit diagnostic; a
similarly named WAV is never substituted.

The new optional player argument is `--media-isos <manifest-file>`.
Direct player users can instead pre-extract the exact AVIs as before.

## Validation

- `make -C port -j2 all test`: all 14 suites pass. Added CA normalization,
  resource refresh/close, and conditional dialogue toggle regression cases.
- `python3 analysis/rea/verify_tk_media.py /tmp/byz-conv`: EA -> MAP -> TK,
  correct CD, one correct actor, answer selection and script continuation.
  This uses diagnostic EA label 1790 before real MAP/TK hotspots; it is not
  claimed as a natural complete EA traversal.
- `verify_tk_hotspots.py`: TKXA 1/2 -> TKXB 3/4/5 -> TKXC 6 -> TKXB -> TKXA.
- `verify_examine_controls.py`: camera action twice, item switch, parent return.
- Real TK entry from the original zero variable pool: TKXA draws record 0;
  after slot 2/action 61, TKXB draws only record 1 / 37,500 pixels.
- The real conversation with choices `0,0,0,0` completed 6 lines, 2 answer
  lists, 5 video clips, `audio_failures=0`, 23 conversation instructions;
  it resumed the script, updated locals 3/4/5/7 and ended with slots 4/5.
  That script state legitimately removes Nuray after the conversation.
- First playable clip `Nt1nur02`: 131 frames, 15 fps, 320x200, PCM audio,
  8,733 ms; 770,252 converted bytes queued to SDL. Later clips also decoded
  and queued. These playback checks used dummy SDL video/audio devices:
  decoded images and audio processing are verified, physical speakers are not.
- A fresh `/tmp/byz-tk-lazy/dialog` verified lazy extraction of `Nt1nur02`
  from CD1. `bash -n port/scene.sh` and `git diff --check` pass.
- Decoded/composed screenshots: `port/output/tk-character-fix/character.png`
  and `conversation.png` (ignored generated assets).

## Saved evidence and bounds

No binary session or decompilation was started. Target SHA-256 matches
`decompile-index.json`:
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`.
Selected exports were checked against the same index before deriving behavior:

| Export | SHA-256 |
| --- | --- |
| 0x42d52f | a2e9bf2e98ad3505496e145c858abee6ba9c7fee2377a03766653f944c18b335 |
| 0x42c8a0 | 6d09fd55ad59f5e18433660b6c565069a3a4d6cc414c4ea40b279efebf6af9a4 |
| 0x431b0f | f229bde142bc8a3a3e0030fdcca05c0bd2dbe7af3e19e83489070ec79d80c8bf |
| 0x411dfc | 554520e53cef4b24e1e8a17068e03da4b72b405f419741a2a70547dd2329abd6 |
| 0x40d2a1 | 7378a7da0065e607e9420a20109a390408309145dd534d34c72a98ebafa8fb7b |
| 0x432303 | fee7728a64cfa38d13fc9c30e0c48c32e9afdc5acb47ecdb8aa5a64bca26bb8e |
| 0x4102f7 | 5ca250b8a916b5efcdec83cff8eeea4a862f8e435e45c2161610f09c691571a0 |
| 0x432401 | e690b490de31cd8076f3f21885fbd2c82a490dc9923fda02bba021228b44922e |
| 0x432602 | 88f88ffd44fd23bd02f17808886b9e322f0f52314a5574f60ec593f7ddfd1073 |
| 0x43340a | 3a9555ec83f82adcb4886ad5784a6c64b3bccd5d723091966c58ae1dbe5666a5 |
| 0x4367eb | 79b12278b4dd0b9697ead6972050c2c25a3dcb247ad0990a0b14806a3e47d132 |

These are shared rules, not proof every game scene is complete. The historical
CONTENT_COVERAGE report's 692 raw-type-255 dead-data conclusion is superseded.
Its runtime census has not been rerun across the game; new normalization may
expose other unsupported assets/commands. Existing generic CF video requests
and other previously documented gaps remain outside this correction.
