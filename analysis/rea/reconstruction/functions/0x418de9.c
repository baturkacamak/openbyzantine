// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418de9
// name: FUN_00418de9
// ranges: 0x418de9-0x418e2b
// calls: 0x418de9

void __fastcall FUN_00418de9(int *param_1,int param_2)

{
  *param_1 = *param_1 + param_2 * 8;
  param_1[2] = param_1[2] + param_2 * -8;
  param_1[1] = param_1[1] + param_2 * 8;
  param_1[3] = param_1[3] + param_2 * -8;
  return;
}
