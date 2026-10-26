// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44f9e0
// name: FUN_0044f9e0
// ranges: 0x44f9e0-0x44fa20; 0x44fa3a-0x44fa51
// calls: 0x44f9e0, 0x454080

void FUN_0044f9e0(void)

{
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  uStack_14 = *unaff_FS_OFFSET;
  puStack_c = &DAT_0045b8c0;
  puStack_10 = &LAB_00453f64;
  *unaff_FS_OFFSET = &uStack_14;
  if (DAT_0046325c != (code *)0x0) {
    local_8 = 1;
    (*DAT_0046325c)();
  }
  local_8 = 0xffffffff;
  FUN_00454080(10);
  _raise(0x16);
  __exit(3);
  return;
}
