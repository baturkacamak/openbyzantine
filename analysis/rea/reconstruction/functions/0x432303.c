// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432303
// name: FUN_00432303
// ranges: 0x432303-0x43237b
// calls: 0x4102f7, 0x432303

void FUN_00432303(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if ((*(int *)(&DAT_0047dd70 + local_8 * 4) != 0) &&
       ((*(char *)(*(int *)(&DAT_0047dd70 + local_8 * 4) + 8) == '\0' ||
        (*(char *)(*(int *)(&DAT_0047dd70 + local_8 * 4) + 8) == '\x01')))) {
      FUN_004102f7(local_8);
    }
  }
  return;
}
