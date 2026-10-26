// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432c9c
// name: FUN_00432c9c
// ranges: 0x432c9c-0x432d13
// calls: 0x432c9c

void __cdecl FUN_00432c9c(uint param_1)

{
  byte bVar1;
  int iVar2;
  
  iVar2 = *(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  bVar1 = *(byte *)(iVar2 + 9);
  if ((((bVar1 & 0x40) != 0) || (((bVar1 & 0x20) != 0 && (*(int *)(iVar2 + 0x20) != 0)))) ||
     (((bVar1 & 4) == 0 && ((bVar1 & 8) != 0)))) {
    *(undefined1 *)(iVar2 + 0x29) = 1;
    DAT_00482acd = 1;
  }
  return;
}
