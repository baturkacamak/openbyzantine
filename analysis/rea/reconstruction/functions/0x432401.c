// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432401
// name: FUN_00432401
// ranges: 0x432401-0x432463
// calls: 0x432401, 0x432602

void FUN_00432401(void)

{
  int iVar1;
  int local_c;
  
  for (local_c = 0; local_c < 0x15e; local_c = local_c + 1) {
    iVar1 = *(int *)(&DAT_004649e8 + local_c * 4);
    if (iVar1 != 0) {
      *(byte *)(iVar1 + 9) = *(byte *)(iVar1 + 9) | 0x20;
      FUN_00432602(iVar1);
    }
  }
  return;
}
