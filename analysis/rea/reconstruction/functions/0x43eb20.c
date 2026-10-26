// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43eb20
// name: FUN_0043eb20
// ranges: 0x43eb20-0x43eb61
// calls: 0x43eb20, 0x43eb70, 0x43ed00

void __cdecl FUN_0043eb20(int *param_1)

{
  FUN_0043eb70(param_1);
  FUN_0043ed00(param_1);
  (**(code **)param_1[0x65])(param_1);
  (**(code **)param_1[0x61])(param_1);
  *(undefined4 *)param_1[99] = *(undefined4 *)(param_1[0x61] + 4);
  return;
}
