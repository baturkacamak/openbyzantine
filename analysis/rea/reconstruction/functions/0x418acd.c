// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418acd
// name: FUN_00418acd
// ranges: 0x418acd-0x418bce
// calls: 0x416220, 0x41624a, 0x418040, 0x41809a, 0x418acd

void FUN_00418acd(void)

{
  int iVar1;
  DWORD DVar2;
  tagRECT local_34;
  DWORD local_24;
  int local_20;
  undefined4 local_1c;
  int local_18 [4];
  int *local_8;
  
  local_1c = FUN_00418040(DAT_0047a8a4);
  local_34.left = 0;
  local_34.top = 0x14;
  local_34.right = 0x280;
  local_34.bottom = 0x1a4;
  local_18[0] = 0;
  local_18[1] = 0x14;
  local_18[2] = 0x280;
  local_18[3] = 0x1a4;
  local_8 = local_18;
  local_24 = timeGetTime();
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    FUN_0041809a(&local_34);
  }
  for (local_20 = 0; local_20 < 0x11; local_20 = local_20 + 1) {
    while( true ) {
      DVar2 = timeGetTime();
      if (local_24 + 0xd <= DVar2) break;
      FUN_0041624a();
    }
    local_24 = timeGetTime();
    local_8[1] = local_8[1] + 3;
    local_8[3] = local_8[3] + -3;
    *local_8 = *local_8 + 3;
    local_8[2] = local_8[2] + -3;
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_34,local_1c,local_8,0x1000000,0);
  }
  return;
}
