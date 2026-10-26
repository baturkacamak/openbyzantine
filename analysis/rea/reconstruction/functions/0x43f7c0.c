// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43f7c0
// name: FUN_0043f7c0
// ranges: 0x43f7c0-0x43f86e
// calls: 0x43f7c0

uint __cdecl FUN_0043f7c0(int *param_1)

{
  int iVar1;
  byte bVar2;
  byte bVar3;
  undefined4 *puVar4;
  uint uVar5;
  undefined4 uVar6;
  byte *pbVar7;
  int iVar8;
  
  puVar4 = (undefined4 *)param_1[5];
  pbVar7 = (byte *)*puVar4;
  iVar8 = puVar4[1];
  if (iVar8 == 0) {
    uVar5 = (*(code *)puVar4[3])(param_1);
    if ((char)uVar5 == '\0') {
      return uVar5 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar4;
    iVar8 = puVar4[1];
  }
  iVar8 = iVar8 + -1;
  bVar2 = *pbVar7;
  pbVar7 = pbVar7 + 1;
  if (iVar8 == 0) {
    uVar5 = (*(code *)puVar4[3])(param_1);
    if ((char)uVar5 == '\0') {
      return uVar5 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar4;
    iVar8 = puVar4[1];
  }
  bVar3 = *pbVar7;
  *(undefined4 *)(*param_1 + 0x14) = 0x5a;
  iVar1 = (uint)bVar2 * 0x100 + (uint)bVar3;
  *(int *)(*param_1 + 0x18) = param_1[0x5e];
  *(int *)(*param_1 + 0x1c) = iVar1;
  (**(code **)(*param_1 + 4))(param_1,1);
  *puVar4 = pbVar7 + 1;
  puVar4[1] = iVar8 + -1;
  uVar6 = (**(code **)(param_1[5] + 0x10))(param_1,iVar1 + -2);
  return CONCAT31((int3)((uint)uVar6 >> 8),1);
}
