# Quick implementation audit — 25 October 2026

Manual source/report correspondence audit; not a new binary derivation.
Sources: port/src/*.cpp, port/include/byzantine/panorama.hpp, README.md and
focused reports referenced by CONTINUE.md. No new target execution or export
behavior analysis. Address attribution reuses existing reports; hashes were not
rechecked for this source-only audit.

This conservative list counts unique original addresses with an identifiable
implemented behavior or subset. It is NOT a list of fully ported functions.
`bounded` means a recognizable algorithm is implemented for the documented
portable subset, NOT whole-function original-runtime equivalence. `partial`
means only a branch, formula, state subset or prototype replacement exists.
Excluded: merely inspected loaders, resource-refresh routines only represented
by a request counter, CRT routines delegated to host libraries and new port-only
helpers. Main interpreter attribution overlaps the command executor and must
not be interpreted as two independently completed systems.

| Original address | Ported behavior | Scope |
|---|---|---|
| 0x4200bb | RLE frame loading | partial |
| 0x40cb26 | RLE frame count | bounded |
| 0x40bbfd | RLE run decoding | bounded |
| 0x4164a0 | RGB555 conversion | partial |
| 0x411b4b | ALL directory parsing | bounded |
| 0x412762 | CC decoding | bounded |
| 0x42d383 | Label table | bounded |
| 0x43194f | Script call stack | bounded |
| 0x42d8ba | Script return | partial |
| 0x4319b3 | Operand resolution | partial |
| 0x431a36 | Comparison operands | partial |
| 0x42db22 | VM command branches | partial |
| 0x43340a | Main interpreter behavior | partial |
| 0x438cf9 | Suspend hotspots | bounded |
| 0x438d78 | Restore hotspots | bounded |
| 0x438173 | Variable metadata reading | partial |
| 0x42c8a0 | New-game zero state | partial |
| 0x41045b | Background loading | partial |
| 0x431ec2 | Background command | partial |
| 0x401c21 | Stage panorama position | bounded |
| 0x401ddf | Consume panorama camera position | bounded |
| 0x402e00 | Pitch angle getter formula | bounded |
| 0x402e70 | Yaw angle getter formula | bounded |
| 0x423310 | Projection tables | partial |
| 0x4236b5 | Panorama geometry and source setup | partial |
| 0x42382b | Seam padding | bounded |
| 0x423a37 | Projection mapping | bounded |
| 0x423c42 | Pitch-limit formula | bounded |
| 0x423cdf | Padding extent formula | bounded |
| 0x4241e2 | Projection reset | partial |
| 0x4242bd | Camera advance and normal RGB16 sample | partial |

Count: 31 unique addresses with full or partial behavioral counterparts.
This is a quick lower-bound correspondence inventory, not exhaustive coverage.
No exact count of fully reconstructed original functions is established.
The previously observed panorama reset test failure remains unresolved.
