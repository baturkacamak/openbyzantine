// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x433309
// name: FUN_00433309
// ranges: 0x433309-0x433383
// calls: 0x408bb2, 0x433309, 0x436652

bool __cdecl FUN_00433309(int param_1)

{
  ushort uVar1;
  ushort uVar2;
  
  uVar1 = FUN_00436652(param_1,0);
  uVar1 = FUN_00408bb2(uVar1);
  uVar2 = FUN_00436652(param_1,1);
  return **(byte **)(&DAT_00486f90 + (uint)uVar1 * 4) == uVar2;
}
