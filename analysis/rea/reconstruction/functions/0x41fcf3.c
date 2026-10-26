// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41fcf3
// name: FUN_0041fcf3
// ranges: 0x41fcf3-0x41fd89
// calls: 0x406ef5, 0x406f99, 0x40d9f7, 0x40da1f, 0x40df05, 0x40f135, 0x418040, 0x419d28, 0x41df43, 0x41fcf3, 0x422500

void FUN_0041fcf3(void)

{
  undefined4 uVar1;
  int local_14 [2];
  ushort *local_c;
  char *local_8;
  
  local_8 = s_setupart_credbk_jpg_0045fbc8;
  local_14[0] = 0x280;
  local_14[1] = 0x1e0;
  FUN_0040d9f7();
  FUN_00419d28(DAT_0047a8a4);
  local_c = FUN_0040f135(local_8,local_14,(uint *)(local_14 + 1),DAT_0047a8a4,0x280);
  if (local_c != (ushort *)0x0) {
    FUN_00406ef5();
    uVar1 = FUN_00418040((int)DAT_0047a8a4);
    FUN_00422500(uVar1);
    FUN_00406f99();
    FUN_0041df43();
  }
  FUN_0040da1f();
  FUN_0040df05(0x1d);
  return;
}
