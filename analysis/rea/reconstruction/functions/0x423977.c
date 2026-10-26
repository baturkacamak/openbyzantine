// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x423977
// name: FUN_00423977
// ranges: 0x423977-0x423a36
// calls: 0x423977

undefined4 FUN_00423977(int *param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_74 [9];
  undefined4 local_50;
  int local_8;
  
  if (param_1 == (int *)0x0) {
    uVar1 = 1;
  }
  else {
    _memset(local_74,0,0x6c);
    local_74[0] = 0x6c;
    local_8 = (**(code **)(*param_1 + 100))(param_1,0,local_74,1,0);
    if (local_8 == -0x7789fe3e) {
      local_8 = (**(code **)(*param_1 + 0x6c))(param_1);
    }
    if (local_8 == 0) {
      iVar2 = (**(code **)(*param_1 + 0x80))(param_1,local_50);
      if (iVar2 == 0) {
        *param_2 = local_50;
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  return uVar1;
}
