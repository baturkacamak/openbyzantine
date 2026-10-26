// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41d2e0
// name: FUN_0041d2e0
// ranges: 0x41d2e0-0x41d335
// calls: 0x41d2e0

void FUN_0041d2e0(void)

{
  int local_8;
  
  if (DAT_0045f3f8 == '\0') {
    DAT_00486881 = 0;
    for (local_8 = 0; local_8 < 0x80; local_8 = local_8 + 1) {
      (&DAT_00486800)[local_8] = 0;
    }
    DAT_0045f3f8 = '\x01';
  }
  return;
}
