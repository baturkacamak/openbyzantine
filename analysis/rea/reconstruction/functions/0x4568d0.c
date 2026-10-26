// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4568d0
// name: _isspace
// ranges: 0x4568d0-0x4568fc
// calls: -

/* Library Function - Single Match
    _isspace
   
   Library: Visual Studio 1998 Release */

int __cdecl _isspace(int _C)

{
  int iVar1;
  
  if (1 < DAT_00462eec) {
    iVar1 = __isctype(_C,8);
    return iVar1;
  }
  return *(ushort *)(PTR_DAT_00462ce0 + _C * 2) & 8;
}
