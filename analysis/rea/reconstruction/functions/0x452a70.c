// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x452a70
// name: __inc
// ranges: 0x452a70-0x452a92
// calls: -

/* Library Function - Single Match
    __inc
   
   Library: Visual Studio 1998 Release */

uint __cdecl __inc(FILE *param_1)

{
  byte *pbVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = param_1->_cnt + -1;
  param_1->_cnt = iVar2;
  if (-1 < iVar2) {
    pbVar1 = (byte *)param_1->_ptr;
    param_1->_ptr = (char *)(pbVar1 + 1);
    return (uint)*pbVar1;
  }
  uVar3 = __filbuf(param_1);
  return uVar3;
}
