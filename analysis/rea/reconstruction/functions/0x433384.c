// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x433384
// name: FUN_00433384
// ranges: 0x433384-0x433409
// calls: 0x408bb2, 0x433384, 0x436652

undefined4 __cdecl FUN_00433384(int param_1)

{
  ushort uVar1;
  undefined4 uVar2;
  uint local_c;
  
  uVar1 = FUN_00436652(param_1,0);
  uVar1 = FUN_00408bb2(uVar1);
  local_c = (uint)uVar1;
  if (((int)*(char *)(*(int *)(&DAT_00486f90 + local_c * 4) + 4) == (uint)DAT_00476338) ||
     ((int)*(char *)(*(int *)(&DAT_00486f90 + local_c * 4) + 4) + 1U == (uint)DAT_00476338)) {
    uVar2 = 1;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}
