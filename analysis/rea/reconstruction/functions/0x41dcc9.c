// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41dcc9
// name: FUN_0041dcc9
// ranges: 0x41dcc9-0x41dd21
// calls: 0x41dcc9

void FUN_0041dcc9(void)

{
  DWORD DVar1;
  int local_8;
  
  DVar1 = timeGetTime();
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if (*(int *)(&DAT_00464f60 + local_8 * 4) != 0) {
      *(DWORD *)(*(int *)(&DAT_00464f60 + local_8 * 4) + 8) = DVar1;
    }
  }
  return;
}
