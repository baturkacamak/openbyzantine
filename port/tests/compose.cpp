#include "byzantine/compose.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

byzantine::DecodedFrame make_frame(std::int16_t width, std::int16_t height,
                                   std::uint16_t colour, std::size_t skip_column) {
    byzantine::DecodedFrame frame;
    frame.width = width;
    frame.height = height;
    const auto size = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
    frame.pixels.assign(size, colour);
    frame.covered.assign(size, 1);
    // One transparent column, so the blit must leave the destination there.
    if (skip_column < static_cast<std::size_t>(width)) {
        for (std::int16_t row = 0; row < height; ++row) {
            frame.covered[static_cast<std::size_t>(row) * static_cast<std::size_t>(width) +
                          skip_column] = 0;
        }
    }
    frame.run_pixels = size - static_cast<std::size_t>(height);
    return frame;
}
}  // namespace

int main() {
    try {
        // A surface must be explicit; a negative size is rejected.
        bool rejected = false;
        try {
            (void)byzantine::make_surface(-1, 4);
        } catch (const std::runtime_error&) {
            rejected = true;
        }
        require(rejected, "accepted a negative surface size");

        auto surface = byzantine::make_surface(8, 6, 0x1111);
        require(surface.width == 8 && surface.height == 6 &&
                    surface.pixels.size() == 48 && surface.at(7, 5) == 0x1111,
                "surface allocation mismatch");
        require(surface.inside(0, 0) && surface.inside(7, 5) &&
                    !surface.inside(8, 0) && !surface.inside(-1, 0) &&
                    !surface.inside(0, 6),
                "surface bounds mismatch");

        // A fully covered 2x2 frame at (1,1) writes its pixels and counts them.
        const auto frame = make_frame(2, 2, 0x2222, 2);  // no transparent column
        const auto first = byzantine::blit_frame(surface, frame, 1, 1);
        require(first.drawn && first.covered == 4 && first.written == 4 &&
                    first.clipped == 0,
                "in-bounds blit counters mismatch");
        require(surface.at(1, 1) == 0x2222 && surface.at(2, 2) == 0x2222 &&
                    surface.at(0, 0) == 0x1111 && surface.at(3, 2) == 0x1111,
                "in-bounds blit pixel mismatch");

        // Transparent pixels keep the destination (command 3 leaves it alone).
        auto transparent = byzantine::make_surface(8, 6, 0x1111);
        const auto holey = make_frame(3, 2, 0x3333, 1);
        const auto hole = byzantine::blit_frame(transparent, holey, 0, 0);
        require(hole.covered == 4 && hole.written == 4 && hole.clipped == 0,
                "transparent column counters mismatch");
        require(transparent.at(0, 0) == 0x3333 && transparent.at(1, 0) == 0x1111 &&
                    transparent.at(2, 0) == 0x3333 && transparent.at(1, 1) == 0x1111,
                "transparent column overwrote the destination");

        // Clipping: a frame partly outside the surface writes only what fits.
        auto clipped = byzantine::make_surface(4, 4, 0);
        const auto big = make_frame(3, 3, 0x4444, 3);
        const auto edge = byzantine::blit_frame(clipped, big, 2, 2);
        require(edge.drawn && edge.covered == 9 && edge.written == 4 &&
                    edge.clipped == 5 && clipped.at(3, 3) == 0x4444,
                "clipped blit counters mismatch");
        const auto outside = byzantine::blit_frame(clipped, big, 9, 9);
        require(!outside.drawn && outside.written == 0 && outside.clipped == 9,
                "fully clipped blit was reported as drawn");
        const auto negative = byzantine::blit_frame(clipped, big, -1, -1);
        require(negative.drawn && negative.written == 4 && negative.clipped == 5,
                "negative-origin blit mismatch");

        // A frame whose buffers do not match its geometry is rejected.
        auto broken = big;
        broken.pixels.pop_back();
        bool rejected_frame = false;
        try {
            (void)byzantine::blit_frame(clipped, broken, 0, 0);
        } catch (const std::runtime_error&) {
            rejected_frame = true;
        }
        require(rejected_frame, "accepted a decoded frame with a short buffer");

        // Image blit (the background/panorama stage): every pixel is copied,
        // nothing outside the surface is written, and clipping is counted.
        {
            auto canvas = byzantine::make_surface(6, 4, 0x1111);
            const std::vector<std::uint16_t> picture(3 * 2, 0x2222);
            const auto full = byzantine::blit_image(canvas, 3, 2, picture.data(), 1, 1);
            require(full.placed && full.written == 6 && full.clipped == 0,
                    "image blit must place every pixel");
            require(canvas.at(1, 1) == 0x2222 && canvas.at(0, 0) == 0x1111,
                    "image blit position");
            const auto clipped =
                byzantine::blit_image(canvas, 3, 2, picture.data(), -2, 3);
            require(clipped.placed && clipped.written == 1 && clipped.clipped == 5,
                    "image blit clipping counts");
            const auto outside =
                byzantine::blit_image(canvas, 3, 2, picture.data(), 10, 10);
            require(!outside.placed && outside.written == 0 &&
                        outside.clipped == 6,
                    "an off-surface image must write nothing");
            const auto empty = byzantine::blit_image(canvas, 0, 2, picture.data(), 0, 0);
            require(!empty.placed && empty.written == 0, "a zero-size image is a no-op");
        }

        // 0x43a248 mode 2: the 320x200 conversation frame fills the 640x400
        // playfield by exact 2x pixel doubling, at the playfield origin.
        {
            auto target = byzantine::make_surface(4, 4, 0x0000);
            const std::vector<std::uint16_t> source{0x1111, 0x2222, 0x3333, 0x4444};
            require(byzantine::blit_image_2x(target, 2, 2, source.data()),
                    "2x image blit rejected a doubled target");
            require(target.at(0, 0) == 0x1111 && target.at(1, 0) == 0x1111 &&
                        target.at(0, 1) == 0x1111 && target.at(1, 1) == 0x1111,
                    "2x image blit did not double the top-left pixel");
            require(target.at(2, 0) == 0x2222 && target.at(3, 1) == 0x2222 &&
                        target.at(0, 2) == 0x3333 && target.at(1, 3) == 0x3333 &&
                        target.at(2, 2) == 0x4444 && target.at(3, 3) == 0x4444,
                    "2x image blit pixel order");
            require(!byzantine::blit_image_2x(target, 2, 3, source.data()),
                    "2x image blit accepted a non-doubled size");
            require(!byzantine::blit_image_2x(target, 0, 2, source.data()),
                    "2x image blit accepted a zero-size source");
        }

        // PPM output: 8x6 -> a P6 header plus 3 bytes per pixel.
        const auto path = std::filesystem::temp_directory_path() / "byz-compose.ppm";
        require(byzantine::write_ppm(surface, path.string()), "PPM write failed");
        std::ifstream input(path, std::ios::binary);
        std::string magic;
        std::int32_t width = 0;
        std::int32_t height = 0;
        std::int32_t levels = 0;
        input >> magic >> width >> height >> levels;
        require(magic == "P6" && width == 8 && height == 6 && levels == 255,
                "PPM header mismatch");
        std::filesystem::remove(path);

        std::cout << "composition tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
