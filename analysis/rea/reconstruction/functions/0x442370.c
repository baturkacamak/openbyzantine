// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x442370
// name: FUN_00442370
// ranges: 0x442370-0x442447
// calls: 0x442370, 0x442450

void __cdecl FUN_00442370(int *param_1,char param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iStack_4;
  
  puVar1 = (undefined4 *)(**(code **)param_1[1])(param_1,1,0x50);
  param_1[0x60] = (int)puVar1;
  *puVar1 = &LAB_00442510;
  if (param_2 != '\0') {
    *(undefined4 *)(*param_1 + 0x14) = 4;
    (**(code **)*param_1)(param_1);
  }
  if (*(char *)(param_1[0x67] + 8) == '\0') {
    iStack_4 = param_1[0x45];
  }
  else {
    if (param_1[0x45] < 2) {
      *(undefined4 *)(*param_1 + 0x14) = 0x2e;
      (**(code **)*param_1)(param_1);
    }
    FUN_00442450((int)param_1);
    iStack_4 = param_1[0x45] + 2;
  }
  iVar3 = 0;
  if (0 < param_1[8]) {
    iVar4 = param_1[0x31];
    puVar1 = puVar1 + 2;
    do {
      iVar3 = iVar3 + 1;
      uVar2 = (**(code **)(param_1[1] + 8))
                        (param_1,1,*(int *)(iVar4 + 0x1c) * *(int *)(iVar4 + 0x24),
                         iStack_4 *
                         ((*(int *)(iVar4 + 0xc) * *(int *)(iVar4 + 0x24)) / param_1[0x45]));
      *puVar1 = uVar2;
      iVar4 = iVar4 + 0x54;
      puVar1 = puVar1 + 1;
    } while (iVar3 < param_1[8]);
  }
  return;
}
