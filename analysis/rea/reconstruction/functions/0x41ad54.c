// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41ad54
// name: FUN_0041ad54
// ranges: 0x41ad54-0x41ae56
// calls: 0x41ad54, 0x44a820, 0x44ac40

undefined4 __fastcall FUN_0041ad54(int *param_1)

{
  if (param_1[5] != 0) {
    FUN_0044a820((LPVOID)param_1[5]);
  }
  if (param_1[6] != 0) {
    FUN_0044a820((LPVOID)param_1[6]);
  }
  param_1[6] = 0;
  param_1[5] = param_1[6];
  if (param_1[0x71] != 0) {
    FUN_0044ac40((LPVOID)param_1[0x71]);
    param_1[0x71] = 0;
  }
  if (param_1[2] != 0) {
    DrawDibClose(param_1[2]);
    param_1[2] = 0;
  }
  if (*param_1 != 0) {
    ICClose(*param_1);
    *param_1 = 0;
  }
  *param_1 = 0;
  param_1[3] = 0;
  return 0;
}
