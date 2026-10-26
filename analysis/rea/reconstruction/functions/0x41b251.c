// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41b251
// name: FUN_0041b251
// ranges: 0x41b251-0x41b413
// calls: 0x41b251, 0x41b414

DWORD __thiscall FUN_0041b251(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  DWORD DVar1;
  BOOL BVar2;
  
  if (*(int *)((int)this + 0x10) == 0) {
    DVar1 = 0;
  }
  else {
    DVar1 = FUN_0041b414(*(undefined4 *)this,param_3,*(undefined4 *)((int)this + 0x14),param_1,
                         *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                         *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                         *(undefined4 *)((int)this + 0x18),param_2,*(undefined4 *)((int)this + 0x2c)
                         ,(*(int *)((int)this + 0x58) - *(int *)((int)this + 0x30)) -
                          (*(int *)((int)this + 0x48) / *(int *)((int)this + 0x38)) *
                          *(int *)((int)this + 0x38),*(undefined4 *)((int)this + 0x34),
                         *(undefined4 *)((int)this + 0x38));
    if (DVar1 == 0) {
      if ((*(int *)((int)this + 0x44) == *(int *)((int)this + 0x34)) &&
         (*(int *)((int)this + 0x48) == *(int *)((int)this + 0x38))) {
        BVar2 = BitBlt(*(HDC *)((int)this + 4),*(int *)((int)this + 0x4c),*(int *)((int)this + 0x50)
                       ,*(int *)((int)this + 0x54),*(int *)((int)this + 0x58),
                       *(HDC *)((int)this + 0x1c8),0,0,0xcc0020);
        if (BVar2 == 0) {
          DVar1 = GetLastError();
          return DVar1;
        }
      }
      else {
        BVar2 = StretchBlt(*(HDC *)((int)this + 4),*(int *)((int)this + 0x3c),
                           *(int *)((int)this + 0x40),*(int *)((int)this + 0x44),
                           *(int *)((int)this + 0x48),*(HDC *)((int)this + 0x1c8),
                           *(int *)((int)this + 0x2c),*(int *)((int)this + 0x30),
                           *(int *)((int)this + 0x34),*(int *)((int)this + 0x38),0xcc0020);
        if (BVar2 == 0) {
          DVar1 = GetLastError();
          return DVar1;
        }
      }
      GdiFlush();
      DVar1 = 0;
    }
  }
  return DVar1;
}
