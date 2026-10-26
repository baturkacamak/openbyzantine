// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42d52f
// name: FUN_0042d52f
// ranges: 0x42d52f-0x42d593
// calls: 0x42d52f

void FUN_0042d52f(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if ((*(int *)(&DAT_0047dd70 + local_8 * 4) != 0) &&
       (*(char *)(*(int *)(&DAT_0047dd70 + local_8 * 4) + 8) == -1)) {
      *(undefined1 *)(*(int *)(&DAT_0047dd70 + local_8 * 4) + 8) = 0;
    }
  }
  return;
}
