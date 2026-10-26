#include "byzantine/panorama_view.hpp"

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
