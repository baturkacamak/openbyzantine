// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40f6a5
// name: FUN_0040f6a5
// ranges: 0x40f6a5-0x40f968
// calls: 0x40f6a5, 0x44ac40

void FUN_0040f6a5(void)

{
  ushort uVar1;
  
  for (uVar1 = 0; uVar1 < 0x15e; uVar1 = uVar1 + 1) {
    if (*(int *)(&DAT_004848b0 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_004848b0 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_004848b0 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_004848b0 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_004848b0 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_0046e280 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_0046e280 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_0046e280 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_0046e280 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_0046e280 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00483080 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_00483080 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00483080 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_00483080 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00483080 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00485400 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_00485400 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00485400 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_00485400 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00485400 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_0047a2b0 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_0047a2b0 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_0047a2b0 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_0047a2b0 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_0047a2b0 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00483640 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00483640 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00483640 + (uint)uVar1 * 4) = 0;
    }
  }
  return;
}
