// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41842b
// name: FUN_0041842b
// ranges: 0x41842b-0x41856f
// calls: 0x40d9f7, 0x40da1f, 0x4161f6, 0x416220, 0x418040, 0x4183aa, 0x41842b

void __cdecl FUN_0041842b(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  tagRECT local_20;
  undefined4 local_10;
  int local_c;
  HWND local_8;
  
  local_c = 0;
  if (param_2 == 0x4b000) {
    uVar1 = FUN_00418040(param_1);
    if (DAT_0045dbd0 != 0) {
      FUN_0040d9f7();
      local_c = 1;
    }
    if (DAT_0045f070 == '\0') {
      iVar2 = FUN_00416220();
      if (iVar2 == 0) {
        local_8 = (HWND)FUN_004161f6();
        GetClientRect(local_8,&local_20);
        ClientToScreen(local_8,(LPPOINT)&local_20);
        local_20.right = local_20.left + 0x280;
        local_20.bottom = local_20.top + 0x1e0;
        local_10 = (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_20,uVar1,0,0x1000000,0);
      }
      else {
        local_10 = (**(code **)(*DAT_004869d4 + 0x1c))(DAT_004869d4,0,0,uVar1,0,0x10);
      }
    }
    else {
      local_10 = (**(code **)(*DAT_004869d4 + 0x2c))(DAT_004869d4,0,1);
    }
    if (local_c != 0) {
      FUN_0040da1f();
    }
  }
  else if (param_2 == 0x3e800) {
    FUN_004183aa(0,0x14,param_1,0,0x14,0x280,400);
  }
  return;
}
