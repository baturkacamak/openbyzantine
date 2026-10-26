#pragma once

#include <cstdint>
#include <vector>

namespace byzantine {

struct RleFrame {
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::int16_t x = 0;
    std::int16_t y = 0;
    std::vector<std::uint8_t> rgba;
};

// Reconstructs the game's RGB555 RLE frames from their original file bytes.
// Unpainted pixels remain transparent. Throws std::runtime_error on invalid input.
std::vector<RleFrame> decode_rle(const std::vector<std::uint8_t>& file);

}  // namespace byzantine
