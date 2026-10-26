// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44cf70
// name: _sscanf
// ranges: 0x44cf70-0x44cfb3
// calls: 0x451d90

/* Library Function - Single Match
    _sscanf
   
   Library: Visual Studio 1998 Release */

int __cdecl _sscanf(char *_Src,char *_Format,...)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  FILE local_20;
  
  uVar3 = 0xffffffff;
  local_20._flag = 0x49;
  local_20._base = _Src;
  local_20._ptr = _Src;
  do {
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    cVar1 = *_Src;
    _Src = _Src + 1;
  } while (cVar1 != '\0');
  local_20._cnt = ~uVar3 - 1;
  iVar2 = FUN_00451d90(&local_20,(byte *)_Format,(undefined4 *)&stack0x0000000c);
  return iVar2;
}
