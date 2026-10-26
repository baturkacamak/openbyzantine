// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4403b0
// name: FUN_004403b0
// ranges: 0x4403b0-0x440556
// calls: 0x4403b0

uint __cdecl FUN_004403b0(int *param_1)

{
  int iVar1;
  byte bVar2;
  undefined4 *puVar3;
  byte bVar4;
  uint uVar5;
  uint uVar6;
  byte *pbVar7;
  byte *pbVar8;
  int local_c;
  
  puVar3 = (undefined4 *)param_1[5];
  pbVar8 = (byte *)*puVar3;
  local_c = puVar3[1];
  if (local_c == 0) {
    uVar5 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar5 == '\0') {
      return uVar5 & 0xffffff00;
    }
    pbVar8 = (byte *)*puVar3;
    local_c = puVar3[1];
  }
  pbVar7 = pbVar8 + 1;
  local_c = local_c + -1;
  bVar4 = *pbVar8;
  if (local_c == 0) {
    uVar5 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar5 == '\0') {
      return uVar5 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    local_c = puVar3[1];
  }
  iVar1 = (uint)bVar4 * 0x100 + (uint)*pbVar7;
  do {
    iVar1 = iVar1 + -2;
    local_c = local_c + -1;
    pbVar8 = pbVar7 + 1;
    if (iVar1 < 1) {
      *puVar3 = pbVar8;
      puVar3[1] = local_c;
      return CONCAT31((int3)((uint)local_c >> 8),1);
    }
    if (local_c == 0) {
      uVar5 = (*(code *)puVar3[3])(param_1);
      if ((char)uVar5 == '\0') {
        return uVar5 & 0xffffff00;
      }
      pbVar8 = (byte *)*puVar3;
      local_c = puVar3[1];
    }
    pbVar7 = pbVar8 + 1;
    local_c = local_c + -1;
    bVar4 = *pbVar8;
    uVar5 = (uint)bVar4;
    if (local_c == 0) {
      uVar6 = (*(code *)puVar3[3])(param_1);
      if ((char)uVar6 == '\0') {
        return uVar6 & 0xffffff00;
      }
      pbVar7 = (byte *)*puVar3;
      local_c = puVar3[1];
    }
    bVar2 = *pbVar7;
    uVar6 = (uint)bVar2;
    *(undefined4 *)(*param_1 + 0x14) = 0x4e;
    *(uint *)(*param_1 + 0x18) = uVar5;
    *(uint *)(*param_1 + 0x1c) = uVar6;
    (**(code **)(*param_1 + 4))(param_1,1);
    if (0x1f < bVar4) {
      *(undefined4 *)(*param_1 + 0x14) = 0x1a;
      *(uint *)(*param_1 + 0x18) = uVar5;
      (**(code **)*param_1)(param_1);
    }
    if (bVar4 < 0x10) {
      *(byte *)((int)param_1 + uVar5 + 0xca) = bVar2 & 0xf;
      bVar4 = (byte)((int)uVar6 >> 4);
      *(byte *)((int)param_1 + uVar5 + 0xda) = bVar4;
      if (bVar4 < *(byte *)((int)param_1 + uVar5 + 0xca)) {
        *(undefined4 *)(*param_1 + 0x14) = 0x1b;
        *(uint *)(*param_1 + 0x18) = uVar6;
        (**(code **)*param_1)(param_1);
      }
    }
    else {
      *(byte *)((int)param_1 + uVar5 + 0xda) = bVar2;
    }
  } while( true );
}
