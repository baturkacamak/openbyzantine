// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420683
// name: FUN_00420683
// ranges: 0x420683-0x4206d5
// calls: 0x418040, 0x420683

void __cdecl FUN_00420683(undefined4 *param_1)

{
  int *piVar1;
  undefined4 uVar2;
  
  piVar1 = (int *)FUN_00418040(DAT_00477344);
  uVar2 = FUN_00418040(DAT_0047a8a4);
  (**(code **)(*piVar1 + 0x1c))(piVar1,*param_1,param_1[1],uVar2,param_1,0x10);
  return;
}
