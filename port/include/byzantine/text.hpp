#pragma once

#include "byzantine/compose.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
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
    // 0x406cbf sets DAT_0045d0ac when MESSAGES.TXT line 0x3e contains
    // "English"; 0x4061e7 then indexes high bytes directly (byte-0x20) instead
    // of the non-English byte-0x41 path. The pack resolver selects this.
    bool english_high_bytes = false;

    const SfnGlyph* glyph(char code) const;
};

// Throws std::runtime_error on a short/inconsistent SFN, a TGA that is not
// 8-bit greyscale of the atlas size, or an empty glyph table.
SfnFont load_sfn_font(const std::string& sfn_path, const std::string& gr_tga_path,
                      const std::string& bk_tga_path, std::uint16_t fill);

// 0x406cfe slot 4 (CAPTIONS.SFN + CAPTIONS.TGA): the original calls only
// 0x40612a for this slot (no 0x406153 bk atlas), so the shadow mask is zero.
// The single TGA is the coverage atlas; the fill is the green 0x03eb.
SfnFont load_sfn_font_single_atlas(const std::string& sfn_path,
                                   const std::string& atlas_tga_path,
                                   std::uint16_t fill);

// MESSAGES.TXT is CP1254, while the SFN atlases index the shipped game bytes
// (0x4061e7 uses byte-0x41). Convert the twelve Turkish letters so the common
// message table (menu text/labels) draws with the game font instead of
// dropping them. ASCII passes through unchanged.
std::string cp1254_to_game_text(const std::string& text);

// UTF-8 -> the game font's Turkish byte encoding (Ç/Ş/İ/Ğ/Ü/Ö and their
// lowercase forms). Used for port-written Turkish UI strings, which live in
// UTF-8 source files rather than the shipped CP1254/game data.
std::string utf8_to_turkish_game_text(const std::string& text);

// 0x4063e0: draws `text` with `font` into `surface` at (x, y), one line per
// '\n', at most `max_lines` lines. `alignment` follows the original: 1 centres
// on x, 2 and 4 right-align to x, 3 left-aligns from x. Each byte is a glyph
// index `byte - 0x20` (the shipped text is CP1254, which fits the 223-glyph
// fonts). Bytes outside the font's range are skipped, never substituted.
// Returns the number of lines drawn.
int draw_text(Rgb16Surface& surface, const SfnFont& font, const std::string& text, int x,
              int y, int max_lines, int alignment,
              std::optional<std::uint16_t> fill = std::nullopt);

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
