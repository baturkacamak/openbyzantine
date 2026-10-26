// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x439dbb
// name: FUN_00439dbb
// ranges: 0x439dbb-0x439df5
// calls: 0x439dbb

undefined4 __fastcall FUN_00439dbb(int param_1)

{
  if (*(int *)(param_1 + 0x14) != 0) {
    AVIFileRelease(*(undefined4 *)(param_1 + 0x14));
    *(undefined4 *)(param_1 + 0x14) = 0;
  }
  return 0;
}
