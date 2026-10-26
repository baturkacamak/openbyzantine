// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408bb2
// name: FUN_00408bb2
// ranges: 0x408bb2-0x408c80
// calls: 0x408bb2, 0x40ac43

ushort __cdecl FUN_00408bb2(ushort param_1)

{
  ushort uVar1;
  char *_Str2;
  int iVar2;
  
  if (DAT_00482540 != 0) {
    _Str2 = (char *)**(undefined4 **)(&DAT_00485400 + (uint)param_1 * 4);
    for (uVar1 = 0; (uVar1 < 0x15e && ((&DAT_00482540)[uVar1] != 0)); uVar1 = uVar1 + 1) {
      iVar2 = __strcmpi((char *)(&DAT_00482540)[uVar1],_Str2);
      if (iVar2 == 0) {
        return uVar1;
      }
    }
    FUN_0040ac43();
    param_1 = 0;
  }
  return param_1;
}
