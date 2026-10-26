// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40f524
// name: FUN_0040f524
// ranges: 0x40f524-0x40f6a4
// calls: 0x40f524

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0040f524(void)

{
  ushort uVar1;
  ushort uVar2;
  
  if (DAT_00485c6c != '\0') {
    _DAT_0047dd60 = 0;
    for (uVar1 = 0; uVar1 < 6; uVar1 = uVar1 + 1) {
      for (uVar2 = 0; uVar2 < 0x15f; uVar2 = uVar2 + 1) {
        *(undefined4 *)(&DAT_00480340 + (uint)uVar2 * 4 + (uint)uVar1 * 0x57c) = 0;
      }
    }
  }
  for (uVar1 = 0; uVar1 < 0x15e; uVar1 = uVar1 + 1) {
    *(undefined4 *)(&DAT_00464300 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_00464f60 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_004649e8 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_00482ad0 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_00486220 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_00486a10 + (uint)uVar1 * 4) = 0;
    (&DAT_00464888)[uVar1] = 0;
  }
  for (uVar1 = 0; uVar1 < 0xdac; uVar1 = uVar1 + 1) {
    *(undefined4 *)(&DAT_004654e0 + (uint)uVar1 * 4) = 0;
  }
  return;
}
