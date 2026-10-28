#include "byzantine/transition.hpp"

#include <algorithm>
#include <cstddef>

namespace byzantine {
namespace {

// One RGB565 channel pair, 0..256 weight on `to`. +128 rounds like the
// original's `(value | 0x2004000) >> 3` bias.
std::uint16_t mix_channel(std::uint16_t from, std::uint16_t to, int weight) {
    const int mixed = (static_cast<int>(from) * (256 - weight) +
                       static_cast<int>(to) * weight + 128) >>
                      8;
    return static_cast<std::uint16_t>(mixed);
}

std::uint16_t mix565(std::uint16_t from, std::uint16_t to, int weight) {
    const auto red = mix_channel((from >> 11) & 0x1f, (to >> 11) & 0x1f, weight);
    const auto green = mix_channel((from >> 5) & 0x3f, (to >> 5) & 0x3f, weight);
    const auto blue = mix_channel(from & 0x1f, to & 0x1f, weight);
    return static_cast<std::uint16_t>((red << 11) | (green << 5) | blue);
}

}  // namespace

TransitionKind transition_kind(std::uint16_t code) {
    switch (code) {
        case 0x65: return TransitionKind::fade8;
        case 0x66: return TransitionKind::interleave27;
        case 0x67: return TransitionKind::fade7;
        case 0x68: return TransitionKind::fade4;
        case 0x69: return TransitionKind::none;
        case 0x6a: return TransitionKind::fade7;
        default: return TransitionKind::none;
    }
}

int transition_steps(TransitionKind kind) {
    switch (kind) {
        case TransitionKind::fade8: return 8;
        case TransitionKind::fade4: return 4;
        case TransitionKind::fade7: return 7;
        case TransitionKind::interleave27: return kInterleavePhases;
        case TransitionKind::none: break;
    }
    return 0;
}

int transition_step_ms(TransitionKind kind) {
    // 0x41920f: `do { now = timeGetTime(); } while (now < start + 0x3c)`.
    if (kind == TransitionKind::fade4) return 60;
    return 16;
}

int transition_weight(TransitionKind kind, int index) {
    switch (kind) {
        case TransitionKind::fade8: {
            const int step = std::clamp(index + 1, 0, 8);
            return step * 32;  // k/8 of 256
        }
        case TransitionKind::fade4: {
            // `local_14` starts at 2 and is shifted left twice per step.
            static const int weights[4] = {2, 8, 32, 128};
            return weights[std::clamp(index, 0, 3)];
        }
        case TransitionKind::fade7: {
            // alpha = 0x7f, 0x3f, 0x1f, 0xf, 7, 3, 1; the new share is
            // 0x100 - alpha.
            static const int weights[7] = {0x81, 0xc1, 0xe1, 0xf1, 0xf9, 0xfd, 0xff};
            return weights[std::clamp(index, 0, 6)];
        }
        case TransitionKind::interleave27:
        case TransitionKind::none:
            break;
    }
    return 256;
}

void blend_playfield(Rgb16Surface& out, const Rgb16Surface& from,
                     const Rgb16Surface& to, int weight) {
    const int share = std::clamp(weight, 0, 256);
    out.width = from.width;
    out.height = from.height;
    out.pixels.resize(from.pixels.size());
    if (to.pixels.size() != from.pixels.size()) {
        // A size mismatch is not a blend; keep the departing picture and let
        // the host report it instead of mixing unrelated pixels.
        out.pixels = from.pixels;
        return;
    }
    for (std::size_t index = 0; index < from.pixels.size(); ++index) {
        out.pixels[index] = mix565(from.pixels[index], to.pixels[index], share);
    }
}

void interleave_playfield(Rgb16Surface& out, const Rgb16Surface& to, int phase) {
    if (phase < 0 || phase >= kInterleavePhases) return;
    const std::size_t count = std::min(out.pixels.size(), to.pixels.size());
    for (std::size_t index = static_cast<std::size_t>(phase); index < count;
         index += kInterleavePhases) {
        out.pixels[index] = to.pixels[index];
    }
}

}  // namespace byzantine
