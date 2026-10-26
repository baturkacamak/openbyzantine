// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40fc31
// name: FUN_0040fc31
// ranges: 0x40fc31-0x40fd7a
// calls: 0x40fc31, 0x4102f7, 0x44ac40

void FUN_0040fc31(void)

{
  ushort uVar1;
  
  for (uVar1 = 0; uVar1 < 0x15e; uVar1 = uVar1 + 1) {
    if (*(int *)(&DAT_00464300 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00464300 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00464300 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00464f60 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00464f60 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00464f60 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00486220 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00486220 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00486220 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00486a10 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00486a10 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00486a10 + (uint)uVar1 * 4) = 0;
    }
    FUN_004102f7((uint)uVar1);
  }
  return;
}
