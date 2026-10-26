// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4164a0
// name: FUN_004164a0
// ranges: 0x4164a0-0x4164f3
// calls: 0x4164a0
// port: RGB555 conversion [partial]

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort __cdecl FUN_004164a0(ushort param_1)

{
  if (_DAT_00485c78 != 10) {
    param_1 = param_1 & 0x1f | (param_1 & 0xffe0) * 2;
  }
  return param_1;
}
