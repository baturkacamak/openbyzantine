// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422770
// name: FUN_00422770
// ranges: 0x422770-0x422800
// calls: 0x422770, 0x44ac40

void FUN_00422770(void)

{
  if (DAT_0045fd28 != -1) {
    if ((DAT_0045fd28 != -2) && (*(int *)(&DAT_00485c90 + DAT_0045fd28 * 4) != 0)) {
      *(undefined4 *)(*(int *)(&DAT_00485c90 + DAT_0045fd28 * 4) + 8) = 0;
      FUN_0044ac40(*(LPVOID *)(&DAT_00485c90 + DAT_0045fd28 * 4));
      *(undefined4 *)(&DAT_00485c90 + DAT_0045fd28 * 4) = 0;
      if (DAT_0046066c == 1) {
        DAT_0046066c = 2;
      }
    }
    DAT_0045fd28 = -1;
  }
  return;
}
