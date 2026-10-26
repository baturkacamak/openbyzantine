// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4181c5
// name: FUN_004181c5
// ranges: 0x4181c5-0x4183a9
// calls: 0x40e627, 0x40e7ed, 0x40e828, 0x4161f6, 0x416220, 0x41624a, 0x4181c5

void __cdecl FUN_004181c5(int *param_1,int *param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  tagRECT local_3c;
  int local_2c;
  int local_28;
  int local_24;
  undefined4 local_20;
  int local_1c;
  int local_18 [4];
  HWND local_8;
  
  local_1c = 0;
  local_2c = 0;
  local_24 = *param_2 - param_3;
  local_28 = param_2[1] - param_4;
  FUN_0041624a();
  if (DAT_0045dbd0 == 1) {
    DAT_0045dbd0 = 0;
    local_1c = 1;
    iVar4 = FUN_0040e828(param_3,param_4,param_2[2] - *param_2,param_2[3] - param_2[1]);
    if (iVar4 != 0) {
      FUN_0040e627(param_1,&local_24,&local_28,local_18);
      local_2c = 1;
    }
  }
  iVar4 = FUN_00416220();
  if (iVar4 == 0) {
    iVar4 = param_2[2];
    iVar1 = *param_2;
    iVar2 = param_2[3];
    iVar3 = param_2[1];
    local_8 = (HWND)FUN_004161f6();
    GetClientRect(local_8,&local_3c);
    ClientToScreen(local_8,(LPPOINT)&local_3c);
    local_3c.top = local_3c.top + param_4;
    local_3c.left = local_3c.left + param_3;
    local_3c.right = local_3c.left + (iVar4 - iVar1);
    local_3c.bottom = local_3c.top + (iVar2 - iVar3);
    local_20 = (**(code **)(*DAT_004869d4 + 0x14))
                         (DAT_004869d4,&local_3c,param_1,param_2,0x1000000,0);
  }
  else if (DAT_0045f070 == '\0') {
    local_20 = (**(code **)(*DAT_004869d4 + 0x1c))
                         (DAT_004869d4,param_3,param_4,param_1,param_2,0x10);
  }
  else {
    local_20 = (**(code **)(*DAT_004869d4 + 0x2c))(DAT_004869d4,0,1);
    DAT_0045f2d8 = DAT_0045f2d8 + 1;
  }
  if (local_1c != 0) {
    if (local_2c != 0) {
      FUN_0040e7ed(param_1,local_24,local_28,local_18);
    }
    DAT_0045dbd0 = 1;
  }
  FUN_0041624a();
  return;
}
