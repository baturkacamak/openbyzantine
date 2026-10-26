# Panorama lookup tables and x87 precision

25 October 2026. Static reconstruction from saved REA/Ghidra exports plus
focused GNU objdump; the original game was not executed. No binary session
was opened. `panorama-projection-evidence.json` now binds 20 exports, 16 retained
assembly ranges and seven PE data ranges. All selected export hashes and target
identity were checked against `pseudocode/decompile-index.json` before use.

## Observations

The startup export `0x44d530` calls `0x44bfd0`. That routine calls through
`0x462aec`, whose on-disk DWORD is `0x44a7a0` (`__fpmath`). Its assembly calls
`0x44e870`, then clears floating-point exception flags with `fnclex`.
`0x44e870` calls `__controlfp(0x10000, 0x30000)`. In `0x454580`, the existing
hardware word is read, abstract flags are merged under the mask, converted
back by `0x4546c0`, and installed with `fldcw`. The conversion at
`0x45472f..0x45473e` maps abstract precision `0x10000` to hardware PC=0x200:
**53 significand bits**, with the x87 exponent range. This mask preserves the
rounding mode; this call does not itself establish nearest/even rounding.
The saved `__hw_cw` pseudocode omits the meaningful register result, so assembly
is necessary here. No claim is made about every subsequent control-word change
or external DLL preserving this state.

`0x423310` allocates a width*height array of 32-bit mapping entries. It also
allocates these binary32 lookup tables:

| Table | Allocated cells | Center cell | Loop | Initialized signed indices | Unwritten allocated indices |
| --- | ---: | ---: | --- | --- | --- |
| cosine | 3141 (0x3114 bytes) | 1570 | i=0..1569 | -1569..1569 | -1570, +1570 |
| arctangent | 4000 (16000 bytes) | 2000 | i=0..1999 | -1999..1999 | -2000 |

Each loop first computes and stores binary64 `a=i/1000.0`, using the literal
1000 at `0x45b138`. It calls the CRT with `a`, stores the result as binary32
at center+i, calls again with `-a`, and stores at center-i. Thus the center is
written twice, with a negative-zero argument on the second call. Cosine is
selected by descriptor `0x463472`; arctangent by `0x463810`. These are not sine
tables. Ghidra obscures the negative cosine argument; `fchs` and the second
argument store at `0x42350f..0x42351a` resolve it.

In mapping function `0x423a37`, bias quantization selects the arctangent table
from reciprocal-depth*x*1000, selects cosine from the returned binary32
angle*1000, and quantizes horizontal/vertical pixel contributions. Neither
allocation size nor the existence of allocated edge cells proves a valid
lookup. The mapping routine has no visible per-access bounds guards.

## Controlled probe and inference

`probe_panorama_bias.cpp` executes a small local x87 sequence using the exact
bias bits `0x4338000000000100`, a controlled PC/RC setting, binary64 store,
low-DWORD extraction and subtraction of 256. It saves/restores the control
word. This is an instruction experiment on the analysis host, **not an original
engine capture**. Run on x86 GCC/Clang:

```sh
c++ -std=c++17 -O2 analysis/rea/probe_panorama_bias.cpp -o /tmp/byzantine-bias-probe
/tmp/byzantine-bias-probe
```

At PC53 / RC=nearest, 140,007 integer/tie/near-tie samples across -10000..10000
agree with signed nearest/even conversion. At input 0.5001, PC53 produces 1,
but PC64 produces 0: the extended addition first rounds near 0.5, and the
binary64 store rounds again. This corrects the earlier assumption that using
host `long double` plus a binary64 store necessarily reproduces the table
conversion. Conditional on PC53 and nearest rounding, the portable bounded
conversion can use explicit ties-to-even. Runtime RC, non-bounded inputs,
CRT internals and all mapping intermediate operations remain separate issues.

## Portable implementation and validation

`PanoramaLookupTables` constructs only the initialized table domain, with the
recovered binary64 argument and binary32 result stores. Indexed access rejects
unwritten/out-of-allocation cells instead of creating missing game values.
The host `atan(-0.0)` result is retained by the second center write; the
original CRT's exact negative-zero result remains unverified.
`panorama_table_coordinate(double)` implements the bounded nearest/even model
without relying on the host's current `nearbyint` rounding mode. Nonfinite and
signed-int overflow inputs are rejected as port safeguards; original wrapping
and exceptional x87 paths are not emulated. It accepts an already evaluated
double and does not emulate an entire extended-precision expression.

`make -C port -j2 all test` passes. New tests cover tie and near-tie conversion,
signed limits/rejections, every initialized table entry's symmetry and
monotonicity, endpoint scaling, zero overwrite, and unwritten boundary cells.
`python3 analysis/rea/verify_panorama_projection.py` verifies the expanded
manifest. Host libm values are formula approximations; original CRT binary32
outputs have not been compared. Debian tested; Windows/macOS remain untested.

The tables and quantizer remain standalone. The next bounded task is to
reconstruct `0x423a37` mapping offsets with explicit state and checked table
and source bounds, accounting for PC53 intermediate arithmetic and hardware
fsin/fcos versus CRT setup calls. Recover/verify the effective rounding state
before claiming exact mapping. `0xb9`, `0xbb`, variable resets, transitions,
resource composition and loader integration remain unsupported.

Follow-up: [`PANORAMA_MAPPING.md`](PANORAMA_MAPPING.md) now recovers mapping
entries with explicit state and checked bounds. The normal RGB16 sampling
path, original numeric comparison and VM loader integration remain pending.
