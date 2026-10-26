// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42c1b8
// name: FUN_0042c1b8
// ranges: 0x42c1b8-0x42c57d
// calls: 0x41842b, 0x42b2db, 0x42c1b8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0042c1b8(ushort *param_1,ushort *param_2,int param_3,int param_4)

{
  int *piVar1;
  ushort uVar2;
  int iVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  ushort *_Src;
  byte local_4c;
  int local_44;
  int local_3c;
  int local_38;
  int local_34;
  ushort *local_28;
  byte local_24;
  byte local_20;
  ushort *local_1c;
  uint local_14;
  int local_10;
  int local_8;
  
  local_24 = 0x1a - (char)_DAT_00485c78;
  local_4c = 0xf - (char)_DAT_00485c80;
  local_20 = 4 - (char)DAT_00485c8c;
  if (((param_1 == DAT_0047a8a4) || (param_1 == DAT_0046e82c)) || (param_1 == DAT_00477344)) {
    local_14 = 0x1e0;
    local_3c = 0x4b000;
  }
  else {
    local_14 = 400;
    local_3c = 0x3e800;
  }
  local_8 = 0;
  local_1c = param_1;
  local_28 = param_2;
  for (local_34 = 0; local_34 < (int)(local_14 / 2); local_34 = local_34 + 1) {
    for (local_38 = 0; local_38 < 0x140; local_38 = local_38 + 1) {
      uVar6 = *local_28;
      uVar4 = DAT_00485c7c & uVar6;
      uVar5 = DAT_00485c86 & uVar6;
      uVar6 = DAT_00485c84 & uVar6;
      uVar2 = *local_1c;
      *(uint *)(param_3 + local_8 * 4) =
           (uint)(DAT_00485c84 & uVar2) << (local_20 & 0x1f) |
           (uint)(DAT_00485c86 & uVar2) << (local_4c & 0x1f) |
           (uint)(DAT_00485c7c & uVar2) << (local_24 & 0x1f);
      *(int *)(param_3 + 0x4b000 + local_8 * 4) =
           (int)(((uint)uVar6 << (local_20 & 0x1f) | (uint)uVar5 << (local_4c & 0x1f) |
                  (uint)uVar4 << (local_24 & 0x1f) | 0x2004000) - *(int *)(param_3 + local_8 * 4))
           >> 3;
      local_8 = local_8 + 1;
      local_1c = local_1c + 2;
      local_28 = local_28 + 2;
    }
    local_1c = local_1c + 0x280;
    local_28 = local_28 + 0x280;
  }
  for (local_44 = 0; local_44 < 8; local_44 = local_44 + 1) {
    local_8 = 0;
    local_10 = 0;
    for (local_38 = 0; local_38 < (int)local_14; local_38 = local_38 + 2) {
      _Src = param_1 + local_8;
      for (local_34 = 0; local_34 < 0x280; local_34 = local_34 + 2) {
        piVar1 = (int *)(param_3 + local_10 * 4);
        *piVar1 = *piVar1 + *(int *)(param_3 + 0x4b000 + local_10 * 4);
        iVar3 = *(int *)(param_3 + local_10 * 4);
        uVar6 = (ushort)(iVar3 >> (local_20 & 0x1f)) & DAT_00485c84 |
                (ushort)(iVar3 >> (local_4c & 0x1f)) & DAT_00485c86 |
                (ushort)(iVar3 >> (local_24 & 0x1f)) & DAT_00485c7c;
        param_1[local_8] = uVar6;
        param_1[local_8 + 1] = uVar6;
        local_8 = local_8 + 2;
        local_10 = local_10 + 1;
      }
      if (param_4 == 0) {
        FID_conflict__memcpy(param_1 + local_8,_Src,0x500);
      }
      else {
        _memset(param_1 + local_8,0,0x500);
      }
      local_8 = local_8 + 0x280;
    }
    FUN_0041842b((int)param_1,local_3c);
  }
  if (param_4 == 0) {
    FUN_0042b2db(param_1,param_2,local_3c);
    FUN_0041842b((int)param_1,local_3c);
  }
  return;
}
