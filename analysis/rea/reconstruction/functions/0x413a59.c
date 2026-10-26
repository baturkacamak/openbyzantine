// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x413a59
// name: FUN_00413a59
// ranges: 0x413a59-0x413ab6
// calls: 0x413a59, 0x44ac40

void FUN_00413a59(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 10; local_8 = local_8 + 1) {
    if ((&DAT_0047a8b0)[local_8] != 0) {
      FUN_0044ac40((LPVOID)(&DAT_0047a8b0)[local_8]);
      (&DAT_0047a8b0)[local_8] = 0;
    }
  }
  return;
}
