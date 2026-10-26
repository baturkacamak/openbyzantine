// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450f70
// name: __ftbuf
// ranges: 0x450f70-0x450fb8
// calls: -

/* Library Function - Single Match
    __ftbuf
   
   Library: Visual Studio 1998 Release */

void __cdecl __ftbuf(int _Flag,FILE *_File)

{
  if (_Flag == 0) {
    if ((_File->_flag & 0x1000) != 0) {
      __flush(_File);
    }
  }
  else if ((_File->_flag & 0x1000) != 0) {
    __flush(_File);
    _File->_flag = _File->_flag & 0xffffeeff;
    _File->_bufsiz = 0;
    _File->_ptr = (char *)0x0;
    _File->_base = (char *)0x0;
    return;
  }
  return;
}
