// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43f2f0
// name: FUN_0043f2f0
// ranges: 0x43f2f0-0x43f36e
// calls: 0x43f2f0, 0x43f5f0, 0x43f7c0, 0x440d50, 0x440de0

void __cdecl FUN_0043f2f0(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = (undefined4 *)(*(code *)**(undefined4 **)(param_1 + 4))(param_1,0,0x5c);
  *(undefined4 **)(param_1 + 400) = puVar1;
  *puVar1 = FUN_00440de0;
  *(undefined1 **)(*(int *)(param_1 + 400) + 4) = &LAB_0043f870;
  *(code **)(*(int *)(param_1 + 400) + 8) = FUN_00440d50;
  *(code **)(*(int *)(param_1 + 400) + 0xc) = FUN_0043f7c0;
  iVar2 = 0;
  do {
    iVar2 = iVar2 + 4;
    *(code **)(*(int *)(param_1 + 400) + 0xc + iVar2) = FUN_0043f7c0;
  } while (iVar2 < 0x40);
  *(undefined1 **)(*(int *)(param_1 + 400) + 0x10) = &LAB_0043f370;
  *(code **)(*(int *)(param_1 + 400) + 0x48) = FUN_0043f5f0;
  FUN_00440de0(param_1);
  return;
}
