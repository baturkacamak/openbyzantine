#pragma once
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <optional>
#include <cstdint>
struct SDL_Window;
namespace byzantine {
struct WindowSize { int width, height; };
// Exact 4:3 client sizes; follow whichever edge changed proportionally more.
inline WindowSize aspect_window_size(WindowSize before, WindowSize requested,
    WindowSize limit = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()}) {
    const auto dw = std::llabs(static_cast<long long>(requested.width) - before.width);
    const auto dh = std::llabs(static_cast<long long>(requested.height) - before.height);
    const bool width_driven = dw * std::max(1, before.height) >= dh * std::max(1, before.width);
    int units = width_driven ? (requested.width + 2) / 4 : (requested.height + 1) / 3;
    const int max_units = std::max(1, std::min(limit.width / 4, limit.height / 3));
    units = std::clamp(units, std::min(80, max_units), max_units);
    return {units * 4, units * 3};
}
// Window managers round rational aspect constraints to device pixels. Treat
// a one-pixel discrepancy as valid instead of snapping it during a live drag.
inline bool near_game_aspect(WindowSize size) {
    return std::llabs(3LL * size.width - 4LL * size.height) <= 4;
}
class AspectResizeSettler {
public:
    static constexpr std::uint64_t quiet_ms = 180;
    bool pending() const { return pending_; }
    void cancel() { pending_ = false; }
    void resize(WindowSize before, std::uint64_t now, bool native) {
        if (native) { cancel(); return; } // The WM owns interactive aspect sizing.
        if (!pending_) before_ = before;
        pending_ = true;
        changed_ = now;
    }
    std::optional<WindowSize> take(std::uint64_t now, bool pointer_down) {
        if (!pending_ || pointer_down || now - changed_ < quiet_ms) return std::nullopt;
        pending_ = false;
        return before_;
    }
private:
    bool pending_ = false;
    std::uint64_t changed_ = 0;
    WindowSize before_{640,480};
};
// Native WM constraint where supported; common SDL resize correction is the
// portable fallback, so lack of an X11 backend never disables aspect fitting.
bool set_window_aspect_hint(SDL_Window* window, bool enabled);
} // namespace byzantine
