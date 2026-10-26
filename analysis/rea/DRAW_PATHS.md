# Shared record draw paths: the snap pass and save record assets

Date: 26 October 2026.

This step follows the per-scene content census. It implements the two shared
record paths that the census exposed as missing or incomplete — `0x43b216`'s
snap pass and `0x426b9d` marker-1 asset loading — and it corrects the census
reading where the counters had overstated a draw gap.

## Evidence

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

| address | digest (first 16) | role |
| --- | --- | --- |
| `0x43b216` | `b2f51a16a7115afc` | record draw loop, snap pass + kind pass |
| `0x43c114` | `a77c14f8911db67d` | rect: mode 0 x/y, mode 1 +0x24/+0x26 |
| `0x40241c` | `d70c4342c1903b9d` | clip/visibility test (top-left rule) |
| `0x43264d` | `d2ede46468ab4ae3` | remove the matching +0x3c trail node |
| `0x426ed0` | `d9ff6212900879b5` | +0x20 frame index (0xffff = NULL) |
| `0x426b9d` | `5094e97ef5708b02` | save node reader; marker 1 loads the asset |
| `0x410078` | `1abee4b493e60ea7` | anims loader; existing-record branch |
| `0x40fe70` | `4ca6defafd38ee24` | `anims\<2ch>\<name>.rle` -> `.avi` fallback |

## What the original does

`0x43b216` phase 1 (the "snap" pass) runs only when `DAT_004859cd == 0` (not
after a load) and the record table is non-empty. For ids from the highest down a
record qualifies when:

* `(+0x09 & 0x20) != 0`, **or**
* its CA type is 1/2 and `(+0x09 & 4) == 0`,

and its `+0x20` frame node is non-NULL. The record is drawn through
`0x43c114(..., 1)`, which uses the `+0x20` frame and the `+0x24/+0x26` position,
then `0x43264d` removes the trail node at the record's current x/y, `+0x20/+0x22`
are zeroed and `+0x29` is cleared. It is a one-shot snapshot, not the normal
sprite pass.

`0x426b9d` restores the 0x40-byte record, then for marker 1 calls
`0x40fe70(slot, &record)`. Because the record already exists, `0x410078` takes
its "existing record" branch: it fills the frame list, `+0x14` count and `+0x28`
signature from the asset but does **not** add the frame-0 x/y offsets again
(the save already stores them). The port previously copied the saved bytes and
left `frames_known = false`, so the frame geometry/count was unavailable even
though the RLE exists.

## Port changes

* `SceneRecordState` gained `snap_x`/`snap_y` (`+0x24/+0x26`). `0x64` writes them
  as x/y (objdump `0x432594`), and `load_save` restores them from the save's raw
  copy. `frame_index_20` now defaults to `0xffff` (NULL) like `0x40ffee`.
* `compose_scene` implements phase 1 before the kind pass, gated by
  `!state.flag_4859cd`. It draws through the `+0x20` frame at `+0x24/+0x26`,
  drops a matching trail entry, clears `frame_index_20` and counts
  `snap_drawn`. The kind pass is byte-for-byte the same path as before.
* `load_save_record_assets(state)` fills marker-1 records from the anims asset
  with `0x410078`'s existing-record semantics (no second offset). It runs after
  the `--ca`/`--anims` inputs exist, in both the player scene setup and
  `byzantine-cc --trace-save`.
* `--print-records` prints the live record fields (kind, flags, x/y,
  snap position/frame, current frame, frame count/known, asset/type).

## Correction to the census

The content census reported 103 records "skipped by the unimplemented snap
pass". That number is not missing art:

* every shipped save record with `+0x09 & 0x20` has `+0x20 = 0xffff` (NULL), so
  the original's phase 1 would reject it too;
* after a save load `DAT_004859cd = 1` disables phase 1 entirely;
* no shipped script sets a `+0x20` frame.

The counter is now described as "carries the 0x20 flag; the kind pass skips it",
and `snap_drawn` reports the phase-1 draws (0 in every shipped arrival). This is
reported, not hidden: the implemented path is unreachable from the shipped
arrival data.

The VH save `25.btb` has records `vh04helm`/`vh04chal`/`vh04divd` at x =
1468/1364/1662. Those are **surface** coordinates; both `0x40241c` and the port
reject the off-window top-left, so they are not drawn in either. They are not
panorama source points silently projected by the camera: a panorama scene's
script must place a record in screen space. The census's "0 drawn" for VH is
therefore the original's outcome, and the save asset load now reports
`record_assets=3` (frame data present) instead of silently lacking it.

## Verification

Unit (`scene-state-test`): a synthetic flag-0x20 record with a `+0x20` frame and
`+0x24/+0x26` is drawn at that position, `snap_drawn == 1`, the pixel is the
asset's, and `frame_index_20` becomes `0xffff`; the same record with
`flag_4859cd = true` is not drawn and keeps its frame index.

Real scenes (save entry, byte counts from the previous step are unchanged):

| scene | save | drawn | pixels | snap_drawn | record_assets |
| --- | --- | --- | --- | --- | --- |
| HSTMED | 43.btb | 10 (32,33,34,35,36,37,38,39,40,41) | 14237 | 0 | 37 |
| MAP | 27.btb | 7 (0,1,2,8,10,14,26) | 37885 | 0 | 6 |
| AM | 62.btb | 0 | 0 | 0 | 1 |
| VT | 72.btb | 0 | 0 | 0 | 0 |
| CLOCK | 70.btb | 3 (4,5,8) | 9949 | 0 | 2 |
| VH | 25.btb | 0 | 0 | 0 | 3 |

Logs: `port/output/recordings/draw_paths/<scene>_entry.log`. The HSTMED/MAP
record ids, pixel counts and skip counters are identical to the previous step,
so the kind pass did not regress; `record_assets` is the new fill.

## Still unsupported (unexercised, not guessed)

* `+0x09 & 0x10` offset pair: the kind pass's `record + 5/6` (`+0x0a/+0x0c`)
  motion path. All shipped save records have `+0x0a/+0x0c = 0`; no shipped label
  writes them (`0x8c`/`0x8e` are unreachable), so the path is not implemented
  from a guess.
* The `+0x3c` trail **draw** (`0x43b216` with `param_3 != 0`): only the removal
  side (`0x43264d`) is modelled. The normal renderer calls `0x43b216(... , 0,
  0)`; the `param_3 != 0` caller is a transition/effect path.
* `+0x09 & 0x80` destination-read blend (`0x42b5b2`) and the transition effects
  remain open from earlier steps.
* `0x40241c`'s panorama visibility branch is a test only; the draw position is
  not camera-projected (the rect is not rewritten in that branch).
