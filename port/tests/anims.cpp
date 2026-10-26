#include "byzantine/anims.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

void put16(std::vector<std::uint8_t>& out, std::uint16_t value) {
    out.push_back(static_cast<std::uint8_t>(value & 0xff));
    out.push_back(static_cast<std::uint8_t>(value >> 8));
}
void put32(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (int shift = 0; shift < 32; shift += 8) {
        out.push_back(static_cast<std::uint8_t>((value >> shift) & 0xff));
    }
}

// One 26-byte frame header followed by `words` 16-bit words, exactly as
// 0x410078 reads it (header 0x1a bytes, then fread(buf, 2, words)).
void add_frame(std::vector<std::uint8_t>& out, std::int16_t width, std::int16_t height,
               std::int16_t x, std::int16_t y, std::uint32_t words) {
    put16(out, 0);                                   // +0x00
    put16(out, static_cast<std::uint16_t>(width));   // +0x02 -> node +0x0c
    put16(out, static_cast<std::uint16_t>(height));  // +0x04 -> node +0x10
    put16(out, static_cast<std::uint16_t>(x));       // +0x06 first-frame x
    put16(out, static_cast<std::uint16_t>(y));       // +0x08 first-frame y
    for (int i = 0; i < 6; ++i) put16(out, 0);       // +0x0a..+0x15 unused
    put32(out, words);                               // +0x16
    for (std::uint32_t i = 0; i < words; ++i) put16(out, static_cast<std::uint16_t>(i));
}

std::vector<std::uint8_t> make_asset(std::uint16_t signature,
                                     const std::vector<std::array<std::int16_t, 4>>& frames,
                                     std::uint32_t words) {
    std::vector<std::uint8_t> out;
    put16(out, signature);          // +0x00 signature (-> record +0x28)
    put32(out, 0);                  // +0x02 skipped
    put16(out, 0);                  // +0x06 skipped
    put16(out, static_cast<std::uint16_t>(frames.size()));  // +0x08 frame count
    for (const auto& frame : frames) {
        add_frame(out, frame[0], frame[1], frame[2], frame[3], words);
    }
    return out;
}
}  // namespace

int main(int argc, char** argv) {
    try {
        // 0x40cb26 reads signature(2) + skip(4) + skip(2) + frame_count(2); each
        // frame is a 26-byte header plus `words*2` bytes.
        const auto bytes = make_asset(
            1, {{640, 400, 0, 0}, {100, 125, 200, 14}, {69, 99, -3, -7}}, 3);
        const auto asset = byzantine::parse_anims(bytes);
        require(asset.signature == 1 && asset.frames.size() == 3,
                "animation header/frame count mismatch");
        require(asset.frames[0].width == 640 && asset.frames[0].height == 400 &&
                    asset.frames[0].x_offset == 0 && asset.frames[0].y_offset == 0,
                "frame 0 geometry mismatch");
        require(asset.frames[1].width == 100 && asset.frames[1].height == 125 &&
                    asset.frames[1].x_offset == 200 && asset.frames[1].y_offset == 14,
                "frame 1 geometry mismatch");
        require(asset.frames[2].x_offset == -3 && asset.frames[2].y_offset == -7,
                "signed frame offsets mismatch");
        require(asset.frames[2].words == 3, "frame word count mismatch");

        // A zero-frame asset is valid (0x40cb26 can return 0).
        const auto empty = byzantine::parse_anims(make_asset(0, {}, 0));
        require(empty.frames.empty(), "empty animation asset not accepted");

        // Truncated header, truncated frame header, truncated payload and
        // trailing bytes are all rejected instead of guessed.
        for (std::size_t cut : {std::size_t{9}, std::size_t{10}, std::size_t{20}}) {
            bool rejected = false;
            try {
                byzantine::parse_anims(std::vector<std::uint8_t>(bytes.begin(),
                                                                bytes.begin() + cut));
            } catch (const std::runtime_error&) {
                rejected = true;
            }
            require(rejected, "truncated animation asset accepted");
        }
        auto extra = bytes;
        extra.push_back(0);
        bool rejected_extra = false;
        try {
            byzantine::parse_anims(extra);
        } catch (const std::runtime_error&) {
            rejected_extra = true;
        }
        require(rejected_extra, "animation asset with trailing bytes accepted");

        // 0x40bbfd frame runs: command 1 opens a row at x = count, command 0 is a
        // literal run, command 2 repeats one colour, command 3 is transparent.
        // 4x2 frame: row 0 = 2 literals + 1 colour + 1 gap, row 1 starts at x=1.
        byzantine::AnimsFrame frame;
        frame.width = 4;
        frame.height = 2;
        frame.words = 13;
        const std::vector<std::uint16_t> runs{
            0x4000,              // row 0 starts at x=0
            0x0002, 0x1111, 0x2222,  // two literals
            0x8001, 0x3333,      // one colour pixel
            0xC001,              // one transparent pixel
            0x4001,              // row 1 starts at x=1
            0x0002, 0x4444, 0x5555,  // two literals
            0xC001,              // one transparent pixel
            0x4000};             // closing marker (row == height)
        const auto raw = byzantine::decode_anims_frame(frame, runs, false);
        require(raw.width == 4 && raw.height == 2 &&
                    raw.pixels.size() == 8 && raw.covered.size() == 8,
                "decoded frame geometry mismatch");
        require(raw.covered[0] == 1 && raw.pixels[0] == 0x1111 &&
                    raw.pixels[1] == 0x2222 && raw.pixels[2] == 0x3333 &&
                    raw.covered[3] == 0 &&
                    raw.covered[4] == 0 && raw.pixels[5] == 0x4444 &&
                    raw.pixels[6] == 0x5555 && raw.covered[7] == 0,
                "decoded frame pixels/coverage mismatch");
        require(raw.run_pixels == 5 && raw.skipped_pixels == 2,
                "decoded frame run counters mismatch");
        // 0x4164a0 (0x40be80's load-time pass): RGB555 -> RGB565 on pixel words.
        const auto converted = byzantine::decode_anims_frame(frame, runs);
        require(converted.pixels[0] == 0x2211 && converted.pixels[1] == 0x4442 &&
                    converted.pixels[2] == 0x6653 && converted.pixels[5] == 0x8884 &&
                    converted.pixels[6] == 0xaa95 && converted.covered[3] == 0,
                "RGB555 -> RGB565 conversion mismatch");

        // A gap wider than the row and a row that starts past the width are
        // clipped exactly like 0x40bbfd's clip test; nothing is invented.
        const byzantine::AnimsFrame wide{4, 1, 0, 0, 7};
        const auto clipped = byzantine::decode_anims_frame(
            wide, {0x4000, 0xC003, 0x0001, 0x9999, 0x0001, 0x8888, 0x4000});
        require(clipped.covered[0] == 0 && clipped.skipped_pixels == 3,
                "transparent gap was not clipped");

        // Rejections: a negative run, a row count that does not match the
        // height, a word count that does not match the header, an empty colour
        // run and a truncated literal run.
        const auto padded = [](std::vector<std::uint16_t> head) {
            head.resize(13, 0xC000);  // transparent fill keeps the header honest
            return head;
        };
        for (const auto& bad : std::vector<std::vector<std::uint16_t>>{
                 padded({0x4000, 0x2001}),  // negative run length
                 padded({0x4000}),          // one row marker for a two-row frame
                 padded({0x4000, 0x0014}),  // literal run past the end
                 [&] {
                     auto words = padded({0x4000});
                     words[12] = 0x8001;    // colour run with no colour word
                     return words;
                 }(),
             }) {
            bool rejected = false;
            try {
                (void)byzantine::decode_anims_frame(frame, bad);
            } catch (const std::runtime_error&) {
                rejected = true;
            }
            require(rejected, "accepted a malformed animation run stream");
        }
        bool rejected_size = false;
        try {
            (void)byzantine::decode_anims_frame(frame, std::vector<std::uint16_t>{0x4000});
        } catch (const std::runtime_error&) {
            rejected_size = true;
        }
        require(rejected_size, "accepted a frame whose word count differs from its header");

        // anims_frame_words re-walks the container for one frame.
        const auto first_words = byzantine::anims_frame_words(bytes, 0);
        require(first_words.size() == 3 && first_words[0] == 0 && first_words[2] == 2,
                "anims_frame_words first frame mismatch");
        const auto second_words = byzantine::anims_frame_words(bytes, 1);
        require(second_words.size() == 3 && second_words[0] == 0,
                "anims_frame_words second frame mismatch");
        bool rejected_index = false;
        try {
            (void)byzantine::anims_frame_words(bytes, 3);
        } catch (const std::runtime_error&) {
            rejected_index = true;
        }
        require(rejected_index, "accepted an animation frame index outside the file");

        // AnimsLibrary: 0x40fe70 builds "anims\<name[0]><name[1]>\<name>.rle".
        // The shipped CD names are upper case while the CA table is mixed case,
        // so the lookup must be case insensitive.
        const auto root = std::filesystem::temp_directory_path() / "byz-anims-test";
        std::filesystem::remove_all(root);
        std::filesystem::create_directories(root / "anims" / "vb");
        {
            std::ofstream out(root / "anims" / "vb" / "VBMEDTS.RLE", std::ios::binary);
            const auto data = make_asset(1234, {{640, 400, 5, 6}}, 2);
            out.write(reinterpret_cast<const char*>(data.data()),
                      static_cast<std::streamsize>(data.size()));
        }
        const byzantine::AnimsLibrary library(root.string());
        require(library.size() == 1, "anims library index size mismatch");
        const auto loaded = library.load("vbmedts");
        require(loaded && loaded->signature == 1234 && loaded->frames.size() == 1 &&
                    loaded->frames[0].x_offset == 5 && loaded->frames[0].y_offset == 6,
                "anims library case-insensitive load mismatch");
        require(library.load("VBMEdts").has_value(), "mixed-case asset lookup failed");
        require(!library.load("nope").has_value(), "invented a missing animation asset");
        require(!library.load("v").has_value(), "accepted a one-character asset name");

        // 0x40fe70's AVI fallback (0x41cb13): the .rle is absent, so the suffix
        // becomes ".avi" and the frame count/dimensions come from the RIFF
        // header. Only the container is read; no frame is decoded.
        {
            std::vector<std::uint8_t> avi;
            const auto put_tag = [&avi](const char* text) {
                for (int i = 0; i < 4; ++i) avi.push_back(static_cast<std::uint8_t>(text[i]));
            };
            const auto put_u32 = [&avi](std::uint32_t value) { put32(avi, value); };
            put_tag("RIFF"); put_u32(0); put_tag("AVI ");
            const auto list_start = avi.size();
            put_tag("LIST"); put_u32(0); put_tag("hdrl");
            const auto strh = avi.size();
            put_tag("strh"); put_u32(0x38);  // 56-byte stream header
            put_tag("vids"); put_tag("DIB "); put_u32(0); put16(avi, 0); put16(avi, 0);
            put_u32(0);      // dwInitialFrames
            put_u32(1);      // dwScale
            put_u32(15);     // dwRate
            put_u32(0);      // dwStart
            put_u32(7);      // dwLength = frame count
            put_u32(0); put_u32(0); put_u32(0); put16(avi, 0); put16(avi, 0);
            avi.resize(strh + 8 + 0x38, 0);
            const auto strf = avi.size();
            put_tag("strf"); put_u32(40);
            put_u32(40); put_u32(200); put_u32(100); put16(avi, 1); put16(avi, 16);
            put_tag("DIB "); put_u32(0); put_u32(0); put_u32(0); put_u32(0); put_u32(0);
            avi.resize(strf + 8 + 40, 0);
            const auto list_size = static_cast<std::uint32_t>(avi.size() - list_start - 8);
            avi[list_start + 4] = static_cast<std::uint8_t>(list_size & 0xff);
            avi[list_start + 5] = static_cast<std::uint8_t>((list_size >> 8) & 0xff);
            avi[list_start + 6] = static_cast<std::uint8_t>((list_size >> 16) & 0xff);
            avi[list_start + 7] = static_cast<std::uint8_t>((list_size >> 24) & 0xff);
            std::filesystem::create_directories(root / "anims" / "vb");
            std::ofstream out(root / "anims" / "vb" / "VBAVI.AVI", std::ios::binary);
            out.write(reinterpret_cast<const char*>(avi.data()),
                      static_cast<std::streamsize>(avi.size()));
        }
        const byzantine::AnimsLibrary avi_library(root.string());
        const auto avi_asset = avi_library.load("vbavi");
        require(avi_asset && avi_asset->frames.size() == 7 &&
                    avi_asset->frames[0].width == 200 &&
                    avi_asset->frames[0].height == 100 &&
                    avi_asset->frames[0].x_offset == 0 &&
                    avi_asset->frames[0].y_offset == 0,
                "AVI animation fallback mismatch");
        require(!byzantine::parse_avi_anims(std::vector<std::uint8_t>{'R', 'I', 'F', 'F'})
                     .has_value(),
                "accepted a truncated AVI container");
        const byzantine::AnimsLibrary missing((root / "absent").string());
        require(missing.size() == 0 && !missing.load("vbmedts").has_value(),
                "absent anims tree was not empty");
        std::filesystem::remove_all(root);

        // Optional real-data pass: every extracted `.RLE` must parse exactly.
        // The shipped tree holds 922 files; 40 other files under anims/ are
        // RIFF AVIs and are skipped by the extension filter.
        if (argc > 1) {
            std::size_t parsed = 0;
            std::size_t decoded_frames = 0;
            std::size_t blank_frames = 0;
            for (const auto& entry :
                 std::filesystem::recursive_directory_iterator(argv[1])) {
                if (!entry.is_regular_file()) continue;
                auto name = entry.path().filename().string();
                for (char& character : name) {
                    if (character >= 'a' && character <= 'z') {
                        character = static_cast<char>(character - 'a' + 'A');
                    }
                }
                if (name.size() < 4 || name.substr(name.size() - 4) != ".RLE") continue;
                std::ifstream input(entry.path(), std::ios::binary);
                const std::vector<std::uint8_t> data{std::istreambuf_iterator<char>(input),
                                                     std::istreambuf_iterator<char>()};
                try {
                    const auto asset = byzantine::parse_anims(data);
                    // Decode every frame and require the recovered invariants:
                    // an exact word stream, one row marker per row, and no row
                    // wider than the header's width.
                    for (std::size_t index = 0; index < asset.frames.size(); ++index) {
                        const auto decoded = byzantine::decode_anims_frame(
                            asset.frames[index],
                            byzantine::anims_frame_words(data, index));
                        // A blank frame is legal (BBBM00A frame 20 is 248 empty
                        // rows); only the geometry must match the header.
                        if (decoded.covered.size() !=
                            static_cast<std::size_t>(decoded.width) *
                                static_cast<std::size_t>(decoded.height)) {
                            throw std::runtime_error("decoded frame size mismatch");
                        }
                        ++decoded_frames;
                        if (decoded.run_pixels == 0) ++blank_frames;
                    }
                } catch (const std::exception& error) {
                    std::cerr << entry.path().string() << ": " << error.what() << '\n';
                    return 1;
                }
                ++parsed;
            }
            require(parsed != 0, "no .RLE files under the supplied anims tree");
            std::cout << "parsed " << parsed << " real animation assets, decoded "
                      << decoded_frames << " frames (" << blank_frames
                      << " blank)\n";
            // Optional visual check: `anims-test <tree> --render <name> <frame>`
            // prints the decoded frame as coarse luminance cells so a sprite can
            // be recognised without a window.
            if (argc > 4 && std::string(argv[2]) == "--render") {
                const byzantine::AnimsLibrary library(argv[1]);
                const auto frame = library.decode(argv[3], std::stoul(argv[4]));
                if (!frame) throw std::runtime_error("cannot decode the requested frame");
                std::size_t covered = 0;
                for (const auto value : frame->covered) covered += value != 0;
                std::cout << "render " << argv[3] << " frame " << argv[4] << ": "
                          << frame->width << "x" << frame->height << " covered="
                          << covered << "/" << frame->pixels.size() << " runs="
                          << frame->run_pixels << " skipped=" << frame->skipped_pixels
                          << '\n';
                const std::string ramp = " .:-=+*#%@";
                const std::size_t step_y = std::max<std::size_t>(
                    1, static_cast<std::size_t>(frame->height) / 30);
                const std::size_t step_x = std::max<std::size_t>(
                    1, static_cast<std::size_t>(frame->width) / 70);
                for (std::size_t y = 0; y < static_cast<std::size_t>(frame->height);
                     y += step_y) {
                    std::string line;
                    for (std::size_t x = 0; x < static_cast<std::size_t>(frame->width);
                         x += step_x) {
                        std::uint32_t sum = 0;
                        std::size_t count = 0;
                        for (std::size_t dy = 0; dy < step_y; ++dy) {
                            for (std::size_t dx = 0; dx < step_x; ++dx) {
                                const auto yy = std::min<std::size_t>(
                                    y + dy, static_cast<std::size_t>(frame->height) - 1);
                                const auto xx = std::min<std::size_t>(
                                    x + dx, static_cast<std::size_t>(frame->width) - 1);
                                const auto index = yy * static_cast<std::size_t>(frame->width) + xx;
                                if (frame->covered[index] == 0) continue;
                                const auto pixel = frame->pixels[index];
                                sum += 2 * ((pixel >> 11) & 0x1f) + ((pixel >> 5) & 0x3f) +
                                       2 * (pixel & 0x1f);
                                ++count;
                            }
                        }
                        line += count == 0 ? ' ' : ramp[std::min<std::size_t>(
                            9, (sum / count) / 20)];
                    }
                    std::cout << line << '\n';
                }
            }
        }
        std::cout << "animation asset tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
