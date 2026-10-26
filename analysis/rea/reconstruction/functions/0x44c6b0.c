// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c6b0
// name: _fgetc
// ranges: 0x44c6b0-0x44c6d0
// calls: -

/* Library Function - Single Match
    _fgetc
   
   Library: Visual Studio 1998 Release */

int __cdecl _fgetc(FILE *_File)

{
  byte bVar1;
  int iVar2;
  
  iVar2 = _File->_cnt + -1;
  _File->_cnt = iVar2;
  if (-1 < iVar2) {
    bVar1 = *_File->_ptr;
    _File->_ptr = _File->_ptr + 1;
    return (uint)bVar1;
  }
  iVar2 = __filbuf(_File);
  return iVar2;
}
