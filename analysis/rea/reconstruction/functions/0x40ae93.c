// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ae93
// name: FUN_0040ae93
// ranges: 0x40ae93-0x40b083
// calls: 0x40ae93

undefined4 __thiscall FUN_0040ae93(void *this,undefined4 param_1,undefined4 param_2,int param_3)

{
  float10 fVar1;
  float10 extraout_ST0;
  longlong lVar2;
  undefined4 local_28;
  undefined1 local_24 [4];
  undefined4 local_20;
  undefined4 local_1c;
  float local_18;
  uint local_14;
  int local_10;
  undefined4 local_c;
  int local_8;
  
  *(undefined4 *)((int)this + 0x20) = 1;
  *(undefined4 *)((int)this + 8) = param_1;
  *(undefined4 *)((int)this + 0x18) = param_2;
  *(undefined4 *)((int)this + 0x2c) = 1;
  *(undefined4 *)((int)this + 0x38) = 0;
  *(undefined4 *)((int)this + 0x40) = 0;
  *(undefined4 *)((int)this + 0x3c) = *(undefined4 *)((int)this + 0x40);
  *(undefined4 *)((int)this + 0x24) = 0;
  if (DAT_00462ae8 == 0) {
    fVar1 = (float10)(uint)(*(int *)((int)this + 0x18) * param_3) / (float10)88200.0;
  }
  else {
    __adj_fdiv_m32(0x47ac4400);
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
                             *(uint *)((int)this + 0x14) / *(uint *)((int)this + 0x18),local_8,
                             local_1c,&local_14,&local_28);
    if (local_14 < *(uint *)((int)this + 0x14)) {
      OutputDebugStringA(s_Size_is_less_than_a_full_segment_0045da08);
      _memset((void *)(local_14 + local_8),0,*(int *)((int)this + 0x14) - local_14);
      *(undefined4 *)((int)this + 0x28) = 1;
    }
    local_10 = (**(code **)(**(int **)((int)this + 4) + 0x4c))
                         (*(undefined4 *)((int)this + 4),local_8,local_14,local_c,0);
    *(undefined4 *)((int)this + 0x30) = local_28;
    *(undefined4 *)((int)this + 0x34) = 2;
    (**(code **)(**(int **)((int)this + 4) + 0x34))(*(undefined4 *)((int)this + 4),0);
    if (*(uint *)((int)this + 0x1c) <= *(uint *)((int)this + 0x34)) {
      OutputDebugStringA(s______DSound__Timer_Done_Writing_0045da2c);
      *(undefined4 *)((int)this + 0x28) = 1;
    }
  }
  else {
    local_20 = 0xffffffff;
  }
  return local_20;
}
