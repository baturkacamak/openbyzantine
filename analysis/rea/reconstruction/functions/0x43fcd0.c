// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43fcd0
// name: FUN_0043fcd0
// ranges: 0x43fcd0-0x44001b
// calls: 0x43fcd0

uint __cdecl FUN_0043fcd0(int *param_1,undefined1 param_2,undefined1 param_3)

{
  byte bVar1;
  byte bVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  byte *pbVar7;
  byte *pbVar8;
  uint *puStack_8;
  uint local_4;
  
  puVar3 = (undefined4 *)param_1[5];
  pbVar7 = (byte *)*puVar3;
  iVar6 = puVar3[1];
  *(undefined1 *)(param_1 + 0x32) = param_2;
  *(undefined1 *)((int)param_1 + 0xc9) = param_3;
  if (iVar6 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar6 = puVar3[1];
  }
  iVar6 = iVar6 + -1;
  bVar1 = *pbVar7;
  pbVar7 = pbVar7 + 1;
  if (iVar6 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar6 = puVar3[1];
  }
  iVar6 = iVar6 + -1;
  bVar2 = *pbVar7;
  pbVar7 = pbVar7 + 1;
  if (iVar6 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar6 = puVar3[1];
  }
  iVar6 = iVar6 + -1;
  pbVar8 = pbVar7 + 1;
  param_1[0x30] = (uint)*pbVar7;
  if (iVar6 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar8 = (byte *)*puVar3;
    iVar6 = puVar3[1];
  }
  iVar6 = iVar6 + -1;
  pbVar7 = pbVar8 + 1;
  param_1[7] = (uint)*pbVar8 << 8;
  if (iVar6 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar6 = puVar3[1];
  }
  iVar6 = iVar6 + -1;
  pbVar8 = pbVar7 + 1;
  param_1[7] = param_1[7] + (uint)*pbVar7;
  if (iVar6 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar8 = (byte *)*puVar3;
    iVar6 = puVar3[1];
  }
  iVar6 = iVar6 + -1;
  pbVar7 = pbVar8 + 1;
  param_1[6] = (uint)*pbVar8 << 8;
  if (iVar6 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar6 = puVar3[1];
  }
  iVar6 = iVar6 + -1;
  pbVar8 = pbVar7 + 1;
  param_1[6] = param_1[6] + (uint)*pbVar7;
  if (iVar6 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar8 = (byte *)*puVar3;
    iVar6 = puVar3[1];
  }
  iVar6 = iVar6 + -1;
  pbVar7 = pbVar8 + 1;
  iVar5 = *param_1;
  param_1[8] = (uint)*pbVar8;
  *(int *)(iVar5 + 0x18) = param_1[0x5e];
  *(int *)(iVar5 + 0x1c) = param_1[6];
  *(int *)(iVar5 + 0x20) = param_1[7];
  *(int *)(iVar5 + 0x24) = param_1[8];
  *(undefined4 *)(*param_1 + 0x14) = 99;
  (**(code **)(*param_1 + 4))(param_1,1);
  if (*(char *)(param_1[100] + 0x51) != '\0') {
    *(undefined4 *)(*param_1 + 0x14) = 0x39;
    (**(code **)*param_1)(param_1);
  }
  if (((param_1[7] == 0) || (param_1[6] == 0)) || (param_1[8] < 1)) {
    *(undefined4 *)(*param_1 + 0x14) = 0x1f;
    (**(code **)*param_1)(param_1);
  }
  if (param_1[8] * 3 != (uint)bVar1 * 0x100 + (uint)bVar2 + -8) {
    *(undefined4 *)(*param_1 + 0x14) = 9;
    (**(code **)*param_1)(param_1);
  }
  if (param_1[0x31] == 0) {
    iVar5 = (**(code **)param_1[1])(param_1,1,param_1[8] * 0x54);
    param_1[0x31] = iVar5;
  }
  local_4 = 0;
  puStack_8 = (uint *)param_1[0x31];
  if (0 < param_1[8]) {
    do {
      puStack_8[1] = local_4;
      if (iVar6 == 0) {
        uVar4 = (*(code *)puVar3[3])(param_1);
        if ((char)uVar4 == '\0') {
          return uVar4 & 0xffffff00;
        }
        pbVar7 = (byte *)*puVar3;
        iVar6 = puVar3[1];
      }
      iVar6 = iVar6 + -1;
      pbVar8 = pbVar7 + 1;
      *puStack_8 = (uint)*pbVar7;
      if (iVar6 == 0) {
        uVar4 = (*(code *)puVar3[3])(param_1);
        if ((char)uVar4 == '\0') {
          return uVar4 & 0xffffff00;
        }
        pbVar8 = (byte *)*puVar3;
        iVar6 = puVar3[1];
      }
      iVar6 = iVar6 + -1;
      bVar1 = *pbVar8;
      pbVar8 = pbVar8 + 1;
      puStack_8[2] = (uint)(bVar1 >> 4);
      puStack_8[3] = bVar1 & 0xf;
      if (iVar6 == 0) {
        uVar4 = (*(code *)puVar3[3])(param_1);
        if ((char)uVar4 == '\0') {
          return uVar4 & 0xffffff00;
        }
        pbVar8 = (byte *)*puVar3;
        iVar6 = puVar3[1];
      }
      iVar6 = iVar6 + -1;
      pbVar7 = pbVar8 + 1;
      puStack_8[4] = (uint)*pbVar8;
      iVar5 = *param_1;
      *(uint *)(iVar5 + 0x18) = *puStack_8;
      *(uint *)(iVar5 + 0x1c) = puStack_8[2];
      *(uint *)(iVar5 + 0x20) = puStack_8[3];
      *(uint *)(iVar5 + 0x24) = puStack_8[4];
      *(undefined4 *)(*param_1 + 0x14) = 100;
      (**(code **)(*param_1 + 4))(param_1,1);
      puStack_8 = puStack_8 + 0x15;
      local_4 = local_4 + 1;
    } while ((int)local_4 < param_1[8]);
  }
  iVar5 = param_1[100];
  *(undefined1 *)(iVar5 + 0x51) = 1;
  *puVar3 = pbVar7;
  puVar3[1] = iVar6;
  return CONCAT31((int3)((uint)iVar5 >> 8),1);
}
