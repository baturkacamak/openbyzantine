// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450a80
// name: __sptype
// ranges: 0x450a80-0x450ae2
// calls: -

/* Library Function - Single Match
    __sptype
   
   Library: Visual Studio 1998 Release */

undefined4 __cdecl __sptype(int param_1,uint param_2)

{
  if ((param_2 == 0x7ff00000) && (param_1 == 0)) {
    return 1;
  }
  if ((param_2 == 0xfff00000) && (param_1 == 0)) {
    return 2;
  }
  if ((param_2._2_2_ & 0x7ff8) == 0x7ff8) {
    return 3;
  }
  if (((param_2._2_2_ & 0x7ff8) == 0x7ff0) && (((param_2 & 0x7ffff) != 0 || (param_1 != 0)))) {
    return 4;
  }
  return 0;
}
