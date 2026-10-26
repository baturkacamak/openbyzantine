// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40892d
// name: FUN_0040892d
// ranges: 0x40892d-0x4089af
// calls: 0x40892d, 0x44a460

void __cdecl FUN_0040892d(FILE *param_1)

{
  char *pcVar1;
  int local_c;
  
  local_c = 0;
  while( true ) {
    if (0x15d < local_c) {
      return;
    }
    pcVar1 = *(char **)(&DAT_00486f90 + local_c * 4);
    if (pcVar1 == (char *)0x0) break;
    _fputc(0,param_1);
    FUN_0044a460(pcVar1,1,1,param_1);
    local_c = local_c + 1;
  }
  _fputc(0xff,param_1);
  return;
}
