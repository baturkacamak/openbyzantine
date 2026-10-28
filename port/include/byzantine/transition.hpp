// The scene/panorama transition family.
//
// Two dispatchers run the same routines:
//   0x40186e  installs a panorama node (0xb9),
//   0x431ec2  loads a background image  (0x69).
// Both switch on an engine transition code and blend the departing picture with
// the new one, inside the playfield rectangle (0,0x14)-(0x280,0x1a4):
//
//   0x65 -> 0x42be07(..., 2) -> 0x42c1b8 : eight-step cross-fade
//   0x66 -> 0x42bce1 (0x42bc8b)          : 27-phase interleaved reveal
//   0x67 -> 0x419cf6 -> 0x4196a4+0x419a51: old dims to black, then the new
//                                          picture reveals from black
//   0x68 -> 0x41917b + 0x41920f          : four-step cross-fade, 60 ms each
//   0x69 -> nothing                      : the new picture appears at once
//   0x6a -> 0x419a51                     : the reveal half of 0x67 only
//
// `0x419a51` was read from the instruction stream (`objdump`, addresses
// 0x419a51-0x419cd4), not only from the decompiler text:
//   * it clears its scratch surface first (`0x42b280` = memset 0, 640x480), so
//     the new picture is revealed *over black*, not cross-faded with the old;
//   * seven steps, alpha 0x7f halving each step, so the brightness of the step's
//     source pixels is 0x81,0xc1,0xe1,0xf1,0xf9,0xfd,0xff of full;
//   * each step writes one 2x2 phase of the playfield - the phase is the row and
//     column parity, selected by `base += 1` / `base += 0x280`, and the loop
//     walks two pixels at a time over two rows at a time;
//   * pixels a later phase has not written yet stay black, and pixels already
//     written keep the brightness of the step that wrote them, so the result is
//     a dark dithered fade-in;
//   * the last frame of the family is a plain blit of the new picture.
// `0x4196a4` (`0x67` only) copies the departing picture into the scratch and
// dims it uniformly with alpha 0x1f,0x3f,0x7f,0xff, i.e. brightness
// 0xe1,0xc1,0x81,0x00, before `0x419a51` clears the scratch and reveals.
#pragma once

#include <cstdint>
#include <vector>

#include "byzantine/compose.hpp"

namespace byzantine {

enum class TransitionKind { none, fade8, interleave27, blink, fade4, reveal };

// 0x40186e/0x431ec2's switch. Every other code (100, 0x6b+, 0) is "no blend".
TransitionKind transition_kind(std::uint16_t code);

// Intermediate frames the routine presents before the final picture.
int transition_steps(TransitionKind kind);

// Minimum spacing between two steps. 0x41920f waits until `start + 0x3c`
// (60 ms) per step; the presenting routines only blit, so one 60 Hz host tick
// is their natural spacing.
int transition_step_ms(TransitionKind kind);

// Brightness (0..256) applied to the step's source pixels, 0 = black.
int transition_weight(TransitionKind kind, int index);

// The stride 0x42bc8b uses (0x66), exposed for the tests and the log line.
constexpr int kInterleavePhases = 27;

// Produces the playfield pixels of every step, in the original's order. The
// caller presents `surface()` and advances; after `done()` the caller presents
// the real new picture.
class TransitionPlayer {
public:
    TransitionPlayer(TransitionKind kind, const Rgb16Surface& from,
                     const Rgb16Surface& to);

    TransitionKind kind() const { return kind_; }
    int steps() const { return transition_steps(kind_); }
    int step_ms() const { return transition_step_ms(kind_); }
    bool done() const { return step_ >= steps(); }
    int step() const { return step_; }
    const Rgb16Surface& surface() const { return surface_; }
    const Rgb16Surface& from() const { return from_; }
    const Rgb16Surface& to() const { return to_; }
    // Produces the current step and advances. Out-of-range calls are no-ops.
    void next();

private:
    TransitionKind kind_;
    Rgb16Surface from_;
    Rgb16Surface to_;
    Rgb16Surface surface_;
    int step_ = 0;
};

// One 2x2 reveal phase: writes the pixels of `out` whose (row, column) parity
// matches `phase` (bit 0 = column, bit 1 = row) from `to`, scaled by `weight`.
// `out` must hold the previous step, so unwritten pixels keep their value.
void reveal_phase(Rgb16Surface& out, const Rgb16Surface& to, int phase, int weight);

// Uniform dimming toward black (0x4196a4's first half).
void dim_playfield(Rgb16Surface& out, const Rgb16Surface& from, int weight);

// Cross-fade used by 0x42c1b8 (0x65) and 0x41920f (0x68): `weight` is the
// share of `to` in 0..256, blended per RGB565 channel.
void blend_playfield(Rgb16Surface& out, const Rgb16Surface& from,
                     const Rgb16Surface& to, int weight);

// 0x42bce1/0x42bc8b (0x66): one phase copies every 27th 16-bit word of `to`
// into `out` (stride 0x1b). `out` must start as the departing playfield.
void interleave_playfield(Rgb16Surface& out, const Rgb16Surface& to, int phase);

}  // namespace byzantine
