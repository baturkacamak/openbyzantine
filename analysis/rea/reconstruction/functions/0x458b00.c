// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x458b00
// name: _atol
// ranges: 0x458b00-0x458ba3
// calls: -

/* Library Function - Single Match
    _atol
   
   Library: Visual Studio 1998 Release */

long __cdecl _atol(char *_Str)

{
  uint uVar1;
  uint uVar2;
  uint _C;
  int iVar3;
  byte *pbVar4;
  
  while( true ) {
    if (DAT_00462eec < 2) {
      uVar1 = *(ushort *)(PTR_DAT_00462ce0 + (uint)(byte)*_Str * 2) & 8;
    }
    else {
      uVar1 = __isctype((uint)(byte)*_Str,8);
    }
    if (uVar1 == 0) break;
    _Str = _Str + 1;
  }
  uVar1 = (uint)(byte)*_Str;
  pbVar4 = (byte *)(_Str + 1);
  if ((uVar1 == 0x2d) || (_C = uVar1, uVar1 == 0x2b)) {
    _C = (uint)*pbVar4;
    pbVar4 = (byte *)(_Str + 2);
  }
  iVar3 = 0;
  while( true ) {
    if (DAT_00462eec < 2) {
      uVar2 = *(ushort *)(PTR_DAT_00462ce0 + _C * 2) & 4;
    }
    else {
      uVar2 = __isctype(_C,4);
    }
    if (uVar2 == 0) break;
    iVar3 = (_C - 0x30) + iVar3 * 10;
    _C = (uint)*pbVar4;
    pbVar4 = pbVar4 + 1;
  }
  if (uVar1 == 0x2d) {
    return -iVar3;
  }
  return iVar3;
}
