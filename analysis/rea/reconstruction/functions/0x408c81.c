// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408c81
// name: FUN_00408c81
// ranges: 0x408c81-0x408d3e
// calls: 0x408c81, 0x40ac43

uint __cdecl FUN_00408c81(uint param_1)

{
  char *_Str2;
  uint uVar1;
  int iVar2;
  uint local_8;
  
  if (DAT_0047a8b0 == 0) {
    uVar1 = param_1 & 0xffff;
  }
  else {
    _Str2 = (char *)**(undefined4 **)(&DAT_0047a2b0 + (param_1 & 0xffff) * 4);
    for (local_8 = 0; ((int)local_8 < 10 && ((&DAT_0047a8b0)[local_8] != 0)); local_8 = local_8 + 1)
    {
      iVar2 = __strcmpi((char *)(&DAT_0047a8b0)[local_8],_Str2);
      if (iVar2 == 0) {
        return local_8;
      }
    }
    FUN_0040ac43();
    uVar1 = 0;
  }
  return uVar1;
}
