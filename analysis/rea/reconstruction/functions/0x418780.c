// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418780
// name: FUN_00418780
// ranges: 0x418780-0x418939
// calls: 0x416220, 0x418040, 0x41809a, 0x418570, 0x418598, 0x418780

void __cdecl FUN_00418780(int param_1,int param_2,int param_3)

{
  undefined2 *puVar1;
  int iVar2;
  bool bVar3;
  tagRECT local_4c;
  int *local_3c;
  undefined2 *local_38;
  undefined4 local_34;
  int local_30;
  uint local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  int local_18 [4];
  int *local_8;
  
  local_4c.left = 0;
  local_4c.top = 0x14;
  local_4c.right = 0x280;
  local_4c.bottom = 0x1a4;
  local_28 = 0;
  local_24 = 0x14;
  local_20 = 0x280;
  local_1c = 0x1a4;
  local_18[0] = 0;
  local_18[1] = 0x14;
  local_18[2] = 0x280;
  local_18[3] = 0x1a4;
  local_8 = local_18;
  local_34 = FUN_00418040(param_1);
  local_3c = (int *)FUN_00418040(param_3);
  local_38 = (undefined2 *)(param_3 + 0x6400);
  puVar1 = (undefined2 *)(param_2 + 0x6400);
  bVar3 = DAT_0045dbd0 != 0;
  if (bVar3) {
    DAT_0045dbd0 = 0;
  }
  local_2c = (uint)bVar3;
  iVar2 = FUN_00416220();
  if (iVar2 == 0) {
    FUN_0041809a(&local_4c);
  }
  for (local_30 = 0; local_30 < 0x1b; local_30 = local_30 + 1) {
    if (local_30 < 0xe) {
      local_8[1] = local_8[1] + 3;
      local_8[3] = local_8[3] + -3;
      *local_8 = *local_8 + 3;
      local_8[2] = local_8[2] + -3;
      (**(code **)(*local_3c + 0x14))(local_3c,&local_28,local_34,local_8,0x1000000,0);
      local_30 = local_30 + 1;
      FUN_00418598(local_38,puVar1,local_30);
    }
    else {
      FUN_00418570(local_38,puVar1,local_30);
      local_30 = local_30 + 1;
      FUN_00418570(local_38,puVar1,local_30);
    }
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_4c,local_3c,&local_28,0x1000000,0);
  }
  if (local_2c != 0) {
    DAT_0045dbd0 = 1;
  }
  return;
}
