// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43e7b0
// name: FUN_0043e7b0
// ranges: 0x43e7b0-0x43e81d
// calls: 0x43e7b0, 0x43e830

void __cdecl FUN_0043e7b0(int param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (*(int *)(param_1 + 0x14) == 0) {
    iVar2 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,0,0x28);
    *(int *)(param_1 + 0x14) = iVar2;
    uVar3 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,0,0x1000);
    *(undefined4 *)(iVar2 + 0x20) = uVar3;
  }
  puVar1 = *(undefined4 **)(param_1 + 0x14);
  puVar1[2] = &LAB_0043e820;
  puVar1[3] = FUN_0043e830;
  puVar1[4] = &LAB_0043e8b0;
  puVar1[5] = &LAB_0043f0c0;
  puVar1[6] = &DAT_0043e8f0;
  puVar1[7] = param_2;
  puVar1[1] = 0;
  *puVar1 = 0;
  return;
}
