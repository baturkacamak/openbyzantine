// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4185e3
// name: FUN_004185e3
// ranges: 0x4185e3-0x41877f
// calls: 0x416220, 0x41624a, 0x418040, 0x41809a, 0x418570, 0x4185e3

void FUN_004185e3(void)

{
  undefined4 uVar1;
  undefined2 *puVar2;
  int iVar3;
  DWORD DVar4;
  bool bVar5;
  tagRECT local_38;
  undefined2 *local_28;
  DWORD local_24;
  int local_20;
  uint local_1c;
  int local_18 [4];
  int *local_8;
  
  local_38.left = 0;
  local_38.top = 0x14;
  local_38.right = 0x280;
  local_38.bottom = 0x1a4;
  local_18[0] = 0;
  local_18[1] = 0x14;
  local_18[2] = 0x280;
  local_18[3] = 0x1a4;
  local_8 = local_18;
  local_24 = timeGetTime();
  uVar1 = FUN_00418040(DAT_0047a8a4);
  local_28 = (undefined2 *)(DAT_0047a8a4 + 0x6400);
  puVar2 = (undefined2 *)(DAT_0046e82c + 0x6400);
  bVar5 = DAT_0045dbd0 != 0;
  if (bVar5) {
    DAT_0045dbd0 = 0;
  }
  local_1c = (uint)bVar5;
  iVar3 = FUN_00416220();
  if (iVar3 == 0) {
    FUN_0041809a(&local_38);
  }
  for (local_20 = 0; local_20 < 0x1b; local_20 = local_20 + 1) {
    while( true ) {
      DVar4 = timeGetTime();
      if (local_24 + 0xd <= DVar4) break;
      FUN_0041624a();
    }
    local_24 = timeGetTime();
    if (local_20 < 0xc) {
      local_8[1] = local_8[1] + 3;
      local_8[3] = local_8[3] + -3;
      *local_8 = *local_8 + 3;
      local_8[2] = local_8[2] + -3;
    }
    else if (local_20 == 0xc) {
      local_8[1] = local_8[1] + -0x24;
      local_8[3] = local_8[3] + 0x24;
      *local_8 = *local_8 + -0x24;
      local_8[2] = local_8[2] + 0x24;
    }
    FUN_00418570(local_28,puVar2,local_20);
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_38,uVar1,local_8,0x1000000,0);
  }
  if (local_1c != 0) {
    DAT_0045dbd0 = 1;
  }
  return;
}
