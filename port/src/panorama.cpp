#include "byzantine/panorama.hpp"

#include <algorithm>
#include <cmath>
#include <cfenv>
#include <limits>
#include <stdexcept>

namespace byzantine {

namespace {
// Literal binary64 constants recovered from the executable, not host M_PI.
constexpr double original_pi = 3.14159265359;
constexpr double original_two_pi = 6.28318530718;

double rounded_mul(double a, double b) { volatile double r = a * b; return r; }
double rounded_add(double a, double b) { volatile double r = a + b; return r; }
double rounded_sub(double a, double b) { volatile double r = a - b; return r; }
double rounded_div(double a, double b) { volatile double r = a / b; return r; }

void require_nearest_binary64() {
    if (!std::numeric_limits<double>::is_iec559 ||
        std::numeric_limits<double>::digits != 53 ||
        std::fegetround() != FE_TONEAREST)
        throw std::runtime_error("panorama projection requires binary64 nearest rounding");
}

int truncate_pixel(long double value) {
    const auto truncated = std::trunc(value);
    if (!std::isfinite(truncated) || truncated < std::numeric_limits<int>::min() ||
        truncated > std::numeric_limits<int>::max())
        throw std::runtime_error("panorama pixel displacement out of range");
    return static_cast<int>(truncated);
}
}

long double panorama_angle_degrees(double radians) {
    if (!std::isfinite(radians))
        throw std::runtime_error("invalid panorama angle");
    return static_cast<long double>(radians) * 180.0L / original_pi;
}

int panorama_yaw_offset(double scale, double yaw) {
    if (!std::isfinite(scale) || scale <= 0 || !std::isfinite(yaw))
        throw std::runtime_error("invalid panorama yaw state");
    // 0x4245a5/0x4245ae: fld QWORD [renderer+0x28], fmul QWORD [renderer+0x50].
    // CRT startup installs PC=53 (see PANORAMA_TABLES.md), so the product is a
    // binary64 store, not a wide intermediate. At the recovered constants this
    // matters: 64/(2*pi)*pi is 32 under PC53 but 31.9999999999999992 under PC64,
    // and __ftol truncation then yields 32 versus 31.
    return truncate_pixel(rounded_mul(scale, yaw));
}

int panorama_table_coordinate(double value) {
    if (!std::isfinite(value))
        throw std::runtime_error("invalid panorama table coordinate");
    // Explicit ties-to-even, independent of the host's nearbyint rounding mode.
    const double lower = std::floor(value);
    const double fraction = value - lower;
    const double rounded = lower + (fraction > 0.5 ||
        (fraction == 0.5 && std::fmod(lower, 2.0) != 0.0) ? 1.0 : 0.0);
    if (rounded < std::numeric_limits<int>::min() ||
        rounded > std::numeric_limits<int>::max())
        throw std::runtime_error("panorama table coordinate out of range");
    return static_cast<int>(rounded);
}

PanoramaLookupTables::PanoramaLookupTables() {
    for (int i = 0; i < 1570; ++i) {
        const double angle = static_cast<double>(i) / 1000.0;
        cosine_[1569 + i] = static_cast<float>(std::cos(angle));
        cosine_[1569 - i] = static_cast<float>(std::cos(-angle));
    }
    for (int i = 0; i < 2000; ++i) {
        const double ratio = static_cast<double>(i) / 1000.0;
        arctangent_[1999 + i] = static_cast<float>(std::atan(ratio));
        // The second write at i=0 deliberately retains atan(-0.0).
        arctangent_[1999 - i] = static_cast<float>(std::atan(-ratio));
    }
}

float PanoramaLookupTables::cosine(int index) const {
    if (index < -1569 || index > 1569)
        throw std::runtime_error("uninitialized/out-of-range panorama cosine index");
    return cosine_[static_cast<std::size_t>(index + 1569)];
}

float PanoramaLookupTables::arctangent(int index) const {
    if (index < -1999 || index > 1999)
        throw std::runtime_error("uninitialized/out-of-range panorama arctangent index");
    return arctangent_[static_cast<std::size_t>(index + 1999)];
}

std::vector<std::int32_t> panorama_mapping(
    const PanoramaLookupTables& tables, int source_width, int source_height,
    int padding, int viewport_width, int viewport_height,
    double pitch, double zoom) {
    if (source_width <= 0 || source_height <= 0 || padding < 0 ||
        padding > source_width || viewport_width <= 0 || viewport_height <= 0 ||
        !std::isfinite(pitch) || !std::isfinite(zoom) || zoom < 1)
        throw std::runtime_error("invalid known panorama mapping state");
    if (!std::numeric_limits<double>::is_iec559 ||
        std::numeric_limits<double>::digits != 53 ||
        std::fegetround() != FE_TONEAREST)
        throw std::runtime_error("panorama mapping requires binary64 nearest rounding");
    const std::int64_t stride = std::int64_t{source_width} + 2LL * padding;
    if (stride > std::numeric_limits<std::int32_t>::max())
        throw std::runtime_error("panorama mapping stride out of range");
    const std::int64_t cells = stride * source_height;
    const std::int64_t count = std::int64_t{viewport_width} * viewport_height;
    // Preserve the supported signed 32-bit offset/allocation domain. Original
    // integer wrapping is deliberately not used as a source-buffer index.
    if (cells > std::numeric_limits<std::int32_t>::max() ||
        count > std::numeric_limits<std::int32_t>::max() / 4)
        throw std::runtime_error("panorama mapping dimensions out of range");
    // Separate stores prevent contraction into FMA or a host long-double
    // expression. Normal binary64 arithmetic has the recovered PC53 precision;
    // x87's larger exponent range and extended trig results are not emulated.
    const auto mul = [](double a, double b) { volatile double r = a * b; return r; };
    const auto add = [](double a, double b) { volatile double r = a + b; return r; };
    const auto sub = [](double a, double b) { volatile double r = a - b; return r; };
    const auto div = [](double a, double b) { volatile double r = a / b; return r; };
    const double scale = div(source_width, original_two_pi);
    const double focal = mul(zoom, scale);
    const double cosine = std::cos(pitch), sine = std::sin(pitch);
    std::vector<std::int32_t> result;
    result.reserve(static_cast<std::size_t>(count));
    for (int row = 0; row < viewport_height; ++row) {
        const double y = row - viewport_height / 2;
        // Instruction order: sin*F + cos*y, then *S; cos*F - sin*y.
        const double numerator = mul(add(mul(sine, focal), mul(cosine, y)), scale);
        const double depth = sub(mul(cosine, focal), mul(sine, y));
        if (!std::isfinite(numerator) || !std::isfinite(depth) || depth <= 0)
            throw std::runtime_error("unsupported panorama mapping depth");
        const double reciprocal = div(1, depth);
        for (int column = 0; column < viewport_width; ++column) {
            const double x = column - viewport_width / 2;
            const int atan_index = panorama_table_coordinate(mul(mul(reciprocal, x), 1000));
            const double angle = tables.arctangent(atan_index);
            const int dx = panorama_table_coordinate(mul(angle, scale));
            const int cos_index = panorama_table_coordinate(mul(angle, 1000));
            const int dy = panorama_table_coordinate(
                mul(mul(tables.cosine(cos_index), numerator), reciprocal));
            const std::int64_t sx = stride / 2 + dx;
            const std::int64_t sy = std::int64_t{source_height / 2} + dy;
            if (sx < 0 || sx >= stride || sy < 0 || sy >= source_height)
                throw std::runtime_error("panorama mapping outside padded source");
            result.push_back(static_cast<std::int32_t>(sy * stride + sx));
        }
    }
    return result;
}

void sample_panorama_rgb16(
    const std::vector<std::uint16_t>& source,
    const std::vector<std::int32_t>& mapping, int yaw_word_offset,
    std::vector<std::uint16_t>& destination,
    PanoramaDestination rectangle, int effect) {
    if (effect != 0)
        throw std::runtime_error("unsupported panorama sampling effect");
    if (&source == &destination)
        throw std::runtime_error("aliased panorama sampling buffers unsupported");
    if (rectangle.pitch_bytes <= 0 || rectangle.pitch_bytes % 2 != 0 ||
        rectangle.left < 0 || rectangle.top < 0 ||
        rectangle.width <= 0 || rectangle.height <= 0)
        throw std::runtime_error("invalid panorama destination rectangle/pitch");
    const std::int64_t stride = rectangle.pitch_bytes / 2;
    const std::int64_t count = std::int64_t{rectangle.width} * rectangle.height;
    const std::int64_t right = std::int64_t{rectangle.left} + rectangle.width;
    const std::int64_t bottom = std::int64_t{rectangle.top} + rectangle.height;
    // Both destination and mapping addresses stay in the supported original
    // signed 32-bit byte-offset domain. Wide intermediates avoid host UB.
    const std::int64_t end = (bottom - 1) * stride + right;
    if (right > stride || count > std::numeric_limits<std::int32_t>::max() / 4 ||
        mapping.size() != static_cast<std::size_t>(count) ||
        end > std::numeric_limits<std::int32_t>::max() / 2 ||
        static_cast<std::uint64_t>(end) > destination.size())
        throw std::runtime_error("panorama destination/mapping bounds exceeded");
    // The binary adds yaw to a FLAT word offset, with no modulo or clamp.
    // Validate the whole map first, so a bad late entry cannot partially draw.
    for (const auto entry : mapping) {
        const std::int64_t offset = std::int64_t{entry} + yaw_word_offset;
        if (entry < 0 || entry > std::numeric_limits<std::int32_t>::max() / 2 ||
            offset < 0 || offset > std::numeric_limits<std::int32_t>::max() / 2 ||
            static_cast<std::uint64_t>(offset) >= source.size())
            throw std::runtime_error("panorama final yaw source bounds exceeded");
    }
    std::size_t cell = 0;
    for (int row = 0; row < rectangle.height; ++row) {
        const auto start = static_cast<std::size_t>(
            (std::int64_t{rectangle.top} + row) * stride + rectangle.left);
        for (int column = 0; column < rectangle.width; ++column, ++cell) {
            const auto offset = static_cast<std::size_t>(
                std::int64_t{mapping[cell]} + yaw_word_offset);
            destination[start + column] = source[offset];
        }
    }
}

long double panorama_pitch_limit_return(double scale, int source_height,
                                        int viewport_height, double zoom) {
    if (!std::isfinite(scale) || scale <= 0 || source_height <= 0 ||
        viewport_height <= 0 || !std::isfinite(zoom) || zoom < 1)
        throw std::runtime_error("invalid panorama pitch geometry");
    require_nearest_binary64();
    const double focal = rounded_mul(scale, zoom);
    if (!std::isfinite(focal))
        throw std::runtime_error("panorama focal length out of range");
    const double sh = source_height / 2;
    const double vh = viewport_height / 2;
    const double ratio = rounded_div(
        rounded_sub(rounded_mul(sh, focal), rounded_mul(scale, vh)),
        rounded_add(rounded_mul(scale, focal), rounded_mul(sh, vh)));
    if (!std::isfinite(ratio))
        throw std::runtime_error("panorama pitch ratio out of range");
    // Keep a separate live result and binary64 store. Host atan(long double)
    // approximates the CRT return, not its implementation or effective RC.
    return std::atan(static_cast<long double>(ratio));
}

double panorama_pitch_limit(double scale, int source_height,
                            int viewport_height, double zoom) {
    return static_cast<double>(panorama_pitch_limit_return(
        scale, source_height, viewport_height, zoom));
}

PanoramaPitchClamp clamp_panorama_pitch(double pitch, long double limit_return) {
    require_nearest_binary64();
    const double stored = static_cast<double>(limit_return);
    if (!std::isfinite(pitch) || !std::isfinite(limit_return) ||
        !std::isfinite(stored) || limit_return < 0)
        throw std::runtime_error("unsupported panorama pitch clamp state");
    if (static_cast<long double>(pitch) >= limit_return) return {stored, true};
    if (pitch <= -stored) return {-stored, true};
    return {pitch, false};
}

PanoramaProjection::PanoramaProjection(const PanoramaLookupTables& tables,
    int sw, int sh, int padding, int vw, int vh)
    : source_width_(sw), source_height_(sh), padding_(padding),
      viewport_width_(vw), viewport_height_(vh) {
    reset(tables);
}

void PanoramaProjection::reset(const PanoramaLookupTables& tables) {
    auto map = panorama_mapping(tables, source_width_, source_height_, padding_,
                                viewport_width_, viewport_height_, 0, 1);
    camera_ = {original_pi, 0, 1, false};
    mapping_.swap(map);
}

bool PanoramaProjection::set_source(const PanoramaLookupTables& tables,
                                    int source_width, int source_height) {
    if (source_width <= 0 || source_height <= 0)
        throw std::runtime_error("invalid panorama source dimensions");
    const bool geometry_changed = source_width != source_width_ ||
                                  source_height != source_height_;
    if (!geometry_changed) {
        // 0x4236b5 re-copies rows/seam then calls 0x4241e2 unconditionally,
        // even when the decoded size is unchanged.
        reset(tables);
        return false;
    }
    // Compute everything before committing so a rejected seam/map cannot leave
    // half-updated dimensions; the original writes +0x24/+0x20 first.
    const auto geometry = panorama_geometry(source_width, source_height,
                                            viewport_width_, viewport_height_);
    auto map = panorama_mapping(tables, source_width, source_height,
                                geometry.padding, viewport_width_,
                                viewport_height_, 0, 1);
    source_width_ = source_width;
    source_height_ = source_height;
    padding_ = geometry.padding;
    camera_ = {original_pi, 0, 1, false};
    mapping_.swap(map);
    return true;
}

bool PanoramaProjection::advance(const PanoramaLookupTables& tables,
    double horizontal, double vertical, double zoom_factor) {
    require_nearest_binary64();
    if (!std::isfinite(horizontal) || std::abs(horizontal) > 360 ||
        !std::isfinite(vertical) || !std::isfinite(zoom_factor))
        throw std::runtime_error("unsupported panorama camera delta");
    auto next = camera_;
    const bool regenerate = vertical != 0 || zoom_factor != 1;
    const double scale = rounded_div(source_width_, original_two_pi);
    if (regenerate) {
        next.pitch_radians = rounded_sub(next.pitch_radians,
            rounded_div(rounded_mul(vertical, original_pi), 180));
        const double product = rounded_mul(next.zoom, zoom_factor);
        if (!std::isfinite(product))
            throw std::runtime_error("panorama zoom product out of range");
        next.zoom = std::max(product, 1.0);
        const auto clamped = clamp_panorama_pitch(next.pitch_radians,
            panorama_pitch_limit_return(scale, source_height_, viewport_height_, next.zoom));
        next.pitch_radians = clamped.pitch_radians;
        next.at_pitch_limit = clamped.at_limit;
    }
    next.yaw_radians = rounded_add(next.yaw_radians,
        rounded_div(rounded_mul(horizontal, original_pi), 180));
    // The delta domain bounds these loops; endpoints are inclusive.
    while (next.yaw_radians > original_pi)
        next.yaw_radians = rounded_sub(next.yaw_radians, original_two_pi);
    while (next.yaw_radians < -original_pi)
        next.yaw_radians = rounded_add(next.yaw_radians, original_two_pi);
    std::vector<std::int32_t> map;
    if (regenerate)
        map = panorama_mapping(tables, source_width_, source_height_, padding_,
            viewport_width_, viewport_height_, next.pitch_radians, next.zoom);
    // Commit only after every numeric/map check succeeds (portable safeguard).
    camera_ = next;
    if (regenerate) mapping_.swap(map);
    return regenerate;
}

int PanoramaProjection::yaw_word_offset() const {
    require_nearest_binary64();
    return panorama_yaw_offset(rounded_div(source_width_, original_two_pi),
                               camera_.yaw_radians);
}

PanoramaGeometry panorama_geometry(int source_width, int source_height,
                                   int viewport_width, int viewport_height) {
    if (source_width <= 0 || viewport_width <= 0)
        throw std::runtime_error("invalid panorama width");
    PanoramaGeometry result;
    result.pixels_per_radian = static_cast<double>(
        static_cast<long double>(source_width) / original_two_pi);
    result.pitch_limit_radians = panorama_pitch_limit(
        result.pixels_per_radian, source_height, viewport_height, 1.0);
    // 0x423d34 stores cos(limit)*scale as binary64 before subtracting sin*H/2.
    const double cosine_scale = static_cast<double>(
        static_cast<long double>(std::cos(result.pitch_limit_radians)) *
        result.pixels_per_radian);
    const long double denominator = cosine_scale -
        static_cast<long double>(std::sin(result.pitch_limit_radians)) *
        (viewport_height / 2);
    const double ratio = static_cast<double>((viewport_width / 2) / denominator);
    const int extent = truncate_pixel(
        static_cast<long double>(std::atan(ratio)) * result.pixels_per_radian);
    if (!std::isfinite(ratio) || denominator <= 0 || extent < 0 ||
        extent >= source_width)
        throw std::runtime_error("unsupported panorama seam geometry");
    result.padding = extent + 1;
    result.stride_words = source_width + 2 * result.padding;
    return result;
}

PanoramaCameraSetup consume_panorama_position(int& staged_x, int panorama_width,
                                             bool alternate_mode) {
    if (panorama_width <= 0 || staged_x < -1 || staged_x > 2495)
        throw std::runtime_error("invalid known panorama camera state");
    PanoramaCameraSetup result;
    if (staged_x == -1) return result;
    // x87 originally uses extended precision for this expression.
    const auto degrees = std::floor(static_cast<long double>(staged_x) /
                                   panorama_width * 360.0L + 0.5L);
    result.horizontal_degrees = static_cast<int>(std::clamp(degrees, 0.0L, 360.0L));
    result.vertical_degrees = alternate_mode ? -6 : 0;
    result.reset_projection = true;
    staged_x = -1;
    return result;
}

std::vector<std::uint16_t> pad_panorama_rows(
    const std::vector<std::uint16_t>& source, std::size_t width,
    std::size_t height, std::size_t padding) {
    const auto limit = std::numeric_limits<std::size_t>::max();
    if (width == 0 || height == 0 || padding > width || height > limit / width ||
        source.size() != width * height || padding > (limit - width) / 2)
        throw std::runtime_error("invalid panorama row dimensions");
    const auto stride = width + padding * 2;
    if (height > limit / stride || stride * height > source.max_size())
        throw std::runtime_error("panorama padded buffer too large");
    std::vector<std::uint16_t> result(stride * height);
    for (std::size_t row = 0; row < height; ++row) {
        const auto* input = source.data() + row * width;
        auto* output = result.data() + row * stride;
        std::copy_n(input + width - padding, padding, output);
        std::copy_n(input, width, output + padding);
        std::copy_n(input, padding, output + padding + width);
    }
    return result;
}

} // namespace byzantine
