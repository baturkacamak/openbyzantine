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
                     std::int32_t x, std::int32_t y);

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
// twice the decoded 320x200 frame and starts at the playfield origin (the
// original passes x=0, y=0x14 in the 640x480 surface). This is an exact 2x
// pixel doubling, not a resampling filter. Returns false when the doubled
// source does not equal the target surface.
bool blit_image_2x(Rgb16Surface& target, std::int32_t image_width,
                   std::int32_t image_height, const std::uint16_t* pixels);

// Writes a binary PPM (P6): RGB565 -> RGB888 with bit replication, which is
// enough to inspect a composed frame. Returns false when the file cannot be
// opened.
bool write_ppm(const Rgb16Surface& surface, const std::string& path);

}  // namespace byzantine
