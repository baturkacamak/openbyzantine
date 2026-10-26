// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x459160
// name: __strdup
// ranges: 0x459160-0x459185
// calls: 0x44ba00, 0x44c2f0, 0x44c580

/* Library Function - Single Match
    __strdup
   
   Library: Visual Studio 1998 Release */

char * __cdecl __strdup(char *_Src)

{
  char *pcVar1;
  uint *puVar2;
  uint *puVar3;
  
  pcVar1 = FUN_0044c580((uint *)_Src);
  puVar2 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar1 + 1));
  puVar3 = (uint *)0x0;
  if (puVar2 != (uint *)0x0) {
    puVar3 = FUN_0044ba00(puVar2,(uint *)_Src);
  }
  return (char *)puVar3;
}
