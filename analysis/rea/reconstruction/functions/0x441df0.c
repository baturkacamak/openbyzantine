// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x441df0
// name: FUN_00441df0
// ranges: 0x441df0-0x441e23
// calls: 0x441df0, 0x441e30

void __cdecl FUN_00441df0(int *param_1)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(**(code **)param_1[1])(param_1,1,0x1c);
  param_1[0x5f] = (int)puVar1;
  *puVar1 = &LAB_004420a0;
  puVar1[1] = &LAB_004421f0;
  *(undefined1 *)(puVar1 + 2) = 0;
  FUN_00441e30(param_1);
  return;
}
