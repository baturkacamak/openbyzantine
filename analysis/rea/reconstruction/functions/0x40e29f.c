// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40e29f
// name: FUN_0040e29f
// ranges: 0x40e29f-0x40e2da; 0x40e2e0-0x40e40e
// calls: 0x40e29f, 0x4161f6, 0x416220

void FUN_0040e29f(void)

{
  int iVar1;
  HWND hWnd;
  tagPOINT *lpPoint;
  tagPOINT local_34;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  
  local_1c = DAT_0046c448;
  local_24 = DAT_0046c44c;
  local_18 = DAT_0046c450;
  local_14 = DAT_0046c454;
  local_10 = DAT_0046c458;
  local_c = DAT_0046c45c;
  if (DAT_0046c448 < 0) {
    local_18 = DAT_0046c450 - DAT_0046c448;
    local_1c = 0;
  }
  else {
    local_20 = DAT_0046c458 + DAT_0046c448 + -0x280;
    if (0 < local_20) {
      local_10 = DAT_0046c458 - local_20;
    }
  }
  if (DAT_0046c44c < 0) {
    local_14 = DAT_0046c454 - DAT_0046c44c;
    local_24 = 0;
  }
  else {
    local_20 = DAT_0046c45c + DAT_0046c44c + -0x1e0;
    if (0 < local_20) {
      local_c = DAT_0046c45c - local_20;
    }
  }
  local_34.x = local_1c;
  local_34.y = local_24;
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    lpPoint = &local_34;
    hWnd = (HWND)FUN_004161f6();
    ClientToScreen(hWnd,lpPoint);
  }
  local_2c = (local_10 - local_18) + local_34.x;
  local_28 = (local_c - local_14) + local_34.y;
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    (**(code **)(*DAT_0045dbd8 + 0x14))(DAT_0045dbd8,&local_18,DAT_004869d4,&local_34,0x1000000,0);
  }
  else {
    (**(code **)(*DAT_0045dbd8 + 0x1c))(DAT_0045dbd8,local_18,local_14,DAT_004869d4,&local_34,0x10);
  }
  return;
}
