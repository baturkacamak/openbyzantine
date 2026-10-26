// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43194f
// name: FUN_0043194f
// ranges: 0x43194f-0x4319b2
// calls: 0x40ac38, 0x40ac43, 0x43194f
// port: Script call stack [bounded]

void __cdecl FUN_0043194f(int param_1)

{
  if (DAT_004848a8 < 0x18) {
    *(undefined4 *)(&DAT_0047a830 + DAT_004848a8 * 4) = *(undefined4 *)(param_1 + 0x2c);
    DAT_004848a8 = DAT_004848a8 + 1;
  }
  else {
    FUN_0040ac43();
    FUN_0040ac38();
  }
  return;
}
