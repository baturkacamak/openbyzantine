// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44bea0
// name: __flush
// ranges: 0x44bea0-0x44bf03
// calls: 0x44d8e0

/* Library Function - Single Match
    __flush
   
   Library: Visual Studio 1998 Release */

int __cdecl __flush(FILE *_File)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = 0;
  if ((((byte)_File->_flag & 3) == 2) && ((_File->_flag & 0x108U) != 0)) {
    uVar2 = (int)_File->_ptr - (int)_File->_base;
    if (0 < (int)uVar2) {
      uVar1 = FUN_0044d8e0(_File->_file,_File->_base,uVar2);
      if (uVar1 == uVar2) {
        if ((_File->_flag & 0x80U) != 0) {
          _File->_flag = _File->_flag & 0xfffffffd;
        }
      }
      else {
        _File->_flag = _File->_flag | 0x20;
        iVar3 = -1;
      }
    }
  }
  _File->_ptr = _File->_base;
  _File->_cnt = 0;
  return iVar3;
}
