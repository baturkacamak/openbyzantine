// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x413ac0
// name: FUN_00413ac0
// ranges: 0x413ac0-0x413b5a; 0x413b72-0x413b80
// calls: 0x413ac0, 0x4397c0, 0x44ac60

bool __cdecl
FUN_00413ac0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            int *param_5,undefined4 param_6)

{
  void *this;
  undefined4 *unaff_FS_OFFSET;
  undefined4 *local_18;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_00413b68;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  this = (void *)FUN_0044ac60(0x98);
  local_8 = 0;
  if (this == (void *)0x0) {
    local_18 = (undefined4 *)0x0;
  }
  else {
    local_18 = FUN_004397c0(this,param_1,param_2,param_3,param_4,param_5,param_6);
  }
  DAT_0046c4dc = local_18;
  *unaff_FS_OFFSET = local_10;
  return local_18 == (undefined4 *)0x0;
}
