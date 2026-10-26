// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c520
// name: FUN_0044c520
// ranges: 0x44c520-0x44c537
// calls: 0x44c520

char * __cdecl FUN_0044c520(char *param_1,char param_2)

{
  while( true ) {
    if (param_2 == *param_1) {
      return param_1;
    }
    if (*param_1 == '\0') break;
    param_1 = param_1 + 1;
  }
  return (char *)0x0;
}
