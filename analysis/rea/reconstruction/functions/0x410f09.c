// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x410f09
// name: FUN_00410f09
// ranges: 0x410f09-0x410fed
// calls: 0x40ac38, 0x40ac43, 0x410f09

ushort __cdecl FUN_00410f09(ushort param_1,char *param_2)

{
  int iVar1;
  
  if (DAT_00485a10 != 0) {
    param_1 = 0;
    while (((param_1 < 0x96 && ((&DAT_00485a10)[param_1] != 0)) &&
           (iVar1 = __strcmpi((char *)(&DAT_00485a10)[param_1],param_2), iVar1 != 0))) {
      param_1 = param_1 + 1;
    }
    if ((param_1 == 0x96) || ((&DAT_00485a10)[param_1] == 0)) {
      FUN_0040ac43();
      FUN_0040ac38();
      param_1 = 0;
    }
  }
  return param_1;
}
