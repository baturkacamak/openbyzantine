// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4461e0
// name: FUN_004461e0
// ranges: 0x4461e0-0x4462aa
// calls: 0x4461e0

void __cdecl FUN_004461e0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uStack_4;
  
  iVar1 = *(int *)(param_1 + 0x1a0);
  uVar2 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x400);
  *(undefined4 *)(iVar1 + 8) = uVar2;
  uVar2 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x400);
  *(undefined4 *)(iVar1 + 0xc) = uVar2;
  uVar2 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x400);
  *(undefined4 *)(iVar1 + 0x10) = uVar2;
  iVar6 = 0x2c8d00;
  uVar2 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x400);
  iVar7 = 0x5b6900;
  iVar5 = -0xb2f480;
  iVar4 = 0;
  uStack_4 = -0xe25100;
  *(undefined4 *)(iVar1 + 0x14) = uVar2;
  do {
    iVar3 = iVar5 >> 0x10;
    iVar4 = iVar4 + 4;
    iVar5 = iVar5 + 0x166e9;
    *(int *)(*(int *)(iVar1 + 8) + -4 + iVar4) = iVar3;
    *(int *)(*(int *)(iVar1 + 0xc) + -4 + iVar4) = uStack_4 >> 0x10;
    uStack_4 = uStack_4 + 0x1c5a2;
    *(int *)(*(int *)(iVar1 + 0x10) + -4 + iVar4) = iVar7;
    iVar7 = iVar7 + -0xb6d2;
    *(int *)(*(int *)(iVar1 + 0x14) + -4 + iVar4) = iVar6;
    iVar6 = iVar6 + -0x581a;
  } while (iVar4 < 0x3fd);
  return;
}
