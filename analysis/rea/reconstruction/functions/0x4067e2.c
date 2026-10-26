// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4067e2
// name: FUN_004067e2
// ranges: 0x4067e2-0x4069a3
// calls: 0x4066c6, 0x4067e2

void __cdecl FUN_004067e2(int *param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 local_34;
  int local_28;
  int local_20;
  int local_18;
  int local_14;
  int local_10;
  uint local_8;
  
  iVar1 = param_1[2];
  iVar2 = *param_1;
  iVar3 = param_1[3];
  iVar4 = param_1[1];
  local_18 = param_1[1] * param_1[5] + *param_1;
  local_14 = param_1[9] + (param_1[7] * param_1[4] + param_1[6]) * 2;
  local_10 = param_1[10] + local_18;
  if (param_1[0xb] == 0) {
    local_18 = 0;
  }
  else {
    local_18 = param_1[0xb] + local_18;
  }
  for (local_20 = 0; local_20 < iVar3 - iVar4; local_20 = local_20 + 1) {
    for (local_28 = 0; local_28 < iVar1 - iVar2; local_28 = local_28 + 1) {
      uVar6 = (uint)*(byte *)(local_28 + local_10);
      local_34 = CONCAT22(local_34._2_2_,*(undefined2 *)(local_14 + local_28 * 2));
      if (local_18 == 0) {
        local_8 = 0;
      }
      else {
        local_8 = (uint)*(byte *)(local_28 + local_18);
        if (local_8 == 0xff) {
          local_34 = (uint)local_34._2_2_ << 0x10;
        }
      }
      if (uVar6 == 0xff) {
        *(undefined2 *)(local_14 + local_28 * 2) = (undefined2)param_2;
      }
      else if ((uVar6 != 0) || (local_8 != 0)) {
        if (local_18 != 0) {
          uVar5 = FUN_004066c6(0,local_34,local_8);
          local_34 = CONCAT22(local_34._2_2_,(short)uVar5);
        }
        uVar5 = FUN_004066c6(param_2,local_34,uVar6);
        *(short *)(local_14 + local_28 * 2) = (short)uVar5;
      }
    }
    local_14 = local_14 + param_1[4] * 2;
    local_10 = local_10 + param_1[5];
    if (local_18 != 0) {
      local_18 = local_18 + param_1[5];
    }
  }
  return;
}
