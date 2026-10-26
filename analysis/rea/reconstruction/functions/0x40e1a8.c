// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40e1a8
// name: FUN_0040e1a8
// ranges: 0x40e1a8-0x40e23d
// calls: 0x40da4c, 0x40e1a8

void FUN_0040e1a8(void)

{
  if (DAT_0045dbd4 != (int *)0x0) {
    (**(code **)(*DAT_0045dbd4 + 8))(DAT_0045dbd4);
  }
  if (DAT_0045dbd8 != (int *)0x0) {
    (**(code **)(*DAT_0045dbd8 + 8))(DAT_0045dbd8);
  }
  if (DAT_0045dbdc != (int *)0x0) {
    (**(code **)(*DAT_0045dbdc + 8))(DAT_0045dbdc);
  }
  DAT_0045dbdc = (int *)0x0;
  DAT_0045dbd8 = (int *)0x0;
  DAT_0045dbd4 = (int *)0x0;
  FUN_0040da4c();
  DAT_0045dbd0 = 0;
  ShowCursor(1);
  return;
}
