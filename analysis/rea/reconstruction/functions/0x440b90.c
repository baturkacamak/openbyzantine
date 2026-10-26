// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x440b90
// name: FUN_00440b90
// ranges: 0x440b90-0x440c96
// calls: 0x440b90

uint __cdecl FUN_00440b90(int *param_1)

{
  int iVar1;
  byte bVar2;
  undefined4 *puVar3;
  uint uVar4;
  undefined4 uVar5;
  byte *pbVar6;
  byte *pbVar7;
  int iVar8;
  
  puVar3 = (undefined4 *)param_1[5];
  pbVar6 = (byte *)*puVar3;
  iVar8 = puVar3[1];
  if (iVar8 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar6 = (byte *)*puVar3;
    iVar8 = puVar3[1];
  }
  iVar8 = iVar8 + -1;
  bVar2 = *pbVar6;
  pbVar6 = pbVar6 + 1;
  if (iVar8 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar6 = (byte *)*puVar3;
    iVar8 = puVar3[1];
  }
  iVar8 = iVar8 + -1;
  pbVar7 = pbVar6 + 1;
  if ((uint)bVar2 * 0x100 + (uint)*pbVar6 != 4) {
    *(undefined4 *)(*param_1 + 0x14) = 9;
    (**(code **)*param_1)(param_1);
  }
  if (iVar8 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar8 = puVar3[1];
  }
  iVar8 = iVar8 + -1;
  bVar2 = *pbVar7;
  pbVar7 = pbVar7 + 1;
  if (iVar8 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar8 = puVar3[1];
  }
  iVar1 = (uint)bVar2 * 0x100 + (uint)*pbVar7;
  *(undefined4 *)(*param_1 + 0x14) = 0x51;
  *(int *)(*param_1 + 0x18) = iVar1;
  uVar5 = (**(code **)(*param_1 + 4))(param_1,1);
  param_1[0x3f] = iVar1;
  *puVar3 = pbVar7 + 1;
  puVar3[1] = iVar8 + -1;
  return CONCAT31((int3)((uint)uVar5 >> 8),1);
}
