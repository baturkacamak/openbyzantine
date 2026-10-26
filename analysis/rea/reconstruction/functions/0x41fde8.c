// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41fde8
// name: FUN_0041fde8
// ranges: 0x41fde8-0x41fe45
// calls: 0x41fde8, 0x44ac40

void FUN_0041fde8(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 10; local_8 = local_8 + 1) {
    if (*(int *)(&DAT_0046c5f8 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_0046c5f8 + local_8 * 4));
      *(undefined4 *)(&DAT_0046c5f8 + local_8 * 4) = 0;
    }
  }
  return;
}
