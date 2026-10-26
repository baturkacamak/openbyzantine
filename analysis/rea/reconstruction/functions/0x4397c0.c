// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4397c0
// name: FUN_004397c0
// ranges: 0x4397c0-0x439957; 0x439989-0x439999
// calls: 0x40ad80, 0x413d50, 0x4397c0, 0x43d4d0, 0x44ac60

undefined4 * __thiscall
FUN_004397c0(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            int *param_5,undefined4 param_6)

{
  void *pvVar1;
  undefined4 *puVar2;
  undefined4 *unaff_FS_OFFSET;
  undefined4 *local_28;
  undefined4 *local_20;
  int local_18;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0043997f;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  pvVar1 = (void *)FUN_0044ac60(0x44);
  local_8 = 0;
  if (pvVar1 == (void *)0x0) {
    local_18 = 0;
  }
  else {
    local_18 = FUN_0040ad80(pvVar1,param_5);
  }
  local_8 = 0xffffffff;
  *(int *)((int)this + 4) = local_18;
  pvVar1 = (void *)FUN_0044ac60(0x9c);
  local_8 = 1;
  if (pvVar1 == (void *)0x0) {
    local_20 = (undefined4 *)0x0;
  }
  else {
    local_20 = FUN_00413d50(pvVar1,param_2,param_3,param_4,param_1);
  }
  local_8 = 0xffffffff;
  *(undefined4 **)((int)this + 8) = local_20;
  puVar2 = (undefined4 *)FUN_0044ac60(8);
  local_8 = 2;
  if (puVar2 == (undefined4 *)0x0) {
    local_28 = (undefined4 *)0x0;
  }
  else {
    local_28 = FUN_0043d4d0(puVar2);
  }
  *(undefined4 **)((int)this + 0xc) = local_28;
  *(undefined4 *)this = param_1;
  *(undefined4 *)((int)this + 0x14) = 0;
  *(undefined4 *)((int)this + 0x28) = 0;
  *(undefined4 *)((int)this + 0x60) = 0;
  *(undefined4 *)((int)this + 0x54) = 0;
  *(undefined4 *)((int)this + 0x18) = 0;
  *(undefined4 *)((int)this + 0x20) = 0;
  *(undefined4 *)((int)this + 0x30) = 0;
  *(undefined4 *)((int)this + 0x2c) = *(undefined4 *)((int)this + 0x30);
  *(undefined4 *)((int)this + 0x1c) = param_6;
  *(undefined4 *)((int)this + 0x10) = 0;
  *(undefined4 *)((int)this + 0x94) = 0;
  *(undefined4 *)((int)this + 0x24) = 0;
  *unaff_FS_OFFSET = local_10;
  return this;
}
