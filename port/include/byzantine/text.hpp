#pragma once

#include "byzantine/compose.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace byzantine {

// One glyph of a `.SFN` font: the atlas rectangle and the pen advance
// (0x4061e7/0x4069a4 index the metrics by `char - 0x20`).
struct SfnGlyph {
    std::uint16_t x = 0;
    std::uint16_t y = 0;
    std::uint16_t advance = 0;
    std::uint16_t height = 0;
};

// One font slot of the registry 0x406cfe/0x406ef5. The metrics come from the
// `.SFN` file; the glyph coverage comes from the matching `gr` (fill) and `bk`
// (shadow) 8-bit TGA companions, which 0x406041 reads, flips top-down and
// 0x4067e2 blends as `(fill*(a+1) + dest*(255-a)) >> 8` (0x4066c6).
//
// The `.SFN` header is 24 bytes: u16 version (16), u16 0, u32 0, u16 glyph
// count, u16 atlas width, u16 atlas height, u16 0, u32 metrics offset (24),
// u32 pixel offset; then count*8 bytes of (x, y, advance, height) and a
// bottom-up 16-bit atlas. Only the metrics are needed for drawing: the glyph
// shape of the shipped fonts is the `gr`/`bk` mask pair, and the atlas is the
// pre-rendered copy the original keeps for 0x4069a4's other path.
struct SfnFont {
    int glyph_count = 0;
    int atlas_width = 0;
    int atlas_height = 0;
    std::vector<SfnGlyph> glyphs;
    std::vector<std::uint8_t> gr;  // atlas_width*atlas_height, top-down
    std::vector<std::uint8_t> bk;  // atlas_width*atlas_height, top-down
    int line_height = 0;           // first glyph's height (0x487508/0x487518 + 6)
    // Fill colour the registry passes to 0x405e7e for this slot, and whether
    // the slot uses the shadow mask (0x4067e2 always blends the bk mask with 0).
    std::uint16_t fill = 0xffff;

    const SfnGlyph* glyph(char code) const;
};

// Throws std::runtime_error on a short/inconsistent SFN, a TGA that is not
// 8-bit greyscale of the atlas size, or an empty glyph table.
SfnFont load_sfn_font(const std::string& sfn_path, const std::string& gr_tga_path,
                      const std::string& bk_tga_path, std::uint16_t fill);

// 0x4063e0: draws `text` with `font` into `surface` at (x, y), one line per
// '\n', at most `max_lines` lines. `alignment` follows the original: 1 centres
// on x, 2 and 4 right-align to x, 3 left-aligns from x. Each byte is a glyph
// index `byte - 0x20` (the shipped text is CP1254, which fits the 223-glyph
// fonts). Bytes outside the font's range are skipped, never substituted.
// Returns the number of lines drawn.
int draw_text(Rgb16Surface& surface, const SfnFont& font, const std::string& text, int x,
              int y, int max_lines, int alignment);

// 0x4061e7/0x4062bc: the maximum line width / the width of one line.
int text_width(const SfnFont& font, const std::string& text);

// One `engine/<scene>ce` text table, as 0x412206 reads it: lines are joined
// with '\n' (CR dropped) and a line starting with '$' terminates the entry.
// Entry order is the id an opcode or an object's +0x14 uses.
struct SceneTextTable {
    std::vector<std::string> entries;
};

SceneTextTable parse_scene_text(const std::vector<std::uint8_t>& data);

}  // namespace byzantine
