// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43a20d
// name: FUN_0043a20d
// ranges: 0x43a20d-0x43a247
// calls: 0x43a20d

undefined4 __fastcall FUN_0043a20d(int param_1)

{
  if (*(int *)(param_1 + 0x60) != 0) {
    AVIStreamRelease(*(undefined4 *)(param_1 + 0x60));
    *(undefined4 *)(param_1 + 0x60) = 0;
  }
  return 0;
}
