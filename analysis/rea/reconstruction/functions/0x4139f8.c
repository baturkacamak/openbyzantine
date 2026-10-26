// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4139f8
// name: FUN_004139f8
// ranges: 0x4139f8-0x413a58
// calls: 0x4139f8, 0x44ac40

void FUN_004139f8(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if ((&DAT_00482540)[local_8] != 0) {
      FUN_0044ac40((LPVOID)(&DAT_00482540)[local_8]);
      (&DAT_00482540)[local_8] = 0;
    }
  }
  return;
}
