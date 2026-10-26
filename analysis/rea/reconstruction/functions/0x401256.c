// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x401256
// name: FUN_00401256
// ranges: 0x401256-0x401387; 0x40139f-0x4013ad
// calls: 0x4011de, 0x401256, 0x40f135, 0x423310, 0x4236b5, 0x423d9e, 0x44ac40, 0x44ac60

undefined4 FUN_00401256(void)

{
  void *this;
  undefined4 uVar1;
  int iVar2;
  undefined4 *unaff_FS_OFFSET;
  undefined4 *local_28;
  undefined4 local_18;
  ushort *local_14;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_00401395;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  FUN_004011de();
  DAT_00468b98 = 0;
  this = (void *)FUN_0044ac60(0x70);
  local_8 = 0;
  if (this == (void *)0x0) {
    local_28 = (undefined4 *)0x0;
  }
  else {
    local_28 = FUN_00423310(this,DAT_00486888,&DAT_0045c048);
  }
  local_8 = 0xffffffff;
  DAT_00464880 = local_28;
  if (local_28 == (undefined4 *)0x0) {
    uVar1 = 1;
  }
  else {
    local_14 = FUN_0040f135(s_nodes_sm_sm00_jpg_0045c0a4,&DAT_0046487c,(uint *)&DAT_004654d8,
                            (ushort *)0x0,0);
    if (local_14 == (ushort *)0x0) {
      uVar1 = 1;
    }
    else {
      iVar2 = FUN_004236b5(DAT_00464880,(int)local_14,DAT_0046487c,DAT_004654d8);
      FUN_0044ac40(local_14);
      if (iVar2 == 0) {
        FUN_00423d9e(DAT_00464880,&DAT_0045c040,&local_18,&local_18,&DAT_00468ca0);
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
  }
  *unaff_FS_OFFSET = local_10;
  return uVar1;
}
