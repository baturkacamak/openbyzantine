#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace byzantine {

// 0x40f135 loads a shipped JPEG through the statically linked IJG library:
//   jerr = jpeg_std_error(&jerr)                  0x43e900
//   jpeg_CreateDecompress(&cinfo, 0x3d, 0x1a8)    0x43de70  (version 61 = 6a,
//                                                            424-byte struct)
//   jpeg_stdio_src(&cinfo, file)                  0x43e7b0
//   jpeg_read_header(&cinfo, TRUE)                0x43dfc0
//   jpeg_start_decompress(&cinfo)                 0x43e3d0
//   jpeg_read_scanlines(&cinfo, &row, 1)          0x43e560  (once per row)
//   0x40f080(row, output_width, destination)
//   jpeg_finish_decompress / jpeg_destroy_decompress  0x43e320 / 0x43df40
// The decompressor keeps libjpeg's defaults: JDCT_ISLOW, "fancy" upsampling and
// out_color_space JCS_RGB, so the 0x40f080 conversion receives a 3-byte RGB row.
//
// The port decodes to the same intermediate RGB888 plane. 0x40f080 is a
// separate, testable step (rgb_to_rgb565).
struct JpegHeader {
    std::int32_t width = 0;
    std::int32_t height = 0;
    std::int32_t components = 0;
    bool progressive = false;      // SOF2 instead of SOF0/SOF1
    int h_sampling[4] = {0, 0, 0, 0};  // per component, 1..4
    int v_sampling[4] = {0, 0, 0, 0};
    int quant_table[4] = {0, 0, 0, 0};
};

// One decoded image: row-major RGB888, 3 bytes per pixel (libjpeg's JCS_RGB).
struct JpegImage {
    std::int32_t width = 0;
    std::int32_t height = 0;
    std::vector<std::uint8_t> rgb;

    std::size_t pixel_count() const {
        return static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
    }
};

// Marker-only walk: validates the stream up to the first SOS and reports the
// frame header. Throws on an unsupported process (SOF3..SOF15), arithmetic
// coding, 12-bit precision or a missing SOF.
JpegHeader read_jpeg_header(const std::vector<std::uint8_t>& data);

// Full decode of a baseline (SOF0), extended sequential (SOF1) or progressive
// (SOF2) 8-bit JPEG with 1..4 components. Everything else is rejected instead
// of guessed: SOF3/SOF5..SOF15 (lossless/arithmetic/hierarchical), DAC, DNL,
// 12-bit precision, 2- or 4-component colour (no unverified transform), a
// second SOF or a stream that does not end at EOI.
//
// Implementation notes (see analysis/rea/JPEG_DECODE.md):
//  * Huffman decoding is sequential (canonical mincode/maxcode/valptr walk).
//  * Coefficients are dequantized with the parsed DQT tables and transformed
//    with libjpeg 6a's jidctint integer IDCT (CONST_BITS 13, PASS1_BITS 2).
//  * Upsampling of 2:1 factors uses the 3:1 triangle filter of libjpeg's
//    "fancy" upsampler; other factors are nearest-neighbour replication.
//  * YCbCr -> RGB uses the jdcolor.c 16.16 fixed-point constants.
// Progressive successive-approximation refinement follows the process-2 rules;
// the shipped CD1..CD6 JPEGs are all baseline SOF0, so that path is covered by
// synthetic streams only.
JpegImage decode_jpeg(const std::vector<std::uint8_t>& data);

// Reads and decodes one file. A missing or unreadable file is an error.
JpegImage decode_jpeg_file(const std::string& path);

// 0x40f080 under display mode 6 (`(p << 8) & 0xf800 | (p >> 5) & 0xffe0 |
// (p >> 19) & 0x1f` for a little-endian RGB byte triple): r>>3, g>>2, b>>3.
std::vector<std::uint16_t> rgb_to_rgb565(const JpegImage& image);

}  // namespace byzantine
