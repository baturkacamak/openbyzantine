// The scene/panorama transition family of 0x40186e.
//
// When 0x40186e installs a node it switches on the engine transition code
// (`switch(param_3)` at 0x4018xx) and runs one of the blend routines before the
// new picture becomes final:
//
//   0x65 -> 0x42be07(..., 2) -> 0x42c1b8 : eight-step fade
//   0x66 -> 0x42bce1 (with 0x42bc8b)     : 27-phase interleaved reveal
//   0x67 -> 0x419cf6 -> 0x419a51         : seven-step fade
//   0x68 -> 0x41917b + 0x41920f          : four-step fade, 60 ms per step
//   0x69 -> nothing                      : the new picture appears at once
//   0x6a -> 0x419a51                     : seven-step fade (skips 0x41920f)
//
// The blend rectangle is (0,0x14)-(0x280,0x1a4), i.e. the 640x400 playfield;
// the inventory strip and the top bar are not part of it. Every routine works
// on two surfaces (the departing picture and the new one), so this module only
// produces the playfield pixels of one step: the host presents them and keeps
// the window/timing policy.
//
// Codes here are the *engine* codes. 0x432133 maps the script's operand-3 id
// (1 -> 0x67, 2 -> 0x65, 3 -> 0x66) and a preceding 0xbd hands 0x6a/0x67/0x65
// through DAT_00460664, which is why the loader's "no code" default is 100.
#pragma once

#include <cstdint>

#include "byzantine/compose.hpp"

namespace byzantine {

enum class TransitionKind { none, fade8, fade4, fade7, interleave27 };

// 0x40186e's switch. Every other code (100, 0x6b+, 0) is "no blend".
TransitionKind transition_kind(std::uint16_t code);

// Intermediate frames the routine presents before the final picture: 8, 4, 7,
// 27, or 0 for `none`.
int transition_steps(TransitionKind kind);

// Minimum spacing between two steps. 0x41920f waits until `start + 0x3c`
// (60 ms) per step; 0x42bce1/0x42c1b8/0x419a51 only present, so one 60 Hz host
// tick is their natural spacing.
int transition_step_ms(TransitionKind kind);

// The new picture's share of the blend (0..256) at step `index` (0-based).
// fade8: k/8.  fade4: 2,8,32,128 of 256 (0x41920f's `local_14`).  fade7:
// 0x81,0xc1,0xe1,0xf1,0xf9,0xfd,0xff (0x419a51's `0x100 - alpha`, alpha
// halving from 0x7f).  interleave27/none: unused (returns 256).
int transition_weight(TransitionKind kind, int index);

// Blends two 640x400 playfields; `weight` is the share of `to` in 0..256.
// RGB565 channels are blended independently, which is what the original's
// RGB555 multiply table (DAT_00487710) does per channel.
void blend_playfield(Rgb16Surface& out, const Rgb16Surface& from,
                     const Rgb16Surface& to, int weight);

// 0x42bce1/0x42bc8b: one phase copies every 27th 16-bit word of `to` into
// `out` (stride 0x1b, `param_1 + param_3` then `+= param_4`). `out` must start
// as the departing playfield; 27 phases leave exactly `to`.
void interleave_playfield(Rgb16Surface& out, const Rgb16Surface& to, int phase);

// The stride 0x42bc8b uses, exposed for the tests and for the log line.
constexpr int kInterleavePhases = 27;

}  // namespace byzantine
