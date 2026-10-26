#include "byzantine/text.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace byzantine {
namespace {

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open font/text file: " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::uint16_t le16(const std::vector<std::uint8_t>& data, std::size_t at) {
    if (at > data.size() || data.size() - at < 2) {
        throw std::runtime_error("truncated font header");
    }
    return static_cast<std::uint16_t>(data[at] | (data[at + 1] << 8));
}

std::uint32_t le32(const std::vector<std::uint8_t>& data, std::size_t at) {
    if (at > data.size() || data.size() - at < 4) {
        throw std::runtime_error("truncated font header");
    }
    return static_cast<std::uint32_t>(data[at]) |
           (static_cast<std::uint32_t>(data[at + 1]) << 8) |
           (static_cast<std::uint32_t>(data[at + 2]) << 16) |
           (static_cast<std::uint32_t>(data[at + 3]) << 24);
}

// 0x406041: an 18-byte TGA header, then either 8-bit greyscale (bpp 8) or a
// 16-bit 555 image converted by 0x405c19. The shipped companions are type 3,
// 8 bpp, with descriptor bit 5 clear (bottom-up rows), so the rows are flipped.
// Anything else is rejected instead of guessed.
std::vector<std::uint8_t> read_alpha_tga(const std::string& path, int width, int height) {
    const auto data = read_file(path);
    if (data.size() < 18) throw std::runtime_error("short TGA: " + path);
    const auto type = data[2];
    const auto bpp = data[16];
    const auto w = le16(data, 12);
    const auto h = le16(data, 14);
    const auto descriptor = data[17];
    if (w != width || h != height) {
        throw std::runtime_error("TGA size does not match the SFN atlas: " + path);
    }
    if (type != 3 || bpp != 8) {
        throw std::runtime_error("TGA is not an 8-bit greyscale atlas: " + path);
    }
    const std::size_t pixels = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
    if (data.size() < 18 + pixels) throw std::runtime_error("truncated TGA: " + path);
    std::vector<std::uint8_t> mask(data.begin() + 18,
                                   data.begin() + static_cast<std::ptrdiff_t>(18 + pixels));
    if ((descriptor & 0x20) == 0) {
        for (int row = 0; row < height / 2; ++row) {
            auto* top = mask.data() + static_cast<std::size_t>(row) * width;
            auto* bottom = mask.data() + static_cast<std::size_t>(height - 1 - row) * width;
            std::swap_ranges(top, top + width, bottom);
        }
    }
    return mask;
}

// 0x4066c6's blend: `(fore*(a+1) + back*(255-a)) >> 8` per channel. The
// original runs it through the 0x405b90 multiply table; the shipped table is
// the plain product of the 5/6/5 channels, so the arithmetic form is exact.
std::uint16_t blend(std::uint16_t fore, std::uint16_t back, std::uint8_t alpha) {
    const auto a = static_cast<unsigned>(alpha) + 1;
    const auto b = 256u - a;
    const unsigned r = (((fore >> 11) & 0x1f) * a + ((back >> 11) & 0x1f) * b) >> 8;
    const unsigned g = (((fore >> 5) & 0x3f) * a + ((back >> 5) & 0x3f) * b) >> 8;
    const unsigned bl = ((fore & 0x1f) * a + (back & 0x1f) * b) >> 8;
    return static_cast<std::uint16_t>((r << 11) | (g << 5) | bl);
}

}  // namespace

const SfnGlyph* SfnFont::glyph(char code) const {
    const auto index = static_cast<std::uint8_t>(code);
    if (index < 0x20) return nullptr;
    // 0x4061e7/0x4063e0 call FUN_00405bf4 for bytes above 0x7f. In the
    // non-English branch that function returns `byte - 0x21`, and the drawer
    // then subtracts the usual 0x20, so the Turkish glyph index is
    // `byte - 0x41` (the shipped Turkish text bytes 0xa6..0xad land on the
    // font's 0x85..0x8c glyphs). Bytes 0x20..0x7f keep `byte - 0x20`.
    const auto glyph_index = index > 0x7f
                                 ? static_cast<std::size_t>(index - 0x41)
                                 : static_cast<std::size_t>(index - 0x20);
    if (glyph_index >= glyphs.size()) return nullptr;
    return &glyphs[glyph_index];
}

SfnFont load_sfn_font(const std::string& sfn_path, const std::string& gr_tga_path,
                      const std::string& bk_tga_path, std::uint16_t fill) {
    const auto data = read_file(sfn_path);
    if (data.size() < 24) throw std::runtime_error("short SFN: " + sfn_path);
    const auto version = le16(data, 0);
    if (version != 16) throw std::runtime_error("unsupported SFN version: " + sfn_path);
    const auto count = le16(data, 8);
    const auto width = le16(data, 10);
    const auto height = le16(data, 12);
    const auto metrics = le32(data, 16);
    const auto pixels = le32(data, 20);
    if (count == 0 || width == 0 || height == 0) {
        throw std::runtime_error("empty SFN glyph table: " + sfn_path);
    }
    if (metrics < 24 || metrics + static_cast<std::size_t>(count) * 8 > data.size()) {
        throw std::runtime_error("SFN metrics outside the file: " + sfn_path);
    }
    if (pixels + static_cast<std::size_t>(width) * height * 2 > data.size()) {
        throw std::runtime_error("SFN atlas outside the file: " + sfn_path);
    }
    SfnFont font;
    font.glyph_count = count;
    font.atlas_width = width;
    font.atlas_height = height;
    font.fill = fill;
    font.glyphs.reserve(count);
    for (std::size_t index = 0; index < count; ++index) {
        const auto at = metrics + index * 8;
        SfnGlyph glyph;
        glyph.x = le16(data, at);
        glyph.y = le16(data, at + 2);
        glyph.advance = le16(data, at + 4);
        glyph.height = le16(data, at + 6);
        // The shipped fonts define fewer glyphs than the header count and fill
        // the rest with 0xcdcd words. That is an undefined code, not a rect.
        if (glyph.x == 0xcdcd && glyph.y == 0xcdcd && glyph.advance == 0xcdcd &&
            glyph.height == 0xcdcd) {
            font.glyphs.push_back(SfnGlyph{});
            continue;
        }
        if (glyph.x + glyph.advance > width || glyph.y + glyph.height > height) {
            throw std::runtime_error("SFN glyph outside the atlas: " + sfn_path);
        }
        font.glyphs.push_back(glyph);
    }
    font.line_height = font.glyphs.front().height;
    font.gr = read_alpha_tga(gr_tga_path, width, height);
    font.bk = read_alpha_tga(bk_tga_path, width, height);
    return font;
}

int text_width(const SfnFont& font, const std::string& text) {
    int width = 0;
    for (std::size_t index = 0; index < text.size(); ++index) {
        if (text[index] == '\n') break;
        const auto* glyph = font.glyph(text[index]);
        if (glyph != nullptr) width += glyph->advance;
    }
    return width;
}

int draw_text(Rgb16Surface& surface, const SfnFont& font, const std::string& text, int x,
              int y, int max_lines, int alignment) {
    if (alignment == 1) {
        x -= text_width(font, text) / 2;
    } else if (alignment == 2 || alignment == 4) {
        x -= text_width(font, text);
    }
    const int line_height = font.line_height > 0 ? font.line_height : 1;
    int pen_x = x;
    int pen_y = y;
    int lines = 1;
    for (const auto character : text) {
        if (character == '\n') {
            if (max_lines > 0 && lines + 1 > max_lines) break;
            ++lines;
            pen_x = x;
            pen_y += line_height;
            continue;
        }
        const auto* glyph = font.glyph(character);
        if (glyph == nullptr) continue;
        for (int row = 0; row < glyph->height; ++row) {
            const int target_y = pen_y + row;
            if (target_y < 0 || target_y >= surface.height) continue;
            for (int column = 0; column < glyph->advance; ++column) {
                const int target_x = pen_x + column;
                if (target_x < 0 || target_x >= surface.width) continue;
                const auto at = static_cast<std::size_t>(glyph->y + row) * font.atlas_width +
                                static_cast<std::size_t>(glyph->x + column);
                const auto shadow = font.bk[at];
                // 0x4067e2: the bk mask blends black, then the gr mask blends
                // the fill colour over that.
                auto& pixel = surface.pixels[static_cast<std::size_t>(target_y) *
                                                 surface.width +
                                             target_x];
                if (shadow != 0) pixel = blend(0, pixel, shadow);
                const auto coverage = font.gr[at];
                if (coverage != 0) pixel = blend(font.fill, pixel, coverage);
            }
        }
        pen_x += glyph->advance;
    }
    return lines;
}

SceneTextTable parse_scene_text(const std::vector<std::uint8_t>& data) {
    // 0x412206: accumulate lines; a line starting with '$' closes the entry.
    // CR is dropped, lines are joined with '\n'. A file that ends inside an
    // entry is rejected rather than silently truncated.
    SceneTextTable table;
    std::string current;
    std::size_t at = 0;
    bool closed = false;
    while (at < data.size()) {
        const auto begin = at;
        while (at < data.size() && data[at] != '\n') ++at;
        std::size_t end = at;
        if (at < data.size()) ++at;
        if (end > begin && data[end - 1] == '\r') --end;
        const std::string line(data.begin() + static_cast<std::ptrdiff_t>(begin),
                               data.begin() + static_cast<std::ptrdiff_t>(end));
        if (!line.empty() && line.front() == '$') {
            table.entries.push_back(current);
            current.clear();
            closed = true;
            continue;
        }
        if (!current.empty()) current += '\n';
        current += line;
        closed = false;
    }
    if (!closed && !current.empty()) {
        throw std::runtime_error("scene text entry is not terminated by '$'");
    }
    return table;
}

}  // namespace byzantine
