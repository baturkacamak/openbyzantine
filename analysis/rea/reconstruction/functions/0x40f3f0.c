// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40f3f0
// name: FUN_0040f3f0
// ranges: 0x40f3f0-0x40f4e6
// calls: 0x40f3f0, 0x44ac40

void FUN_0040f3f0(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if (*(int *)(&DAT_00464300 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00464300 + local_8 * 4));
      *(undefined4 *)(&DAT_00464300 + local_8 * 4) = 0;
    }
    if (*(int *)(&DAT_00486220 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00486220 + local_8 * 4));
      *(undefined4 *)(&DAT_00486220 + local_8 * 4) = 0;
    }
    if (*(int *)(&DAT_00486a10 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00486a10 + local_8 * 4));
      *(undefined4 *)(&DAT_00486a10 + local_8 * 4) = 0;
    }
    if (*(int *)(&DAT_00464f60 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00464f60 + local_8 * 4));
      *(undefined4 *)(&DAT_00464f60 + local_8 * 4) = 0;
    }
  }
  return;
}
