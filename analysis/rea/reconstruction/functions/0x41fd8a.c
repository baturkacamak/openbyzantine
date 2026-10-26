// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41fd8a
// name: FUN_0041fd8a
// ranges: 0x41fd8a-0x41fde7
// calls: 0x41fd8a, 0x44ac40

void FUN_0041fd8a(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 100; local_8 = local_8 + 1) {
    if ((&DAT_0046c660)[local_8] != 0) {
      FUN_0044ac40((LPVOID)(&DAT_0046c660)[local_8]);
      (&DAT_0046c660)[local_8] = 0;
    }
  }
  return;
}
