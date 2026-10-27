// PanoramaView: 0x4242bd's sampling and 0x4240ae's inverse mouse mapping must
// agree. The synthetic source encodes its own (row, column) in every pixel, so
// a rendered pixel can be checked against the coordinate the inverse map
// returns for the same viewport point.
#include "byzantine/panorama_view.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

constexpr int kWidth = 256;   // column fits in the low byte of a pixel
constexpr int kHeight = 96;   // row fits in the high byte
constexpr int kViewWidth = 64;
constexpr int kViewHeight = 40;

std::vector<std::uint16_t> make_source() {
    std::vector<std::uint16_t> pixels(
        static_cast<std::size_t>(kWidth) * static_cast<std::size_t>(kHeight));
    for (int row = 0; row < kHeight; ++row) {
        for (int column = 0; column < kWidth; ++column) {
            pixels[static_cast<std::size_t>(row) * kWidth + column] =
                static_cast<std::uint16_t>((row << 8) | column);
        }
    }
    return pixels;
}

// Every viewport pixel must sample exactly the source pixel the inverse map
// reports for it.
void check_round_trip(const byzantine::PanoramaView& view, const char* where) {
    auto target = byzantine::make_surface(kViewWidth, kViewHeight, 0xffff);
    view.render(target);
    for (int y = 0; y < kViewHeight; ++y) {
        for (int x = 0; x < kViewWidth; ++x) {
            const auto point = view.canvas_point(x, y);
            if (!point.mapped || point.source_x < 0 || point.source_x >= kWidth ||
                point.source_y < 0 || point.source_y >= kHeight) {
                throw std::runtime_error(std::string("inverse map out of range at ") +
                                         where);
            }
            const auto pixel = target.at(x, y);
            if (static_cast<int>(pixel >> 8) != point.source_y ||
                static_cast<int>(pixel & 0xff) != point.source_x) {
                throw std::runtime_error(std::string("inverse map disagrees with "
                                                     "sampling at ") +
                                         where);
            }
        }
    }
}

// 0x423e03 must be the forward direction of the same projection: a source pixel
// the sampler shows must project back to where it is actually painted on screen.
// The rendered synthetic source encodes its own (row, column), so the expected
// viewport position of every visible source pixel is read from the frame itself;
// when the seam padding paints one source column twice, the copy nearest the
// viewport centre is the one 0x423e03's angle normalisation selects. The
// sampler's table arithmetic and this analytic inverse are not bit-identical
// (host libm vs the original's x87/table path), so up to two pixels of
// difference are allowed.
void check_projection_matches_render(const byzantine::PanoramaView& view,
                                     const char* where) {
    auto target = byzantine::make_surface(view.viewport_width(),
                                         view.viewport_height(), 0xffff);
    view.render(target);
    const int centre_x = view.viewport_width() / 2;
    const int centre_y = view.viewport_height() / 2;
    struct Where {
        int x = 0;
        int y = 0;
        int distance = 0;
        bool set = false;
    };
    std::map<std::pair<int, int>, Where> nearest;
    for (int y = 0; y < view.viewport_height(); ++y) {
        for (int x = 0; x < view.viewport_width(); ++x) {
            const auto pixel = target.at(x, y);
            const auto key = std::make_pair(static_cast<int>(pixel & 0xff),
                                            static_cast<int>(pixel >> 8));
            const int distance = std::abs(x - centre_x) + std::abs(y - centre_y);
            auto& entry = nearest[key];
            if (!entry.set || distance < entry.distance) {
                entry = Where{x, y, distance, true};
            }
        }
    }
    std::size_t checked = 0;
    for (const auto& [source, screen] : nearest) {
        // The sampler rounds the outermost columns/rows up to a bit past the
        // viewport bound 0x423e03 tests, so the very border is not comparable.
        if (screen.x < 2 || screen.x >= view.viewport_width() - 2 || screen.y < 2 ||
            screen.y >= view.viewport_height() - 2) {
            continue;
        }
        int projected_x = 0;
        int projected_y = 0;
        if (!view.project_source(source.first, source.second, projected_x,
                                 projected_y)) {
            throw std::runtime_error(std::string("0x423e03 rejected a painted "
                                                 "source pixel at ") +
                                     where);
        }
        if (std::abs(projected_x - screen.x) > 2 ||
            std::abs(projected_y - screen.y) > 2) {
            throw std::runtime_error(std::string("0x423e03 is not where the sampler "
                                                 "paints the source pixel at ") +
                                     where);
        }
        ++checked;
    }
    require(checked > 200, "projection round trip checked too few pixels");
}
}  // namespace

int main() {
    try {
        const auto source = make_source();
        byzantine::PanoramaView view("synthetic", kWidth, kHeight, source,
                                     kViewWidth, kViewHeight);
        require(view.source_width() == kWidth && view.source_height() == kHeight &&
                    view.viewport_width() == kViewWidth &&
                    view.viewport_height() == kViewHeight,
                "panorama geometry mismatch");
        // 0x4236b5 builds [pad | source | pad] rows; the padding comes from the
        // recovered 0x423d34 formula, so it is a real value here, not a default.
        require(view.padding() == 34 && view.stride_words() == kWidth + 68 &&
                    view.padded_rows().size() ==
                        static_cast<std::size_t>(view.stride_words()) * kHeight,
                "panorama seam rows mismatch");
        // 0x4241e2 resets the camera at yaw = +pi (180 degrees).
        require(std::abs(view.yaw_degrees() - 180.0) < 0.001 && view.pitch_degrees() == 0,
                "panorama reset camera mismatch");

        check_round_trip(view, "reset");
        check_projection_matches_render(view, "reset");

        // 0x4242bd: a pure yaw delta does not rebuild the map, it only moves the
        // flat yaw offset, so the inverse mapping must follow it.
        require(!view.advance(90, 0, 1), "yaw-only advance rebuilt the map");
        // 0x4242bd wraps the yaw into [-pi, pi], so 180 + 90 degrees is -90.
        require(std::abs(view.yaw_degrees() + 90.0) < 0.001, "yaw delta mismatch");
        check_round_trip(view, "yaw");
        check_projection_matches_render(view, "yaw");

        // A pitch delta regenerates the map (0x423a37) and must stay invertible.
        // 0x4242bd SUBTRACTS the vertical delta from the stored pitch.
        require(view.advance(0, 5, 1), "pitch advance did not rebuild the map");
        require(std::abs(view.pitch_degrees() + 5.0) < 0.001,
                "pitch delta sign mismatch");
        check_round_trip(view, "pitch");
        check_projection_matches_render(view, "pitch");

        // 0x401ddf: the staged pixel position becomes a 0..360 degree delta; the
        // caller then advances the camera (0x42423d) and draws. The staged
        // position is ABSOLUTE: 0x401ddf resets first (0x4241e2, yaw = +pi).
        view.advance(0, -5, 1);  // back to pitch 0
        view.advance(70, 0, 1);  // any previous view must not leak into a reset
        const auto setup = view.apply_staged_x(128, false);
        require(setup.horizontal_degrees == 180 && setup.vertical_degrees == 0 &&
                    setup.reset_projection,
                "staged position conversion mismatch");
        require(std::abs(view.yaw_degrees()) < 0.001 && view.pitch_degrees() == 0,
                "a staged position must reset the camera (0x4241e2)");
        check_round_trip(view, "staged");
        check_projection_matches_render(view, "staged");
        // The same staged position must land in the same view from any state.
        const auto staged_yaw = view.yaw_degrees();
        view.advance(-95, 3, 1);
        view.apply_staged_x(128, false);
        require(std::abs(view.yaw_degrees() - staged_yaw) < 1e-9,
                "staged position is not absolute across camera states");

        // Nothing staged: 0x401ddf leaves both deltas at zero.
        const auto yaw_before = view.yaw_degrees();
        const auto idle = view.apply_staged_x(-1, false);
        require(idle.horizontal_degrees == 0 && idle.vertical_degrees == 0 &&
                    std::abs(view.yaw_degrees() - yaw_before) < 1e-9,
                "an idle camera setup moved the view");

        // DAT_00464878 != 0 adds the fixed -6 degree vertical delta.
        const auto alternate = view.apply_staged_x(0, true);
        require(alternate.vertical_degrees == -6 && view.pitch_degrees() > 0,
                "alternate-mode vertical delta mismatch");

        // Many rotations must keep the wrapped column inside the source width.
        for (int step = 0; step < 24; ++step) view.advance(37, 0, 1);
        check_round_trip(view, "wrapped");
        check_projection_matches_render(view, "wrapped");

        // The viewport ends: 0x4240ae's bounds test is 0 <= x < width and
        // 0 <= y < height, and a rejected point carries no coordinate.
        for (const auto& outside : {std::pair<int, int>{-1, 0}, {0, -1},
                                    {kViewWidth, 0}, {0, kViewHeight}}) {
            const auto point = view.canvas_point(outside.first, outside.second);
            require(!point.mapped, "inverse map accepted a point outside the viewport");
        }

        // The renderer samples a viewport-sized destination only.
        bool rejected = false;
        try {
            auto wrong = byzantine::make_surface(kViewWidth + 1, kViewHeight, 0);
            view.render(wrong);
        } catch (const std::runtime_error&) {
            rejected = true;
        }
        require(rejected, "render accepted a mismatched viewport");

        // A source that does not match its stated geometry is rejected at load.
        rejected = false;
        try {
            auto short_source = source;
            short_source.pop_back();
            byzantine::PanoramaView bad("bad", kWidth, kHeight, short_source,
                                        kViewWidth, kViewHeight);
        } catch (const std::runtime_error&) {
            rejected = true;
        }
        require(rejected, "accepted a short panorama source");

        // 0x423e03's own rejects: a point outside the source, or one whose
        // camera-relative angle is beyond the +/-60 degree window.
        for (const auto& outside : {std::pair<int, int>{-1, 0}, {0, -1},
                                    {kWidth, 0}, {0, kHeight}}) {
            int projected_x = 0;
            int projected_y = 0;
            require(!view.project_source(outside.first, outside.second, projected_x,
                                         projected_y),
                    "0x423e03 accepted a point outside the source");
        }
        view.apply_staged_x(0, false);  // yaw = +180, pitch 0
        int projected_x = 0;
        int projected_y = 0;
        // yaw = +180 looks at the seam, so the opposite half of the cylinder
        // (column kWidth/2) is 180 degrees away: beyond the +/-60 degree window.
        require(!view.project_source(kWidth / 2, kHeight / 2, projected_x,
                                     projected_y),
                "0x423e03 accepted a source point behind the camera");

        // A staged position outside 0..2495 is not a known camera request.
        rejected = false;
        try {
            view.apply_staged_x(2496, false);
        } catch (const std::runtime_error&) {
            rejected = true;
        }
        require(rejected, "accepted a staged position outside the panorama");

        std::cout << "panorama view/inverse-mapping tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
