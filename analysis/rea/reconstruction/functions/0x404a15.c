// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x404a15
// name: FUN_00404a15
// ranges: 0x404a15-0x404b96
// calls: 0x404a15, 0x44ba00, 0x44ba08, 0x44bc20, 0x44c000

undefined4 __cdecl FUN_00404a15(uint *param_1,short param_2,uint *param_3,int param_4)

{
  wchar_t local_820 [1024];
  char *local_20;
  FILE *local_1c;
  uint local_18 [5];
  
  FUN_0044ba00((uint *)local_820,param_1);
  if (DAT_0045c220 == 0) {
    local_20 = &DAT_0045d018;
  }
  else {
    local_20 = &DAT_0045d014;
  }
  local_1c = (FILE *)FUN_0044bc20("MSG.LOG",local_20);
  if (local_1c == (FILE *)0x0) {
    FUN_0044c000(1);
  }
  if (DAT_0045c220 != 0) {
    FID_conflict__fwprintf(local_1c,(wchar_t *)s_COLUMBUS_LOG_FILE_0045d01c);
    FID_conflict__fwprintf(local_1c,(wchar_t *)s__________________________________0045d030);
    DAT_0045c220 = 0;
  }
  if (param_2 != -0x29a) {
    _sprintf((char *)local_18,s_Line____d__0045d074,(int)param_2);
    FUN_0044ba08((uint *)local_820,local_18);
    FUN_0044ba08((uint *)local_820,param_3);
    FUN_0044ba08((uint *)local_820,(uint *)&DAT_0045d080);
  }
  if (param_4 != -0x29a) {
    _sprintf((char *)local_18,s_Data____d_0045d084,param_4);
    FUN_0044ba08((uint *)local_820,local_18);
  }
  FUN_0044ba08((uint *)local_820,(uint *)&DAT_0045d090);
  FID_conflict__fwprintf(local_1c,local_820);
  _fflush(local_1c);
  _fclose(local_1c);
  return 1;
}
