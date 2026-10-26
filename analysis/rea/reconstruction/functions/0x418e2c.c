// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418e2c
// name: FUN_00418e2c
// ranges: 0x418e2c-0x418e6e
// calls: 0x418e2c

void __fastcall FUN_00418e2c(int *param_1,int param_2)

{
  *param_1 = *param_1 + param_2 * 0x10;
  param_1[2] = param_1[2] + param_2 * -0x10;
  param_1[1] = param_1[1] + param_2 * 0x10;
  param_1[3] = param_1[3] + param_2 * -0x10;
  return;
}
