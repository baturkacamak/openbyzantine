# TK animation visual defects — 27 October 2026

User evidence: two screenshots, wide TK entrance with a rectangular patch and
close-up character with holes. Pinned run:
`run-20261027T105116.045293Z-vk3qe1h9`, EA602/26.btb -> MAP -> TK -> slot2/action61.

## Two independent causes

**Delta reconstruction.** TKNUXB has a zero animation signature, but its first
frame is a full image and later RLE frames skip unchanged pixels. Command3
leaves the original destination untouched (`0x40bbfd`). The original normal
renderer uses a persistent surface (`0x43c466` -> `0x43b130`); this host clears
and repaints its surface each frame. Its signature-only head-walk consequently
lost prior pixels on signature-zero animations, showing background through
the character. The host now reconstructs frames0..current regardless of
signature, then draws the result at the existing position. This is adaptation
to a repainted surface, not a claim that the original always runs a head walk.
Packed RLE data is cached per asset to avoid repeated disk opens during replay.

**Wrong disc variant.** The old launcher extracted six animation trees into
one directory with overwrite enabled. Same filenames do not guarantee the
same content. Across the supplied ISOs:

| Asset | CD1/2 | CD3/5 |
| --- | --- | --- |
| TKNUXA.RLE | 86056 bytes; signature1;16 frames;120x120 first frame | 33128 bytes;signature0;2 frames;90x107 first frame |
| TKNUXB.RLE | 1267362 bytes;18 frames | 475748 bytes;18 frames |

TKXA.jpg is byte-identical across all six CDs. The CD3/5 TKNUXA first frame
contains the different background patch seen in the screenshot. The selected
26.btb save says cd=2, but the flattened tree held CD5's art.

Observed SHA-256 values (read-only extraction from the original ISOs):

- CD1/2 TKNUXA: `8976d555b22dbb98fd927fbca4931fadbb3c8551fef5612423fb03b893b3637e`
- CD3/5 TKNUXA: `b466019331c0fcd0b473da4fed923f036d541f088865e864234c109e3f31a3f8`
- All TKXA.jpg: `e92c63bb186b7a2c890c811da80d8997bbe433de7853e22eeae24fef6260ec43`

## Selection and validation

`scene.sh` reads the existing save parser's cd field, then calls
`prepare_disc_anims.py` to extract that ISO's `anims/` into a separate
`$BYZ_DATA/disc-media/cdN-<identity>/` cache. A source path/size/mtime marker
avoids repeating extraction; temporary directories are renamed only after
successful extraction. Available matching-disc assets override union entries
through the new optional `--anims-overlay` path. The union remains fallback
for absent assets. Originals and prior extracted copies are preserved.
This applies to all scene loads and does not hardcode TK names. A session's
overlay follows the initial saved disc; runtime CD-change requests remain
explicitly unsupported as before, rather than guessing their result.

- `make -C port -j2 all test`: all14 native suites plus logging checks pass.
- Synthetic signature-zero animation: base/intermediate pixels survive a
  cleared surface, and backward seek does not retain future pixels.
- Preferred-disc lookup test proves collision precedence while the base
  library still resolves its own version.
- Independent real-data reference: frames1/3/9/17 after10/50/180/350 draws,
  37500 actor pixels per sample, zero mismatches on both legacy and CD2 data.
  CD2 run durations approximately0.34/0.78/2.49/4.91 seconds, dummy SDL devices.
- Visual inspection: CD2 wide shot's baked background now aligns; close-up
  frame9 has a solid character without the reported holes. Generated files
  under `port/output/tk-animation-fix/` are ignored assets.
- Real TK hotspot/travel/conversation and inventory examine checks pass.
  `bash -n port/scene.sh` and `git diff --check` pass.

No new binary session or decompilation. Target and selected export hashes were
verified against decompile-index.json before interpreting original behavior:

| Export | SHA-256 |
| --- | --- |
| 0x43b130 | 529ef379c272785ef3c004cf974f7c697f8a59d5e8fe5df0ad87c2366c2b98b3 |
| 0x43c466 | 8fc43489cbdbe12b34fb437ee7f400523d4c36e316842123dc50828440bc22d1 |
| 0x40bbfd | e24b78d2eae4297cfacff33fa99cb661b7cd9519e4b1a087ebc7703debeb138d |
| 0x410078 | 1abee4b493e60ea7b6d399184ace0c50335d20be3f35f636fd47bccaac67bed9 |
| 0x40ffee | 16955dd55a53620bf9ba170ac2432c6fcc80c80d08424816cbcca9de2ad983cf |

These checks establish this rendering/data-selection correction, not complete
original-renderer equivalence or validation of every multi-disc asset collision.
