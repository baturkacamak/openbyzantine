#ifndef BYZANTINE_FRAME_PACING_HPP
#define BYZANTINE_FRAME_PACING_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace byzantine {

// The interactive loop draws the record pass (0x43b216) once per composed
// frame, and 0x43bb77 steps a record's frame list once per drawn pass. The
// drawn-frame rate therefore decides how fast an animation plays.
//
// Observed in the target: 0x43b216 gates a pass with
// `0x10 < GetTickCount() - _DAT_0046187c` and then stores the *delta* back
// into _DAT_0046187c (objdump 0x43b2dc..0x43b33a: `call [0x4a9444]` =
// GetTickCount, `sub ecx,[0x46187c]`, `cmp [0x47ae58],0x11`, `jae`,
// `mov [0x46187c],eax` with eax = the delta). Because the loop is uncapped
// (0x42d594: pump messages, render, run one VM command) the stored delta makes
// the next comparison large, so the gate stops throttling after the first
// passes: the executable's animation rate is its raw pass rate, which is
// machine dependent. The 16 ms figure is a host model, not an original rule.
//
// The host keeps two clocks:
//
// * A 16 ms render tick (`FramePacer`): compose and present at most once per
//   tick, so the host does not burn CPU on a scene that barely changes.
// * A record-pass clock (`PassClock`): the original's draw+step rate. It is not
//   the render rate; the passes that elapsed between two host frames are
//   applied to the records the last compose drew (`advance_record_passes`).
//   The reference rate is a host-model value (`--anim-pass-hz`, default 60):
//   the shipped delays are pass-rate divisors and read as a 60 Hz tick scale
//   (the record default `+0x04` is 10 in 0x410021, so 5.5 steps/s), not the
//   250 Hz that was chosen for one TK delay-20 record. See
//   ANIMATION_RATE_GLOBAL.md. The effective step rate is pass_hz/(delay+1).
class FramePacer {
public:
    static constexpr std::uint64_t tick_ms = 16;
    static constexpr std::size_t max_catch_up = 4;

    explicit FramePacer(std::uint64_t start_ms = 0) : next_(start_ms) {}

    // Record passes due at now_ms: none before the next tick, and at most
    // max_catch_up so a long stall cannot replay the whole backlog.
    std::size_t due(std::uint64_t now_ms) const {
        if (now_ms < next_) return 0;
        const auto ticks = (now_ms - next_) / tick_ms + 1;
        return static_cast<std::size_t>(std::min<std::uint64_t>(ticks, max_catch_up));
    }

    // Account for `passes` drawn record passes at now_ms. A stall longer than
    // one tick after the catch-up window (scene load, conversation, media clip)
    // is not animation debt: restart the deadline at now_ms instead of
    // replaying it frame by frame.
    void consumed(std::uint64_t now_ms, std::size_t passes) {
        if (passes == 0) return;
        next_ += static_cast<std::uint64_t>(passes) * tick_ms;
        if (now_ms > next_ + tick_ms) next_ = now_ms;
    }

    // Milliseconds to wait before the next pass is due (0 when overdue).
    std::uint64_t wait_ms(std::uint64_t now_ms) const {
        return now_ms < next_ ? next_ - now_ms : 0;
    }

    std::uint64_t next_tick_ms() const { return next_; }

private:
    std::uint64_t next_;
};

// The record-pass clock. 0x43b216 has no frame cap (see above), so its rate is
// a property of the machine, not of a script: the shipped `+0x04` delays are
// divisors of that pass rate. The host renders at 16 ms but must not tie the
// animation to the compose cost, so it keeps the pass rate as its own clock and
// applies the passes that elapsed to the records the last compose drew.
// `passes_per_second` is the host's reference for the original pass rate; the
// default is a host-model value (see ANIMATION_RATE_GLOBAL.md), overridable
// with `--anim-pass-hz`.
class PassClock {
public:
    PassClock(std::uint64_t start_ms, unsigned passes_per_second)
        : start_ms_(start_ms), hz_(passes_per_second == 0 ? 1 : passes_per_second) {}

    // Record passes the clock has covered since start (monotonic).
    std::uint64_t owed(std::uint64_t now_ms) const {
        if (now_ms <= start_ms_) return 0;
        return (now_ms - start_ms_) * hz_ / 1000;
    }
    // Passes owed but not applied yet.
    std::uint64_t remaining(std::uint64_t now_ms) const {
        const auto covered = owed(now_ms);
        return covered > applied_ ? covered - applied_ : 0;
    }
    void applied(std::uint64_t passes) { applied_ += passes; }
    std::uint64_t applied_total() const { return applied_; }
    // Drop the outstanding debt (no animation running, or a long stall).
    void skip_to(std::uint64_t now_ms) { applied_ = owed(now_ms); }

private:
    std::uint64_t start_ms_;
    unsigned hz_;
    std::uint64_t applied_ = 0;
};

}  // namespace byzantine

#endif
