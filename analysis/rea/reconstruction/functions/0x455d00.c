// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x455d00
// name: __getpath
// ranges: 0x455d00-0x455d83
// calls: -

/* Library Function - Single Match
    __getpath
   
   Library: Visual Studio 1998 Release */

char * __cdecl __getpath(char *_Src,char *_Dst,size_t _SizeInChars)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  
  cVar1 = *_Src;
  while (cVar1 == ';') {
    _Src = _Src + 1;
    cVar1 = *_Src;
  }
  iVar2 = _SizeInChars - 1;
  pcVar3 = _Src;
  pcVar4 = _Src;
  if (iVar2 != 0) {
    cVar1 = *_Src;
    while ((cVar1 != '\0' && (cVar1 = *pcVar3, cVar1 != ';'))) {
      if (cVar1 == '\"') {
        pcVar3 = pcVar3 + 1;
        while ((cVar1 = *pcVar3, cVar1 != '\0' && (cVar1 != '\"'))) {
          *_Dst = cVar1;
          pcVar3 = pcVar3 + 1;
          _Dst = _Dst + 1;
          iVar2 = iVar2 + -1;
          pcVar4 = pcVar3;
          if (iVar2 == 0) goto LAB_00455d6d;
        }
        if (*pcVar3 == '\0') break;
        pcVar3 = pcVar3 + 1;
      }
      else {
        *_Dst = cVar1;
        pcVar3 = pcVar3 + 1;
        _Dst = _Dst + 1;
        iVar2 = iVar2 + -1;
        pcVar4 = pcVar3;
        if (iVar2 == 0) goto LAB_00455d6d;
      }
      cVar1 = *pcVar3;
    }
    cVar1 = *pcVar3;
    while (pcVar4 = _Src, cVar1 == ';') {
      pcVar3 = pcVar3 + 1;
      cVar1 = *pcVar3;
    }
  }
LAB_00455d6d:
  *_Dst = '\0';
  return (char *)((pcVar3 == pcVar4) - 1 & (uint)pcVar3);
}
