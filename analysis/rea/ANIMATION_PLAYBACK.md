# Record animation playback: `0x43bb77` and the head-walk redraw

Date: 26 October 2026.

This step adds the per-frame animation the record pass was missing. Before it,
every record kept whatever frame the save or the creating `0x64` left in
`+0x1c`; a save whose index pointed at a delta frame showed fragments, and a
save whose index pointed at a blank frame showed nothing at all.

## Evidence

Target `analysis/rea/BETRAYAL.EXE`, SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`; profile
`e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`. Exports
verified against `pseudocode/decompile-index.json`.

| address | sha256 (first 16) | role |
| --- | --- | --- |
| `0x43bb77` | `26c112e41980face` | frame-list step after every drawn record |
| `0x43b130` | `529ef379c272785e` | frame node draw loop (head walk vs current) |
| `0x410078` | `1abee4b493e60ea7` | anims loader; circular node list, `node[5]` = frame index |
| `0x40cf73` | `a63b8280c8f6c629` | `+0x1c` index setter (walks `node[1]`) |
| `0x40ffee` | `16955dd55a53620b` | record initialiser (`+0x04 = 10`, `+0x06 = 0`) |
| `0x42c8a0` | `6d09fd55ad59f5e1` | scene entry; sets `_DAT_004859fc = 1` |
| `0x43b216` | `b2f51a16a7115afc` | record pass; calls `0x43b130` then `0x43bb77` |

### The frame node is circular and carries its index

`0x410078` allocates the head node, links every later node with
`prev->next = new`, `new->prev = prev`, keeps `head->prev = newest`, and after
the loop writes `last->next = head`. `node[5]` (`node + 0x14`) is the frame
index. `0x40cf73` walks `node[1]` (next) from `record + 0x18` (head) `index`
times, which is why the saved index is a frame position.

### `0x43bb77` (called by `0x43b216` after a drawn record)

```
if (flags & 0x04) { +0x20 = +0x1c; return; }        // frozen record
if (+0x06 != 0) { +0x06 -= DAT_004859fc; if (borrow) +0x06 = 0; return; }
+0x06 = +0x04; +0x20 = +0x1c;                       // reload the delay
node = +0x1c
if (!(flags & 0x02)) {                              // forward leg
    if (node->next == +0x18) {                      // tail of the circular list
        if (flags & 0x08) flags |= 0x04;            // stop at the last frame
        if (!(flags & 0x01)) { if (!(flags & 0x04)) +0x1c = head; }
        else { +0x1c = node->prev; flags |= 0x02; }
    } else +0x1c = node->next;
} else {                                            // reverse leg
    if (node + 0x14 == 0) {                         // frame index 0
        if (!(flags & 0x01)) { if (!(flags & 0x04)) +0x1c = head->prev; }
        else { +0x1c = head->next; flags &= ~0x02; }
    } else +0x1c = node->prev;
}
```

`_DAT_004859fc` is 1 (`0x42c8a0`), so `+0x04` is the number of drawn frames per
animation frame: 10 by default, 32 for the HL receptionist, 1 for some
records. Flags bit 1 (`0x02`) is the reverse leg, bit 2 (`0x04`) freezes the
record (`+0x20` is still refreshed for the snap pass), bit 3 (`0x08`) stops at
the tail. Bits 0/1 are exercised by shipped scripts (`0x85` mask indices 0..3
in BC, CLOCK and VT).

### Animations are deltas of the full head frame

`0x40bbfd` command 3 leaves the destination pixel untouched, so a frame only
carries the pixels that differ from the reference. Measured on the HL
receptionist `HLDc00b` (39 frames, 180x160):

| frame | covered pixels | note |
| --- | --- | --- |
| 0 | 28800 / 28800 | full photograph, the reference frame |
| 1 | 0 | blank (a one-frame blink) |
| 2 | 7059 | the man with the unchanged background transparent |

Drawing frame 2 alone over a repainted background therefore shows the man with
holes. The full picture needs frame 0 first.

`0x43b130` has exactly that path: it starts at `record + 0x18` (the head) when
`param_7 == 1` **and** the animation signature at `+0x28` is non-zero, then draws
every node up to `record + 0x1c`. `0x43b216` passes its own `param_2` as
`param_7`; the normal renderer `0x43c466` passes 0, while `0x41df43` and
`0x431ec2` call `0x43b216(..., 1, ...)`, which is the replay form.

## Port changes

* `byzantine::advance_record_animation(SceneRecordState&)` implements `0x43bb77`
  over the port's flat frame list, using the recovered circular-list semantics
  (frame 0 is the head, `frame_count - 1` is the tail, `node + 0x14 == 0` is
  frame 0). `compose_scene` calls it only for records it actually drew in the
  kind pass, exactly like `0x43b216`'s `if (param_2 != 1) 0x43bb77(...)`.
* `compose_scene`'s draw now uses `0x43b130`'s head walk when the record's
  animation signature is non-zero: frames are drawn 0..current in order at the
  record's position. The reported `pixels`/`drawn_ids` describe what the surface
  actually received, so a record whose current frame is blank still reports the
  pixels its head frame wrote.
* The interactive loop composes and presents at its 16 ms cadence while
  `scene_animates(state)` is true (a record with a known frame list, an index,
  at least two frames, no `0x20`/`0x40`/`0x04` flag, kind 0..4 and an on-window
  origin, because an off-window record is never drawn and therefore never
  advances). Static scenes keep the event-driven redraw.
* `--compose-frames <n>` draws `n` extra frames before `--shot`, so the frame
  walk is observable headlessly and in tests.

**Why the head walk.** The original keeps one surface between frames and only
repaints the layer when `DAT_0046edf4` says the scene changed; this port clears
and repaints the background/panorama on every compose. With the engine's
current-frame-only draw (`param_7 == 0`) the delta frames would land on a fresh
background and the earlier frames would be lost. The head walk is the engine's
own complete-draw branch and produces the same picture under the port's
stateless compose. It is a port decision documented here, not a new game rule.

## Verification

Unit (`scene-state-test`, new block): the forward step, the countdown clamp, the
circular wrap, the frozen flag (`+0x20` still refreshed), the ping-pong
turnaround (`3 → 2`, bit `0x02` set), the reverse step, the forward resume and
the stop-at-last bit (`0x08` → `0x04`).

Scene entry, before/after this step (all other numbers unchanged):

| scene | save | before | after |
| --- | --- | --- | --- |
| HL | 28.btb | drawn 1, pixels 4000, ids `0` | drawn 2, pixels 32800, ids `4,0` |
| IU | 61.btb | drawn 4, pixels 41160 | drawn 4, pixels 41160 |
| HSTMED | 43.btb | 10 / 14237 | 10 / 14237 |
| MAP | 27.btb | 7 / 37885 | 7 / 37885 |
| CLOCK | 70.btb | 3 / 9949 | 3 / 9949 |

The HL save is the concrete "character invisible" case: `28.btb` slot 4
(`HLDc00b`, marker 1) stores frame index 1, `+0x04 = 32`, `+0x06 = 8`; index 1
is the blank frame, so before this step the receptionist was never drawn. After
the head walk the entry draws frame 0 and the animation continues from there.

Real window (Xephyr `:8`, 1280x960, 640x480 logical): HL 28.btb animates the
receptionist (successive root captures differ in the head/eyes), IU 61.btb
animates the man at the desk without eroding the patch. Headless
`--compose-frames 100` on IU keeps the full patch (the earlier accumulation
experiment lost it).

Performance: HSTMED 43.btb (42 records, 10 drawn) composes 201 frames in ~2.9 s
wall time (~14 ms per frame including the 640x400 background JPEG blit), so the
16 ms animation cadence is met for the shipped entry states.

## Not claimed

* No original execution; the frame rate is derived (`+0x04` drawn frames per
  step at the renderer's cadence), not measured against the game.
* The `+0x29`/`DAT_00482acd` "redraw again" loop (`0x43c466`) and the `+0x3c`
  trail draw are still not modelled.
* Per-record patch caching is not implemented: a high frame index replays every
  node up to it on each compose. All shipped entry states are small enough, but
  a 640x400 record with many frames would be expensive.
* The background is still repainted every compose; the original's
  `DAT_0046edf4` layer-change gate is not reproduced.
