// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41c36f
// name: FUN_0041c36f
// ranges: 0x41c36f-0x41c48a
// calls: 0x41b152, 0x41c36f, 0x41cde0, 0x44ac40

void __fastcall FUN_0041c36f(int *param_1)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x4c; local_8 = local_8 + 1) {
    if (param_1[local_8 + 1] != 0) {
      FUN_0044ac40((LPVOID)param_1[local_8 + 1]);
    }
  }
  if (param_1[0x9a] != 0) {
    (**(code **)(*(int *)param_1[0x9a] + 8))(param_1[0x9a]);
  }
  if (*param_1 != 0) {
    FUN_0044ac40((LPVOID)*param_1);
  }
  if (param_1[0x9c] != 0) {
    AVIStreamRelease(param_1[0x9c]);
  }
  if (param_1[0x9b] != 0) {
    FUN_0041b152(param_1[0x9b]);
    if ((void *)param_1[0x9b] != (void *)0x0) {
      FUN_0041cde0((void *)param_1[0x9b],1);
    }
  }
  if (param_1[0xa0] != 0) {
    DAT_0045d1d0 = 1;
  }
  return;
}
