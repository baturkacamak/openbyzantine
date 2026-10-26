// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x448520
// name: FUN_00448520
// ranges: 0x448520-0x44858b
// calls: 0x448520, 0x448590

void __cdecl FUN_00448520(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  
  iVar7 = 0;
  iVar1 = *(int *)(param_1 + 0x1a4);
  if (0 < *(int *)(param_1 + 100)) {
    piVar2 = (int *)(iVar1 + 0x20);
    piVar5 = piVar2;
    do {
      iVar3 = 0;
      iVar4 = 0;
      piVar6 = piVar2;
      if (0 < iVar7) {
        do {
          if (*piVar6 == *piVar5) {
            iVar3 = *(int *)(iVar1 + 0x34 + iVar4 * 4);
            break;
          }
          iVar4 = iVar4 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar4 < iVar7);
      }
      if (iVar3 == 0) {
        iVar3 = FUN_00448590(param_1,*piVar5);
      }
      piVar5[5] = iVar3;
      piVar5 = piVar5 + 1;
      iVar7 = iVar7 + 1;
    } while (iVar7 < *(int *)(param_1 + 100));
  }
  return;
}
