// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407fe5
// name: FUN_00407fe5
// ranges: 0x407fe5-0x408075
// calls: 0x407fe5

undefined4 __cdecl FUN_00407fe5(int param_1)

{
  int local_10;
  
  if (param_1 != 0) {
    for (local_10 = 0; local_10 < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4);
        local_10 = local_10 + 1) {
      if (*(int *)(&DAT_00468d78 + local_10 * 4 + DAT_0045d1c4 * 0x578) == *(int *)(param_1 + 8)) {
        return 1;
      }
    }
  }
  return 0;
}
