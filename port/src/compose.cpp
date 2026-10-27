#include "byzantine/compose.hpp"

#include <fstream>
#include <stdexcept>

namespace byzantine {

Rgb16Surface make_surface(std::int32_t width, std::int32_t height,
                          std::uint16_t fill) {
    if (width < 0 || height < 0) {
        throw std::runtime_error("surface size must not be negative");
    }
    Rgb16Surface surface;
    surface.width = width;
    surface.height = height;
    surface.pixels.assign(static_cast<std::size_t>(width) *
                              static_cast<std::size_t>(height),
                          fill);
    return surface;
}

FrameBlit blit_frame(Rgb16Surface& target, const DecodedFrame& frame,
                     std::int32_t x, std::int32_t y) {
    FrameBlit result;
    result.x = x;
    result.y = y;
    if (frame.width <= 0 || frame.height <= 0) return result;
    const auto width = static_cast<std::size_t>(frame.width);
    const auto height = static_cast<std::size_t>(frame.height);
    if (frame.pixels.size() != width * height || frame.covered.size() != width * height) {
        throw std::runtime_error("decoded frame buffer does not match its geometry");
    }
    for (std::size_t row = 0; row < height; ++row) {
        for (std::size_t column = 0; column < width; ++column) {
            const auto index = row * width + column;
            if (frame.covered[index] == 0) continue;
            ++result.covered;
            const auto tx = x + static_cast<std::int32_t>(column);
            const auto ty = y + static_cast<std::int32_t>(row);
            if (!target.inside(tx, ty)) {
                ++result.clipped;
                continue;
            }
            target.at(tx, ty) = frame.pixels[index];
            ++result.written;
            result.drawn = true;
        }
    }
    return result;
}

ImageBlit blit_image(Rgb16Surface& target, std::int32_t image_width,
                     std::int32_t image_height,
                     const std::uint16_t* pixels, std::int32_t x,
                     std::int32_t y) {
    ImageBlit result;
    if (image_width <= 0 || image_height <= 0 || pixels == nullptr) return result;
    for (std::int32_t row = 0; row < image_height; ++row) {
        const auto ty = y + row;
        for (std::int32_t column = 0; column < image_width; ++column) {
            const auto pixel =
                pixels[static_cast<std::size_t>(row) *
                           static_cast<std::size_t>(image_width) +
                       static_cast<std::size_t>(column)];
            const auto tx = x + column;
            if (!target.inside(tx, ty)) {
                ++result.clipped;
                continue;
            }
            target.at(tx, ty) = pixel;
            ++result.written;
            result.placed = true;
        }
    }
    return result;
}

bool blit_image_2x_at(Rgb16Surface& target, std::int32_t image_width,
                      std::int32_t image_height, const std::uint16_t* pixels,
                      std::int32_t x, std::int32_t y) {
    if (image_width <= 0 || image_height <= 0 || pixels == nullptr) return false;
    bool placed = false;
    for (std::int32_t row = 0; row < image_height; ++row) {
        const auto ty = y + row * 2;
        for (std::int32_t column = 0; column < image_width; ++column) {
            const auto pixel =
                pixels[static_cast<std::size_t>(row) *
                           static_cast<std::size_t>(image_width) +
                       static_cast<std::size_t>(column)];
            const auto tx = x + column * 2;
            for (std::int32_t dy = 0; dy < 2; ++dy) {
                for (std::int32_t dx = 0; dx < 2; ++dx) {
                    if (!target.inside(tx + dx, ty + dy)) continue;
                    target.at(tx + dx, ty + dy) = pixel;
                    placed = true;
                }
            }
        }
    }
    return placed;
}

bool blit_image_2x(Rgb16Surface& target, std::int32_t image_width,
                   std::int32_t image_height, const std::uint16_t* pixels) {
    if (image_width <= 0 || image_height <= 0 || pixels == nullptr) return false;
    if (image_width * 2 != target.width || image_height * 2 != target.height) {
        return false;
    }
    return blit_image_2x_at(target, image_width, image_height, pixels, 0, 0);
}

bool write_ppm(const Rgb16Surface& surface, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    out << "P6\n" << surface.width << ' ' << surface.height << "\n255\n";
    for (const auto pixel : surface.pixels) {
        const auto r = static_cast<std::uint16_t>((pixel >> 11) & 0x1f);
        const auto g = static_cast<std::uint16_t>((pixel >> 5) & 0x3f);
        const auto b = static_cast<std::uint16_t>(pixel & 0x1f);
        // Bit replication keeps full white at 255 instead of 248/252.
        const unsigned char rgb[3] = {
            static_cast<unsigned char>((r << 3) | (r >> 2)),
            static_cast<unsigned char>((g << 2) | (g >> 4)),
            static_cast<unsigned char>((b << 3) | (b >> 2))};
        out.write(reinterpret_cast<const char*>(rgb), 3);
    }
    return out.good();
}

}  // namespace byzantine
