// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41194b
// name: FUN_0041194b
// ranges: 0x41194b-0x41199d
// calls: 0x41194b, 0x428a2b

void FUN_0041194b(void)

{
  int local_8;
  
  FUN_00428a2b();
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if (*(int *)(&DAT_0047a8e0 + local_8 * 4) != 0) {
      *(undefined1 *)(*(int *)(&DAT_0047a8e0 + local_8 * 4) + 9) = 0;
    }
  }
  return;
}
