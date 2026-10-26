// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44fa70
// name: FUN_0044fa70
// ranges: 0x44fa70-0x44fab0; 0x44faca-0x44fae1
// calls: 0x44f9e0, 0x44fa70

void FUN_0044fa70(void)

{
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  uStack_14 = *unaff_FS_OFFSET;
  puStack_c = &DAT_0045b8d8;
  puStack_10 = &LAB_00453f64;
  *unaff_FS_OFFSET = &uStack_14;
  if (PTR_FUN_00463264 != (undefined *)0x0) {
    local_8 = 1;
    (*(code *)PTR_FUN_00463264)();
  }
  local_8 = 0xffffffff;
  FUN_0044f9e0();
  return;
}
