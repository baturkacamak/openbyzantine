// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d470
// name: _fputs
// ranges: 0x44d470-0x44d4c1
// calls: 0x44a460

/* Library Function - Single Match
    _fputs
   
   Library: Visual Studio 1998 Release */

int __cdecl _fputs(char *_Str,FILE *_File)

{
  char cVar1;
  int _Flag;
  uint uVar2;
  uint uVar3;
  char *pcVar4;
  
  uVar3 = 0xffffffff;
  pcVar4 = _Str;
  do {
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar4 + 1;
  } while (cVar1 != '\0');
  _Flag = __stbuf(_File);
  uVar2 = FUN_0044a460(_Str,1,~uVar3 - 1,_File);
  __ftbuf(_Flag,_File);
  return (uVar2 == ~uVar3 - 1) - 1;
}
