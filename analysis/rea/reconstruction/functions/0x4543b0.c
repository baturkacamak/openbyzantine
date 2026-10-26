// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4543b0
// name: _wctomb
// ranges: 0x4543b0-0x454441
// calls: -

/* Library Function - Single Match
    _wctomb
   
   Library: Visual Studio 1998 Release */

int __cdecl _wctomb(char *_MbCh,wchar_t _WCh)

{
  int iVar1;
  BOOL local_4;
  
  if (_MbCh == (char *)0x0) {
    return 0;
  }
  if (DAT_00463548 == 0) {
    if (0xff < (ushort)_WCh) {
      DAT_00462c74 = 0x2a;
      return -1;
    }
    *_MbCh = (char)_WCh;
    return 1;
  }
  local_4 = 0;
  iVar1 = WideCharToMultiByte(DAT_00463558,0x220,&_WCh,1,_MbCh,DAT_00462eec,(LPCSTR)0x0,&local_4);
  if ((iVar1 == 0) || (local_4 != 0)) {
    DAT_00462c74 = 0x2a;
    iVar1 = -1;
  }
  return iVar1;
}
