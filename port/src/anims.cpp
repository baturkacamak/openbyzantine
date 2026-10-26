#include "byzantine/anims.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace byzantine {
namespace {

constexpr std::size_t kFileHeaderBytes = 10;
constexpr std::size_t kFrameHeaderBytes = 0x1a;

std::uint16_t read_u16(const std::vector<std::uint8_t>& data, std::size_t at) {
    return static_cast<std::uint16_t>(data.at(at) | (data.at(at + 1) << 8));
}

std::int16_t read_i16(const std::vector<std::uint8_t>& data, std::size_t at) {
    return static_cast<std::int16_t>(read_u16(data, at));
}

std::uint32_t read_u32(const std::vector<std::uint8_t>& data, std::size_t at) {
    return static_cast<std::uint32_t>(data.at(at)) |
           (static_cast<std::uint32_t>(data.at(at + 1)) << 8) |
           (static_cast<std::uint32_t>(data.at(at + 2)) << 16) |
           (static_cast<std::uint32_t>(data.at(at + 3)) << 24);
}

std::string lowercase(std::string text) {
    for (char& character : text) {
        if (character >= 'A' && character <= 'Z') {
            character = static_cast<char>(character - 'A' + 'a');
        }
    }
    return text;
}

}  // namespace

AnimsAsset parse_anims(const std::vector<std::uint8_t>& data) {
    if (data.size() < kFileHeaderBytes) {
        throw std::runtime_error("truncated animation file header");
    }
    AnimsAsset asset;
    asset.signature = read_u16(data, 0);
    // +0x02 is a u32 and +0x06 a u16 that 0x40cb26 skips; both are zero in the
    // shipped files and their meaning is not recovered.
    const auto frame_count = read_u16(data, 8);
    asset.frames.reserve(frame_count);
    std::size_t at = kFileHeaderBytes;
    for (std::size_t index = 0; index < frame_count; ++index) {
        if (data.size() - at < kFrameHeaderBytes) {
            throw std::runtime_error("truncated animation frame header");
        }
        AnimsFrame frame;
        frame.width = read_i16(data, at + 0x02);
        frame.height = read_i16(data, at + 0x04);
        frame.x_offset = read_i16(data, at + 0x06);
        frame.y_offset = read_i16(data, at + 0x08);
        frame.words = read_u32(data, at + 0x16);
        at += kFrameHeaderBytes;
        const auto bytes = static_cast<std::size_t>(frame.words) * 2;
        if (data.size() - at < bytes) {
            throw std::runtime_error("truncated animation frame data");
        }
        at += bytes;
        asset.frames.push_back(frame);
    }
    if (at != data.size()) {
        throw std::runtime_error("animation file has trailing bytes");
    }
    return asset;
}

std::vector<std::uint16_t> anims_frame_words(const std::vector<std::uint8_t>& data,
                                             std::size_t frame_index) {
    if (data.size() < kFileHeaderBytes) {
        throw std::runtime_error("truncated animation file header");
    }
    const auto frame_count = read_u16(data, 8);
    if (frame_index >= frame_count) {
        throw std::runtime_error("animation frame index outside the file");
    }
    std::size_t at = kFileHeaderBytes;
    for (std::size_t index = 0; index < frame_count; ++index) {
        if (data.size() - at < kFrameHeaderBytes) {
            throw std::runtime_error("truncated animation frame header");
        }
        const auto words = read_u32(data, at + 0x16);
        at += kFrameHeaderBytes;
        const auto bytes = static_cast<std::size_t>(words) * 2;
        if (data.size() - at < bytes) {
            throw std::runtime_error("truncated animation frame data");
        }
        if (index == frame_index) {
            std::vector<std::uint16_t> packed;
            packed.reserve(words);
            for (std::size_t word = 0; word < words; ++word) {
                packed.push_back(read_u16(data, at + word * 2));
            }
            return packed;
        }
        at += bytes;
    }
    throw std::runtime_error("animation frame index outside the file");
}

DecodedFrame decode_anims_frame(const AnimsFrame& frame,
                                const std::vector<std::uint16_t>& words,
                                bool rgb565) {
    if (frame.width < 0 || frame.height < 0) {
        throw std::runtime_error("animation frame has a negative size");
    }
    if (frame.words != words.size()) {
        throw std::runtime_error("animation frame word count does not match its header");
    }
    DecodedFrame out;
    out.width = frame.width;
    out.height = frame.height;
    const auto width = static_cast<std::size_t>(frame.width);
    const auto height = static_cast<std::size_t>(frame.height);
    if (width != 0 && height > (0xffffffffu / width)) {
        throw std::runtime_error("animation frame size overflows");
    }
    out.pixels.assign(width * height, 0);
    out.covered.assign(width * height, 0);

    std::size_t source = 0;
    std::int32_t row = -1;   // 0x40bbb0 starts at -1; the first command 1 opens row 0
    std::int32_t column = 0;
    // 0x4164a0: RGB555 -> RGB565. Applied to pixel words only, never to a run
    // header (0x40be80 walks the same stream and skips the header positions).
    const auto convert = [rgb565](std::uint16_t pixel) {
        return rgb565 ? static_cast<std::uint16_t>((pixel & 0x1f) |
                                                   ((pixel & 0xffe0) << 1))
                      : pixel;
    };
    const auto put = [&](std::uint16_t pixel) {
        if (row >= 0 && static_cast<std::size_t>(row) < height && column >= 0 &&
            static_cast<std::size_t>(column) < width) {
            const auto index = static_cast<std::size_t>(row) * width +
                               static_cast<std::size_t>(column);
            out.pixels[index] = convert(pixel);
            out.covered[index] = 1;
            ++out.run_pixels;
        }
        ++column;
    };
    while (source < words.size()) {
        const std::uint16_t word = words[source++];
        const auto command = static_cast<std::uint8_t>(word >> 14);
        // The count is read back as a signed 16-bit value by 0x40bbfd, so a
        // value with bit 13 set is negative there; no shipped frame has one.
        const auto raw = static_cast<std::int32_t>(word & 0x3fff);
        const std::int32_t count = raw >= 0x2000 ? raw - 0x4000 : raw;
        if (count < 0) {
            throw std::runtime_error("animation run has a negative length");
        }
        switch (command) {
        case 0: {
            // Literal run: `count` pixel words follow.
            if (static_cast<std::size_t>(count) > words.size() - source) {
                throw std::runtime_error("animation literal run overruns the frame");
            }
            for (std::int32_t index = 0; index < count; ++index) {
                put(words[source++]);
            }
            break;
        }
        case 1:
            // Row marker: the next row starts at x = count.
            ++row;
            column = count;
            break;
        case 2: {
            // One colour word repeated `count` times.
            if (source >= words.size()) {
                throw std::runtime_error("animation colour run has no colour word");
            }
            const std::uint16_t pixel = words[source++];
            for (std::int32_t index = 0; index < count; ++index) put(pixel);
            break;
        }
        case 3:
            // Transparent gap: the destination keeps whatever it held.
            column += count;
            out.skipped_pixels += static_cast<std::size_t>(count);
            break;
        }
    }
    // A frame opens each row with a command-1 marker and carries one closing
    // marker after the last row, so `height + 1` markers leave row == height.
    // The original's loop stops on the same marker (row < height fails).
    if (row != static_cast<std::int32_t>(height)) {
        throw std::runtime_error("animation frame rows do not match its height");
    }
    return out;
}

std::optional<AnimsAsset> parse_avi_anims(const std::vector<std::uint8_t>& data) {
    const auto u32 = [&data](std::size_t at) {
        return static_cast<std::uint32_t>(data.at(at)) |
               (static_cast<std::uint32_t>(data.at(at + 1)) << 8) |
               (static_cast<std::uint32_t>(data.at(at + 2)) << 16) |
               (static_cast<std::uint32_t>(data.at(at + 3)) << 24);
    };
    const auto tag = [&data](std::size_t at, const char* expected) {
        for (int index = 0; index < 4; ++index) {
            if (data.at(at + static_cast<std::size_t>(index)) !=
                static_cast<std::uint8_t>(expected[index])) {
                return false;
            }
        }
        return true;
    };
    if (data.size() < 12 || !tag(0, "RIFF") || !tag(8, "AVI ")) return std::nullopt;

    bool have_stream = false;
    std::uint32_t frame_count = 0;
    std::int16_t width = 0;
    std::int16_t height = 0;
    // Depth-first walk over the chunk list; the first `vids` stream wins, which
    // is what the shipped anims AVIs carry (one video stream each).
    const auto walk = [&](auto&& self, std::size_t start, std::size_t end) -> void {
        std::size_t at = start;
        while (at + 8 <= end) {
            const std::uint32_t size = u32(at + 4);
            const std::size_t body = at + 8;
            if (size > end - body) return;  // truncated chunk: stop, keep what we have
            if (tag(at, "LIST") && size >= 4) {
                self(self, body + 4, body + size);
            } else if (tag(at, "strh") && size >= 0x24 && !have_stream &&
                       body + 4 <= data.size() && tag(body, "vids")) {
                have_stream = true;
                frame_count = u32(body + 0x20);  // dwLength
            } else if (tag(at, "strf") && size >= 12 && width == 0) {
                width = static_cast<std::int16_t>(
                    static_cast<std::uint16_t>(u32(body + 4) & 0xffff));
                height = static_cast<std::int16_t>(
                    static_cast<std::uint16_t>(u32(body + 8) & 0xffff));
            }
            at = body + size + (size & 1);
        }
    };
    walk(walk, 12, data.size());
    if (!have_stream || width == 0 || height == 0) return std::nullopt;
    if (frame_count == 0 || frame_count > 0xffff) return std::nullopt;
    AnimsAsset asset;
    asset.frames.assign(frame_count, AnimsFrame{width, height, 0, 0, 0});
    return asset;
}

AnimsLibrary::AnimsLibrary(const std::string& root) : root_(root) {
    namespace fs = std::filesystem;
    std::error_code error;
    // Accept either the directory that CONTAINS `anims/` (the extraction root)
    // or the `anims` directory itself; the index key stays `<two>/<name>.rle`.
    auto base = fs::path(root_);
    if (fs::is_directory(base / "anims", error)) {
        base /= "anims";
    } else if (!fs::is_directory(base, error)) {
        return;
    }
    for (fs::recursive_directory_iterator it(base, error), end; it != end;
         it.increment(error)) {
        if (error) break;
        if (!it->is_regular_file(error)) continue;
        const auto relative = fs::relative(it->path(), base, error).generic_string();
        if (error) continue;
        entries_.emplace(lowercase(relative), it->path().string());
    }
}

std::optional<DecodedFrame> AnimsLibrary::decode(const std::string& anim_name,
                                                 std::size_t frame_index) const {
    if (anim_name.size() < 2) return std::nullopt;
    const auto stem = std::string(1, anim_name[0]) + anim_name[1] + "/" + anim_name;
    const auto rle = entries_.find(lowercase(stem + ".rle"));
    if (rle == entries_.end()) return std::nullopt;  // AVI frames are not decoded
    std::ifstream input(rle->second, std::ios::binary);
    if (!input) return std::nullopt;
    const std::vector<std::uint8_t> data{std::istreambuf_iterator<char>(input),
                                         std::istreambuf_iterator<char>()};
    const auto asset = parse_anims(data);
    if (frame_index >= asset.frames.size()) return std::nullopt;
    return decode_anims_frame(asset.frames[frame_index],
                              anims_frame_words(data, frame_index));
}

std::optional<AnimsAsset> AnimsLibrary::load(const std::string& anim_name) const {
    if (anim_name.size() < 2) return std::nullopt;
    const auto read = [](const std::string& path) {
        std::ifstream input(path, std::ios::binary);
        if (!input) return std::vector<std::uint8_t>{};
        return std::vector<std::uint8_t>{std::istreambuf_iterator<char>(input),
                                         std::istreambuf_iterator<char>()};
    };
    // 0x40fe70: sprintf("%c%c\\%s", name[0], name[1], name) then 0x40c624 with
    // the "anims" directory and the ".rle" suffix; on failure the suffix is
    // rewritten to ".avi" and 0x41cb13 loads the AVI instead.
    const auto stem = std::string(1, anim_name[0]) + anim_name[1] + "/" + anim_name;
    const auto rle = entries_.find(lowercase(stem + ".rle"));
    if (rle != entries_.end()) {
        // A present-but-unparsable .RLE is a format error, not a fallback.
        return parse_anims(read(rle->second));
    }
    const auto avi = entries_.find(lowercase(stem + ".avi"));
    if (avi != entries_.end()) return parse_avi_anims(read(avi->second));
    return std::nullopt;
}

}  // namespace byzantine
