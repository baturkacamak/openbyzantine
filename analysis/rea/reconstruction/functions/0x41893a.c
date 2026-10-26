// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41893a
// name: FUN_0041893a
// ranges: 0x41893a-0x418a1d
// calls: 0x416220, 0x418040, 0x41809a, 0x41893a, 0x42b4be

void __cdecl FUN_0041893a(int param_1,int param_2)

{
  int iVar1;
  tagRECT local_34;
  ushort *local_24;
  ushort *local_20;
  undefined4 local_1c;
  int local_18;
  LONG local_14;
  LONG local_10;
  LONG local_c;
  LONG local_8;
  
  local_14 = 0;
  local_10 = 0x14;
  local_c = 0x280;
  local_8 = 0x1a4;
  local_1c = FUN_00418040(param_2);
  local_24 = (ushort *)(param_2 + 0x6400);
  local_20 = (ushort *)(param_1 + 0x6400);
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    FUN_0041809a(&local_34);
  }
  else {
    local_34.left = local_14;
    local_34.top = local_10;
    local_34.right = local_c;
    local_34.bottom = local_8;
  }
  for (local_18 = 2; local_18 < 9; local_18 = local_18 + 2) {
    FUN_0042b4be(local_20,local_24,local_18,0x3e800);
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_34,local_1c,&local_14,0x1000000,0);
  }
  return;
}
