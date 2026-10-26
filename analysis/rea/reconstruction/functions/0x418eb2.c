// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418eb2
// name: FUN_00418eb2
// ranges: 0x418eb2-0x41917a
// calls: 0x4066c6, 0x416220, 0x418040, 0x41809a, 0x418e2c, 0x418e6f, 0x418eb2, 0x42b280

void __cdecl FUN_00418eb2(int param_1,int param_2,void *param_3)

{
  int iVar1;
  undefined4 uVar2;
  DWORD DVar3;
  int local_90 [4];
  int *local_80;
  tagRECT local_7c;
  int local_6c [10];
  int local_44;
  int local_40;
  int local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  byte local_10;
  void *local_c;
  int local_8;
  
  local_38 = 0;
  local_34 = 0x14;
  local_30 = 0x280;
  local_2c = 0x1a4;
  local_90[0] = 0;
  local_90[1] = 0x14;
  local_90[2] = 0x280;
  local_90[3] = 0x1a4;
  local_6c[0] = 0;
  local_6c[1] = 0;
  local_6c[2] = 0x140;
  local_6c[3] = 200;
  local_20 = 0x140;
  local_1c = 0;
  local_18 = 0x280;
  local_14 = 200;
  local_10 = 1;
  local_24 = FUN_00418040(param_1);
  local_28 = FUN_00418040(param_2);
  local_80 = (int *)FUN_00418040((int)param_3);
  local_7c.left = 0;
  local_7c.top = 0x14;
  local_7c.right = 0x280;
  local_7c.bottom = 0x1a4;
  local_6c[4] = 0;
  local_6c[5] = 200;
  local_6c[6] = 0x140;
  local_6c[7] = 400;
  local_6c[9] = timeGetTime();
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    FUN_0041809a(&local_7c);
  }
  FUN_0042b280(param_3,0x1f400);
  FUN_00418e2c(local_6c,4);
  for (local_6c[8] = 0; local_6c[8] < 4; local_6c[8] = local_6c[8] + 1) {
    FUN_00418e2c(local_90,1);
    FUN_00418e6f(local_6c,1);
    (**(code **)(*local_80 + 0x14))(local_80,local_6c,local_28,&local_38,0x1000000,0);
    (**(code **)(*local_80 + 0x14))(local_80,&local_20,local_24,local_90,0x1000000,0);
    local_3c = (int)param_3 + 0x280;
    local_c = param_3;
    local_8 = (int)param_3 + 0x3e800;
    for (local_40 = 0; local_40 < 200; local_40 = local_40 + 1) {
      for (local_44 = 0; local_44 < 0x140; local_44 = local_44 + 1) {
        uVar2 = FUN_004066c6((uint)*(ushort *)((int)local_c + local_44 * 2),
                             (uint)*(ushort *)(local_3c + local_44 * 2),(uint)local_10);
        *(short *)(local_8 + local_44 * 2) = (short)uVar2;
      }
      local_3c = local_3c + 0x500;
      local_c = (void *)((int)local_c + 0x500);
      local_8 = local_8 + 0x500;
    }
    do {
      DVar3 = timeGetTime();
    } while (DVar3 < local_6c[9] + 0x3cU);
    local_6c[9] = timeGetTime();
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_7c,local_80,local_6c + 4,0x1000000,0);
    local_10 = local_10 << 2 | 0x11;
  }
  (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_7c,local_28,&local_38,0x1000000,0);
  return;
}
