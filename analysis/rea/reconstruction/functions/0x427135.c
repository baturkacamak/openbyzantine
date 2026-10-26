// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427135
// name: FUN_00427135
// ranges: 0x427135-0x42722a
// calls: 0x42295d, 0x427135, 0x44a460

void __cdecl FUN_00427135(FILE *param_1)

{
  int local_10;
  LONG local_c;
  char *local_8;
  
  local_8 = DAT_0046019c;
  local_c = FUN_0042295d();
  FUN_0044a460((char *)&local_c,4,1,param_1);
  for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
    if (*(int *)(&DAT_00486220 + local_10 * 4) == 0) {
      _fputc(0xff,param_1);
    }
    else {
      _fputc(0,param_1);
      FUN_0044a460(*(char **)(&DAT_00486220 + local_10 * 4),0x3c,1,param_1);
    }
  }
  for (local_8 = DAT_0046019c; local_8 != (char *)0x0; local_8 = *(char **)(local_8 + 0x58)) {
    _fputc(0,param_1);
    FUN_0044a460(local_8,0x5c,1,param_1);
  }
  _fputc(0xff,param_1);
  return;
}
