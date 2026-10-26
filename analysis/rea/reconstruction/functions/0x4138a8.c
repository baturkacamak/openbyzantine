// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4138a8
// name: FUN_004138a8
// ranges: 0x4138a8-0x41393b
// calls: 0x4138a8, 0x44ac40

void FUN_004138a8(void)

{
  short local_8;
  
  local_8 = DAT_0047a8a0;
  while (local_8 = local_8 + -1, -1 < local_8) {
    if (*(int *)(&DAT_0047ae80 + local_8 * 4) != 0) {
      if (**(int **)(&DAT_0047ae80 + local_8 * 4) != 0) {
        FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_0047ae80 + local_8 * 4));
      }
      FUN_0044ac40(*(LPVOID *)(&DAT_0047ae80 + local_8 * 4));
      *(undefined4 *)(&DAT_0047ae80 + local_8 * 4) = 0;
    }
  }
  return;
}
