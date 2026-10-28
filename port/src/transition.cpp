#include "byzantine/transition.hpp"

#include <algorithm>
#include <cstddef>

namespace byzantine {
namespace {

// The original's multiply table (DAT_00487710) is `channel * alpha / 256`; the
// +128 rounds the same way its `>> 8` does.
std::uint16_t scale_channel(std::uint16_t value, int weight) {
    const int scaled = (static_cast<int>(value) * std::clamp(weight, 0, 256) + 128) >> 8;
    return static_cast<std::uint16_t>(std::clamp(scaled, 0, 255));
}

std::uint16_t scale565(std::uint16_t pixel, int weight) {
    const auto red = scale_channel((pixel >> 11) & 0x1f, weight) & 0x1f;
    const auto green = scale_channel((pixel >> 5) & 0x3f, weight) & 0x3f;
    const auto blue = scale_channel(pixel & 0x1f, weight) & 0x1f;
    return static_cast<std::uint16_t>((red << 11) | (green << 5) | blue);
}

// One RGB565 channel pair, 0..256 weight on `to`.
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

void blank(const Rgb16Surface& like, Rgb16Surface& out) {
    out.width = like.width;
    out.height = like.height;
    out.pixels.assign(like.pixels.size(), 0);
}

}  // namespace

TransitionKind transition_kind(std::uint16_t code) {
    switch (code) {
        case 0x65: return TransitionKind::fade8;
        case 0x66: return TransitionKind::interleave27;
        case 0x67: return TransitionKind::blink;
        case 0x68: return TransitionKind::fade4;
        case 0x69: return TransitionKind::none;
        case 0x6a: return TransitionKind::reveal;
        default: return TransitionKind::none;
    }
}

int transition_steps(TransitionKind kind) {
    switch (kind) {
        case TransitionKind::fade8: return 8;
        case TransitionKind::interleave27: return kInterleavePhases;
        case TransitionKind::blink: return 4 + 7;  // 0x4196a4 then 0x419a51
        case TransitionKind::fade4: return 4;
        case TransitionKind::reveal: return 7;
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
        case TransitionKind::reveal: {
            // alpha = 0x7f, 0x3f, 0x1f, 0xf, 7, 3, 1; the source share is
            // 0x100 - alpha.
            static const int weights[7] = {0x81, 0xc1, 0xe1, 0xf1, 0xf9, 0xfd, 0xff};
            return weights[std::clamp(index, 0, 6)];
        }
        case TransitionKind::blink: {
            // 0x4196a4 dims the departing picture with alpha 0x1f,0x3f,0x7f,0xff
            // (brightness 0xe1,0xc1,0x81,0x00), then 0x419a51 reveals.
            static const int dim[4] = {0xe1, 0xc1, 0x81, 0x00};
            if (index < 4) return dim[std::clamp(index, 0, 3)];
            return transition_weight(TransitionKind::reveal, index - 4);
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

void dim_playfield(Rgb16Surface& out, const Rgb16Surface& from, int weight) {
    out.width = from.width;
    out.height = from.height;
    out.pixels.resize(from.pixels.size());
    for (std::size_t index = 0; index < from.pixels.size(); ++index) {
        out.pixels[index] = scale565(from.pixels[index], weight);
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

void reveal_phase(Rgb16Surface& out, const Rgb16Surface& to, int phase, int weight) {
    if (to.width != out.width || to.height != out.height) return;
    if (out.pixels.size() != to.pixels.size()) return;
    const int column_parity = phase & 1;
    const int row_parity = (phase >> 1) & 1;
    for (int row = row_parity; row < out.height; row += 2) {
        const auto base = static_cast<std::size_t>(row) * out.width;
        for (int column = column_parity; column < out.width; column += 2) {
            out.pixels[base + static_cast<std::size_t>(column)] =
                scale565(to.pixels[base + static_cast<std::size_t>(column)], weight);
        }
    }
}

TransitionPlayer::TransitionPlayer(TransitionKind kind, const Rgb16Surface& from,
                                   const Rgb16Surface& to)
    : kind_(kind), from_(from), to_(to) {
    if (kind_ == TransitionKind::interleave27) {
        surface_ = from_;
    } else if (kind_ == TransitionKind::blink) {
        surface_ = from_;
    } else if (kind_ == TransitionKind::reveal) {
        blank(to_, surface_);
    } else {
        surface_ = from_;
    }
}

void TransitionPlayer::next() {
    if (done()) return;
    const int index = step_;
    switch (kind_) {
        case TransitionKind::fade8:
        case TransitionKind::fade4:
            blend_playfield(surface_, from_, to_, transition_weight(kind_, index));
            break;
        case TransitionKind::interleave27:
            interleave_playfield(surface_, to_, index % kInterleavePhases);
            break;
        case TransitionKind::blink:
            if (index < 4) {
                // 0x4196a4: the departing picture dims to black, in place.
                dim_playfield(surface_, from_, transition_weight(kind_, index));
            } else {
                if (index == 4) blank(to_, surface_);  // 0x419a51 clears first
                reveal_phase(surface_, to_, (index - 4) % 4,
                             transition_weight(kind_, index));
            }
            break;
        case TransitionKind::reveal:
            reveal_phase(surface_, to_, index % 4, transition_weight(kind_, index));
            break;
        case TransitionKind::none:
            break;
    }
    ++step_;
}

}  // namespace byzantine
