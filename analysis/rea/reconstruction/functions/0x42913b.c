// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42913b
// name: FUN_0042913b
// ranges: 0x42913b-0x42998b; 0x429ae9-0x429af6
// calls: 0x42913b, 0x42a7b0, 0x44ac60

void FUN_0042913b(void)

{
  void *pvVar1;
  undefined4 *unaff_FS_OFFSET;
  void *local_d0;
  void *local_c8;
  void *local_c0;
  void *local_b8;
  void *local_b0;
  void *local_a8;
  void *local_a0;
  void *local_98;
  void *local_90;
  void *local_88;
  void *local_80;
  void *local_78;
  void *local_70;
  void *local_68;
  void *local_60;
  void *local_58;
  void *local_50;
  void *local_48;
  void *local_40;
  void *local_38;
  void *local_30;
  void *local_28;
  void *local_20;
  void *local_18;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_00429adf;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0;
  if (pvVar1 == (void *)0x0) {
    local_18 = (void *)0x0;
  }
  else {
    local_18 = FUN_0042a7b0(pvVar1,1,2,9,0x15,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046ca98 = local_18;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 1;
  if (pvVar1 == (void *)0x0) {
    local_20 = (void *)0x0;
  }
  else {
    local_20 = FUN_0042a7b0(pvVar1,0,2,4,7,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046ca9c = local_20;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 2;
  if (pvVar1 == (void *)0x0) {
    local_28 = (void *)0x0;
  }
  else {
    local_28 = FUN_0042a7b0(pvVar1,0,1,0xe,0x17,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046caa0 = local_28;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 3;
  if (pvVar1 == (void *)0x0) {
    local_30 = (void *)0x0;
  }
  else {
    local_30 = FUN_0042a7b0(pvVar1,4,5,10,0x12,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046caa4 = local_30;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 4;
  if (pvVar1 == (void *)0x0) {
    local_38 = (void *)0x0;
  }
  else {
    local_38 = FUN_0042a7b0(pvVar1,3,5,1,7,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046caa8 = local_38;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 5;
  if (pvVar1 == (void *)0x0) {
    local_40 = (void *)0x0;
  }
  else {
    local_40 = FUN_0042a7b0(pvVar1,3,4,0xd,0x14,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046caac = local_40;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 6;
  if (pvVar1 == (void *)0x0) {
    local_48 = (void *)0x0;
  }
  else {
    local_48 = FUN_0042a7b0(pvVar1,7,8,0xb,0xf,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046cab0 = local_48;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 7;
  if (pvVar1 == (void *)0x0) {
    local_50 = (void *)0x0;
  }
  else {
    local_50 = FUN_0042a7b0(pvVar1,6,8,1,4,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cab4 = local_50;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 8;
  if (pvVar1 == (void *)0x0) {
    local_58 = (void *)0x0;
  }
  else {
    local_58 = FUN_0042a7b0(pvVar1,6,7,0xc,0x11,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046cab8 = local_58;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 9;
  if (pvVar1 == (void *)0x0) {
    local_60 = (void *)0x0;
  }
  else {
    local_60 = FUN_0042a7b0(pvVar1,0,0x15,10,0xb,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cabc = local_60;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 10;
  if (pvVar1 == (void *)0x0) {
    local_68 = (void *)0x0;
  }
  else {
    local_68 = FUN_0042a7b0(pvVar1,9,0xb,3,0x12,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cac0 = local_68;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0xb;
  if (pvVar1 == (void *)0x0) {
    local_70 = (void *)0x0;
  }
  else {
    local_70 = FUN_0042a7b0(pvVar1,9,10,6,0xf,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cac4 = local_70;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0xc;
  if (pvVar1 == (void *)0x0) {
    local_78 = (void *)0x0;
  }
  else {
    local_78 = FUN_0042a7b0(pvVar1,0xd,0xe,8,0x11,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cac8 = local_78;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0xd;
  if (pvVar1 == (void *)0x0) {
    local_80 = (void *)0x0;
  }
  else {
    local_80 = FUN_0042a7b0(pvVar1,0xc,0xe,5,0x14,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cacc = local_80;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0xe;
  if (pvVar1 == (void *)0x0) {
    local_88 = (void *)0x0;
  }
  else {
    local_88 = FUN_0042a7b0(pvVar1,0xc,0xd,2,0x17,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cad0 = local_88;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0xf;
  if (pvVar1 == (void *)0x0) {
    local_90 = (void *)0x0;
  }
  else {
    local_90 = FUN_0042a7b0(pvVar1,0x10,0x11,6,0xb,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046cad4 = local_90;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0x10;
  if (pvVar1 == (void *)0x0) {
    local_98 = (void *)0x0;
  }
  else {
    local_98 = FUN_0042a7b0(pvVar1,0xf,0x11,0x13,0x16,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cad8 = local_98;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0x11;
  if (pvVar1 == (void *)0x0) {
    local_a0 = (void *)0x0;
  }
  else {
    local_a0 = FUN_0042a7b0(pvVar1,0xf,0x10,8,0xc,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046cadc = local_a0;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0x12;
  if (pvVar1 == (void *)0x0) {
    local_a8 = (void *)0x0;
  }
  else {
    local_a8 = FUN_0042a7b0(pvVar1,0x13,0x14,3,10,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046cae0 = local_a8;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0x13;
  if (pvVar1 == (void *)0x0) {
    local_b0 = (void *)0x0;
  }
  else {
    local_b0 = FUN_0042a7b0(pvVar1,0x12,0x14,0x10,0x16,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046cae4 = local_b0;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0x14;
  if (pvVar1 == (void *)0x0) {
    local_b8 = (void *)0x0;
  }
  else {
    local_b8 = FUN_0042a7b0(pvVar1,0x12,0x13,5,0xd,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046cae8 = local_b8;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0x15;
  if (pvVar1 == (void *)0x0) {
    local_c0 = (void *)0x0;
  }
  else {
    local_c0 = FUN_0042a7b0(pvVar1,0x16,0x17,0,9,0x15,4);
  }
  local_8 = 0xffffffff;
  DAT_0046caec = local_c0;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0x16;
  if (pvVar1 == (void *)0x0) {
    local_c8 = (void *)0x0;
  }
  else {
    local_c8 = FUN_0042a7b0(pvVar1,0x15,0x17,0x10,0x13,0x14,0);
  }
  local_8 = 0xffffffff;
  DAT_0046caf0 = local_c8;
  pvVar1 = (void *)FUN_0044ac60(0x10);
  local_8 = 0x17;
  if (pvVar1 == (void *)0x0) {
    local_d0 = (void *)0x0;
  }
  else {
    local_d0 = FUN_0042a7b0(pvVar1,0x15,0x16,2,0xe,0x15,4);
  }
  DAT_0046caf4 = local_d0;
  *unaff_FS_OFFSET = local_10;
  return;
}
