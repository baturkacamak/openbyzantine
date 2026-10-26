// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4418d0
// name: FUN_004418d0
// ranges: 0x4418d0-0x4419ca
// calls: 0x4418d0, 0x4422f0, 0x442310

void __cdecl FUN_004418d0(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  
  iVar1 = param_1[1];
  if ((param_2 < 0) || (1 < param_2)) {
    *(undefined4 *)(*param_1 + 0x14) = 0xc;
    *(int *)(*param_1 + 0x18) = param_2;
    (**(code **)*param_1)(param_1);
  }
  if (param_2 == 1) {
    for (iVar2 = *(int *)(iVar1 + 0x40); iVar2 != 0; iVar2 = *(int *)(iVar2 + 0x24)) {
      if (*(char *)(iVar2 + 0x22) != '\0') {
        *(undefined1 *)(iVar2 + 0x22) = 0;
        (**(code **)(iVar2 + 0x30))(param_1,iVar2 + 0x28);
      }
    }
    *(undefined4 *)(iVar1 + 0x40) = 0;
    for (iVar2 = *(int *)(iVar1 + 0x44); iVar2 != 0; iVar2 = *(int *)(iVar2 + 0x24)) {
      if (*(char *)(iVar2 + 0x22) != '\0') {
        *(undefined1 *)(iVar2 + 0x22) = 0;
        (**(code **)(iVar2 + 0x30))(param_1,iVar2 + 0x28);
      }
    }
    *(undefined4 *)(iVar1 + 0x44) = 0;
  }
  puVar3 = *(undefined4 **)(iVar1 + 0x38 + param_2 * 4);
  *(undefined4 *)(iVar1 + 0x38 + param_2 * 4) = 0;
  while (puVar3 != (undefined4 *)0x0) {
    iVar2 = puVar3[2];
    puVar4 = (undefined4 *)*puVar3;
    iVar5 = puVar3[1];
    FUN_00442310(param_1,puVar3);
    *(int *)(iVar1 + 0x48) = *(int *)(iVar1 + 0x48) - (iVar5 + 0x10 + iVar2);
    puVar3 = puVar4;
  }
  puVar3 = *(undefined4 **)(iVar1 + 0x30 + param_2 * 4);
  *(undefined4 *)(iVar1 + 0x30 + param_2 * 4) = 0;
  while (puVar3 != (undefined4 *)0x0) {
    iVar2 = puVar3[1];
    puVar4 = (undefined4 *)*puVar3;
    iVar5 = puVar3[2];
    FUN_004422f0(param_1,puVar3);
    *(int *)(iVar1 + 0x48) = *(int *)(iVar1 + 0x48) - (iVar5 + 0x10 + iVar2);
    puVar3 = puVar4;
  }
  return;
}
