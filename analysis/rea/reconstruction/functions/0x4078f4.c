// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4078f4
// name: FUN_004078f4
// ranges: 0x4078f4-0x407932
// calls: 0x4078f4

ushort __cdecl FUN_004078f4(short param_1,int *param_2)

{
  if (param_2 != (int *)0x0) {
    *param_2 = (int)(short)(param_1 - 0x20U) >> 1;
  }
  return param_1 - 0x20U & 1;
}
