// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x445360
// name: FUN_00445360
// ranges: 0x445360-0x4453da
// calls: 0x445360, 0x4453e0

void __cdecl FUN_00445360(int param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iStack_4;
  
  puVar1 = (undefined4 *)(*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x54);
  *(undefined4 **)(param_1 + 0x198) = puVar1;
  *puVar1 = FUN_004453e0;
  iStack_4 = 0;
  if (0 < *(int *)(param_1 + 0x20)) {
    iVar4 = *(int *)(param_1 + 0xc4);
    puVar1 = puVar1 + 0xb;
    do {
      puVar2 = (undefined4 *)(*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x100);
      *(undefined4 **)(iVar4 + 0x50) = puVar2;
      for (iVar3 = 0x40; iVar3 != 0; iVar3 = iVar3 + -1) {
        *puVar2 = 0;
        puVar2 = puVar2 + 1;
      }
      iStack_4 = iStack_4 + 1;
      *puVar1 = 0xffffffff;
      iVar4 = iVar4 + 0x54;
      puVar1 = puVar1 + 1;
    } while (iStack_4 < *(int *)(param_1 + 0x20));
  }
  return;
}
