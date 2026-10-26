// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40d2a1
// name: FUN_0040d2a1
// ranges: 0x40d2a1-0x40d3ae
// calls: 0x4075a1, 0x40c624, 0x40d2a1, 0x40d5ff, 0x44bc20

undefined4 __cdecl FUN_0040d2a1(uint *param_1,undefined4 param_2)

{
  FILE *local_210;
  uint local_20c [65];
  uint local_108 [65];
  
  _sprintf((char *)local_108,s_avi__s_avi_0045dba4,param_1);
  local_210 = (FILE *)FUN_0044bc20((char *)local_108,&DAT_0045dbb0);
  if (local_210 == (FILE *)0x0) {
    _sprintf((char *)local_20c,s_dialog__s_0045dbb4,param_2);
    FUN_0040c624(local_108,local_20c,param_1,(uint *)&DAT_0045dbc0);
    local_210 = (FILE *)FUN_0044bc20((char *)local_108,&DAT_0045dbc8);
    if (local_210 == (FILE *)0x0) {
      return 0;
    }
  }
  _fclose(local_210);
  if (DAT_00460684 != 0) {
    FUN_004075a1(1);
    DAT_00460684 = 0;
  }
  FUN_0040d5ff(local_108,4,0,0);
  DAT_00460664 = 0x67;
  return 1;
}
