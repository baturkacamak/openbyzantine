// Controlled local x87 instruction probe, NOT execution of BETRAYAL.EXE.
// Build/run on x86 GCC/Clang: c++ -std=c++17 -O2 probe_panorama_bias.cpp -o /tmp/byzantine-bias-probe
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#if !defined(__i386__) && !defined(__x86_64__)
#error This evidence probe requires x87 inline assembly on x86.
#endif

int quantize(double value, unsigned short precision) {
    unsigned short saved;
    asm volatile("fnstcw %0" : "=m"(saved));
    const unsigned short control = (saved & ~0x0f00u) | precision;
    const std::uint64_t bits = 0x4338000000000100ULL;
    double bias, result;
    std::memcpy(&bias, &bits, sizeof bias);
    asm volatile("fldcw %2\n\tfldl %3\n\tfaddl %4\n\tfstpl %0\n\tfldcw %1"
                 : "=m"(result)
                 : "m"(saved), "m"(control), "m"(value), "m"(bias)
                 : "st");
    std::uint64_t output;
    std::memcpy(&output, &result, sizeof output);
    const std::uint32_t low = static_cast<std::uint32_t>(output) - 0x100u;
    std::int32_t signed_low;
    std::memcpy(&signed_low, &low, sizeof low);
    return signed_low;
}

int main() {
    // Exhaustive tie/near-tie samples over a bounded coordinate interval.
    std::size_t count = 0;
    for (int n = -10000; n <= 10000; ++n) {
        for (double delta : {-0.5001, -0.5, -0.4999, 0.0, 0.4999, 0.5, 0.5001}) {
            const int tie_up = n % 2 != 0;
            const int expected = n + (delta > 0.5 ? 1 : delta < -0.5 ? -1 :
                delta == 0.5 ? tie_up : delta == -0.5 ? -tie_up : 0);
            if (quantize(n + delta, 0x0200) != expected)
                throw std::runtime_error("PC53 bias conversion mismatch");
            ++count;
        }
    }
    if (quantize(0.5001, 0x0300) != 0 || quantize(0.5001, 0x0200) != 1)
        throw std::runtime_error("precision-sensitive example mismatch");
    std::cout << "PC53 nearest/even: " << count << " bounded samples passed\n"
              << "0.5001: PC53 => 1; PC64 => 0 (double rounding)\n";
}
