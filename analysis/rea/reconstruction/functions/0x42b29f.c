// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42b29f
// name: FUN_0042b29f
// ranges: 0x42b29f-0x42b2da
// calls: 0x42b29f

void __cdecl FUN_0042b29f(int param_1,undefined2 param_2,int param_3)

{
  undefined4 local_8;
  
  for (local_8 = 0; local_8 < param_3; local_8 = local_8 + 1) {
    *(undefined2 *)(param_1 + local_8 * 2) = param_2;
  }
  return;
}
