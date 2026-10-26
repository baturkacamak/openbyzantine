// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422318
// name: FUN_00422318
// ranges: 0x422318-0x422374
// calls: 0x422318, 0x44ac40

void __cdecl FUN_00422318(int param_1,int param_2)

{
  undefined4 local_8;
  
  for (local_8 = 0; local_8 < param_2; local_8 = local_8 + 1) {
    if (*(int *)(param_1 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(param_1 + local_8 * 4));
      *(undefined4 *)(param_1 + local_8 * 4) = 0;
    }
  }
  return;
}
