// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450810
// name: __set_errno
// ranges: 0x450810-0x450839
// calls: -

/* Library Function - Single Match
    __set_errno
   
   Library: Visual Studio 1998 Release */

errno_t __cdecl __set_errno(int _Value)

{
  if (_Value == 1) {
    DAT_00462c74 = 0x21;
    return _Value;
  }
  if (1 < _Value) {
    if (3 < _Value) {
      return _Value;
    }
    DAT_00462c74 = 0x22;
  }
  return _Value;
}
