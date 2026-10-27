#pragma once

#include "byzantine/compose.hpp"
#include "byzantine/panorama.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace byzantine {

// 0x4240ae's result: the panorama source coordinate a screen point samples.
// `mapped == false` means the point was outside the viewport, where 0x4240ae
// returns 1 and leaves the caller's output untouched.
struct PanoramaPoint {
    bool mapped = false;
    std::int32_t source_x = 0;  // 0..source_width-1, wrapped
    std::int32_t source_y = 0;  // 0..source_height-1
};

// The panorama renderer object behind DAT_00464880: the decoded node image with
// its seam-padded rows (0x4236b5/0x42382b), the camera (0x4241e2 reset and
// 0x4242bd advance) and the per-viewport-pixel word-offset map that BOTH the
// sampling loop (0x4242bd) and the mouse inverse mapping (0x4240ae) read.
//
// The renderer's own state is presentation state, not VM state: the script only
// stages a position (0xba -> DAT_0045c064) or a delta (0xcb -> 0x402285), and
// the camera changes when a frame is drawn.
class PanoramaView : public SceneProjector {
public:
    // `path` is the asset name for reporting (0x40186e stores the resolved
    // path in DAT_00468b98 and only reloads when it changes). The source must
    // have KNOWN dimensions; the viewport is the 640x400 playfield band
    // (0x401ca6 only transforms points with 0x14 <= y <= 0x1a3).
    PanoramaView(std::string path, int source_width, int source_height,
                 const std::vector<std::uint16_t>& pixels, int viewport_width,
                 int viewport_height);

    const std::string& path() const { return path_; }
    int source_width() const { return source_width_; }
    int source_height() const { return source_height_; }
    int padding() const { return projection_.padding(); }
    int stride_words() const { return projection_.stride_words(); }
    int viewport_width() const { return viewport_width_; }
    int viewport_height() const { return viewport_height_; }
    const std::vector<std::uint16_t>& padded_rows() const { return padded_; }

    // 0x401ddf: consumes the staged pixel position (DAT_0045c064, already
    // clamped to 0..0x9bf by 0x401c21) and returns the camera delta that
    // 0x42423d applies. `staged_x < 0` means nothing was staged: the original
    // then leaves both pending deltas at 0. `alternate_mode` is DAT_00464878
    // != 0 and only affects the pending vertical delta (0 vs -6).
    PanoramaCameraSetup apply_staged_x(int staged_x, bool alternate_mode);

    // 0x4242bd's camera phase. Returns true when the offset map was
    // regenerated (a nonzero vertical delta or a zoom change).
    bool advance(double horizontal_degrees, double vertical_degrees,
                 double zoom = 1.0);

    // 0x4242bd's sampling phase (effect 0) into a viewport-sized surface.
    void render(Rgb16Surface& target) const;

    // 0x4240ae: screen point -> panorama source coordinate. Input is the
    // viewport coordinate, i.e. 0x401ca6 has already subtracted 0x14 from the
    // screen y and the caller adds it back to the result (see PanoramaPoint).
    PanoramaPoint canvas_point(int viewport_x, int viewport_y) const;

    // SceneProjector: 0x423e03's inverse direction, source pixel -> viewport
    // pixel. 0x40241c calls it on a record's rect corners when a panorama node
    // is the current layer; the shipped panorama scenes place sprites in the
    // same SOURCE coordinate space as their 0x72 objects, so 0x43c114's raw
    // +0x00/+0x02 only become a screen position through this projection.
    // Recovered geometry (the exact inverse of `panorama_mapping`), with
    // S = pixels per radian, F = zoom*S, dy = y - source_height/2:
    //   angle  = (x - source_width/2)/S - yaw        wrapped to [-pi, pi],
    //                                                rejected beyond 60 degrees
    //   depth  = S*cos(pitch)*cos(angle) + dy*sin(pitch)
    //   offset_x = F*S*sin(angle)/depth
    //   offset_y = F*(dy*cos(pitch) - S*cos(angle)*sin(pitch))/depth
    // A false result is the original's own reject: outside the source, beyond
    // the +/-60 degree window or the projected point leaving the viewport.
    bool project_source(int source_x, int source_y, int& viewport_x,
                        int& viewport_y) const override;

    // SceneProjector: 0x4240ae's exact inverse. `canvas_point` already returns
    // the wrapped source column; this is the mapping the background sampler
    // used for the same viewport pixel.
    bool map_viewport(int viewport_x, int viewport_y, int& source_x,
                      int& source_y) const override;

    // Conservative bbox of the source rect in viewport coordinates. It uses the
    // same x87-independent analytic projection as `project_source`, expanded by
    // a small margin because the sampler's table arithmetic can differ by a few
    // pixels. A rectangle that crosses the camera plane falls back to the whole
    // viewport; the per-pixel inverse test still decides the actual pixels.
    bool source_rect_viewport_bbox(int source_x, int source_y, int width,
                                   int height, int& x0, int& y0, int& x1,
                                   int& y1) const override;

    double yaw_degrees() const;
    double pitch_degrees() const;
    bool at_pitch_limit() const;

private:
    // Analytic source -> viewport projection. With `apply_window` it also
    // rejects the +/-60 degree window (0x423e03); without it, only points at or
    // behind the camera are rejected. The viewport bounds check is left to the
    // caller so the bbox can see partially visible rectangles.
    bool project_source_raw(int source_x, int source_y, int& viewport_x,
                            int& viewport_y, bool apply_window) const;
    std::string path_;
    int source_width_ = 0;
    int source_height_ = 0;
    int viewport_width_ = 0;
    int viewport_height_ = 0;
    double pixels_per_radian_ = 0;
    PanoramaLookupTables tables_;
    PanoramaProjection projection_;
    std::vector<std::uint16_t> padded_;
};

}  // namespace byzantine
