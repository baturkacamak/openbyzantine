// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c660
// name: _fputc
// ranges: 0x44c660-0x44c68d
// calls: 0x44d790

/* Library Function - Single Match
    _fputc
   
   Library: Visual Studio 1998 Release */

int __cdecl _fputc(int _Ch,FILE *_File)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = _File->_cnt + -1;
  _File->_cnt = iVar2;
  if (-1 < iVar2) {
    *_File->_ptr = (char)_Ch;
    bVar1 = *_File->_ptr;
    _File->_ptr = _File->_ptr + 1;
    return (uint)bVar1;
  }
  uVar3 = FUN_0044d790(_Ch,_File);
  return uVar3;
}
