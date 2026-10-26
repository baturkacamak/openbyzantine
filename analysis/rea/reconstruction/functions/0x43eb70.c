// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43eb70
// name: FUN_0043eb70
// ranges: 0x43eb70-0x43ecfb
// calls: 0x43eb70, 0x442220

uint __cdecl FUN_0043eb70(int *param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  iVar3 = param_1[0x48];
  if (iVar3 == 1) {
    iVar3 = param_1[0x49];
    param_1[0x4d] = *(int *)(iVar3 + 0x1c);
    param_1[0x4e] = *(int *)(iVar3 + 0x20);
    *(undefined4 *)(iVar3 + 0x34) = 1;
    *(undefined4 *)(iVar3 + 0x38) = 1;
    *(undefined4 *)(iVar3 + 0x3c) = 1;
    *(undefined4 *)(iVar3 + 0x40) = *(undefined4 *)(iVar3 + 0x24);
    *(undefined4 *)(iVar3 + 0x44) = 1;
    uVar4 = *(uint *)(iVar3 + 0xc);
    uVar1 = *(uint *)(iVar3 + 0x20);
    uVar5 = uVar1 % uVar4;
    if (uVar5 == 0) {
      uVar5 = uVar4;
    }
    *(uint *)(iVar3 + 0x48) = uVar5;
    param_1[0x4f] = 1;
    param_1[0x50] = 0;
    return uVar1 / uVar4;
  }
  if ((iVar3 < 1) || (4 < iVar3)) {
    *(undefined4 *)(*param_1 + 0x14) = 0x18;
    *(int *)(*param_1 + 0x18) = param_1[0x48];
    *(undefined4 *)(*param_1 + 0x1c) = 4;
    (**(code **)*param_1)(param_1);
  }
  iVar3 = FUN_00442220(param_1[6],param_1[0x43] << 3);
  param_1[0x4d] = iVar3;
  iVar3 = 0;
  uVar4 = FUN_00442220(param_1[7],param_1[0x44] << 3);
  param_1[0x4e] = uVar4;
  param_1[0x4f] = 0;
  if (0 < param_1[0x48]) {
    do {
      iVar2 = param_1[iVar3 + 0x49];
      *(undefined4 *)(iVar2 + 0x34) = *(undefined4 *)(iVar2 + 8);
      *(int *)(iVar2 + 0x38) = *(int *)(iVar2 + 0xc);
      *(int *)(iVar2 + 0x3c) = *(int *)(iVar2 + 0x34) * *(int *)(iVar2 + 0xc);
      *(int *)(iVar2 + 0x40) = *(int *)(iVar2 + 0x24) * *(int *)(iVar2 + 0x34);
      uVar4 = *(uint *)(iVar2 + 0x1c) % *(uint *)(iVar2 + 0x34);
      if (uVar4 == 0) {
        uVar4 = *(uint *)(iVar2 + 0x34);
      }
      *(uint *)(iVar2 + 0x44) = uVar4;
      uVar4 = *(uint *)(iVar2 + 0x20) % *(uint *)(iVar2 + 0x38);
      if (uVar4 == 0) {
        uVar4 = *(uint *)(iVar2 + 0x38);
      }
      *(uint *)(iVar2 + 0x48) = uVar4;
      uVar4 = *(uint *)(iVar2 + 0x3c);
      uVar1 = uVar4;
      if (10 < (int)(param_1[0x4f] + uVar4)) {
        *(undefined4 *)(*param_1 + 0x14) = 0xb;
        (**(code **)*param_1)(param_1);
      }
      for (; 0 < (int)uVar1; uVar1 = uVar1 - 1) {
        uVar4 = param_1[0x4f];
        param_1[uVar4 + 0x50] = iVar3;
        param_1[0x4f] = param_1[0x4f] + 1;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < param_1[0x48]);
  }
  return uVar4;
}
