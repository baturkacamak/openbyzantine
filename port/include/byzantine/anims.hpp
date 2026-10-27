#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace byzantine {

// One frame header of an `anims/<first two chars>/<name>.RLE` asset, as read by
// 0x410078. The 26-byte header feeds four values: 0x410078 copies width to
// record +0x0c and height to +0x10 through the frame node, adds the x/y pair to
// the record's position on the FIRST frame only, and keeps the word count for
// the `malloc(words*2)` + `fread(buf, 2, words)` allocation.
struct AnimsFrame {
    std::int16_t width = 0;      // header +0x02 -> node +0x0c
    std::int16_t height = 0;     // header +0x04 -> node +0x10
    std::int16_t x_offset = 0;   // header +0x06, record->x += this on frame 0
    std::int16_t y_offset = 0;   // header +0x08, record->y += this on frame 0
    std::uint32_t words = 0;     // header +0x16, packed 16-bit words that follow
};

// One animation asset. 0x40cb26 reads the 10-byte file header:
//   u16 signature     -> copied to record +0x28 (0x0000 and 0x0001 both occur)
//   u32 unused        -> skipped
//   u16 unused        -> skipped
//   u16 frame count   -> 0x410078's loop bound and 0x40ffee's +0x14 value
// Each frame is the 26-byte header above followed by `words*2` bytes.
//
// The pixel encoding is NOT decoded here. Rendering is out of scope, and the
// exact meaning of the packed words (0x40be80 walks them with a two-bit
// selector) is documented as unknown in `analysis/rea/ANIMATION_ASSET.md`.
struct AnimsAsset {
    std::uint16_t signature = 0;
    std::vector<AnimsFrame> frames;
};

// Parses one `.RLE` animation asset. Every frame must fit and the file must be
// consumed exactly; a truncated or padded input is an error, never a guess.
AnimsAsset parse_anims(const std::vector<std::uint8_t>& data);

// One decoded frame. 0x40bbfd writes only the pixels a run covers and leaves
// the destination untouched for a transparent (command 3) gap, so every pixel
// carries a coverage flag instead of a guessed colour.
struct DecodedFrame {
    std::int16_t width = 0;
    std::int16_t height = 0;
    std::vector<std::uint16_t> pixels;  // row-major, width*height, RGB16
    std::vector<std::uint8_t> covered;  // 1 = a run wrote this pixel
    std::size_t run_pixels = 0;         // pixels written by command 0 or 2
    std::size_t skipped_pixels = 0;     // pixels stepped over by command 3
};

// The packed 16-bit words of one frame, re-walked from the container bytes.
// Throws on a truncated/absent frame.
std::vector<std::uint16_t> anims_frame_words(const std::vector<std::uint8_t>& data,
                                             std::size_t frame_index);

// 0x40bbfd: decodes one frame. Each row starts with a command-1 word
// (0x4000 | first_x), then runs:
//   command 0: `n` literal pixel words follow
//   command 2: one colour word, repeated `n` times
//   command 3: `n` transparent pixels (nothing is written)
// `n` is the low 14 bits; a negative n cannot occur in the shipped data and is
// rejected. Writes outside 0..width-1 are clipped exactly like the original's
// clip test.
//
// `rgb565` applies 0x4164a0 to every literal/colour word, which is what
// 0x410078's load-time 0x40be80 pass does: `(p & 0x1f) | ((p & 0xffe0) << 1)`,
// i.e. RGB555 -> RGB565. The original skips it only when DAT_00485c78 == 10
// (an unrecovered mode), so the default converts.
DecodedFrame decode_anims_frame(const AnimsFrame& frame,
                                const std::vector<std::uint16_t>& words,
                                bool rgb565 = true);

// 0x40fe70's fallback: when 0x410078 fails, it rewrites the ".rle" suffix to
// ".avi" and 0x41cb13 builds an AVIFile object instead. Every frame node then
// receives the video stream's width and height and the record's position is
// NOT offset. Only the container header is read here (frame count, dimensions);
// no frame is decoded. Returns nullopt when the file is not a RIFF AVI with a
// video stream.
std::optional<AnimsAsset> parse_avi_anims(const std::vector<std::uint8_t>& data);

// Case-insensitive lookup over an extracted `anims` tree. 0x40fe70 builds
// "anims\<name[0]><name[1]>\<name>.rle" from the CA record, and the shipped CD
// names are upper case ("anims/vb/VBMEDTS.RLE") while the CA table and the
// scripts use mixed case, so a case-sensitive filesystem needs this index.
class AnimsLibrary {
public:
    // Walks `<root>/anims/<two chars>/<file>`; a missing tree yields an empty
    // library rather than an error, so a caller can supply a partial CD set.
    explicit AnimsLibrary(const std::string& root, const std::string& preferred_root = "");

    // Returns the parsed asset for a CA record name, or nullopt when the file
    // is absent. 0x40fe70 treats that absence as a failed record creation.
    std::optional<AnimsAsset> load(const std::string& anim_name) const;

    // Loads and decodes one frame of a `.RLE` asset. nullopt for an absent
    // asset, an out-of-range frame or an AVI fallback (frames are not decoded).
    std::optional<DecodedFrame> decode(const std::string& anim_name,
                                       std::size_t frame_index) const;

    std::size_t size() const { return entries_.size(); }

private:
    std::string root_;
    std::map<std::string, std::string> entries_;  // lowercase relative -> path
    // Immutable extracted assets: avoid reopening the file for every node of
    // a delta reconstruction on each rendered frame. No per-frame image cache.
    mutable std::map<std::string, std::vector<std::uint8_t>> packed_cache_;
};

}  // namespace byzantine
