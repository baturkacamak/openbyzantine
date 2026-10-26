// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x442450
// name: FUN_00442450
// ranges: 0x442450-0x44250c
// calls: 0x442450

void __cdecl FUN_00442450(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uStack_8;
  
  iVar1 = *(int *)(param_1 + 0x180);
  iVar2 = *(int *)(param_1 + 0x114);
  uVar3 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,*(int *)(param_1 + 0x20) << 3);
  iVar8 = 0;
  *(undefined4 *)(iVar1 + 0x38) = uVar3;
  uStack_8 = 0;
  *(int *)(iVar1 + 0x3c) = *(int *)(param_1 + 0x20) * 4 + *(int *)(iVar1 + 0x38);
  iVar6 = *(int *)(param_1 + 0xc4);
  if (0 < *(int *)(param_1 + 0x20)) {
    do {
      iVar8 = iVar8 + 4;
      iVar4 = (*(int *)(iVar6 + 0x24) * *(int *)(iVar6 + 0xc)) / *(int *)(param_1 + 0x114);
      iVar6 = iVar6 + 0x54;
      iVar7 = iVar4 * (iVar2 + 4);
      iVar5 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,iVar7 * 8);
      uStack_8 = uStack_8 + 1;
      iVar5 = iVar5 + iVar4 * 4;
      *(int *)(*(int *)(iVar1 + 0x38) + -4 + iVar8) = iVar5;
      *(int *)(*(int *)(iVar1 + 0x3c) + -4 + iVar8) = iVar5 + iVar7 * 4;
    } while (uStack_8 < *(int *)(param_1 + 0x20));
  }
  return;
}
