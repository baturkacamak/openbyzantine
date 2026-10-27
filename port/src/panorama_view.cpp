#include "byzantine/panorama_view.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

namespace byzantine {
namespace {

int padding_for(int source_width, int source_height, int viewport_width,
                int viewport_height) {
    return panorama_geometry(source_width, source_height, viewport_width,
                             viewport_height)
        .padding;
}

}  // namespace

PanoramaView::PanoramaView(std::string path, int source_width, int source_height,
                           const std::vector<std::uint16_t>& pixels,
                           int viewport_width, int viewport_height)
    : path_(std::move(path)),
      source_width_(source_width),
      source_height_(source_height),
      viewport_width_(viewport_width),
      viewport_height_(viewport_height),
      pixels_per_radian_(panorama_geometry(source_width, source_height, viewport_width,
                                           viewport_height)
                             .pixels_per_radian),
      projection_(tables_, source_width, source_height,
                  padding_for(source_width, source_height, viewport_width,
                              viewport_height),
                  viewport_width, viewport_height),
      padded_(pad_panorama_rows(
          pixels, static_cast<std::size_t>(source_width),
          static_cast<std::size_t>(source_height),
          static_cast<std::size_t>(padding_for(source_width, source_height,
                                               viewport_width, viewport_height)))) {
    // pad_panorama_rows already rejects a source buffer that does not match
    // source_width*source_height, so a malformed image cannot reach sampling.
}

PanoramaCameraSetup PanoramaView::apply_staged_x(int staged_x, bool alternate_mode) {
    int staged = staged_x;
    const auto setup = consume_panorama_position(staged, source_width_, alternate_mode);
    // 0x401ddf calls 0x4241e2 (camera + map reset) before it exposes the delta,
    // so a staged position is ABSOLUTE: reset to yaw = +pi, then turn.
    if (setup.reset_projection) projection_.reset(tables_);
    if (setup.horizontal_degrees != 0 || setup.vertical_degrees != 0) {
        advance(setup.horizontal_degrees, setup.vertical_degrees, 1.0);
    }
    return setup;
}

bool PanoramaView::advance(double horizontal_degrees, double vertical_degrees,
                           double zoom) {
    return projection_.advance(tables_, horizontal_degrees, vertical_degrees, zoom);
}

void PanoramaView::render(Rgb16Surface& target) const {
    if (target.width != viewport_width_ || target.height != viewport_height_) {
        throw std::runtime_error("panorama viewport " + std::to_string(viewport_width_) +
                                 "x" + std::to_string(viewport_height_) +
                                 " does not match the destination " +
                                 std::to_string(target.width) + "x" +
                                 std::to_string(target.height));
    }
    sample_panorama_rgb16(padded_, projection_.mapping(),
                          projection_.yaw_word_offset(), target.pixels,
                          {target.width * 2, 0, 0, target.width, target.height}, 0);
}

PanoramaPoint PanoramaView::canvas_point(int viewport_x, int viewport_y) const {
    PanoramaPoint point;
    if (viewport_x < 0 || viewport_y < 0 || viewport_x >= viewport_width_ ||
        viewport_y >= viewport_height_) {
        return point;
    }
    const auto& map = projection_.mapping();
    const auto cell = static_cast<std::size_t>(viewport_y) *
                          static_cast<std::size_t>(viewport_width_) +
                      static_cast<std::size_t>(viewport_x);
    if (cell >= map.size()) {
        throw std::runtime_error("panorama offset map is smaller than its viewport");
    }
    const std::int64_t linear =
        static_cast<std::int64_t>(map[cell]) + projection_.yaw_word_offset();
    if (linear < std::numeric_limits<std::int32_t>::min() ||
        linear > std::numeric_limits<std::int32_t>::max()) {
        throw std::runtime_error("panorama inverse offset out of 32-bit range");
    }
    // 0x4240ae uses the C truncating `%` and `/` on the flat word offset, then
    // subtracts the padding (row layout [pad | source | pad]) and wraps the
    // column into the unpadded source width.
    std::int32_t column =
        static_cast<std::int32_t>(linear % projection_.stride_words()) - padding();
    while (column >= source_width_) column -= source_width_;
    while (column < 0) column += source_width_;
    point.mapped = true;
    point.source_x = column;
    point.source_y = static_cast<std::int32_t>(linear / projection_.stride_words());
    return point;
}

bool PanoramaView::project_source_raw(int source_x, int source_y,
                                      int& viewport_x, int& viewport_y,
                                      bool apply_window) const {
    // Constants as stored in the shipped image: 0x45b140 2*pi, 0x45b158/-160
    // -/+ pi and 0x45b168/-170 the +/-60 degree window of 0x423e03.
    constexpr double kPi = 3.141592653589793;
    constexpr double kTwoPi = 6.283185307179586;
    constexpr double kHalfFovLimit = 1.0471975511966667;
    const auto camera = projection_.camera();
    const double scale = pixels_per_radian_;
    const double focal = camera.zoom * scale;
    // 0x423e03: angle = (x - source_width/2 - yaw*scale) / scale, then the two
    // x87 loops bring it into [-pi, pi].
    double angle = (static_cast<double>(source_x) - source_width_ / 2) / scale -
                   camera.yaw_radians;
    while (angle < -kPi) angle += kTwoPi;
    while (angle > kPi) angle -= kTwoPi;
    // A point at or behind the camera plane cannot be projected; this is also
    // what keeps the bbox finite when a rectangle straddles the horizon.
    if (angle <= -1.5707963267948966 || angle >= 1.5707963267948966) return false;
    if (apply_window && (angle < -kHalfFovLimit || kHalfFovLimit < angle)) {
        return false;
    }
    const double dy = static_cast<double>(source_y) - source_height_ / 2;
    const double cosine_pitch = std::cos(camera.pitch_radians);
    const double sine_pitch = std::sin(camera.pitch_radians);
    const double cosine_angle = std::cos(angle);
    const double sine_angle = std::sin(angle);
    // depth is the sampler's `cos*F - sin*yc` after substituting the recovered
    // row: it is 0x423e03's `scale*cos(pitch)*cos(angle) + dy*sin(pitch)`.
    const double depth = scale * cosine_pitch * cosine_angle + dy * sine_pitch;
    if (!(depth > 0) || !std::isfinite(depth)) return false;
    const double offset_x = focal * scale * sine_angle / depth;
    const double offset_y =
        focal * (dy * cosine_pitch - scale * cosine_angle * sine_pitch) / depth;
    if (!std::isfinite(offset_x) || !std::isfinite(offset_y)) return false;
    // 0x423e03 truncates with __ftol.
    viewport_x = viewport_width_ / 2 + static_cast<int>(std::trunc(offset_x));
    viewport_y = viewport_height_ / 2 + static_cast<int>(std::trunc(offset_y));
    return true;
}

bool PanoramaView::project_source(int source_x, int source_y, int& viewport_x,
                                  int& viewport_y) const {
    // 0x423e03's input bounds: the point must be inside the UNPADDED source.
    if (source_x < 0 || source_y < 0 || source_x >= source_width_ ||
        source_y >= source_height_) {
        return false;
    }
    if (!project_source_raw(source_x, source_y, viewport_x, viewport_y, true)) {
        return false;
    }
    // 0x423e03 rejects a point that leaves the viewport, which is exactly the
    // `0 < x < 0x280 && 0 < y < 0x190` test.
    if (viewport_x < 0 || viewport_y < 0 || viewport_x >= viewport_width_ ||
        viewport_y >= viewport_height_) {
        return false;
    }
    return true;
}

bool PanoramaView::map_viewport(int viewport_x, int viewport_y, int& source_x,
                                int& source_y) const {
    const auto point = canvas_point(viewport_x, viewport_y);
    if (!point.mapped) return false;
    source_x = point.source_x;
    source_y = point.source_y;
    return true;
}

bool PanoramaView::source_rect_viewport_bbox(int source_x, int source_y,
                                             int width, int height, int& x0,
                                             int& y0, int& x1, int& y1) const {
    if (width <= 0 || height <= 0) {
        x0 = 1;
        x1 = 0;
        y0 = 0;
        y1 = 0;
        return true;
    }
    const int right = source_x + width - 1;
    const int bottom = source_y + height - 1;
    const int corners[4][2] = {{source_x, source_y},
                               {right, source_y},
                               {source_x, bottom},
                               {right, bottom}};
    int min_x = viewport_width_;
    int min_y = viewport_height_;
    int max_x = -1;
    int max_y = -1;
    int front = 0;
    bool behind = false;
    for (const auto& corner : corners) {
        int vx = 0;
        int vy = 0;
        if (!project_source_raw(corner[0], corner[1], vx, vy, false)) {
            // This corner is at/behind the camera. A rectangle that straddles
            // the camera plane can still show a sliver at the viewport edge, so
            // scan the whole viewport and let the inverse test decide. One that
            // is entirely behind cannot be visible at all.
            behind = true;
            continue;
        }
        ++front;
        min_x = std::min(min_x, vx);
        min_y = std::min(min_y, vy);
        max_x = std::max(max_x, vx);
        max_y = std::max(max_y, vy);
    }
    if (front == 0) {
        x0 = 1;
        x1 = 0;
        y0 = 0;
        y1 = 0;
        return true;
    }
    if (behind) {
        x0 = 0;
        y0 = 0;
        x1 = viewport_width_ - 1;
        y1 = viewport_height_ - 1;
        return true;
    }
    // The sampler's table arithmetic and this analytic projection differ by up
    // to two pixels (PANORAMA_SPRITES.md); keep a margin so a partially visible
    // patch is never clipped by the bbox. The inverse test rejects the extra
    // pixels. Pitch can widen the gap, so use eight rather than two.
    constexpr int kMargin = 8;
    x0 = std::max(0, min_x - kMargin);
    y0 = std::max(0, min_y - kMargin);
    x1 = std::min(viewport_width_ - 1, max_x + kMargin);
    y1 = std::min(viewport_height_ - 1, max_y + kMargin);
    if (x0 > x1 || y0 > y1) {
        x0 = 1;
        x1 = 0;
        y0 = 0;
        y1 = 0;
    }
    return true;
}

double PanoramaView::yaw_degrees() const {
    return static_cast<double>(panorama_angle_degrees(projection_.camera().yaw_radians));
}

double PanoramaView::pitch_degrees() const {
    return static_cast<double>(
               panorama_angle_degrees(projection_.camera().pitch_radians));
}

bool PanoramaView::at_pitch_limit() const {
    return projection_.camera().at_pitch_limit;
}

}  // namespace byzantine
