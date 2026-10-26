// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x448000
// name: FUN_00448000
// ranges: 0x448000-0x44816b
// calls: 0x448000, 0x448170, 0x448250

void __cdecl FUN_00448000(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int iStack_18;
  int iStack_14;
  int iStack_10;
  int iStack_c;
  int *piStack_8;
  
  iVar1 = param_1[0x69];
  piStack_8 = (int *)(iVar1 + 0x20);
  iVar3 = FUN_00448170(param_1,piStack_8);
  if (param_1[0x19] == 3) {
    iVar2 = *param_1;
    *(int *)(iVar2 + 0x18) = iVar3;
    *(int *)(iVar2 + 0x1c) = *piStack_8;
    *(undefined4 *)(iVar2 + 0x20) = *(undefined4 *)(iVar1 + 0x24);
    *(undefined4 *)(iVar2 + 0x24) = *(undefined4 *)(iVar1 + 0x28);
    *(undefined4 *)(*param_1 + 0x14) = 0x5d;
  }
  else {
    *(undefined4 *)(*param_1 + 0x14) = 0x5e;
    *(int *)(*param_1 + 0x18) = iVar3;
  }
  (**(code **)(*param_1 + 4))(param_1,1);
  piVar4 = (int *)(**(code **)(param_1[1] + 8))(param_1,1,iVar3,param_1[0x19]);
  iStack_c = 0;
  piVar9 = piVar4;
  iStack_10 = iVar3;
  if (0 < param_1[0x19]) {
    do {
      iVar2 = *piStack_8;
      iVar5 = iStack_10 / iVar2;
      iStack_14 = 0;
      if (0 < iVar2) {
        iStack_18 = 0;
        do {
          iVar6 = FUN_00448250(param_1,iStack_c,iStack_14,iVar2 + -1);
          for (iVar8 = iStack_18; iVar8 < iVar3; iVar8 = iVar8 + iStack_10) {
            iVar7 = 0;
            if (0 < iVar5) {
              do {
                iVar7 = iVar7 + 1;
                *(char *)(*piVar9 + iVar8 + -1 + iVar7) = (char)iVar6;
              } while (iVar7 < iVar5);
            }
          }
          iStack_18 = iStack_18 + iVar5;
          iStack_14 = iStack_14 + 1;
        } while (iStack_14 < iVar2);
      }
      iStack_c = iStack_c + 1;
      piStack_8 = piStack_8 + 1;
      piVar9 = piVar9 + 1;
      iStack_10 = iVar5;
    } while (iStack_c < param_1[0x19]);
  }
  *(int **)(iVar1 + 0x10) = piVar4;
  *(int *)(iVar1 + 0x14) = iVar3;
  return;
}
