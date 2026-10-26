// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4566b0
// name: _mbtowc
// ranges: 0x4566b0-0x4567b2
// calls: -

/* Library Function - Single Match
    _mbtowc
   
   Library: Visual Studio 1998 Release */

int __cdecl _mbtowc(wchar_t *_DstCh,char *_SrcCh,size_t _SrcSizeInBytes)

{
  byte bVar1;
  int iVar2;
  
  if ((_SrcCh == (char *)0x0) || (_SrcSizeInBytes == 0)) {
    return 0;
  }
  bVar1 = *_SrcCh;
  if (bVar1 == 0) {
    if (_DstCh != (wchar_t *)0x0) {
      *_DstCh = L'\0';
    }
    return 0;
  }
  if (DAT_00463548 != 0) {
    if ((PTR_DAT_00462ce0[(uint)bVar1 * 2 + 1] & 0x80) == 0) {
      iVar2 = MultiByteToWideChar(DAT_00463558,9,_SrcCh,1,_DstCh,(uint)(_DstCh != (wchar_t *)0x0));
      if (iVar2 != 0) {
        return 1;
      }
      DAT_00462c74 = 0x2a;
      return -1;
    }
    if (((((int)DAT_00462eec < 2) || ((int)_SrcSizeInBytes < (int)DAT_00462eec)) ||
        (iVar2 = MultiByteToWideChar(DAT_00463558,9,_SrcCh,DAT_00462eec,_DstCh,
                                     (uint)(_DstCh != (wchar_t *)0x0)), iVar2 == 0)) &&
       ((_SrcSizeInBytes < DAT_00462eec || (_SrcCh[1] == '\0')))) {
      DAT_00462c74 = 0x2a;
      return -1;
    }
    return DAT_00462eec;
  }
  if (_DstCh != (wchar_t *)0x0) {
    *_DstCh = (ushort)bVar1;
  }
  return 1;
}
