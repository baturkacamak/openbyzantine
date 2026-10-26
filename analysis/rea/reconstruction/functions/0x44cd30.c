// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44cd30
// name: FID_conflict:__toupper_lk
// ranges: 0x44cd30-0x44ce14
// calls: -

/* Library Function - Multiple Matches With Different Base Names
    __toupper_lk
    _toupper
   
   Library: Visual Studio 1998 Release */

int __cdecl FID_conflict___toupper_lk(int _C)

{
  uint uVar1;
  LPCSTR _LpSrcStr;
  int iVar2;
  int unaff_EBX;
  uint in_stack_fffffff8;
  byte local_4;
  byte local_3;
  undefined1 local_2;
  
  if (DAT_00463548 == (_locale_t)0x0) {
    if ((0x60 < _C) && (_C < 0x7b)) {
      _C = _C + -0x20;
    }
    return _C;
  }
  if (_C < 0x100) {
    if (DAT_00462eec < 2) {
      uVar1 = *(ushort *)(PTR_DAT_00462ce0 + _C * 2) & 2;
    }
    else {
      uVar1 = __isctype(_C,2);
    }
    if (uVar1 == 0) {
      return _C;
    }
  }
  local_4 = (byte)((uint)_C >> 8);
  if ((PTR_DAT_00462ce0[(uint)local_4 * 2 + 1] & 0x80) == 0) {
    _LpSrcStr = (LPCSTR)0x1;
    local_3 = 0;
    local_4 = (byte)_C;
  }
  else {
    _LpSrcStr = (LPCSTR)0x2;
    local_2 = 0;
    local_3 = (byte)_C;
  }
  iVar2 = ___crtLCMapStringA(DAT_00463548,(LPCWSTR)0x200,(DWORD)&local_4,_LpSrcStr,
                             (int)&stack0xfffffff8,(LPSTR)0x3,0,unaff_EBX,in_stack_fffffff8);
  if (iVar2 == 0) {
    return _C;
  }
  if (iVar2 == 1) {
    return in_stack_fffffff8 & 0xff;
  }
  return in_stack_fffffff8 & 0xffff;
}
