// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408d3f
// name: FUN_00408d3f
// ranges: 0x408d3f-0x408dbc
// calls: 0x408d3f, 0x44a460

void __cdecl FUN_00408d3f(FILE *param_1)

{
  char *pcVar1;
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    pcVar1 = *(char **)(&DAT_00486a10 + local_8 * 4);
    if (pcVar1 == (char *)0x0) {
      _fputc(0xff,param_1);
    }
    else {
      _fputc(0,param_1);
      FUN_0044a460(pcVar1,0x68,1,param_1);
    }
  }
  return;
}
