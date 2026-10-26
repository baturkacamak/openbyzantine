// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x452ac0
// name: __whiteout
// ranges: 0x452ac0-0x452aea
// calls: -

/* Library Function - Single Match
    __whiteout
   
   Library: Visual Studio 1998 Release */

uint __cdecl __whiteout(int *param_1,FILE *param_2)

{
  uint _C;
  int iVar1;
  
  do {
    *param_1 = *param_1 + 1;
    _C = __inc(param_2);
    iVar1 = _isspace(_C);
  } while (iVar1 != 0);
  return _C;
}
