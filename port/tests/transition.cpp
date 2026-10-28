#include "byzantine/transition.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace {

int failures = 0;

void check(bool condition, const std::string& what) {
    if (condition) return;
    std::fprintf(stderr, "FAIL: %s\n", what.c_str());
    ++failures;
}

void check_equal(long long got, long long want, const std::string& what) {
    if (got == want) return;
    std::fprintf(stderr, "FAIL: %s (got %lld, want %lld)\n", what.c_str(), got, want);
    ++failures;
}

byzantine::Rgb16Surface plain_surface(std::uint16_t colour, int width = 8,
                                     int height = 4) {
    byzantine::Rgb16Surface surface;
    surface.width = width;
    surface.height = height;
    surface.pixels.assign(static_cast<std::size_t>(width) * height, colour);
    return surface;
}

// RGB565 channels of a pixel.
int red(std::uint16_t pixel) { return (pixel >> 11) & 0x1f; }
int green(std::uint16_t pixel) { return (pixel >> 5) & 0x3f; }
int blue(std::uint16_t pixel) { return pixel & 0x1f; }

}  // namespace

int main() {
    using namespace byzantine;

    // 0x40186e's switch: only these six codes reach a blend routine.
    check(transition_kind(0x65) == TransitionKind::fade8, "0x65 is the 8-step fade");
    check(transition_kind(0x66) == TransitionKind::interleave27,
          "0x66 is the 27-phase reveal");
    check(transition_kind(0x67) == TransitionKind::fade7, "0x67 is the 7-step fade");
    check(transition_kind(0x68) == TransitionKind::fade4, "0x68 is the 4-step fade");
    check(transition_kind(0x69) == TransitionKind::none, "0x69 draws nothing");
    check(transition_kind(0x6a) == TransitionKind::fade7, "0x6a is 0x419a51");
    // The loader's default (100) and anything outside the table is no blend.
    check(transition_kind(100) == TransitionKind::none, "100 is no transition");
    check(transition_kind(0) == TransitionKind::none, "0 is no transition");
    check(transition_kind(0x6b) == TransitionKind::none, "0x6b is outside the table");

    // Step counts and pacing: 0x41920f waits 60 ms, the others present.
    check_equal(transition_steps(TransitionKind::fade8), 8, "fade8 steps");
    check_equal(transition_steps(TransitionKind::fade4), 4, "fade4 steps");
    check_equal(transition_steps(TransitionKind::fade7), 7, "fade7 steps");
    check_equal(transition_steps(TransitionKind::interleave27), 27,
                "interleave steps");
    check_equal(transition_steps(TransitionKind::none), 0, "none steps");
    check_equal(transition_step_ms(TransitionKind::fade4), 60, "fade4 pacing");
    check_equal(transition_step_ms(TransitionKind::fade8), 16, "fade8 pacing");

    // 0x41920f's `local_14` and 0x419a51's halving alpha.
    check_equal(transition_weight(TransitionKind::fade4, 0), 2, "fade4 weight 0");
    check_equal(transition_weight(TransitionKind::fade4, 1), 8, "fade4 weight 1");
    check_equal(transition_weight(TransitionKind::fade4, 2), 32, "fade4 weight 2");
    check_equal(transition_weight(TransitionKind::fade4, 3), 128, "fade4 weight 3");
    check_equal(transition_weight(TransitionKind::fade7, 0), 0x81, "fade7 weight 0");
    check_equal(transition_weight(TransitionKind::fade7, 6), 0xff, "fade7 weight 6");
    check_equal(transition_weight(TransitionKind::fade8, 0), 32, "fade8 weight 0");
    check_equal(transition_weight(TransitionKind::fade8, 7), 256, "fade8 weight 7");

    // blend_playfield: the endpoints are exact and the middle is a real mix.
    const auto black = plain_surface(0x0000);
    const auto white = plain_surface(0xffff);
    Rgb16Surface out;
    blend_playfield(out, black, white, 0);
    check(out.pixels.size() == black.pixels.size(), "blend keeps the size");
    check(out.pixels[0] == 0x0000, "weight 0 is the departing picture");
    blend_playfield(out, black, white, 256);
    check(out.pixels[0] == 0xffff, "weight 256 is exactly the new picture");
    blend_playfield(out, black, white, 128);
    check_equal(red(out.pixels[0]), 16, "half-blend red channel (31/2)");
    check_equal(green(out.pixels[0]), 32, "half-blend green channel (63/2)");
    check_equal(blue(out.pixels[0]), 16, "half-blend blue channel (31/2)");

    // Channels must not bleed into each other: red -> blue keeps green at 0.
    const auto pure_red = plain_surface(0xf800);
    const auto pure_blue = plain_surface(0x001f);
    blend_playfield(out, pure_red, pure_blue, 128);
    check_equal(red(out.pixels[0]), 16, "red fades out of a red pixel");
    check_equal(blue(out.pixels[0]), 16, "blue fades into a red pixel");
    check_equal(green(out.pixels[0]), 0, "green stays zero");
    // Monotonic: every step moves the pixel closer to the new picture.
    const auto dark = plain_surface(0x0000);
    const auto bright = plain_surface(0xffff);
    int previous = -1;
    for (int step = 0; step < transition_steps(TransitionKind::fade8); ++step) {
        blend_playfield(out, dark, bright,
                        transition_weight(TransitionKind::fade8, step));
        check(red(out.pixels[0]) >= previous, "fade8 is monotonic");
        previous = red(out.pixels[0]);
    }
    check_equal(previous, 31, "the last fade8 step reaches the new picture");

    // A size mismatch must not blend unrelated pixels.
    const auto odd = plain_surface(0xffff, 4, 2);
    blend_playfield(out, black, odd, 128);
    check(out.pixels.size() == black.pixels.size(), "mismatch keeps the size");
    check(out.pixels[0] == 0x0000, "mismatch keeps the departing picture");

    // 0x42bc8b: phase k copies exactly the indices i % 27 == k, and 27 phases
    // leave the new playfield.
    const auto old_field = plain_surface(0x0000, 640, 400);
    const auto new_field = plain_surface(0xffff, 640, 400);
    Rgb16Surface reveal = old_field;
    blend_playfield(reveal, old_field, new_field, 0);
    for (int phase = 0; phase < kInterleavePhases; ++phase) {
        const auto before = reveal.pixels;
        interleave_playfield(reveal, new_field, phase);
        std::size_t changed = 0;
        std::size_t wrong = 0;
        for (std::size_t index = 0; index < reveal.pixels.size(); ++index) {
            if (reveal.pixels[index] == before[index]) continue;
            ++changed;
            if (index % static_cast<std::size_t>(kInterleavePhases) !=
                static_cast<std::size_t>(phase)) {
                ++wrong;
            }
            if (reveal.pixels[index] != 0xffff) ++wrong;
        }
        if (changed == 0 || wrong != 0) {
            check(false, "phase " + std::to_string(phase) +
                             " copied the wrong pixels");
        }
        const std::size_t expected =
            (reveal.pixels.size() + kInterleavePhases - 1 - phase) /
            static_cast<std::size_t>(kInterleavePhases);
        if (changed != expected) {
            check(false, "phase " + std::to_string(phase) + " copied " +
                             std::to_string(changed) + " pixels, expected " +
                             std::to_string(expected));
        }
    }
    check(reveal.pixels == new_field.pixels, "27 phases reach the new playfield");

    // Out-of-range phases are ignored rather than touching memory.
    Rgb16Surface untouched = old_field;
    interleave_playfield(untouched, new_field, -1);
    interleave_playfield(untouched, new_field, kInterleavePhases);
    check(untouched.pixels == old_field.pixels, "out-of-range phase is ignored");

    if (failures != 0) {
        std::fprintf(stderr, "transition-test: %d failures\n", failures);
        return 1;
    }
    std::printf("transition-test ok\n");
    return 0;
}
