// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40b084
// name: FUN_0040b084
// ranges: 0x40b084-0x40b262
// calls: 0x40b084

undefined4 __thiscall
FUN_0040b084(void *this,undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  float10 fVar1;
  float10 extraout_ST0;
  longlong lVar2;
  undefined4 local_28;
  undefined1 local_24 [4];
  undefined4 local_20;
  undefined4 local_1c;
  float local_18;
  undefined4 local_14;
  int local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  *(undefined4 *)((int)this + 0x20) = 0;
  *(undefined4 *)((int)this + 8) = param_1;
  *(undefined4 *)((int)this + 0x18) = param_2;
  *(undefined4 *)((int)this + 0x2c) = 0;
  *(undefined4 *)((int)this + 0x38) = 0;
  *(undefined4 *)((int)this + 0x40) = 0;
  *(undefined4 *)((int)this + 0x3c) = *(undefined4 *)((int)this + 0x40);
  *(undefined4 *)((int)this + 0x24) = 0;
  if (DAT_00462ae8 == 0) {
    fVar1 = (float10)(uint)(*(int *)((int)this + 0x18) * param_3) / (float10)5880.0;
  }
  else {
    __adj_fdiv_m32(0x45b7c000);
    fVar1 = extraout_ST0;
  }
  local_18 = (float)fVar1;
  _ceil((double)local_18);
  lVar2 = __ftol();
  *(int *)((int)this + 0x1c) = (int)lVar2;
  *(undefined4 *)((int)this + 0x28) = 0;
  local_10 = (**(code **)(**(int **)((int)this + 4) + 0x2c))
                       (*(undefined4 *)((int)this + 4),0,*(undefined4 *)((int)this + 0x14),&local_8,
                        &local_1c,&local_c,local_24,0);
  if (local_10 == 0) {
    local_20 = AVIStreamRead(*(undefined4 *)((int)this + 8),0,
                             (uint)(param_4 * 0x16f8) / *(uint *)((int)this + 0x18),local_8,local_1c
                             ,&local_14,&local_28);
    local_10 = (**(code **)(**(int **)((int)this + 4) + 0x4c))
                         (*(undefined4 *)((int)this + 4),local_8,local_14,local_c,0);
    *(undefined4 *)((int)this + 0x30) = local_28;
    *(int *)((int)this + 0x34) = param_4;
    *(undefined4 *)((int)this + 0x38) = local_14;
    if (*(uint *)((int)this + 0x14) <= *(uint *)((int)this + 0x38)) {
      *(int *)((int)this + 0x38) = *(int *)((int)this + 0x38) - *(int *)((int)this + 0x14);
    }
    if (*(uint *)((int)this + 0x1c) <= *(uint *)((int)this + 0x34)) {
      *(undefined4 *)((int)this + 0x28) = 1;
    }
    (**(code **)(**(int **)((int)this + 4) + 0x34))(*(undefined4 *)((int)this + 4),0);
  }
  else {
    local_20 = 0xffffffff;
  }
  return local_20;
}
