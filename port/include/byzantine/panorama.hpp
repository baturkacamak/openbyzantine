#pragma once

#include <cstddef>
#include <array>
#include <cstdint>
#include <vector>

namespace byzantine {

// Deltas submitted to the projection renderer, not absolute camera angles.
struct PanoramaCameraSetup {
    int horizontal_degrees = 0;
    int vertical_degrees = 0;
    bool reset_projection = false;
};

// Reconstructs 0x401ddf. The caller must supply KNOWN original state:
// staged_x == -1 means explicitly no request; 0..2495 is the clamped request.
// SceneState's unknown optional value must not be converted to -1 implicitly.
// On success consumes staged_x. A reset request applies if a renderer exists.
PanoramaCameraSetup consume_panorama_position(int& staged_x, int panorama_width,
                                             bool alternate_mode);

struct PanoramaGeometry {
    double pixels_per_radian = 0;
    double pitch_limit_radians = 0;
    int padding = 0;
    // 0x4236b5 +0x1c: source_width + 2*padding, in 16-bit WORD units.
    int stride_words = 0;
};

// Formula recovery from 0x4236b5/0x423c42/0x423cdf, for a known viewport.
// Uses host libm; does not promise the original x87/CRT's bit-exact results.
// The loader computes padding at zoom=1, independently of current zoom.
PanoramaGeometry panorama_geometry(int source_width, int source_height,
                                   int viewport_width, int viewport_height);
double panorama_pitch_limit(double pixels_per_radian, int source_height,
                            int viewport_height, double zoom);
// Host approximation of the live ST0 result; the double helper returns its
// binary64 store. Host atan/long double are not the original CRT/x87 oracle.
long double panorama_pitch_limit_return(double pixels_per_radian,
    int source_height, int viewport_height, double zoom);

struct PanoramaPitchClamp {
    double pitch_radians;
    bool at_limit;
};
// 0x424466: upper comparison uses the live return, lower comparison and both
// assignments use its binary64 store. Also accepts an independently known
// return value, so that distinction can be tested without a host atan oracle.
PanoramaPitchClamp clamp_panorama_pitch(double pitch, long double limit_return);
// 0x42459f: signed pixel displacement, with __ftol truncation toward zero.
int panorama_yaw_offset(double pixels_per_radian, double yaw_radians);
// x87 return expressions recovered from 0x402e00 (pitch)/0x402e70 (yaw).
long double panorama_angle_degrees(double radians);

// Bias conversion's bounded nearest/even model, assuming original RC=nearest
// and PC=53. Rejects overflow instead of emulating low-DWORD wrapping.
int panorama_table_coordinate(double value);

// 0x423310: only initialized entries are exposed. Host libm is approximate
// relative to the original CRT; arguments are signed thousandth-unit indices.
class PanoramaLookupTables {
public:
    PanoramaLookupTables();
    float cosine(int index) const;
    float arctangent(int index) const;
private:
    std::array<float, 3139> cosine_;
    std::array<float, 3999> arctangent_;
};

// 0x423a37 formula model: row-major signed word offsets into the seam-padded
// RGB16 source, before the renderer's separate yaw displacement. Every state
// argument must be known. No implicit camera, viewport or padding is supplied.
// Requires host FE_TONEAREST; binary64 stores separate arithmetic operations
// (PC53 model in the normal binary64 range). Host sin/cos and CRT table values
// remain approximations, so this is NOT an original-engine pixel-exact map.
// Rejects invalid depth, uninitialized table access, source/32-bit overflow.
std::vector<std::int32_t> panorama_mapping(
    const PanoramaLookupTables& tables, int source_width, int source_height,
    int padding, int viewport_width, int viewport_height,
    double pitch_radians, double zoom);

struct PanoramaProjectionCamera {
    double yaw_radians;
    double pitch_radians;
    double zoom;
    bool at_pitch_limit;
};

// Explicit RESET state (0x4241e2), not a restore of unknown game state.
// Dimensions/padding are supplied by the caller; source pixels are external.
// Reset always rebuilds. Advance models the successful-lock camera/map phase
// of 0x4242bd, with binary64 basic arithmetic and approximate host trig.
// Horizontal deltas are limited to [-360,360] degrees. Failed updates are
// atomic. This does not acquire a surface, sample pixels or implement effects.
class PanoramaProjection {
public:
    PanoramaProjection(const PanoramaLookupTables& tables, int source_width,
        int source_height, int padding, int viewport_width, int viewport_height);
    void reset(const PanoramaLookupTables& tables);
    // 0x4236b5 load path: a decoded image with KNOWN dimensions was supplied.
    // Scale/padding/stride are recomputed only when the stored width/height
    // differ; the camera reset and mapping rebuild then happen unconditionally,
    // exactly as 0x4236b5 calls 0x4241e2 after its rows and seam are refreshed.
    // Returns true iff the dimension-dependent geometry was recomputed.
    // The allocation-failure branch (+0x34 stays null, no seam/reset) and the
    // row copies themselves are not modelled here.
    bool set_source(const PanoramaLookupTables& tables, int source_width,
                    int source_height);
    // Returns true iff the map was regenerated, including unchanged geometry
    // after a zoom floor/clamp. Neutral pitch/zoom preserves the limit flag.
    bool advance(const PanoramaLookupTables& tables, double horizontal_degrees,
                 double vertical_degrees, double zoom_factor);
    PanoramaProjectionCamera camera() const { return camera_; }
    const std::vector<std::int32_t>& mapping() const { return mapping_; }
    int yaw_word_offset() const;
    // Known 0x4236b5 geometry fields for the current source dimensions.
    int padding() const { return padding_; }
    int stride_words() const { return source_width_ + 2 * padding_; }
private:
    int source_width_, source_height_, padding_, viewport_width_, viewport_height_;
    PanoramaProjectionCamera camera_;
    std::vector<std::int32_t> mapping_;
};

// Explicit destination rectangle in pixels; pitch is in BYTES, as in the
// locked surface used by 0x4242bd. Only positive, even RGB16 pitch is supported.
struct PanoramaDestination {
    int pitch_bytes;
    int left;
    int top;
    int width;
    int height;
};

// Normal sampling branch of 0x4242bd. Copies complete 16-bit words without
// decoding colors or applying the effect mask. Mapping is before yaw; the
// caller supplies a known signed yaw offset (panorama_yaw_offset).
// Nonzero effect is explicitly unsupported. All final source/destination
// addresses are checked before any write. Source/destination must not alias.
void sample_panorama_rgb16(
    const std::vector<std::uint16_t>& padded_source,
    const std::vector<std::int32_t>& mapping, int yaw_word_offset,
    std::vector<std::uint16_t>& destination,
    PanoramaDestination rectangle, int effect);

// Reconstructs the row layout in 0x4236b5/0x42382b, preserving 16-bit words.
// Returns [last padding pixels | source row | first padding pixels] per row.
// Padding must be supplied by the caller (see panorama_geometry).
// This function does not project or decode a JPEG.
std::vector<std::uint16_t> pad_panorama_rows(
    const std::vector<std::uint16_t>& source, std::size_t width,
    std::size_t height, std::size_t padding);

} // namespace byzantine
