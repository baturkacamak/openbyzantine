#include "byzantine/jpeg.hpp"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

void put16(std::vector<std::uint8_t>& out, int value) {
    out.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
    out.push_back(static_cast<std::uint8_t>(value & 0xff));
}

// Marker with a length-prefixed payload (the length includes its own 2 bytes).
void put_segment(std::vector<std::uint8_t>& out, std::uint8_t marker,
                 const std::vector<std::uint8_t>& payload) {
    out.push_back(0xff);
    out.push_back(marker);
    put16(out, static_cast<int>(payload.size()) + 2);
    out.insert(out.end(), payload.begin(), payload.end());
}

// MSB-first bit writer for entropy-coded data (jdchuff.c order).
class BitWriter {
public:
    void put(int count, std::uint32_t value) {
        for (int index = count - 1; index >= 0; --index) {
            current_ = static_cast<std::uint8_t>(
                (current_ << 1) | ((value >> index) & 1u));
            if (++bits_ == 8) flush_byte();
        }
    }
    // The last byte is padded with 1 bits, as jdhuff/emit_bits does.
    std::vector<std::uint8_t> finish() {
        while (bits_ != 0) put(1, 1);
        return std::move(bytes_);
    }

private:
    void flush_byte() {
        bytes_.push_back(current_);
        if (current_ == 0xff) bytes_.push_back(0x00);  // byte stuffing
        current_ = 0;
        bits_ = 0;
    }
    std::vector<std::uint8_t> bytes_;
    std::uint8_t current_ = 0;
    int bits_ = 0;
};

struct JpegSpec {
    bool progressive = false;
    int width = 16;
    int height = 16;
    // Per block DC coefficient (already quantised). Block order is MCU order.
    std::vector<int> block_dc;
};

// One-component, one-quant-table JPEG whose blocks carry a DC value only. The
// quantisation table is all 8, so jidctint maps a quantised DC of `c` to the
// sample value c + 128.
std::vector<std::uint8_t> build_jpeg(const JpegSpec& spec) {
    std::vector<std::uint8_t> out{0xff, 0xd8};  // SOI

    std::vector<std::uint8_t> dqt{0x00};
    dqt.insert(dqt.end(), 64, 8);
    put_segment(out, 0xdb, dqt);

    std::vector<std::uint8_t> sof;
    sof.push_back(8);
    put16(sof, spec.height);
    put16(sof, spec.width);
    sof.push_back(1);           // one component
    sof.push_back(1);           // id
    sof.push_back(0x11);        // 1x1 sampling
    sof.push_back(0);           // quant table 0
    put_segment(out, spec.progressive ? 0xc2 : 0xc0, sof);

    // DC table 0: two 2-bit codes -> symbol 0 (category 0) and symbol 4.
    // AC table 0: one 2-bit code -> symbol 0x00 (end of block).
    std::vector<std::uint8_t> dht{0x00};  // (tc=0, th=0)
    dht.push_back(0);
    dht.push_back(2);  // count[2] = 2
    for (int index = 2; index < 16; ++index) dht.push_back(0);
    dht.push_back(0);  // symbol 0 -> category 0
    dht.push_back(4);  // symbol 4 -> category 4
    dht.push_back(0x10);  // AC: (tc=1, th=0)
    dht.push_back(0);
    dht.push_back(1);  // count[2] = 1
    for (int index = 2; index < 16; ++index) dht.push_back(0);
    dht.push_back(0x00);  // symbol 0 -> end of block
    put_segment(out, 0xc4, dht);

    const auto put_dc = [](BitWriter& writer, int& previous, int value) {
        const int difference = value - previous;
        previous = value;
        if (difference == 0) {
            writer.put(2, 0);  // symbol 0, no magnitude bits
            return;
        }
        // Category 4 covers -15..-8 and 8..15; the fixture stays inside it.
        if (difference < -15 || difference > 15 || (difference > -8 && difference < 8)) {
            throw std::runtime_error("fixture DC difference outside category 4");
        }
        writer.put(2, 1);  // symbol 4
        const std::uint32_t bits =
            difference > 0 ? static_cast<std::uint32_t>(difference)
                           : static_cast<std::uint32_t>(difference + 15);
        writer.put(4, bits);
    };

    const auto add_scan = [&](int ss, int se, const std::vector<std::uint8_t>& data) {
        std::vector<std::uint8_t> sos;
        sos.push_back(1);     // components in scan
        sos.push_back(1);     // component id
        sos.push_back(0x00);  // DC table 0, AC table 0
        sos.push_back(static_cast<std::uint8_t>(ss));
        sos.push_back(static_cast<std::uint8_t>(se));
        sos.push_back(0);  // Ah = 0, Al = 0
        put_segment(out, 0xda, sos);
        out.insert(out.end(), data.begin(), data.end());
    };

    if (!spec.progressive) {
        BitWriter writer;
        int previous = 0;
        for (const int dc : spec.block_dc) {
            put_dc(writer, previous, dc);
            writer.put(2, 0);  // AC end of block
        }
        add_scan(0, 63, writer.finish());
    } else {
        BitWriter dc_scan;
        int previous = 0;
        for (const int dc : spec.block_dc) put_dc(dc_scan, previous, dc);
        add_scan(0, 0, dc_scan.finish());
        BitWriter ac_scan;
        for (std::size_t index = 0; index < spec.block_dc.size(); ++index) {
            ac_scan.put(2, 0);  // every block is an empty AC band
        }
        add_scan(1, 63, ac_scan.finish());
    }

    out.push_back(0xff);
    out.push_back(0xd9);  // EOI
    return out;
}

std::uint64_t fnv1a(const std::vector<std::uint8_t>& data) {
    std::uint64_t hash = 1469598103934665603ull;
    for (const auto byte : data) {
        hash ^= byte;
        hash *= 1099511628211ull;
    }
    return hash;
}

std::vector<std::uint8_t> fnv_bytes(const std::vector<std::uint16_t>& words) {
    std::vector<std::uint8_t> bytes;
    bytes.reserve(words.size() * 2);
    for (const auto word : words) {
        bytes.push_back(static_cast<std::uint8_t>(word & 0xff));
        bytes.push_back(static_cast<std::uint8_t>(word >> 8));
    }
    return bytes;
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open fixture: " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void check_requires(const std::vector<std::uint8_t>& data, const std::string& what) {
    bool rejected = false;
    try {
        (void)byzantine::decode_jpeg(data);
    } catch (const std::runtime_error&) {
        rejected = true;
    }
    require(rejected, "decode must reject " + what);
}
}  // namespace

int main(int argc, char** argv) {
    try {
        // Corpus mode: decode every file named after --scan and print one line
        // per image (used for the shipped-JPEG census in JPEG_DECODE.md).
        if (argc >= 3 && std::string(argv[1]) == "--scan") {
            std::size_t ok = 0;
            for (int index = 2; index < argc; ++index) {
                const auto image = byzantine::decode_jpeg_file(argv[index]);
                const auto header = byzantine::read_jpeg_header(read_file(argv[index]));
                std::printf("ok %dx%d comps=%d prog=%d samp=%dx%d %dx%d %dx%d "
                            "hash=%016llx %s\n",
                            header.width, header.height, header.components,
                            header.progressive ? 1 : 0, header.h_sampling[0],
                            header.v_sampling[0], header.h_sampling[1],
                            header.v_sampling[1], header.h_sampling[2],
                            header.v_sampling[2],
                            static_cast<unsigned long long>(fnv1a(image.rgb)),
                            argv[index]);
                ++ok;
            }
            std::printf("jpeg scan: %zu files decoded\n", ok);
            return 0;
        }
        // 0x40f080 (display mode 6) is an exact bit-field split.
        byzantine::JpegImage colours;
        colours.width = 4;
        colours.height = 1;
        colours.rgb = {255, 0,   0,   0,   255, 0,   0,   0,   255, 128, 128, 128};
        const auto pixels = byzantine::rgb_to_rgb565(colours);
        require(pixels.size() == 4, "RGB565 size mismatch");
        require(pixels[0] == 0xf800, "pure red must map to 0xf800");
        require(pixels[1] == 0x07e0, "pure green must map to 0x07e0");
        require(pixels[2] == 0x001f, "pure blue must map to 0x001f");
        require(pixels[3] == 0x8410, "mid grey must map to 0x8410");

        // Four 8x8 blocks: DC 0, 10, 20, 5 with an all-8 quantisation table.
        JpegSpec spec;
        spec.block_dc = {0, 10, 20, 5};
        const auto baseline = build_jpeg(spec);
        const auto header = byzantine::read_jpeg_header(baseline);
        require(header.width == 16 && header.height == 16, "synthetic header size");
        require(header.components == 1 && !header.progressive, "synthetic header kind");
        require(header.h_sampling[0] == 1 && header.v_sampling[0] == 1,
                "synthetic header sampling");
        const auto decoded = byzantine::decode_jpeg(baseline);
        require(decoded.width == 16 && decoded.height == 16, "synthetic decode size");
        require(decoded.rgb.size() == 16 * 16 * 3, "synthetic decode buffer");
        const auto sample = [&](int x, int y) {
            return static_cast<int>(decoded.rgb[static_cast<std::size_t>(y * 16 + x) * 3]);
        };
        require(sample(0, 0) == 128 && sample(7, 7) == 128, "block 0 DC");
        require(sample(8, 0) == 138 && sample(15, 7) == 138, "block 1 DC");
        require(sample(0, 8) == 148 && sample(7, 15) == 148, "block 2 DC");
        require(sample(8, 8) == 133 && sample(15, 15) == 133, "block 3 DC (negative diff)");
        for (const auto index : {0, 1, 2}) {
            require(decoded.rgb[static_cast<std::size_t>(3)] ==
                        decoded.rgb[static_cast<std::size_t>(index)],
                    "greyscale components must be equal");
        }

        // The same coefficients through process 2 (progressive): a DC scan and
        // an empty AC scan must reproduce the sequential result exactly.
        JpegSpec progressive = spec;
        progressive.progressive = true;
        const auto progressive_header = byzantine::read_jpeg_header(build_jpeg(progressive));
        require(progressive_header.progressive, "progressive SOF2 must be reported");
        const auto progressive_image = byzantine::decode_jpeg(build_jpeg(progressive));
        require(progressive_image.rgb == decoded.rgb,
                "progressive scan must match the sequential decode");

        // Strict rejections.
        check_requires({}, "an empty stream");
        check_requires({0xff, 0xd8}, "a stream without SOF");
        check_requires({0x00, 0x01, 0x02}, "a stream without SOI");
        auto truncated = baseline;
        truncated.resize(truncated.size() - 2);  // drop EOI
        check_requires(truncated, "a truncated stream");
        auto short_entropy = baseline;
        short_entropy.resize(short_entropy.size() - 6);
        check_requires(short_entropy, "a stream whose scan data is short");
        auto lossless = baseline;
        for (std::size_t at = 0; at + 1 < lossless.size(); ++at) {
            if (lossless[at] == 0xff && lossless[at + 1] == 0xc0) lossless[at + 1] = 0xc3;
        }
        check_requires(lossless, "an unsupported SOF (lossless)");
        auto arithmetic = baseline;
        for (std::size_t at = 0; at + 1 < arithmetic.size(); ++at) {
            if (arithmetic[at] == 0xff && arithmetic[at + 1] == 0xc0) arithmetic[at + 1] = 0xcc;
        }
        check_requires(arithmetic, "arithmetic coding (DAC)");
        auto deep = baseline;
        for (std::size_t at = 0; at + 1 < deep.size(); ++at) {
            if (deep[at] == 0xff && deep[at + 1] == 0xc0) deep[at + 4] = 12;  // precision
        }
        check_requires(deep, "12-bit sample precision");
        auto zero_sampling = baseline;
        for (std::size_t at = 0; at + 1 < zero_sampling.size(); ++at) {
            if (zero_sampling[at] == 0xff && zero_sampling[at + 1] == 0xc0) {
                zero_sampling[at + 11] = 0x00;  // component sampling factors 0x0
            }
        }
        check_requires(zero_sampling, "a zero sampling factor");

        // Shipped data: one `backs` image (640x400) and one panorama node
        // (2496x768). Both are baseline 4:2:0 SOF0; the hashes lock the port's
        // current RGB plane and the 0x40f080 RGB565 conversion.
        if (argc >= 3) {
            const auto back = read_file(argv[1]);
            const auto back_header = byzantine::read_jpeg_header(back);
            require(back_header.width == 640 && back_header.height == 400,
                    "backs fixture dimensions");
            require(back_header.components == 3 && !back_header.progressive,
                    "backs fixture kind");
            require(back_header.h_sampling[0] == 2 && back_header.v_sampling[0] == 2 &&
                        back_header.h_sampling[1] == 1 && back_header.v_sampling[1] == 1,
                    "backs fixture sampling");
            const auto back_image = byzantine::decode_jpeg(back);
            require(fnv1a(back_image.rgb) == 0x2e281d7511acda46ull,
                    "backs fixture RGB plane hash");
            require(fnv1a(fnv_bytes(byzantine::rgb_to_rgb565(back_image))) ==
                        0x526bd1a48e740f63ull,
                    "backs fixture RGB565 hash");

            const auto node = read_file(argv[2]);
            const auto node_header = byzantine::read_jpeg_header(node);
            require(node_header.width == 2496 && node_header.height == 768,
                    "panorama fixture dimensions");
            const auto node_image = byzantine::decode_jpeg(node);
            require(fnv1a(node_image.rgb) == 0x4b5f10afeee6c24eull,
                    "panorama fixture RGB plane hash");
            require(fnv1a(fnv_bytes(byzantine::rgb_to_rgb565(node_image))) ==
                        0xe6ae2e47246397baull,
                    "panorama fixture RGB565 hash");
        } else {
            std::cout << "jpeg-test: fixture arguments omitted\n";
        }

        std::cout << "jpeg decode tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
