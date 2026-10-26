// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41ae57
// name: FUN_0041ae57
// ranges: 0x41ae57-0x41b063
// calls: 0x41ae57, 0x41b064, 0x41b0db

DWORD __thiscall FUN_0041ae57(void *this,int *param_1)

{
  HDC pHVar1;
  HBITMAP pHVar2;
  HGDIOBJ pvVar3;
  undefined4 local_c;
  DWORD local_8;
  
  (**(code **)(*param_1 + 0x44))(param_1,&local_c);
  *(undefined4 *)((int)this + 4) = local_c;
  pHVar1 = CreateCompatibleDC(*(HDC *)((int)this + 4));
  *(HDC *)((int)this + 0x1c8) = pHVar1;
  local_8 = FUN_0041b0db(*(undefined4 *)this,0,*(undefined4 *)((int)this + 0x14),0,
                         *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                         *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                         *(undefined4 *)((int)this + 0x18),0,*(undefined4 *)((int)this + 0x2c),
                         *(undefined4 *)((int)this + 0x30),*(undefined4 *)((int)this + 0x34),
                         *(undefined4 *)((int)this + 0x38));
  if (local_8 == 0) {
    (**(code **)(*param_1 + 0x68))(param_1,local_c);
    local_8 = FUN_0041b064(*(undefined4 *)this,0,*(undefined4 *)((int)this + 0x14),0,
                           *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                           *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                           *(undefined4 *)((int)this + 0x18),0,*(undefined4 *)((int)this + 0x2c),
                           *(undefined4 *)((int)this + 0x30),*(undefined4 *)((int)this + 0x34),
                           *(undefined4 *)((int)this + 0x38));
    if (local_8 == 0) {
      *(undefined4 *)((int)this + 0x1d4) = 0;
      *(undefined4 *)((int)this + 0x1d0) = *(undefined4 *)((int)this + 0x1d4);
      *(undefined4 *)((int)this + 0x1bc) = *(undefined4 *)((int)this + 0x1d0);
      pHVar2 = CreateDIBSection(*(HDC *)((int)this + 4),*(BITMAPINFO **)((int)this + 0x18),0,
                                (void **)((int)this + 0x1b8),(HANDLE)0x0,0);
      *(HBITMAP *)((int)this + 0x1b4) = pHVar2;
      if (*(int *)((int)this + 0x1b4) == 0) {
        local_8 = GetLastError();
      }
      if ((*(int *)((int)this + 0x1b4) == 0) || (*(int *)((int)this + 0x1b8) == 0)) {
        local_8 = 1;
      }
      else {
        pvVar3 = SelectObject(*(HDC *)((int)this + 0x1c8),*(HGDIOBJ *)((int)this + 0x1b4));
        *(HGDIOBJ *)((int)this + 0x1cc) = pvVar3;
        SetStretchBltMode(*(HDC *)((int)this + 4),3);
        *(undefined4 *)((int)this + 0x10) = 1;
        local_8 = 0;
      }
    }
  }
  return local_8;
}
