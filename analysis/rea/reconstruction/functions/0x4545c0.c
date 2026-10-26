// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4545c0
// name: __controlfp
// ranges: 0x4545c0-0x4545d7
// calls: -

/* Library Function - Single Match
    __controlfp
   
   Library: Visual Studio 1998 Release */

uint __cdecl __controlfp(uint _NewValue,uint _Mask)

{
  uint uVar1;
  
  uVar1 = __control87(_NewValue,_Mask & 0xfff7ffff);
  return uVar1;
}
