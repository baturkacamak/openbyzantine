// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d6f0
// name: _getenv
// ranges: 0x44d6f0-0x44d785
// calls: -

/* Library Function - Single Match
    _getenv
   
   Library: Visual Studio 1998 Release */

char * __cdecl _getenv(char *_VarName)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int *piVar5;
  char *pcVar6;
  
  if (DAT_00462c9c == (int *)0x0) {
    if ((DAT_00462ca4 != 0) && (iVar2 = ___wtomb_environ(), iVar2 != 0)) {
      return (char *)0x0;
    }
    if (DAT_00462c9c == (int *)0x0) {
      return (char *)0x0;
    }
  }
  if (_VarName != (char *)0x0) {
    uVar3 = 0xffffffff;
    pcVar6 = _VarName;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    uVar3 = ~uVar3 - 1;
    iVar2 = *DAT_00462c9c;
    piVar5 = DAT_00462c9c;
    while (iVar2 != 0) {
      uVar4 = 0xffffffff;
      pcVar6 = (char *)*piVar5;
      do {
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        cVar1 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar1 != '\0');
      if (((uVar3 < ~uVar4 - 1) && (((uchar *)*piVar5)[uVar3] == '=')) &&
         (iVar2 = __mbsnbicoll((uchar *)*piVar5,(uchar *)_VarName,uVar3), iVar2 == 0)) {
        return (char *)(*piVar5 + 1 + uVar3);
      }
      piVar5 = piVar5 + 1;
      iVar2 = *piVar5;
    }
  }
  return (char *)0x0;
}
