// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41db33
// name: FUN_0041db33
// ranges: 0x41db33-0x41dcc8
// calls: 0x411b4b, 0x41db33, 0x44ac40, 0x44ba00, 0x44bc20, 0x44c2f0, 0x44c580, 0x44ce20

void __cdecl FUN_0041db33(int param_1,int param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  uint local_120 [65];
  int local_1c;
  int local_18;
  uint *local_14;
  uint *local_10;
  char *local_c;
  FILE *local_8;
  
  local_14 = (uint *)FUN_0044c2f0(0x800);
  local_10 = (uint *)0x0;
  if (DAT_0045e7a8 == 0) {
    _sprintf((char *)local_120,s_engine__s_byz_0045f928,(&DAT_00485a10)[param_1]);
    local_8 = (FILE *)FUN_0044bc20((char *)local_120,&DAT_0045f938);
  }
  else {
    _sprintf((char *)local_120,s__s_byz_0045f920,(&DAT_00485a10)[param_1]);
    local_8 = FUN_00411b4b(local_120,&local_18);
  }
  if (local_8 != (FILE *)0x0) {
    for (local_1c = 0; local_1c <= param_2; local_1c = local_1c + 1) {
      local_10 = (uint *)FUN_0044ce20((char *)local_14,0x800,local_8);
    }
    _fclose(local_8);
  }
  if (local_10 == (uint *)0x0) {
    *(undefined1 *)local_14 = 0;
    local_10 = local_14;
    local_c = (char *)0x0;
  }
  else {
    local_c = FUN_0044c580(local_14);
    if ((local_c + -1)[(int)local_14] == '\n') {
      local_c = local_c + -1;
      local_c[(int)local_14] = '\0';
    }
    if ((local_c + -1)[(int)local_14] == '\r') {
      local_c = local_c + -1;
      local_c[(int)local_14] = '\0';
    }
  }
  uVar1 = FUN_0044c2f0((SIZE_T)(local_c + 1));
  *param_3 = uVar1;
  FUN_0044ba00((uint *)*param_3,local_10);
  FUN_0044ac40(local_14);
  return;
}
