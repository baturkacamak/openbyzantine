// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41393c
// name: FUN_0041393c
// ranges: 0x41393c-0x413999
// calls: 0x41393c, 0x44ac40

void FUN_0041393c(void)

{
  int local_8;
  
  for (local_8 = 2999; -1 < local_8; local_8 = local_8 + -1) {
    if (*(int *)(&DAT_00477350 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00477350 + local_8 * 4));
      *(undefined4 *)(&DAT_00477350 + local_8 * 4) = 0;
    }
  }
  return;
}
