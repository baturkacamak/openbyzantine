// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x452a30
// name: __hextodec
// ranges: 0x452a30-0x452a6d
// calls: -

/* Library Function - Single Match
    __hextodec
   
   Library: Visual Studio 1998 Release */

uint __cdecl __hextodec(uint param_1)

{
  uint uVar1;
  
  if (DAT_00462eec < 2) {
    uVar1 = *(ushort *)(PTR_DAT_00462ce0 + param_1 * 2) & 4;
  }
  else {
    uVar1 = __isctype(param_1,4);
  }
  if (uVar1 != 0) {
    return param_1;
  }
  return (param_1 & 0xffffffdf) - 7;
}
