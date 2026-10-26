// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x428143
// name: FUN_00428143
// ranges: 0x428143-0x4281b0
// calls: 0x428143

undefined4 __cdecl FUN_00428143(int param_1)

{
  byte local_c [4];
  int *local_8;
  
  if (((*(int *)(&DAT_00485c90 + param_1 * 4) != 0) &&
      (local_8 = *(int **)(*(int *)(&DAT_00485c90 + param_1 * 4) + 8), local_8 != (int *)0x0)) &&
     ((**(code **)(*local_8 + 0x24))(local_8,local_c), (local_c[0] & 1) != 0)) {
    return 0;
  }
  return 1;
}
