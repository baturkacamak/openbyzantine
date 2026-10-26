// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418598
// name: FUN_00418598
// ranges: 0x418598-0x4185e2
// calls: 0x418598, 0x42bc8b

void __cdecl FUN_00418598(undefined2 *param_1,undefined2 *param_2,int param_3)

{
  int local_8;
  
  for (local_8 = 0; local_8 < param_3; local_8 = local_8 + 1) {
    FUN_0042bc8b(param_1,param_2,*(int *)(&DAT_0045f2e0 + local_8 * 4),0x1b);
  }
  return;
}
