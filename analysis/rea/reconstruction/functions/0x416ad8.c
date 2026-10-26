// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x416ad8
// name: FUN_00416ad8
// ranges: 0x416ad8-0x416b4a
// calls: 0x416ad8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort __cdecl FUN_00416ad8(ushort param_1)

{
  return param_1 & 0x1f | param_1 >> ((char)_DAT_00485c80 - 5U & 0x1f) & 0x3e0 |
         param_1 >> ((char)_DAT_00485c78 - 10U & 0x1f) & 0x7c00;
}
