// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43237c
// name: FUN_0043237c
// ranges: 0x43237c-0x432400
// calls: 0x427b60, 0x428143, 0x43237c

void FUN_0043237c(void)

{
  int iVar1;
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if ((*(int *)(&DAT_0047a8e0 + local_8 * 4) != 0) &&
       (*(char *)(*(int *)(&DAT_0047a8e0 + local_8 * 4) + 8) == '\x01')) {
      iVar1 = FUN_00428143(local_8);
      if (iVar1 != 0) {
        FUN_00427b60(local_8);
        *(undefined1 *)(*(int *)(&DAT_0047a8e0 + local_8 * 4) + 9) = 0;
      }
    }
  }
  return;
}
