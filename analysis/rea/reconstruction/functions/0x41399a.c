// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41399a
// name: FUN_0041399a
// ranges: 0x41399a-0x4139f7
// calls: 0x41399a, 0x44ac40

void FUN_0041399a(void)

{
  int local_8;
  
  for (local_8 = 0x95; -1 < local_8; local_8 = local_8 + -1) {
    if ((&DAT_00485a10)[local_8] != 0) {
      FUN_0044ac40((LPVOID)(&DAT_00485a10)[local_8]);
      (&DAT_00485a10)[local_8] = 0;
    }
  }
  return;
}
