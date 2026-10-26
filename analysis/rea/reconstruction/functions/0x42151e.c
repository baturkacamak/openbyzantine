// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42151e
// name: FUN_0042151e
// ranges: 0x42151e-0x4215d5
// calls: 0x42151e, 0x44ac40

void FUN_0042151e(void)

{
  LPVOID pvVar1;
  int local_10;
  int local_8;
  
  for (local_8 = 0;
      (local_8 < 100 && (pvVar1 = *(LPVOID *)(&DAT_0046c8d0 + local_8 * 4), pvVar1 != (LPVOID)0x0));
      local_8 = local_8 + 1) {
    for (local_10 = 0; local_10 < *(int *)((int)pvVar1 + 4); local_10 = local_10 + 1) {
      FUN_0044ac40(*(LPVOID *)(*(int *)((int)pvVar1 + 0x10) + local_10 * 4));
    }
    FUN_0044ac40(*(LPVOID *)((int)pvVar1 + 0x10));
    FUN_0044ac40(*(LPVOID *)((int)pvVar1 + 0xc));
    FUN_0044ac40(pvVar1);
    *(undefined4 *)(&DAT_0046c8d0 + local_8 * 4) = 0;
  }
  return;
}
