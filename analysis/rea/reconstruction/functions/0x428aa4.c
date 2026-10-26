// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x428aa4
// name: FUN_00428aa4
// ranges: 0x428aa4-0x428b1d
// calls: 0x427b60, 0x4281b1, 0x428aa4

void FUN_00428aa4(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if ((*(int *)(&DAT_00486220 + local_8 * 4) != 0) &&
       (*(int *)(*(int *)(&DAT_00486220 + local_8 * 4) + 0x18) == 0)) {
      FUN_004281b1(local_8);
      FUN_00427b60(local_8);
      *(undefined1 *)(*(int *)(&DAT_0047a8e0 + local_8 * 4) + 9) = 0;
    }
  }
  return;
}
