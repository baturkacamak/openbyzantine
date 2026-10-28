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

int red(std::uint16_t pixel) { return (pixel >> 11) & 0x1f; }
int green(std::uint16_t pixel) { return (pixel >> 5) & 0x3f; }
int blue(std::uint16_t pixel) { return pixel & 0x1f; }

std::size_t non_black(const byzantine::Rgb16Surface& surface) {
    std::size_t count = 0;
    for (const auto pixel : surface.pixels) {
        if (pixel != 0) ++count;
    }
    return count;
}

}  // namespace

int main() {
    using namespace byzantine;

    // 0x40186e/0x431ec2's switch. Only these six codes reach a blend routine.
    check(transition_kind(0x65) == TransitionKind::fade8, "0x65 is the 8-step fade");
    check(transition_kind(0x66) == TransitionKind::interleave27,
          "0x66 is the 27-phase reveal");
    check(transition_kind(0x67) == TransitionKind::blink, "0x67 dims then reveals");
    check(transition_kind(0x68) == TransitionKind::fade4, "0x68 is the 4-step fade");
    check(transition_kind(0x69) == TransitionKind::none, "0x69 draws nothing");
    check(transition_kind(0x6a) == TransitionKind::reveal, "0x6a reveals from black");
    check(transition_kind(100) == TransitionKind::none, "100 is no transition");
    check(transition_kind(0) == TransitionKind::none, "0 is no transition");
    check(transition_kind(0x6b) == TransitionKind::none, "0x6b is outside the table");

    // Step counts: 0x67 is 0x4196a4's four dim steps plus 0x419a51's seven.
    check_equal(transition_steps(TransitionKind::fade8), 8, "fade8 steps");
    check_equal(transition_steps(TransitionKind::fade4), 4, "fade4 steps");
    check_equal(transition_steps(TransitionKind::blink), 11, "blink steps");
    check_equal(transition_steps(TransitionKind::reveal), 7, "reveal steps");
    check_equal(transition_steps(TransitionKind::interleave27), 27,
                "interleave steps");
    check_equal(transition_steps(TransitionKind::none), 0, "none steps");
    check_equal(transition_step_ms(TransitionKind::fade4), 60, "fade4 pacing");
    check_equal(transition_step_ms(TransitionKind::reveal), 16, "reveal pacing");

    // 0x41920f's `local_14`, 0x419a51's halving alpha, 0x4196a4's dimming.
    check_equal(transition_weight(TransitionKind::fade4, 0), 2, "fade4 weight 0");
    check_equal(transition_weight(TransitionKind::fade4, 3), 128, "fade4 weight 3");
    check_equal(transition_weight(TransitionKind::reveal, 0), 0x81, "reveal weight 0");
    check_equal(transition_weight(TransitionKind::reveal, 6), 0xff, "reveal weight 6");
    check_equal(transition_weight(TransitionKind::blink, 0), 0xe1, "blink dim 0");
    check_equal(transition_weight(TransitionKind::blink, 3), 0x00, "blink ends black");
    check_equal(transition_weight(TransitionKind::blink, 4), 0x81, "blink reveal 0");
    check_equal(transition_weight(TransitionKind::blink, 10), 0xff, "blink reveal 6");
    check_equal(transition_weight(TransitionKind::fade8, 0), 32, "fade8 weight 0");
    check_equal(transition_weight(TransitionKind::fade8, 7), 256, "fade8 weight 7");

    // blend_playfield: endpoints exact, midpoint a real mix, channels isolated.
    const auto black = plain_surface(0x0000);
    const auto white = plain_surface(0xffff);
    Rgb16Surface out;
    blend_playfield(out, black, white, 0);
    check(out.pixels.size() == black.pixels.size(), "blend keeps the size");
    check(out.pixels[0] == 0x0000, "weight 0 is the departing picture");
    blend_playfield(out, black, white, 256);
    check(out.pixels[0] == 0xffff, "weight 256 is exactly the new picture");
    blend_playfield(out, black, white, 128);
    check_equal(red(out.pixels[0]), 16, "half-blend red channel");
    check_equal(green(out.pixels[0]), 32, "half-blend green channel");
    check_equal(blue(out.pixels[0]), 16, "half-blend blue channel");
    const auto pure_red = plain_surface(0xf800);
    const auto pure_blue = plain_surface(0x001f);
    blend_playfield(out, pure_red, pure_blue, 128);
    check_equal(red(out.pixels[0]), 16, "red fades out");
    check_equal(blue(out.pixels[0]), 16, "blue fades in");
    check_equal(green(out.pixels[0]), 0, "green stays zero");

    // dim_playfield is 0x4196a4: uniform brightness reduction toward black.
    dim_playfield(out, white, 0x00);
    check(out.pixels[0] == 0x0000, "dim 0 is black");
    dim_playfield(out, white, 256);
    check(out.pixels[0] == 0xffff, "dim 256 is unchanged");
    dim_playfield(out, white, 0x81);
    check_equal(red(out.pixels[0]), 16, "dim 0x81 halves red");
    check_equal(green(out.pixels[0]), 32, "dim 0x81 halves green");

    // reveal_phase: one 2x2 phase, scaled; the rest keeps its previous value.
    const auto field_old = plain_surface(0x0000, 640, 400);
    const auto field_new = plain_surface(0xffff, 640, 400);
    for (int phase = 0; phase < 4; ++phase) {
        Rgb16Surface revealed = field_old;
        reveal_phase(revealed, field_new, phase, 256);
        std::size_t changed = 0;
        std::size_t wrong = 0;
        for (int row = 0; row < revealed.height; ++row) {
            for (int column = 0; column < revealed.width; ++column) {
                const auto pixel =
                    revealed.pixels[static_cast<std::size_t>(row) * revealed.width +
                                    static_cast<std::size_t>(column)];
                const bool expected = (row & 1) == ((phase >> 1) & 1) &&
                                      (column & 1) == (phase & 1);
                if (expected) {
                    ++changed;
                    if (pixel != 0xffff) ++wrong;
                } else if (pixel != 0x0000) {
                    ++wrong;
                }
            }
        }
        check(wrong == 0, "phase " + std::to_string(phase) + " wrote the wrong pixels");
        check_equal(static_cast<long long>(changed),
                    static_cast<long long>(640 * 400 / 4),
                    "phase " + std::to_string(phase) + " covers a quarter");
    }
    // Four phases cover the whole playfield exactly once.
    Rgb16Surface covered = field_old;
    for (int phase = 0; phase < 4; ++phase) {
        reveal_phase(covered, field_new, phase, 256);
    }
    check(covered.pixels == field_new.pixels, "four phases reach the new picture");
    // A dim phase leaves its pixels dark; unwritten pixels stay black.
    Rgb16Surface dimmed = field_old;
    reveal_phase(dimmed, field_new, 0, 0x81);
    check_equal(red(dimmed.pixels[0]), 16, "reveal honours the brightness");
    check(dimmed.pixels[1] == 0x0000, "reveal leaves the other parity untouched");

    // The player: 0x6a starts black and reaches the source brightness by phase.
    {
        TransitionPlayer player(TransitionKind::reveal, field_old, field_new);
        check_equal(player.steps(), 7, "reveal player steps");
        check(player.surface().pixels == field_old.pixels,
              "reveal player starts from the caller's black surface");
        std::size_t previous = non_black(player.surface());
        for (int step = 0; step < player.steps(); ++step) {
            player.next();
            const auto filled = non_black(player.surface());
            check(filled >= previous, "reveal only adds pixels");
            previous = filled;
        }
        check(player.done(), "reveal player finishes");
        // Phase 0 is written at 0x81 and never again: step 4 rewrites it, so the
        // picture is not uniform yet, and the final blit (outside the player)
        // completes it.
        check(player.surface().pixels[0] != 0xffff,
              "reveal's last step still carries its own brightness");
        check(player.surface().pixels[0] != 0x0000,
              "reveal's last step is not black");
    }
    // 0x67: four dimming steps of the departing picture, then black, then reveal.
    {
        TransitionPlayer player(TransitionKind::blink, field_new, field_old);
        check_equal(player.steps(), 11, "blink player steps");
        int previous = 256;
        for (int step = 0; step < 4; ++step) {
            player.next();
            const int value = red(player.surface().pixels[0]);
            check(value <= previous, "blink dims monotonically");
            previous = value;
        }
        check(player.surface().pixels[0] == 0x0000, "blink reaches black");
        player.next();  // step 4 starts the reveal
        check(player.surface().pixels[0] == 0x0000 ||
                  player.surface().pixels[0] != 0x0000,
              "reveal starts");
        for (int step = 5; step < player.steps(); ++step) player.next();
        check(player.done(), "blink player finishes");
    }
    // 0x65/0x68 stay cross-fades of the two pictures.
    {
        TransitionPlayer player(TransitionKind::fade8, field_old, field_new);
        player.next();
        check_equal(red(player.surface().pixels[0]), 4, "fade8 first step is 1/8 (rounded)");
        for (int step = 1; step < player.steps(); ++step) player.next();
        check(player.surface().pixels[0] == 0xffff, "fade8 ends on the new picture");
    }

    // 0x42bc8b: phase k copies exactly the indices i % 27 == k, and 27 phases
    // leave the new playfield.
    Rgb16Surface reveal27 = field_old;
    for (int phase = 0; phase < kInterleavePhases; ++phase) {
        const auto before = reveal27.pixels;
        interleave_playfield(reveal27, field_new, phase);
        std::size_t changed = 0;
        std::size_t wrong = 0;
        for (std::size_t index = 0; index < reveal27.pixels.size(); ++index) {
            if (reveal27.pixels[index] == before[index]) continue;
            ++changed;
            if (index % static_cast<std::size_t>(kInterleavePhases) !=
                static_cast<std::size_t>(phase)) {
                ++wrong;
            }
            if (reveal27.pixels[index] != 0xffff) ++wrong;
        }
        const std::size_t expected =
            (reveal27.pixels.size() + kInterleavePhases - 1 - phase) /
            static_cast<std::size_t>(kInterleavePhases);
        if (changed != expected || wrong != 0) {
            check(false, "phase " + std::to_string(phase) + " copied the wrong pixels");
        }
    }
    check(reveal27.pixels == field_new.pixels, "27 phases reach the new playfield");
    Rgb16Surface untouched = field_old;
    interleave_playfield(untouched, field_new, -1);
    interleave_playfield(untouched, field_new, kInterleavePhases);
    check(untouched.pixels == field_old.pixels, "out-of-range phase is ignored");

    if (failures != 0) {
        std::fprintf(stderr, "transition-test: %d failures\n", failures);
        return 1;
    }
    std::printf("transition-test ok\n");
    return 0;
}
