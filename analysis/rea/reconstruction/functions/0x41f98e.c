// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f98e
// name: FUN_0041f98e
// ranges: 0x41f98e-0x41f9e9
// calls: 0x41f94e, 0x41f96e, 0x41f98e

void FUN_0041f98e(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 6; local_8 = local_8 + 1) {
    if ((&DAT_0046c5d0)[local_8] == '\x01') {
      FUN_0041f94e(local_8);
    }
    else {
      FUN_0041f96e(local_8);
    }
  }
  return;
}
