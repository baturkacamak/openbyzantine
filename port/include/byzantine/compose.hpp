#pragma once

#include "byzantine/anims.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace byzantine {

// One RGB16 destination surface. The renderer's clip window is 640x400
// (0x40241c tests 0 <= x <= 0x27f and 0 <= y <= 399), but the surface size is
// explicit so a caller can compose a smaller frame.
struct Rgb16Surface {
    std::int32_t width = 0;
    std::int32_t height = 0;
    std::vector<std::uint16_t> pixels;  // row-major, RGB565

    bool inside(std::int32_t x, std::int32_t y) const {
        return x >= 0 && y >= 0 && x < width && y < height;
    }
    std::uint16_t& at(std::int32_t x, std::int32_t y) {
        return pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) +
                      static_cast<std::size_t>(x)];
    }
    std::uint16_t at(std::int32_t x, std::int32_t y) const {
        return pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) +
                      static_cast<std::size_t>(x)];
    }
};

// Allocates width*height pixels filled with `fill`.
Rgb16Surface make_surface(std::int32_t width, std::int32_t height,
                          std::uint16_t fill = 0);

// The original scene surface is 640x480 words: 0x42b280 clears 0x4b000 words
// (= 640*480) before the background is drawn, 0x41045b stores the 640x400
// background image at +0x6400 bytes (= 20 rows of 640 words), and 0x40241c
// clips a record against `0 <= y - 0x14 <= 399`. The 640x400 playfield is
// therefore rows 0x14..0x413 of that surface, and both the background and a
// record's stored y use the 480-row coordinate space. A port surface models the
// playfield band, so record y must be reduced by kPlayfieldTop.
constexpr std::int32_t kPlayfieldTop = 0x14;

// One 0x40bbfd draw into an existing surface: the frame's covered pixels are
// written at (x + column, y + row). Uncovered pixels keep the destination, and
// everything outside the surface is clipped (the original clips through
// 0x40241c before drawing).
struct FrameBlit {
    bool drawn = false;       // at least one pixel landed inside the surface
    std::int32_t x = 0;       // requested position
    std::int32_t y = 0;
    std::size_t covered = 0;  // pixels the frame covers
    std::size_t written = 0;  // covered pixels that landed inside
    std::size_t clipped = 0;  // covered pixels outside the surface
};

FrameBlit blit_frame(Rgb16Surface& target, const DecodedFrame& frame,
                     std::int32_t x, std::int32_t y, bool shadow = false);

// 0x40bbfd's `param_6` (the record's `+0x09 & 0x80`): the destination is read
// per covered pixel and written back at 5/8 (`0x42b5b2` -> `0x42b395`:
// `(value * 5) >> 3` per channel), instead of the frame's own colour. The
// record therefore draws as a shadow of its own silhouette over the picture
// that is already there. `0x43b130` passes the flag to the blitter, so this is
// the same draw with a different pixel source.
std::uint16_t shade_pixel(std::uint16_t pixel);

// Copies an RGB565 image row by row at (x, y), clipping everything outside the
// surface. This is the same unconditional row copy 0x42b679 performs for the
// background/panorama stage: no transparency, no blending.
struct ImageBlit {
    bool placed = false;      // at least one pixel landed inside the surface
    std::size_t written = 0;
    std::size_t clipped = 0;
};

ImageBlit blit_image(Rgb16Surface& target, std::int32_t image_width,
                     std::int32_t image_height,
                     const std::uint16_t* pixels, std::int32_t x,
                     std::int32_t y);

// 0x43a248 mode 2 for an Indeo IV41 AVI: the destination rectangle is exactly
// twice the decoded 320x200 frame. 0xbd's operands 2/3 (0x40d031's param_3/
// param_4) are the destination x/y in the 640x480 surface, so the doubled
// rectangle can start away from the playfield origin; the shipped clips pass
// 0,0. This is an exact 2x pixel doubling, not a resampling filter.
bool blit_image_2x_at(Rgb16Surface& target, std::int32_t image_width,
                      std::int32_t image_height, const std::uint16_t* pixels,
                      std::int32_t x, std::int32_t y);

// Same as blit_image_2x_at with the destination at the playfield origin.
// Returns false when the doubled source does not equal the target surface.
bool blit_image_2x(Rgb16Surface& target, std::int32_t image_width,
                   std::int32_t image_height, const std::uint16_t* pixels);

// 0x423e03: the panorama renderer's SOURCE -> VIEWPORT projection. 0x40241c
// calls it on a record's rect corners when DAT_0045c060 != 0 (a 0xb9 node is
// the current layer) and DAT_00464880 != 0. The input is a panorama SOURCE
// pixel (0 <= x < source width, 0 <= y < source height), the output a playfield
// viewport pixel (0 <= x < 640, 0 <= y < 400); false means the original's own
// reject (outside the source, beyond the +/-60 degree window, or the projected
// point leaving the viewport).
//
// Shipped panorama scenes place a sprite (e.g. SM's `Smim00b`) in the SAME
// source coordinate space as their 0x72 objects, so a record's +0x00/+0x02
// cannot be used as a screen position there; the projection is what puts it on
// screen. A caller without a panorama passes nullptr and the record coordinates
// stay screen coordinates (background/close-up scenes).
struct SceneProjector {
    virtual ~SceneProjector() = default;
    virtual bool project_source(int source_x, int source_y, int& viewport_x,
                                int& viewport_y) const = 0;

    // 0x4240ae's inverse direction, used to draw a source-space record patch.
    // The background sampler and the mouse mapping read the same offset map, so
    // this is the exact mapping the panorama renderer used for that pixel. A
    // projector without a source-space layer (or a synthetic test projector)
    // leaves the default false and keeps the old screen-space placement.
    virtual bool map_viewport(int viewport_x, int viewport_y, int& source_x,
                              int& source_y) const {
        (void)viewport_x;
        (void)viewport_y;
        (void)source_x;
        (void)source_y;
        return false;
    }

    // Conservative viewport bbox of a source rectangle, including the part of a
    // rectangle that is only partially visible. `x0 > x1` means no viewport
    // pixel can map into it. Returns false when the projector has no inverse
    // mapping (old screen-space path).
    virtual bool source_rect_viewport_bbox(int source_x, int source_y, int width,
                                           int height, int& x0, int& y0, int& x1,
                                           int& y1) const {
        (void)source_x;
        (void)source_y;
        (void)width;
        (void)height;
        (void)x0;
        (void)y0;
        (void)x1;
        (void)y1;
        return false;
    }
};

// Writes a binary PPM (P6): RGB565 -> RGB888 with bit replication, which is
// enough to inspect a composed frame. Returns false when the file cannot be
// opened.
bool write_ppm(const Rgb16Surface& surface, const std::string& path);

}  // namespace byzantine
