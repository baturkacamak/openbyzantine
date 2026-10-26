// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41dd22
// name: FUN_0041dd22
// ranges: 0x41dd22-0x41de4a
// calls: 0x408f32, 0x40c7b4, 0x41dd22, 0x424dc0, 0x428a16, 0x44ba00

undefined4 __cdecl FUN_0041dd22(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int local_1c;
  int local_14 [4];
  
  iVar3 = DAT_0045da78;
  local_14[0] = 3;
  local_14[1] = 4;
  local_14[2] = 5;
  local_14[3] = 6;
  iVar1 = FUN_0040c7b4(local_14[param_1]);
  if (iVar1 == 0) {
    if (param_2 != 0) {
      FUN_0044ba00((uint *)&DAT_00484e30,(uint *)s_quick_save_0045f93c);
      DAT_0045da78 = iVar3;
      iVar3 = FUN_00424dc0(s_savegame_qsave_000_0045f948);
      DAT_0045da78 = local_14[param_1];
      if (iVar3 != 0) {
        return 1;
      }
      DAT_0045f7e0 = 1;
    }
    FUN_00428a16();
    for (local_1c = 0; local_1c < 0x15e; local_1c = local_1c + 1) {
      if (*(int *)(&DAT_00486220 + local_1c * 4) != 0) {
        *(undefined4 *)(*(int *)(&DAT_00486220 + local_1c * 4) + 0x34) = 0;
      }
    }
    FUN_00408f32(0);
    DAT_0046066c = 0;
    DAT_00460668 = 0;
    DAT_004848a8 = 0;
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}
