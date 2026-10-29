// Host-only notebook help animation, not a recovered game transition.
#pragma once
#include "byzantine/compose.hpp"
#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

namespace byzantine {
class MenuHelpFade {
public:
    static constexpr std::uint64_t duration_ms = 220;
    static constexpr int left = 48, top = 54, width = 196, height = 300;

    bool active() const { return active_; }
    void reset() { from_.clear(); target_.clear(); shown_.clear(); active_ = false; }

    // Called after drawing the desired menu and before the existing presenter.
    // Only the left-page help rectangle is composited. New input retargets from
    // the last displayed pixels, so rapid hovering cannot queue or jump fades.
    // Returns true only when an animated target change begins.
    bool compose(Rgb16Surface& screen, std::uint64_t now, bool animate) {
        if (!screen.inside(left + width - 1, top + height - 1)) {
            reset();
            return false;
        }
        std::vector<std::uint16_t> wanted;
        wanted.reserve(width * height);
        for (int y = top; y < top + height; ++y)
            for (int x = left; x < left + width; ++x) wanted.push_back(screen.at(x, y));
        if (target_.empty() || !animate) {
            target_ = shown_ = std::move(wanted);
            active_ = false;
            return false;
        }
        const bool changed = wanted != target_;
        if (changed) {
            from_ = shown_;
            target_ = std::move(wanted);
            started_ = now;
            active_ = true;
        }
        if (!active_) return false;
        const double t = std::min(1.0, static_cast<double>(now - started_) / duration_ms);
        const int weight = static_cast<int>(256.0 * t * t * (3.0 - 2.0 * t) + 0.5);
        for (std::size_t i = 0; i < target_.size(); ++i) {
            const auto a = from_[i], b = target_[i];
            const auto channel = [&](int shift, int mask) {
                return ((((a >> shift) & mask) * (256 - weight) +
                         ((b >> shift) & mask) * weight + 128) >> 8) << shift;
            };
            shown_[i] = static_cast<std::uint16_t>(channel(11, 31) |
                                                   channel(5, 63) | channel(0, 31));
        }
        std::size_t i = 0;
        for (int y = top; y < top + height; ++y)
            for (int x = left; x < left + width; ++x) screen.at(x, y) = shown_[i++];
        if (t >= 1.0) active_ = false;
        return changed;
    }

private:
    std::vector<std::uint16_t> from_, target_, shown_;
    std::uint64_t started_ = 0;
    bool active_ = false;
};
} // namespace byzantine
