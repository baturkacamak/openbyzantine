// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43a03b
// name: FUN_0043a03b
// ranges: 0x43a03b-0x43a0a7
// calls: 0x43a03b, 0x44a820

undefined4 __fastcall FUN_0043a03b(int param_1)

{
  if (*(int *)(param_1 + 0x54) != 0) {
    FUN_0044a820(*(LPVOID *)(param_1 + 0x54));
    *(undefined4 *)(param_1 + 0x54) = 0;
  }
  if (*(int *)(param_1 + 0x14) != 0) {
    AVIStreamRelease(*(undefined4 *)(param_1 + 0x28));
    *(undefined4 *)(param_1 + 0x14) = 0;
  }
  return 0;
}
