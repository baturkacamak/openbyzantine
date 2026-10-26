#include "byzantine/text.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

void write_file(const std::filesystem::path& path, const std::vector<std::uint8_t>& bytes) {
    std::ofstream output(path, std::ios::binary);
    if (!output) throw std::runtime_error("cannot write " + path.string());
    output.write(reinterpret_cast<const char*>(bytes.data()),
                 static_cast<std::streamsize>(bytes.size()));
}

void push16(std::vector<std::uint8_t>& out, unsigned value) {
    out.push_back(static_cast<std::uint8_t>(value));
    out.push_back(static_cast<std::uint8_t>(value >> 8));
}
void push32(std::vector<std::uint8_t>& out, unsigned value) {
    push16(out, value & 0xffff);
    push16(out, value >> 16);
}

// version, 0, 0, glyph count, atlas w/h, 0, metrics offset 24, pixel offset,
// then count*8 metrics and a 16-bit atlas. 0x406cfe's loader only needs the
// metrics; the atlas is present so the parser's bounds check is exercised.
std::vector<std::uint8_t> make_sfn(int width, int height,
                                   const std::vector<std::array<unsigned, 4>>& metrics) {
    std::vector<std::uint8_t> out;
    push16(out, 16);
    push16(out, 0);
    push32(out, 0);
    push16(out, static_cast<unsigned>(metrics.size()));
    push16(out, static_cast<unsigned>(width));
    push16(out, static_cast<unsigned>(height));
    push16(out, 0);
    const auto pixels = 24 + metrics.size() * 8;
    push32(out, 24);
    push32(out, static_cast<unsigned>(pixels));
    for (const auto& glyph : metrics) {
        for (const auto value : glyph) push16(out, value);
    }
    for (int i = 0; i < width * height; ++i) push16(out, 0x7fff);
    return out;
}

// 18-byte TGA header + top-down 8-bit greyscale (descriptor bit 5 set).
std::vector<std::uint8_t> make_tga(int width, int height,
                                   const std::vector<std::uint8_t>& pixels) {
    std::vector<std::uint8_t> out = {0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 8, 0x20};
    out[12] = static_cast<std::uint8_t>(width);
    out[13] = static_cast<std::uint8_t>(width >> 8);
    out[14] = static_cast<std::uint8_t>(height);
    out[15] = static_cast<std::uint8_t>(height >> 8);
    out.insert(out.end(), pixels.begin(), pixels.end());
    return out;
}

}  // namespace

int main() {
    try {
        namespace fs = std::filesystem;
        const auto root = fs::temp_directory_path() / "byz-text-test";
        fs::remove_all(root);
        fs::create_directories(root);

        // 8x8 atlas, two glyphs: glyph 0 = ' ' (4x4, empty mask) and glyph 1 =
        // '!' (3x3 at 4,0) with gr=0xff at its top-left pixel and 0x80 at the
        // centre; bk=0x40 at the top-left too.
        std::vector<std::uint8_t> gr(64, 0);
        std::vector<std::uint8_t> bk(64, 0);
        gr[0 * 8 + 4] = 0xff;
        gr[1 * 8 + 5] = 0x80;
        bk[0 * 8 + 4] = 0x40;
        const auto sfn = make_sfn(8, 8, {{0, 0, 4, 4}, {4, 0, 3, 3}});
        write_file(root / "t.SFN", sfn);
        write_file(root / "GR.TGA", make_tga(8, 8, gr));
        write_file(root / "BK.TGA", make_tga(8, 8, bk));

        const auto font = byzantine::load_sfn_font((root / "t.SFN").string(),
                                                   (root / "GR.TGA").string(),
                                                   (root / "BK.TGA").string(), 0x0360);
        require(font.glyph_count == 2 && font.atlas_width == 8 &&
                    font.atlas_height == 8 && font.line_height == 4 &&
                    font.glyphs[1].advance == 3 && font.fill == 0x0360,
                "SFN metrics/line height mismatch");
        require(font.glyph(' ') != nullptr && font.glyph('!') != nullptr &&
                    font.glyph('\x1f') == nullptr && font.glyph('\xff') == nullptr,
                "glyph lookup outside the table");
        require(byzantine::text_width(font, "  !") == 4 + 4 + 3,
                "text width/advance mismatch");
        require(byzantine::text_width(font, " !\n!!") == 4 + 3,
                "text width did not stop at the newline");

        // 0x4067e2: the bk mask blends black, the gr mask the fill colour.
        {
            byzantine::Rgb16Surface surface = byzantine::make_surface(8, 8, 0xffff);
            const auto lines = byzantine::draw_text(surface, font, "!", 1, 1, 3, 3);
            require(lines == 1, "draw_text line count mismatch");
            require(surface.at(1, 1) == 0x0360, "glyph fill pixel missing");
            require(surface.at(2, 2) != 0xffff, "glyph mask blend did not run");
            require(surface.at(3, 1) == 0xffff, "unmasked pixel changed");
        }
        {
            byzantine::Rgb16Surface surface = byzantine::make_surface(8, 8, 0xffff);
            byzantine::draw_text(surface, font, "!", 0, 0, 3, 3);
            // 0x40 shadow over white is darker than white, then 0xff fills.
            require(surface.at(0, 0) == 0x0360, "fill over the shadow is missing");
            require(surface.at(1, 1) != 0xffff, "0x80 mask was not blended");
            require(surface.at(0, 1) == 0xffff, "unmasked pixel changed");
        }
        // Right alignment (0x4063e0 param 2/4): the 3-pixel glyph ends at x.
        {
            byzantine::Rgb16Surface surface = byzantine::make_surface(16, 8, 0);
            byzantine::draw_text(surface, font, "!", 10, 0, 3, 4);
            require(surface.at(7, 0) == 0x0360 && surface.at(6, 0) == 0,
                    "right alignment mismatch");
        }
        // max_lines stops before the third line.
        {
            byzantine::Rgb16Surface surface = byzantine::make_surface(8, 16, 0);
            const auto lines = byzantine::draw_text(surface, font, "!\n!\n!", 0, 0, 2, 3);
            require(lines == 2, "max_lines did not stop the draw");
            require(surface.at(0, 0) == 0x0360 && surface.at(0, 4) == 0x0360 &&
                        surface.at(0, 8) == 0,
                    "max_lines drew an extra line");
        }
        // An unsupported code is skipped, never substituted.
        {
            byzantine::Rgb16Surface surface = byzantine::make_surface(8, 8, 0);
            const auto lines = byzantine::draw_text(surface, font, "\xff!", 0, 0, 3, 3);
            require(lines == 1 && surface.at(0, 0) == 0x0360,
                    "unknown glyph was not skipped");
        }

        // Rejections.
        {
            const auto rejects = [&](const std::vector<std::uint8_t>& sf, const char* what) {
                write_file(root / "bad.SFN", sf);
                bool rejected = false;
                try {
                    byzantine::load_sfn_font((root / "bad.SFN").string(),
                                             (root / "GR.TGA").string(),
                                             (root / "BK.TGA").string(), 0);
                } catch (const std::runtime_error&) {
                    rejected = true;
                }
                require(rejected, what);
            };
            auto bad = sfn;
            bad[0] = 15;
            rejects(bad, "accepted a wrong SFN version");
            bad = sfn;
            bad.resize(24 + 8 + 2);
            rejects(bad, "accepted a truncated SFN atlas");
            bad = make_sfn(8, 8, {{7, 0, 4, 4}});
            rejects(bad, "accepted a glyph outside the atlas");
            auto bad_tga = make_tga(8, 8, gr);
            bad_tga[16] = 24;
            write_file(root / "BAD.TGA", bad_tga);
            bool rejected = false;
            try {
                byzantine::load_sfn_font((root / "t.SFN").string(),
                                         (root / "BAD.TGA").string(),
                                         (root / "BK.TGA").string(), 0);
            } catch (const std::runtime_error&) {
                rejected = true;
            }
            require(rejected, "accepted a non-8-bit TGA");
            rejected = false;
            try {
                byzantine::load_sfn_font((root / "t.SFN").string(),
                                         (root / "MISSING.TGA").string(),
                                         (root / "BK.TGA").string(), 0);
            } catch (const std::runtime_error&) {
                rejected = true;
            }
            require(rejected, "accepted a missing TGA");
        }

        // 0x412206: '$'-terminated entries, CR dropped, lines joined by '\n'.
        {
            const std::string text = "Birinci satir\r\nIkinci satir\r\n$\r\nUc\r\n$";
            const std::vector<std::uint8_t> bytes(text.begin(), text.end());
            const auto table = byzantine::parse_scene_text(bytes);
            require(table.entries.size() == 2 &&
                        table.entries[0] == "Birinci satir\nIkinci satir" &&
                        table.entries[1] == "Uc",
                    "scene text table mismatch");
            const std::string open = "Yarim";
            const std::vector<std::uint8_t> truncated(open.begin(), open.end());
            bool rejected = false;
            try {
                byzantine::parse_scene_text(truncated);
            } catch (const std::runtime_error&) {
                rejected = true;
            }
            require(rejected, "accepted an unterminated scene text entry");
        }

        fs::remove_all(root);
        std::cout << "text tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
