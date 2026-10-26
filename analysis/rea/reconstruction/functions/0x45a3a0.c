// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x45a3a0
// name: write_multi_char
// ranges: 0x45a3a0-0x45a3c8
// calls: -

/* Library Function - Single Match
    _write_multi_char
   
   Library: Visual Studio 1998 Release */

void __cdecl write_multi_char(byte param_1,int param_2,int *param_3)

{
  do {
    if (param_2 < 1) {
      return;
    }
    write_char(param_1,param_3);
    param_2 = param_2 + -1;
  } while (*param_3 != -1);
  return;
}
