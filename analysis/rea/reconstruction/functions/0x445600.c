// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x445600
// name: FUN_00445600
// ranges: 0x445600-0x445689
// calls: 0x442230, 0x445600

void __cdecl FUN_00445600(int param_1,char param_2)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  
  puVar1 = (undefined4 *)(*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x1c);
  *(undefined4 **)(param_1 + 0x188) = puVar1;
  *puVar1 = &LAB_00445690;
  puVar1[2] = 0;
  puVar1[3] = 0;
  if (*(char *)(param_1 + 0x4a) != '\0') {
    iVar2 = *(int *)(param_1 + 0x110);
    puVar1[4] = iVar2;
    if (param_2 != '\0') {
      iVar2 = FUN_00442230(*(int *)(param_1 + 0x60),iVar2);
      uVar3 = (**(code **)(*(int *)(param_1 + 4) + 0x10))
                        (param_1,1,0,*(int *)(param_1 + 0x5c) * *(int *)(param_1 + 100),iVar2);
      puVar1[2] = uVar3;
      return;
    }
    uVar3 = (**(code **)(*(int *)(param_1 + 4) + 8))
                      (param_1,1,*(int *)(param_1 + 0x5c) * *(int *)(param_1 + 100),iVar2);
    puVar1[3] = uVar3;
  }
  return;
}
