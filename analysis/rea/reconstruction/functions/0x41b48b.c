// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41b48b
// name: FUN_0041b48b
// ranges: 0x41b48b-0x41b673
// calls: 0x41b414, 0x41b48b

DWORD __thiscall FUN_0041b48b(void *this,undefined4 param_1,undefined4 param_2,int *param_3)

{
  DWORD DVar1;
  HDC local_14;
  undefined4 local_10;
  int local_c;
  DWORD local_8;
  
  if (*(int *)((int)this + 0x10) == 0) {
    local_8 = 0;
  }
  else {
    local_10 = *(undefined4 *)((int)this + 0x2c);
    local_c = *(int *)((int)this + 0x30);
    local_8 = FUN_0041b414(*(undefined4 *)this,param_2,*(undefined4 *)((int)this + 0x14),param_1,
                           *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                           *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                           *(undefined4 *)((int)this + 0x18),*(undefined4 *)((int)this + 0x1b8),
                           local_10,(*(int *)((int)this + 0x58) - local_c) -
                                    (*(int *)((int)this + 0x48) / *(int *)((int)this + 0x38)) *
                                    *(int *)((int)this + 0x38),*(undefined4 *)((int)this + 0x34),
                           *(undefined4 *)((int)this + 0x38));
    if (local_8 == 0) {
      (**(code **)(*param_3 + 0x44))(param_3,&local_14);
      if ((*(int *)((int)this + 0x44) == *(int *)((int)this + 0x34)) &&
         (*(int *)((int)this + 0x48) == *(int *)((int)this + 0x38))) {
        local_8 = BitBlt(local_14,*(int *)((int)this + 0x4c),*(int *)((int)this + 0x50),
                         *(int *)((int)this + 0x54),*(int *)((int)this + 0x58),
                         *(HDC *)((int)this + 0x1c8),0,0,0xcc0020);
        if (local_8 == 0) {
          DVar1 = GetLastError();
          return DVar1;
        }
      }
      else {
        local_8 = StretchBlt(local_14,*(int *)((int)this + 0x3c),*(int *)((int)this + 0x40),
                             *(int *)((int)this + 0x44),*(int *)((int)this + 0x48),
                             *(HDC *)((int)this + 0x1c8),*(int *)((int)this + 0x2c),
                             *(int *)((int)this + 0x30),*(int *)((int)this + 0x34),
                             *(int *)((int)this + 0x38),0xcc0020);
        if (local_8 == 0) {
          DVar1 = GetLastError();
          return DVar1;
        }
      }
      GdiFlush();
      (**(code **)(*param_3 + 0x68))(param_3,local_14);
      local_8 = 0;
    }
  }
  return local_8;
}
