# Clip audio tail guard — the Nargile Kahvesi pop (28 October 2026)

## Report

User run `run-20261028T111107.512507Z-o5eqjnag` (MR → MAP → NB, `30.btb`,
`--entry`): entering the Nargile Kahvesi the picture flash was gone, but the
sound popped at the end of the `NBEst1` clip.

The presentation trace confirms the visual side of the previous fix in this
run: the last clip frame (`NBEst1:181/181`) was presented at 16627 ms and the
first NB frame (`panorama:NB00.jpg yaw=-49`) at 16857 ms (`prev_held_ms=224`),
with no MAP frame in between.

## What the pop is (observation)

`NBEST1.avi` (`video/site/NBEST1.avi`, 4,149,696 bytes,
sha256 `816697bf343ed80f14650f5c7f706ce8011de7af12614d65354dce7e297457c5`)
carries 182 `01wb` chunks of ~66 ms each and an `idx1` entry for every one of
them. The **last full chunk** (`01wb`, 5,824 bytes at 3,926,718) is not audio:

- its 1,456 frames start at exactly **12.000 s** (the last video frame's start)
  and are near-full-scale noise: peak 32,765, RMS ≈ 26,000, e.g. samples
  `25341, 29437, 31997, -23555, 32765, 31485, -29699, 30979, …`;
- the 100 ms before it are quiet (peak 1,056, RMS 645);
- the picture fades to black over the same frames (`f0180` mean luminance 18.7,
  `f0181` 16.2), so the burst plays over a black screen.

The clip is byte-identical in all six ISOs and in the extracted tree, and a
fresh FFmpeg decode reproduces the same samples, so this is shipped data, not an
extraction or mixer artifact. In the SDL output capture (`SDL_AUDIODRIVER=disk`)
the burst appears as one 66 ms window at RMS 24,865 with a 65,024 single-sample
jump; a normalized correlation against the cached WAV is 1.0, i.e. the mixer
plays exactly those bytes.

All other extracted `video/{site,transit,char,slide}` clips end quietly (tail
RMS 91…1,142, no tail discontinuity), so this is one defective file, not a
general AVI property.

## Guard (port change)

`byzantine::trim_torn_audio_tail(WavClip&)`
(`port/{include/byzantine,src}/ambient_audio.*`) is called by
`play_decoded_clip` before the audio is queued. It trims only a *torn* tail and
the caller logs the trim:

```
video_audio clip=NBEst1 tail_guard trimmed_at_ms=11999 removed_ms=67
            flip_at_ms=12011 peak=32765 reason=single-sample-sign-flip
```

The test is deliberately narrow, so ordinary loud endings survive:

1. a single-sample sign flip with both sides ≥ 0.45 FS inside the last 200 ms;
2. a quiet 10 ms window before it (the burst start; the flip sits a few samples
   inside the burst) and the burst must run to the end (≤ 150 ms left);
3. the 60 ms after the burst start at ≥ 0.9 FS peak and ≥ 0.35 FS RMS;
4. the 100 ms before it quiet (peak ≤ 0.6 FS, RMS ≤ 0.12 FS).

The clip is cut at the burst start (not at the flip inside it), so the kept
audio ends on quiet samples. The clip's video still runs its full 181 frames;
only the defective tail is silent. `--no-audio-tail-guard` disables the guard
for an A/B against the unmodified stream; every trim is logged either way.

## Validation

- `audio-test` (SDL dummy device): a synthetic torn buffer (quiet 0.5 s + 66 ms
  of the shipped pattern) is trimmed at ~500 ms with the peak reported; a
  continuous 0.5 FS sine ending, a decaying 0.95 FS slam and a one-sample click
  are **not** trimmed.
- Real data (`run-20261028T112416.416218Z-p_s7knfl`, dummy video, disk
  audio, same clicks as the user):
  `duration_ms=12066 → 11999`, `queued_bytes 1064308 → 1058332`; the
  `SDL_AUDIODRIVER=disk` capture of the same chain has **0** 66 ms windows above
  RMS 12,000 (before: one at RMS 24,865) and the largest single-sample jump
  drops from 65,024 to 30,059 (ordinary music content). The video still logs
  `written=181 … (script continues)`, travels to NB and enters
  `entry scene=NB steps=138 … hotspots=5`.
- `make -C port -j2 all test` passes.

## Boundaries

- This is a deliberate, logged deviation from the shipped bytes. Whether the
  original engine played this tail is **not established** here; the guard does
  not claim the original was silent, only that the tail is a torn buffer (a
  full-scale sign flip after quiet audio inside a fade-to-black) rather than a
  sound effect.
- Audio was validated on SDL's dummy/disk drivers and by PCM analysis, not on a
  physical speaker.
- The rule is content-based and narrow by construction; a clip that genuinely
  ends with a loud transient right after quiet audio would need its own
  evidence.
