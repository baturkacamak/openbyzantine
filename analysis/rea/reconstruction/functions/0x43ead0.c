// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43ead0
// name: FUN_0043ead0
// ranges: 0x43ead0-0x43eb10
// calls: 0x43ead0, 0x43eb20

void __cdecl FUN_0043ead0(int param_1)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(*(code *)**(undefined4 **)(param_1 + 4))(param_1,0,0x18);
  *(undefined4 **)(param_1 + 0x18c) = puVar1;
  *puVar1 = &LAB_0043edc0;
  puVar1[1] = &LAB_0043f080;
  puVar1[2] = FUN_0043eb20;
  puVar1[3] = &LAB_0043eda0;
  *(undefined1 *)(puVar1 + 4) = 0;
  *(undefined1 *)((int)puVar1 + 0x11) = 0;
  *(undefined1 *)(puVar1 + 5) = 1;
  return;
}
