// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4465c0
// name: FUN_004465c0
// ranges: 0x4465c0-0x44663b
// calls: 0x4465c0, 0x446640

void __cdecl FUN_004465c0(int param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = (undefined4 *)(*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x30);
  *(undefined4 **)(param_1 + 0x19c) = puVar1;
  *puVar1 = &LAB_00446710;
  *(undefined1 *)(puVar1 + 2) = 0;
  puVar1[10] = *(int *)(param_1 + 0x5c) * *(int *)(param_1 + 100);
  if (*(int *)(param_1 + 0x110) == 2) {
    puVar1[1] = &LAB_00446730;
    puVar1[3] = &LAB_004469b0;
    uVar2 = (**(code **)(*(int *)(param_1 + 4) + 4))(param_1,1,puVar1[10]);
    puVar1[8] = uVar2;
  }
  else {
    puVar1[1] = &LAB_004467f0;
    puVar1[3] = &LAB_00446830;
    puVar1[8] = 0;
  }
  FUN_00446640(param_1);
  return;
}
