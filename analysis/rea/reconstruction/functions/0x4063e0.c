// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4063e0
// name: FUN_004063e0
// ranges: 0x4063e0-0x4066c5
// calls: 0x4061e7, 0x4062bc, 0x406358, 0x4063e0, 0x4069a4

int __cdecl
FUN_004063e0(char *param_1,int param_2,int param_3,int param_4,int *param_5,int param_6,int param_7,
            int param_8)

{
  byte *pbVar1;
  int iVar2;
  int local_a0;
  undefined4 local_98 [4];
  int local_88;
  int local_74;
  int local_2c;
  undefined4 local_28;
  int local_24;
  uint local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_24 = 0;
  local_1c = 0;
  local_2c = 0x12;
  local_18 = param_2;
  if (param_1 == (char *)0x0) {
    iVar2 = 0;
  }
  else if (*(int *)(&DAT_00487510 + param_4 * 0x48) == 0) {
    iVar2 = 0;
  }
  else {
    if (param_7 == 1) {
      iVar2 = FUN_004061e7((int)param_1,param_4);
      param_2 = param_2 - (iVar2 >> 1);
    }
    else if ((param_7 == 2) || (param_7 == 4)) {
      iVar2 = FUN_004062bc(param_1,param_4,1);
      param_2 = param_2 - iVar2;
    }
    if ((param_7 == 3) || (param_7 == 4)) {
      local_2c = 0;
    }
    local_c = param_2;
    local_a0 = 1;
    if (param_2 < 0) {
      iVar2 = 0;
    }
    else {
      _memset(local_98,0,0x6c);
      local_98[0] = 0x6c;
      iVar2 = (**(code **)(*param_5 + 100))(param_5,0,local_98,1,0);
      if (iVar2 == 0) {
        local_10 = local_74;
        local_14 = local_88 >> 1;
        local_28 = 0;
        while (param_1[local_24] != '\0') {
          if (param_1[local_24] == '\n') {
            local_24 = local_24 + 1;
            param_3 = param_3 + (uint)*(ushort *)((&DAT_00487518)[param_4 * 0x12] + 6);
            if (local_1c < param_2 - local_c) {
              local_1c = param_2 - local_c;
            }
            local_a0 = local_a0 + 1;
            if (param_8 < local_a0) break;
            if ((param_7 == 2) || (param_7 == 4)) {
              iVar2 = FUN_004062bc(param_1,param_4,local_a0);
              param_2 = (local_18 - iVar2) - local_2c;
            }
            else {
              param_2 = local_2c + local_c;
            }
          }
          else {
            local_8 = FUN_00406358(param_1[local_24],param_4);
            if (0 < local_8) {
              param_2 = param_2 - local_8;
              local_20 = local_20 - local_8;
            }
            pbVar1 = (byte *)(param_1 + local_24);
            local_24 = local_24 + 1;
            local_20 = FUN_004069a4(*pbVar1,param_2,param_3,param_4,local_10,param_6,local_14);
            if (local_8 < 0) {
              local_20 = local_20 + local_8;
            }
            param_2 = param_2 + local_20;
          }
        }
        (**(code **)(*param_5 + 0x80))(param_5,0);
        iVar2 = param_2 - local_c;
        if (param_2 - local_c <= local_1c) {
          iVar2 = local_1c;
        }
      }
      else {
        iVar2 = 0;
      }
    }
  }
  return iVar2;
}
