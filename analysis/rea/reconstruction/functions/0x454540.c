// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x454540
// name: __statusfp
// ranges: 0x454540-0x454555
// calls: -

/* Library Function - Single Match
    __statusfp
   
   Library: Visual Studio 1998 Release */

uint __cdecl __statusfp(void)

{
  uint uVar1;
  ushort in_FPUStatusWord;
  
  uVar1 = __abstract_sw(in_FPUStatusWord);
  return uVar1;
}
