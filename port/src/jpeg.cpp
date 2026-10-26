#include "byzantine/jpeg.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace byzantine {
namespace {

[[noreturn]] void fail(const std::string& message) {
    throw std::runtime_error("jpeg: " + message);
}

int ceil_div(int value, int divisor) { return (value + divisor - 1) / divisor; }

std::uint16_t read_be16(const std::vector<std::uint8_t>& data, std::size_t at) {
    if (at + 1 >= data.size()) fail("truncated 16-bit field");
    return static_cast<std::uint16_t>((data[at] << 8) | data[at + 1]);
}

// Zigzag position -> natural (row-major) coefficient index.
constexpr std::array<int, 64> kZigzag = {
    0,  1,  8,  16, 9,  2,  3,  10, 17, 24, 32, 25, 18, 11, 4,  5,
    12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6,  7,  14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63};

struct HuffmanTable {
    std::array<int, 17> count{};
    std::array<int, 17> mincode{};
    std::array<int, 17> maxcode{};
    std::array<int, 17> valptr{};
    std::vector<std::uint8_t> values;
    bool defined = false;
};

// Sequential Huffman-decoding byte reader with libjpeg jdhuff.c semantics:
// inside entropy-coded data 0xFF is always followed by a stuffed 0x00, so any
// other following byte is a marker and therefore ends the scan. One byte is
// loaded at a time, so the reader never consumes past a marker.
class BitReader {
public:
    BitReader(const std::uint8_t* data, const std::uint8_t* end)
        : data_(data), end_(end) {}

    int get_bit() {
        if (bit_count_ == 0) refill();
        --bit_count_;
        return static_cast<int>((buffer_ >> bit_count_) & 1u);
    }

    std::uint32_t get_bits(int count) {
        std::uint32_t value = 0;
        for (int index = 0; index < count; ++index) {
            value = (value << 1) | static_cast<std::uint32_t>(get_bit());
        }
        return value;
    }

    // JPEG "extend": n bits carry a signed magnitude.
    int receive(int count) {
        if (count == 0) return 0;
        const int value = static_cast<int>(get_bits(count));
        if (value < (1 << (count - 1))) return value - ((1 << count) - 1);
        return value;
    }

    const std::uint8_t* position() const { return data_; }

    // A DRI interval ends byte-aligned, then an RSTn marker follows.
    void restart(int expected) {
        bit_count_ = 0;
        while (data_ + 1 < end_ && data_[0] == 0xff && data_[1] == 0xff) {
            ++data_;  // fill bytes before the marker
        }
        if (data_ + 1 >= end_ || data_[0] != 0xff) fail("missing restart marker");
        const auto marker = data_[1];
        if (marker < 0xd0 || marker > 0xd7) {
            fail("expected a restart marker inside the scan");
        }
        if (marker - 0xd0 != expected) fail("restart marker out of sequence");
        data_ += 2;
    }

private:
    void refill() {
        if (data_ >= end_) fail("entropy-coded data ended before the scan completed");
        std::uint8_t byte = *data_++;
        if (byte == 0xff) {
            if (data_ >= end_) fail("truncated byte stuffing at the end of the data");
            if (*data_ != 0x00) fail("marker inside entropy-coded data");
            ++data_;
        }
        buffer_ = byte;
        bit_count_ = 8;
    }

    const std::uint8_t* data_;
    const std::uint8_t* end_;
    std::uint32_t buffer_ = 0;
    int bit_count_ = 0;
};

struct Component {
    int id = 0;
    int h = 1;
    int v = 1;
    int quant_index = 0;
    int sample_w = 0;  // ceil(width * h / max_h)
    int sample_h = 0;  // ceil(height * v / max_v)
    int blocks_w = 0;  // MCU-aligned block columns
    int blocks_h = 0;  // MCU-aligned block rows
    std::vector<std::int32_t> coefficients;

    std::int32_t* block(int row, int column) {
        return &coefficients[(static_cast<std::size_t>(row) *
                                  static_cast<std::size_t>(blocks_w) +
                              static_cast<std::size_t>(column)) *
                             64];
    }
};

struct Decoder {
    int width = 0;
    int height = 0;
    int max_h = 1;
    int max_v = 1;
    int mcus_x = 0;
    int mcus_y = 0;
    bool progressive = false;
    bool sof_seen = false;
    bool scan_seen = false;
    bool adobe_seen = false;
    int adobe_transform = -1;
    int restart_interval = 0;
    int component_count = 0;
    std::array<Component, 4> components;
    std::array<std::array<std::uint16_t, 64>, 4> quant{};
    std::array<bool, 4> quant_defined{};
    std::array<HuffmanTable, 4> dc_tables{};
    std::array<HuffmanTable, 4> ac_tables{};
    std::array<int, 4> dc_pred{};
};

struct ScanComponent {
    int index = 0;
    int dc_table = 0;
    int ac_table = 0;
};

int decode_huffman(const HuffmanTable& table, BitReader& reader) {
    if (!table.defined) fail("Huffman table was not defined");
    int code = 0;
    for (int length = 1; length <= 16; ++length) {
        code = (code << 1) | reader.get_bit();
        if (code <= table.maxcode[static_cast<std::size_t>(length)]) {
            const int index = table.valptr[static_cast<std::size_t>(length)] +
                              code - table.mincode[static_cast<std::size_t>(length)];
            if (index < 0 || index >= static_cast<int>(table.values.size())) {
                fail("Huffman code index outside the table");
            }
            return table.values[static_cast<std::size_t>(index)];
        }
    }
    fail("bad Huffman code");
}

void parse_dqt(Decoder& decoder, const std::uint8_t* segment, std::size_t size) {
    std::size_t at = 0;
    while (at < size) {
        const int precision = segment[at] >> 4;
        const int index = segment[at] & 15;
        ++at;
        if (precision > 1) fail("unknown quantisation table precision");
        if (index > 3) fail("quantisation table index outside 0..3");
        const std::size_t bytes = precision == 0 ? 64 : 128;
        if (at + bytes > size) fail("truncated quantisation table");
        for (int k = 0; k < 64; ++k) {
            const std::size_t offset = at + static_cast<std::size_t>(k) *
                                                (precision == 0 ? 1 : 2);
            const std::uint16_t value =
                precision == 0
                    ? segment[offset]
                    : static_cast<std::uint16_t>((segment[offset] << 8) | segment[offset + 1]);
            decoder.quant[static_cast<std::size_t>(index)]
                         [static_cast<std::size_t>(kZigzag[static_cast<std::size_t>(k)])] =
                value;
        }
        at += bytes;
        decoder.quant_defined[static_cast<std::size_t>(index)] = true;
    }
}

void parse_dht(Decoder& decoder, const std::uint8_t* segment, std::size_t size) {
    std::size_t at = 0;
    while (at < size) {
        const int table_class = segment[at] >> 4;
        const int index = segment[at] & 15;
        ++at;
        if (table_class > 1) fail("unknown Huffman table class");
        if (index > 3) fail("Huffman table index outside 0..3");
        if (at + 16 > size) fail("truncated Huffman table");
        HuffmanTable table;
        int total = 0;
        for (int length = 1; length <= 16; ++length) {
            table.count[static_cast<std::size_t>(length)] = segment[at];
            total += segment[at];
            ++at;
        }
        if (total > 256) fail("Huffman code size table overflow");
        if (at + static_cast<std::size_t>(total) > size) {
            fail("truncated Huffman symbol table");
        }
        table.values.assign(segment + at, segment + at + total);
        at += static_cast<std::size_t>(total);
        // Canonical code assignment (libjpeg jdhuff.c).
        int code = 0;
        int symbol = 0;
        for (int length = 1; length <= 16; ++length) {
            table.valptr[static_cast<std::size_t>(length)] = symbol;
            table.mincode[static_cast<std::size_t>(length)] = code;
            code += table.count[static_cast<std::size_t>(length)];
            symbol += table.count[static_cast<std::size_t>(length)];
            table.maxcode[static_cast<std::size_t>(length)] = code - 1;
            if (code > (1 << length)) fail("Huffman code overflow at this length");
            code <<= 1;
        }
        table.defined = true;
        if (table_class == 0) {
            decoder.dc_tables[static_cast<std::size_t>(index)] = std::move(table);
        } else {
            decoder.ac_tables[static_cast<std::size_t>(index)] = std::move(table);
        }
    }
}

void parse_sof(Decoder& decoder, std::uint8_t marker, const std::uint8_t* segment,
               std::size_t size) {
    if (decoder.sof_seen) fail("two SOF markers");
    if (marker != 0xc0 && marker != 0xc1 && marker != 0xc2) {
        fail("unsupported JPEG process (SOF type " +
             std::to_string(static_cast<unsigned>(marker)) + ")");
    }
    if (size < 6) fail("truncated SOF segment");
    if (segment[0] != 8) fail("unsupported JPEG data precision");
    decoder.height = (segment[1] << 8) | segment[2];
    decoder.width = (segment[3] << 8) | segment[4];
    decoder.component_count = segment[5];
    if (decoder.width <= 0) fail("empty JPEG image width");
    if (decoder.height <= 0) fail("empty JPEG image height (DNL is not supported)");
    if (decoder.component_count != 1 && decoder.component_count != 3) {
        fail("unsupported colour transform for this component count");
    }
    if (size < 6 + static_cast<std::size_t>(decoder.component_count) * 3) {
        fail("truncated SOF component list");
    }
    for (int index = 0; index < decoder.component_count; ++index) {
        auto& component = decoder.components[static_cast<std::size_t>(index)];
        const std::size_t at = 6 + static_cast<std::size_t>(index) * 3;
        component.id = segment[at];
        component.h = segment[at + 1] >> 4;
        component.v = segment[at + 1] & 15;
        component.quant_index = segment[at + 2];
        if (component.h < 1 || component.h > 4 || component.v < 1 ||
            component.v > 4) {
            fail("unsupported sampling factor");
        }
        if (component.quant_index > 3) fail("quantisation table index outside 0..3");
        for (int other = 0; other < index; ++other) {
            if (decoder.components[static_cast<std::size_t>(other)].id ==
                component.id) {
                fail("duplicate component identifier");
            }
        }
        decoder.max_h = std::max(decoder.max_h, component.h);
        decoder.max_v = std::max(decoder.max_v, component.v);
    }
    decoder.progressive = marker == 0xc2;
    decoder.mcus_x = ceil_div(decoder.width, 8 * decoder.max_h);
    decoder.mcus_y = ceil_div(decoder.height, 8 * decoder.max_v);
    int blocks_per_mcu = 0;
    for (int index = 0; index < decoder.component_count; ++index) {
        auto& component = decoder.components[static_cast<std::size_t>(index)];
        if (decoder.max_h % component.h != 0 || decoder.max_v % component.v != 0) {
            fail("sampling factor does not divide the maximum");
        }
        component.sample_w = ceil_div(decoder.width * component.h, decoder.max_h);
        component.sample_h = ceil_div(decoder.height * component.v, decoder.max_v);
        component.blocks_w = decoder.mcus_x * component.h;
        component.blocks_h = decoder.mcus_y * component.v;
        component.coefficients.assign(static_cast<std::size_t>(component.blocks_w) *
                                          static_cast<std::size_t>(component.blocks_h) * 64,
                                      0);
        blocks_per_mcu += component.h * component.v;
    }
    if (blocks_per_mcu > 10) fail("too many blocks per MCU");
    decoder.sof_seen = true;
}

void decode_block_sequential(Decoder& decoder, const ScanComponent& entry,
                             std::int32_t* block, BitReader& reader) {
    const auto component = static_cast<std::size_t>(entry.index);
    const int bits = decode_huffman(decoder.dc_tables[static_cast<std::size_t>(entry.dc_table)],
                                    reader);
    if (bits > 15) fail("DC magnitude category outside 0..15");
    decoder.dc_pred[component] += reader.receive(bits);
    block[0] = decoder.dc_pred[component];
    const auto& ac = decoder.ac_tables[static_cast<std::size_t>(entry.ac_table)];
    int k = 1;
    while (k < 64) {
        const int symbol = decode_huffman(ac, reader);
        const int run = symbol >> 4;
        const int size = symbol & 15;
        if (size == 0) {
            if (run != 15) break;  // end of block
            k += 16;
            if (k > 64) fail("AC run past the end of the block");
            continue;
        }
        k += run;
        if (k > 63) fail("AC run past the end of the block");
        block[kZigzag[static_cast<std::size_t>(k)]] = reader.receive(size);
        ++k;
    }
}

// Process-2 (progressive) block, including successive-approximation refinement.
// `eobrun` is libjpeg's entropy-decoder EOB run counter, shared by the scan.
void decode_block_progressive(Decoder& decoder, const ScanComponent& entry,
                              std::int32_t* block, int ss, int se, int ah, int al,
                              int& eobrun, BitReader& reader) {
    const auto component = static_cast<std::size_t>(entry.index);
    if (ss == 0) {
        if (ah == 0) {
            const int bits =
                decode_huffman(decoder.dc_tables[static_cast<std::size_t>(entry.dc_table)],
                               reader);
            if (bits > 15) fail("DC magnitude category outside 0..15");
            decoder.dc_pred[component] += reader.receive(bits);
            block[0] = decoder.dc_pred[component] * (1 << al);
        } else if (reader.get_bit()) {
            block[0] |= 1 << al;
        }
        return;
    }
    const auto& ac = decoder.ac_tables[static_cast<std::size_t>(entry.ac_table)];
    const std::int32_t positive = 1 << al;
    const std::int32_t negative = -(1 << al);
    int k = ss;
    if (ah == 0) {
        while (k <= se) {
            const int symbol = decode_huffman(ac, reader);
            const int run = symbol >> 4;
            const int size = symbol & 15;
            if (size == 0) {
                if (run != 15) break;  // end of band
                k += 16;
                continue;
            }
            k += run;
            if (k > se) fail("AC run past the end of the band");
            block[kZigzag[static_cast<std::size_t>(k)]] =
                reader.receive(size) * (1 << al);
            ++k;
        }
        return;
    }
    // AC refinement: correction bits for already-nonzero coefficients.
    if (eobrun > 0) {
        for (; k <= se; ++k) {
            auto& coefficient = block[kZigzag[static_cast<std::size_t>(k)]];
            if (coefficient != 0 && reader.get_bit()) {
                if ((coefficient & positive) == 0) {
                    coefficient += coefficient > 0 ? positive : negative;
                }
            }
        }
        --eobrun;
        return;
    }
    while (k <= se) {
        const int symbol = decode_huffman(ac, reader);
        const int run = symbol >> 4;
        const int size = symbol & 15;
        std::int32_t new_value = 0;
        if (size == 0) {
            if (run != 15) {
                // EOB run: 2^run plus the appended bits, minus the current block.
                eobrun = 1 << run;
                if (run != 0) eobrun += static_cast<int>(reader.get_bits(run));
                break;
            }
        } else {
            if (size != 1) fail("progressive AC refinement size is not 1");
            new_value = reader.get_bit() ? positive : negative;
        }
        int remaining = run;
        do {
            auto& coefficient = block[kZigzag[static_cast<std::size_t>(k)]];
            if (coefficient != 0) {
                if (reader.get_bit() && (coefficient & positive) == 0) {
                    coefficient += coefficient > 0 ? positive : negative;
                }
            } else {
                if (--remaining < 0) break;
            }
            ++k;
        } while (k <= se);
        if (new_value != 0) {
            if (k > se) fail("progressive AC refinement past the band");
            block[kZigzag[static_cast<std::size_t>(k)]] = new_value;
            ++k;
        }
    }
    // Finish the band: correction bits for the remaining nonzero coefficients.
    if (eobrun > 0) {
        for (; k <= se; ++k) {
            auto& coefficient = block[kZigzag[static_cast<std::size_t>(k)]];
            if (coefficient != 0 && reader.get_bit() && (coefficient & positive) == 0) {
                coefficient += coefficient > 0 ? positive : negative;
            }
        }
        --eobrun;
    }
}

void decode_scan(Decoder& decoder, const std::uint8_t* header, std::size_t size,
                 const std::uint8_t* data, const std::uint8_t* end,
                 const std::uint8_t** next) {
    if (!decoder.sof_seen) fail("SOS before SOF");
    if (size < 1) fail("truncated SOS segment");
    const int count = header[0];
    if (count < 1 || count > 4) fail("unsupported scan component count");
    if (size < 1 + static_cast<std::size_t>(count) * 2 + 3) {
        fail("truncated SOS component list");
    }
    std::array<ScanComponent, 4> scan{};
    std::array<bool, 4> seen{};
    for (int index = 0; index < count; ++index) {
        const std::size_t at = 1 + static_cast<std::size_t>(index) * 2;
        const int id = header[at];
        int found = -1;
        for (int component = 0; component < decoder.component_count; ++component) {
            if (decoder.components[static_cast<std::size_t>(component)].id == id) {
                found = component;
            }
        }
        if (found < 0) fail("scan references an unknown component");
        if (seen[static_cast<std::size_t>(found)]) fail("duplicate component in one scan");
        seen[static_cast<std::size_t>(found)] = true;
        scan[static_cast<std::size_t>(index)] =
            ScanComponent{found, header[at + 1] >> 4, header[at + 1] & 15};
        if (scan[static_cast<std::size_t>(index)].dc_table > 3 ||
            scan[static_cast<std::size_t>(index)].ac_table > 3) {
            fail("Huffman table selector outside 0..3");
        }
    }
    const std::size_t tail = 1 + static_cast<std::size_t>(count) * 2;
    const int ss = header[tail];
    const int se = header[tail + 1];
    const int ah = header[tail + 2] >> 4;
    const int al = header[tail + 2] & 15;
    if (ss > 63 || se > 63 || ss > se) fail("invalid spectral selection");
    if (decoder.progressive && ss == 0 && se != 0) fail("DC scan with Se != 0");
    if (count > 1 && ss != 0) fail("interleaved scan with a spectral selection");
    if (!decoder.progressive && (ss != 0 || se != 63 || ah != 0 || al != 0)) {
        fail("invalid sequential scan parameters");
    }
    if (decoder.progressive) {
        if (ah != 0 && al != ah - 1) fail("invalid successive approximation");
        if (al > 13) fail("successive approximation shift too large");
    }
    if (count > 1) {
        int blocks_per_mcu = 0;
        for (int index = 0; index < count; ++index) {
            const auto& component =
                decoder.components[static_cast<std::size_t>(scan[static_cast<std::size_t>(index)]
                                                                .index)];
            blocks_per_mcu += component.h * component.v;
        }
        if (blocks_per_mcu > 10) fail("too many blocks per MCU in this scan");
    }

    BitReader reader(data, end);
    int eobrun = 0;
    decoder.dc_pred.fill(0);
    const bool interleaved = count > 1;
    const auto& first = decoder.components[static_cast<std::size_t>(scan[0].index)];
    const int mcus_x = interleaved ? decoder.mcus_x : ceil_div(first.sample_w, 8);
    const int mcus_y = interleaved ? decoder.mcus_y : ceil_div(first.sample_h, 8);
    int mcu = 0;
    for (int mcu_y = 0; mcu_y < mcus_y; ++mcu_y) {
        for (int mcu_x = 0; mcu_x < mcus_x; ++mcu_x) {
            if (decoder.restart_interval != 0 && mcu != 0 &&
                mcu % decoder.restart_interval == 0) {
                reader.restart((mcu / decoder.restart_interval - 1) & 7);
                decoder.dc_pred.fill(0);
                eobrun = 0;
            }
            ++mcu;
            if (interleaved) {
                for (int index = 0; index < count; ++index) {
                    const auto& entry = scan[static_cast<std::size_t>(index)];
                    auto& component =
                        decoder.components[static_cast<std::size_t>(entry.index)];
                    for (int v = 0; v < component.v; ++v) {
                        for (int h = 0; h < component.h; ++h) {
                            auto* block =
                                component.block(mcu_y * component.v + v,
                                                mcu_x * component.h + h);
                            if (decoder.progressive) {
                                decode_block_progressive(decoder, entry, block, ss, se,
                                                         ah, al, eobrun, reader);
                            } else {
                                decode_block_sequential(decoder, entry, block, reader);
                            }
                        }
                    }
                }
            } else {
                const auto& entry = scan[0];
                auto& component = decoder.components[static_cast<std::size_t>(entry.index)];
                auto* block = component.block(mcu_y, mcu_x);
                if (decoder.progressive) {
                    decode_block_progressive(decoder, entry, block, ss, se, ah, al,
                                             eobrun, reader);
                } else {
                    decode_block_sequential(decoder, entry, block, reader);
                }
            }
        }
    }
    decoder.scan_seen = true;
    *next = reader.position();
}

// libjpeg 6a jidctint.c (JDCT_ISLOW): the integer IDCT the shipped engine runs.
// CONST_BITS 13, PASS1_BITS 2; jidctint's range-limit table folds the +128
// level shift in, so the stored sample is clamp(value + 128).
void idct_islow(const std::int32_t* input, const std::uint16_t* quant,
                std::uint8_t* out) {
    constexpr int CONST_BITS = 13;
    constexpr int PASS1_BITS = 2;
    constexpr std::int64_t FIX_0_298631336 = 2446;
    constexpr std::int64_t FIX_0_390180644 = 3196;
    constexpr std::int64_t FIX_0_541196100 = 4433;
    constexpr std::int64_t FIX_0_765366865 = 6270;
    constexpr std::int64_t FIX_0_899976223 = 7373;
    constexpr std::int64_t FIX_1_175875602 = 9633;
    constexpr std::int64_t FIX_1_501321110 = 12299;
    constexpr std::int64_t FIX_1_847759065 = 15137;
    constexpr std::int64_t FIX_1_961570560 = 16069;
    constexpr std::int64_t FIX_2_053119869 = 16819;
    constexpr std::int64_t FIX_2_562915447 = 20995;
    constexpr std::int64_t FIX_3_072711026 = 25172;

    const auto descale = [](std::int64_t value, int bits) {
        return static_cast<std::int32_t>((value + (static_cast<std::int64_t>(1) << (bits - 1))) >>
                                         bits);
    };

    std::array<std::int32_t, 64> work{};

    // Pass 1: columns.
    for (int column = 0; column < 8; ++column) {
        const std::int32_t* in = input + column;
        const std::uint16_t* q = quant + column;
        const auto dequant = [&](int row) {
            return static_cast<std::int64_t>(in[row * 8]) *
                   static_cast<std::int64_t>(q[row * 8]);
        };
        if (in[8] == 0 && in[16] == 0 && in[24] == 0 && in[32] == 0 &&
            in[40] == 0 && in[48] == 0 && in[56] == 0) {
            const auto dc = static_cast<std::int32_t>(dequant(0)) * (1 << PASS1_BITS);
            for (int row = 0; row < 8; ++row) work[row * 8 + column] = dc;
            continue;
        }
        std::int64_t z2 = dequant(2);
        std::int64_t z3 = dequant(6);
        std::int64_t z1 = (z2 + z3) * FIX_0_541196100;
        const std::int64_t tmp2 = z1 + z3 * -FIX_1_847759065;
        const std::int64_t tmp3 = z1 + z2 * FIX_0_765366865;
        z2 = dequant(0);
        z3 = dequant(4);
        const std::int64_t tmp0 = (z2 + z3) * (1 << CONST_BITS);
        const std::int64_t tmp1 = (z2 - z3) * (1 << CONST_BITS);
        const std::int64_t tmp10 = tmp0 + tmp3;
        const std::int64_t tmp13 = tmp0 - tmp3;
        const std::int64_t tmp11 = tmp1 + tmp2;
        const std::int64_t tmp12 = tmp1 - tmp2;

        std::int64_t o0 = dequant(7);
        std::int64_t o1 = dequant(5);
        std::int64_t o2 = dequant(3);
        std::int64_t o3 = dequant(1);
        z1 = o0 + o3;
        z2 = o1 + o2;
        z3 = o0 + o2;
        std::int64_t z4 = o1 + o3;
        const std::int64_t z5 = (z3 + z4) * FIX_1_175875602;
        o0 *= FIX_0_298631336;
        o1 *= FIX_2_053119869;
        o2 *= FIX_3_072711026;
        o3 *= FIX_1_501321110;
        z1 *= -FIX_0_899976223;
        z2 *= -FIX_2_562915447;
        z3 = z3 * -FIX_1_961570560 + z5;
        z4 = z4 * -FIX_0_390180644 + z5;
        o0 += z1 + z3;
        o1 += z2 + z4;
        o2 += z2 + z3;
        o3 += z1 + z4;

        work[0 * 8 + column] = descale(tmp10 + o3, CONST_BITS - PASS1_BITS);
        work[7 * 8 + column] = descale(tmp10 - o3, CONST_BITS - PASS1_BITS);
        work[1 * 8 + column] = descale(tmp11 + o2, CONST_BITS - PASS1_BITS);
        work[6 * 8 + column] = descale(tmp11 - o2, CONST_BITS - PASS1_BITS);
        work[2 * 8 + column] = descale(tmp12 + o1, CONST_BITS - PASS1_BITS);
        work[5 * 8 + column] = descale(tmp12 - o1, CONST_BITS - PASS1_BITS);
        work[3 * 8 + column] = descale(tmp13 + o0, CONST_BITS - PASS1_BITS);
        work[4 * 8 + column] = descale(tmp13 - o0, CONST_BITS - PASS1_BITS);
    }

    // Pass 2: rows.
    for (int row = 0; row < 8; ++row) {
        const std::int32_t* ws = &work[row * 8];
        std::uint8_t* destination = out + row * 8;
        const auto store = [&](int column, std::int64_t value) {
            const auto sample = static_cast<int>(descale(value, CONST_BITS + PASS1_BITS + 3)) + 128;
            destination[column] = static_cast<std::uint8_t>(
                sample < 0 ? 0 : (sample > 255 ? 255 : sample));
        };
        if (ws[1] == 0 && ws[2] == 0 && ws[3] == 0 && ws[4] == 0 && ws[5] == 0 &&
            ws[6] == 0 && ws[7] == 0) {
            const int dc = (ws[0] >> (PASS1_BITS + 3)) + 128;
            const auto sample = static_cast<std::uint8_t>(dc < 0 ? 0 : (dc > 255 ? 255 : dc));
            for (int column = 0; column < 8; ++column) destination[column] = sample;
            continue;
        }
        std::int64_t z2 = ws[2];
        std::int64_t z3 = ws[6];
        std::int64_t z1 = (z2 + z3) * FIX_0_541196100;
        const std::int64_t tmp2 = z1 + z3 * -FIX_1_847759065;
        const std::int64_t tmp3 = z1 + z2 * FIX_0_765366865;
        const std::int64_t tmp0 =
            (static_cast<std::int64_t>(ws[0]) + ws[4]) * (1 << CONST_BITS);
        const std::int64_t tmp1 =
            (static_cast<std::int64_t>(ws[0]) - ws[4]) * (1 << CONST_BITS);
        const std::int64_t tmp10 = tmp0 + tmp3;
        const std::int64_t tmp13 = tmp0 - tmp3;
        const std::int64_t tmp11 = tmp1 + tmp2;
        const std::int64_t tmp12 = tmp1 - tmp2;

        std::int64_t o0 = ws[7];
        std::int64_t o1 = ws[5];
        std::int64_t o2 = ws[3];
        std::int64_t o3 = ws[1];
        z1 = o0 + o3;
        z2 = o1 + o2;
        z3 = o0 + o2;
        std::int64_t z4 = o1 + o3;
        const std::int64_t z5 = (z3 + z4) * FIX_1_175875602;
        o0 *= FIX_0_298631336;
        o1 *= FIX_2_053119869;
        o2 *= FIX_3_072711026;
        o3 *= FIX_1_501321110;
        z1 *= -FIX_0_899976223;
        z2 *= -FIX_2_562915447;
        z3 = z3 * -FIX_1_961570560 + z5;
        z4 = z4 * -FIX_0_390180644 + z5;
        o0 += z1 + z3;
        o1 += z2 + z4;
        o2 += z2 + z3;
        o3 += z1 + z4;

        store(0, tmp10 + o3);
        store(7, tmp10 - o3);
        store(1, tmp11 + o2);
        store(6, tmp11 - o2);
        store(2, tmp12 + o1);
        store(5, tmp12 - o1);
        store(3, tmp13 + o0);
        store(4, tmp13 - o0);
    }
}

std::vector<std::uint8_t> component_plane(const Decoder& decoder,
                                          const Component& component) {
    std::vector<std::uint8_t> plane(static_cast<std::size_t>(component.sample_w) *
                                    static_cast<std::size_t>(component.sample_h));
    const auto& quant = decoder.quant[static_cast<std::size_t>(component.quant_index)];
    std::array<std::uint8_t, 64> block{};
    const int block_columns = ceil_div(component.sample_w, 8);
    const int block_rows = ceil_div(component.sample_h, 8);
    for (int block_row = 0; block_row < block_rows; ++block_row) {
        for (int block_column = 0; block_column < block_columns; ++block_column) {
            const auto* coefficients =
                component.coefficients.data() +
                (static_cast<std::size_t>(block_row) *
                     static_cast<std::size_t>(component.blocks_w) +
                 static_cast<std::size_t>(block_column)) *
                    64;
            idct_islow(coefficients, quant.data(), block.data());
            for (int row = 0; row < 8; ++row) {
                const int y = block_row * 8 + row;
                if (y >= component.sample_h) break;
                for (int column = 0; column < 8; ++column) {
                    const int x = block_column * 8 + column;
                    if (x >= component.sample_w) break;
                    plane[static_cast<std::size_t>(y) *
                              static_cast<std::size_t>(component.sample_w) +
                          static_cast<std::size_t>(x)] = block[row * 8 + column];
                }
            }
        }
    }
    return plane;
}

// 3:1 triangle interpolation: the filter family libjpeg's "fancy" upsampler
// applies for 2:1 factors. Non-2:1 factors are replicated.
std::vector<std::uint8_t> expand_plane(const std::vector<std::uint8_t>& plane,
                                       int width, int height, int horizontal,
                                       int vertical) {
    if (horizontal == 1 && vertical == 1) return plane;
    const int full_w = width * horizontal;
    const int full_h = height * vertical;
    std::vector<std::uint8_t> vertical_pass(
        static_cast<std::size_t>(width) * static_cast<std::size_t>(full_h));
    for (int y = 0; y < full_h; ++y) {
        const int source_y = y / vertical;
        const std::uint8_t* row =
            plane.data() + static_cast<std::size_t>(source_y) * static_cast<std::size_t>(width);
        std::uint8_t* destination =
            vertical_pass.data() + static_cast<std::size_t>(y) * static_cast<std::size_t>(width);
        if (vertical != 2) {
            std::copy(row, row + width, destination);
            continue;
        }
        const int partner = (y & 1) ? std::min(source_y + 1, height - 1)
                                    : std::max(source_y - 1, 0);
        const std::uint8_t* other =
            plane.data() + static_cast<std::size_t>(partner) * static_cast<std::size_t>(width);
        for (int x = 0; x < width; ++x) {
            destination[x] = static_cast<std::uint8_t>((3 * row[x] + other[x] + 2) >> 2);
        }
    }
    if (horizontal == 1) return vertical_pass;
    std::vector<std::uint8_t> result(static_cast<std::size_t>(full_w) *
                                     static_cast<std::size_t>(full_h));
    for (int y = 0; y < full_h; ++y) {
        const std::uint8_t* row =
            vertical_pass.data() + static_cast<std::size_t>(y) * static_cast<std::size_t>(width);
        std::uint8_t* destination =
            result.data() + static_cast<std::size_t>(y) * static_cast<std::size_t>(full_w);
        for (int x = 0; x < full_w; ++x) {
            const int source_x = x / horizontal;
            if (horizontal != 2) {
                destination[x] = row[source_x];
                continue;
            }
            const int partner = (x & 1) ? std::min(source_x + 1, width - 1)
                                        : std::max(source_x - 1, 0);
            destination[x] = static_cast<std::uint8_t>((3 * row[source_x] + row[partner] + 2) >> 2);
        }
    }
    return result;
}

struct Plane {
    int width = 0;
    int height = 0;
    std::vector<std::uint8_t> samples;
};

bool is_rgb_component_ids(const Decoder& decoder) {
    return decoder.component_count == 3 &&
           decoder.components[0].id == 0x52 && decoder.components[1].id == 0x47 &&
           decoder.components[2].id == 0x42;
}

}  // namespace

JpegHeader read_jpeg_header(const std::vector<std::uint8_t>& data) {
    if (data.size() < 2 || data[0] != 0xff || data[1] != 0xd8) {
        fail("not a JPEG file (missing SOI)");
    }
    Decoder decoder;
    std::size_t pos = 2;
    while (pos < data.size()) {
        if (data[pos] != 0xff) fail("expected a marker where the header continues");
        while (pos < data.size() && data[pos] == 0xff) ++pos;
        if (pos >= data.size()) fail("marker without a code");
        const auto marker = data[pos++];
        if (marker == 0xd9) break;
        if (marker == 0xd8) fail("two SOI markers");
        if (marker >= 0xd0 && marker <= 0xd7) fail("restart marker outside a scan");
        if (marker == 0x01) continue;
        if (pos + 2 > data.size()) fail("truncated segment length");
        const int length = read_be16(data, pos);
        if (length < 2 || pos + static_cast<std::size_t>(length) > data.size()) {
            fail("truncated segment");
        }
        const std::uint8_t* segment = data.data() + pos + 2;
        const std::size_t size = static_cast<std::size_t>(length) - 2;
        if (marker == 0xc0 || marker == 0xc1 || marker == 0xc2) {
            parse_sof(decoder, marker, segment, size);
            break;
        }
        if (marker == 0xc3 || (marker >= 0xc5 && marker <= 0xc7) ||
            (marker >= 0xc9 && marker <= 0xcb) || (marker >= 0xcd && marker <= 0xcf)) {
            fail("unsupported JPEG process (SOF type " +
                 std::to_string(static_cast<unsigned>(marker)) + ")");
        }
        if (marker == 0xcc) fail("arithmetic coding is not supported");
        if (marker == 0xda) break;
        pos += static_cast<std::size_t>(length);
    }
    if (!decoder.sof_seen) fail("missing SOF marker");
    JpegHeader header;
    header.width = decoder.width;
    header.height = decoder.height;
    header.components = decoder.component_count;
    header.progressive = decoder.progressive;
    for (int index = 0; index < decoder.component_count; ++index) {
        header.h_sampling[index] = decoder.components[static_cast<std::size_t>(index)].h;
        header.v_sampling[index] = decoder.components[static_cast<std::size_t>(index)].v;
        header.quant_table[index] =
            decoder.components[static_cast<std::size_t>(index)].quant_index;
    }
    return header;
}

JpegImage decode_jpeg(const std::vector<std::uint8_t>& data) {
    if (data.size() < 2 || data[0] != 0xff || data[1] != 0xd8) {
        fail("not a JPEG file (missing SOI)");
    }
    Decoder decoder;
    std::size_t pos = 2;
    bool eoi = false;
    while (pos < data.size()) {
        if (data[pos] != 0xff) fail("expected a marker where the stream continues");
        while (pos < data.size() && data[pos] == 0xff) ++pos;
        if (pos >= data.size()) fail("marker without a code");
        const auto marker = data[pos++];
        if (marker == 0xd9) {
            eoi = true;
            break;
        }
        if (marker == 0xd8) fail("two SOI markers");
        if (marker >= 0xd0 && marker <= 0xd7) fail("restart marker outside a scan");
        if (marker == 0x01) continue;
        if (pos + 2 > data.size()) fail("truncated segment length");
        const int length = read_be16(data, pos);
        if (length < 2 || pos + static_cast<std::size_t>(length) > data.size()) {
            fail("truncated segment");
        }
        const std::uint8_t* segment = data.data() + pos + 2;
        const std::size_t size = static_cast<std::size_t>(length) - 2;
        switch (marker) {
        case 0xc0:
        case 0xc1:
        case 0xc2:
            parse_sof(decoder, marker, segment, size);
            pos += static_cast<std::size_t>(length);
            break;
        case 0xc4:
            parse_dht(decoder, segment, size);
            pos += static_cast<std::size_t>(length);
            break;
        case 0xdb:
            parse_dqt(decoder, segment, size);
            pos += static_cast<std::size_t>(length);
            break;
        case 0xdd: {
            if (size != 2) fail("malformed DRI segment");
            decoder.restart_interval = (segment[0] << 8) | segment[1];
            pos += static_cast<std::size_t>(length);
            break;
        }
        case 0xda: {
            if (!decoder.sof_seen) fail("SOS before SOF");
            const std::uint8_t* next = nullptr;
            decode_scan(decoder, segment, size, data.data() + pos + length,
                        data.data() + data.size(), &next);
            pos = static_cast<std::size_t>(next - data.data());
            break;
        }
        case 0xee: {  // APP14 Adobe
            decoder.adobe_seen = true;
            if (size >= 12 && segment[0] == 'A' && segment[1] == 'd' &&
                segment[2] == 'o' && segment[3] == 'b' && segment[4] == 'e') {
                decoder.adobe_transform = segment[11];
            }
            pos += static_cast<std::size_t>(length);
            break;
        }
        default:
            if (marker >= 0xe0 && marker <= 0xef) {  // APPn
                pos += static_cast<std::size_t>(length);
                break;
            }
            if (marker == 0xfe) {  // COM
                pos += static_cast<std::size_t>(length);
                break;
            }
            if (marker == 0xc3 || (marker >= 0xc5 && marker <= 0xc7) ||
                (marker >= 0xc9 && marker <= 0xcb) ||
                (marker >= 0xcd && marker <= 0xcf)) {
                fail("unsupported JPEG process (SOF type " +
                     std::to_string(static_cast<unsigned>(marker)) + ")");
            }
            if (marker == 0xcc) fail("arithmetic coding is not supported");
            fail("unsupported marker 0x" + std::to_string(static_cast<unsigned>(marker)));
        }
    }
    if (!decoder.sof_seen) fail("missing SOF marker");
    if (!decoder.scan_seen) fail("missing SOS marker");
    if (!eoi) fail("missing EOI marker (truncated file)");
    for (int index = 0; index < decoder.component_count; ++index) {
        const auto& component = decoder.components[static_cast<std::size_t>(index)];
        if (!decoder.quant_defined[static_cast<std::size_t>(component.quant_index)]) {
            fail("quantisation table was not defined");
        }
    }

    const bool direct_rgb = is_rgb_component_ids(decoder) ||
                            (decoder.adobe_seen && decoder.adobe_transform == 0);
    std::array<Plane, 3> planes{};
    for (int index = 0; index < decoder.component_count; ++index) {
        const auto& component = decoder.components[static_cast<std::size_t>(index)];
        auto plane = component_plane(decoder, component);
        planes[static_cast<std::size_t>(index)] = Plane{
            component.sample_w, component.sample_h,
            expand_plane(plane, component.sample_w, component.sample_h,
                         decoder.max_h / component.h, decoder.max_v / component.v)};
    }

    JpegImage image;
    image.width = decoder.width;
    image.height = decoder.height;
    image.rgb.assign(image.pixel_count() * 3, 0);
    const int width = decoder.width;
    // jdcolor.c's 16.16 fixed-point constants and ONE_HALF rounding.
    constexpr int SCALEBITS = 16;
    constexpr std::int64_t ONE_HALF = 1 << (SCALEBITS - 1);
    constexpr std::int64_t FIX_1_40200 = 91881;
    constexpr std::int64_t FIX_1_77200 = 116130;
    constexpr std::int64_t FIX_0_34414 = 22554;
    constexpr std::int64_t FIX_0_71414 = 46802;
    const auto clamp8 = [](std::int64_t value) {
        return static_cast<std::uint8_t>(value < 0 ? 0 : (value > 255 ? 255 : value));
    };
    if (decoder.component_count == 1) {
        for (int y = 0; y < decoder.height; ++y) {
            for (int x = 0; x < width; ++x) {
                const auto sample = planes[0].samples[static_cast<std::size_t>(y) *
                                                           static_cast<std::size_t>(width) +
                                                       static_cast<std::size_t>(x)];
                const std::size_t at =
                    (static_cast<std::size_t>(y) * static_cast<std::size_t>(width) +
                     static_cast<std::size_t>(x)) *
                    3;
                image.rgb[at] = sample;
                image.rgb[at + 1] = sample;
                image.rgb[at + 2] = sample;
            }
        }
    } else if (direct_rgb) {
        for (int y = 0; y < decoder.height; ++y) {
            for (int x = 0; x < width; ++x) {
                const std::size_t source =
                    static_cast<std::size_t>(y) * static_cast<std::size_t>(width) +
                    static_cast<std::size_t>(x);
                image.rgb[source * 3] = planes[0].samples[source];
                image.rgb[source * 3 + 1] = planes[1].samples[source];
                image.rgb[source * 3 + 2] = planes[2].samples[source];
            }
        }
    } else {
        for (int y = 0; y < decoder.height; ++y) {
            for (int x = 0; x < width; ++x) {
                const std::size_t source =
                    static_cast<std::size_t>(y) * static_cast<std::size_t>(width) +
                    static_cast<std::size_t>(x);
                const auto luma = static_cast<std::int64_t>(planes[0].samples[source]);
                const std::int64_t cb = static_cast<std::int64_t>(planes[1].samples[source]) - 128;
                const std::int64_t cr = static_cast<std::int64_t>(planes[2].samples[source]) - 128;
                image.rgb[source * 3] =
                    clamp8(luma + ((FIX_1_40200 * cr + ONE_HALF) >> SCALEBITS));
                image.rgb[source * 3 + 1] =
                    clamp8(luma + ((FIX_0_34414 * -cb + FIX_0_71414 * -cr + ONE_HALF) >>
                                   SCALEBITS));
                image.rgb[source * 3 + 2] =
                    clamp8(luma + ((FIX_1_77200 * cb + ONE_HALF) >> SCALEBITS));
            }
        }
    }
    return image;
}

JpegImage decode_jpeg_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("jpeg: cannot open " + path);
    const std::vector<std::uint8_t> data((std::istreambuf_iterator<char>(input)),
                                         std::istreambuf_iterator<char>());
    if (data.empty()) throw std::runtime_error("jpeg: empty file " + path);
    return decode_jpeg(data);
}

std::vector<std::uint16_t> rgb_to_rgb565(const JpegImage& image) {
    if (image.rgb.size() != image.pixel_count() * 3) {
        throw std::runtime_error("jpeg: RGB buffer does not match the image size");
    }
    std::vector<std::uint16_t> pixels(image.pixel_count());
    for (std::size_t index = 0; index < pixels.size(); ++index) {
        const auto r = image.rgb[index * 3];
        const auto g = image.rgb[index * 3 + 1];
        const auto b = image.rgb[index * 3 + 2];
        pixels[index] = static_cast<std::uint16_t>(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
    }
    return pixels;
}

}  // namespace byzantine
