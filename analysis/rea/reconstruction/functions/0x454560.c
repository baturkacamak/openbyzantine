// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x454560
// name: __clearfp
// ranges: 0x454560-0x454576
// calls: -

/* Library Function - Single Match
    __clearfp
   
   Library: Visual Studio 1998 Release */

uint __cdecl __clearfp(void)

{
  uint uVar1;
  ushort in_FPUStatusWord;
  
  uVar1 = __abstract_sw(in_FPUStatusWord);
  return uVar1;
}
