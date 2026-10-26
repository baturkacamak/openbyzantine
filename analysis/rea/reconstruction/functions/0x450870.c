// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450870
// name: __errcode
// ranges: 0x450870-0x4508a1
// calls: -

/* Library Function - Single Match
    __errcode
   
   Library: Visual Studio 1998 Release */

int __cdecl __errcode(uint param_1)

{
  if ((param_1 & 0x20) != 0) {
    return 5;
  }
  if ((param_1 & 8) != 0) {
    return 1;
  }
  if ((param_1 & 4) != 0) {
    return 2;
  }
  if ((param_1 & 1) != 0) {
    return 3;
  }
  return (param_1 & 2) * 2;
}
