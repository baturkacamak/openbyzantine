#include "byzantine/rle.hpp"

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>

namespace byzantine {
namespace {

std::uint16_t read_u16(const std::vector<std::uint8_t>& data, std::size_t at) {
    if (at > data.size() || data.size() - at < 2) {
        throw std::runtime_error("truncated RLE word");
    }
    return static_cast<std::uint16_t>(data[at] | (data[at + 1] << 8));
}

std::uint32_t read_u32(const std::vector<std::uint8_t>& data, std::size_t at) {
    return static_cast<std::uint32_t>(read_u16(data, at)) |
           (static_cast<std::uint32_t>(read_u16(data, at + 2)) << 16);
}

void paint(RleFrame& frame, std::int32_t row, std::uint32_t column, std::uint16_t rgb555) {
    if (row < 0 || row >= frame.height || column >= frame.width) {
        throw std::runtime_error("RLE pixel is outside the frame");
    }
    const std::size_t at = (static_cast<std::size_t>(row) * frame.width + column) * 4;
    const auto expand = [](std::uint16_t value) -> std::uint8_t {
        return static_cast<std::uint8_t>((value << 3) | (value >> 2));
    };
    frame.rgba[at] = expand((rgb555 >> 10) & 31);
    frame.rgba[at + 1] = expand((rgb555 >> 5) & 31);
    frame.rgba[at + 2] = expand(rgb555 & 31);
    frame.rgba[at + 3] = 255;
}

}  // namespace

std::vector<RleFrame> decode_rle(const std::vector<std::uint8_t>& file) {
    if (file.size() < 10) {
        throw std::runtime_error("truncated RLE file header");
    }
    const std::uint16_t frame_count = read_u16(file, 8);
    std::size_t offset = 10;
    std::vector<RleFrame> frames;
    frames.reserve(frame_count);

    for (std::size_t frame_index = 0; frame_index < frame_count; ++frame_index) {
        if (offset > file.size() || file.size() - offset < 26) {
            throw std::runtime_error("truncated RLE frame header");
        }
        RleFrame frame;
        frame.width = read_u16(file, offset + 2);
        frame.height = read_u16(file, offset + 4);
        frame.x = static_cast<std::int16_t>(read_u16(file, offset + 6));
        frame.y = static_cast<std::int16_t>(read_u16(file, offset + 8));
        const std::uint32_t word_count = read_u32(file, offset + 22);
        offset += 26;
        if (frame.width == 0 || frame.height == 0 || frame.width > 4096 || frame.height > 4096) {
            throw std::runtime_error("invalid RLE frame dimensions");
        }
        if (word_count > (file.size() - offset) / 2) {
            throw std::runtime_error("truncated RLE frame payload");
        }
        const std::size_t payload_end = offset + static_cast<std::size_t>(word_count) * 2;
        frame.rgba.resize(static_cast<std::size_t>(frame.width) * frame.height * 4, 0);
        std::int32_t row = -1;
        std::uint32_t column = 0;

        auto next_word = [&]() -> std::uint16_t {
            if (offset >= payload_end) {
                throw std::runtime_error("RLE command exceeds frame payload");
            }
            const auto value = read_u16(file, offset);
            offset += 2;
            return value;
        };

        while (offset < payload_end && row < frame.height) {
            const std::uint16_t command = next_word();
            const std::uint16_t kind = command >> 14;
            const std::uint16_t count = command & 0x3fff;
            if (kind == 1) {
                ++row;
                column = count;
            } else if (kind == 3) {
                column += count;
            } else if (kind == 0) {
                if (row < 0 || row >= frame.height || column + count > frame.width) {
                    throw std::runtime_error("RLE literal run is outside the frame");
                }
                for (std::uint16_t i = 0; i < count; ++i) {
                    paint(frame, row, column++, next_word());
                }
            } else {
                const std::uint16_t color = next_word();
                if (row < 0 || row >= frame.height || column + count > frame.width) {
                    throw std::runtime_error("RLE repeated run is outside the frame");
                }
                for (std::uint16_t i = 0; i < count; ++i) {
                    paint(frame, row, column++, color);
                }
            }
        }
        if (row != frame.height || offset != payload_end) {
            throw std::runtime_error("RLE frame command stream ended unexpectedly");
        }
        frames.push_back(std::move(frame));
    }
    if (offset != file.size()) {
        throw std::runtime_error("unexpected bytes after RLE frames");
    }
    return frames;
}

}  // namespace byzantine
