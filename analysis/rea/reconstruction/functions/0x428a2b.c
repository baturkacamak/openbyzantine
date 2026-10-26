// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x428a2b
// name: FUN_00428a2b
// ranges: 0x428a2b-0x428aa3
// calls: 0x41624a, 0x427b60, 0x428071, 0x4281b1, 0x428a2b

void FUN_00428a2b(void)

{
  uint local_8;
  
  while (DAT_00460198 != 0) {
    FUN_0041624a();
  }
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    FUN_004281b1(local_8);
    FUN_00427b60(local_8);
  }
  FUN_00428071();
  _memset(&DAT_00486210,0,0xc);
  return;
}
