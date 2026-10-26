// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x453120
// name: FUN_00453120
// ranges: 0x453120-0x453185
// calls: 0x453120

void __cdecl FUN_00453120(uint param_1)

{
  int iVar1;
  uint *puVar2;
  
  iVar1 = 0;
  puVar2 = &DAT_00463668;
  DAT_00462c78 = param_1;
  do {
    if (*puVar2 == param_1) {
      DAT_00462c74 = *(undefined4 *)(iVar1 * 8 + 0x46366c);
      return;
    }
    puVar2 = puVar2 + 2;
    iVar1 = iVar1 + 1;
  } while (puVar2 < &DAT_004637d0);
  if ((0x12 < param_1) && (param_1 < 0x25)) {
    DAT_00462c74 = 0xd;
    return;
  }
  if ((0xbb < param_1) && (param_1 < 0xcb)) {
    DAT_00462c74 = 8;
    return;
  }
  DAT_00462c74 = 0x16;
  return;
}
