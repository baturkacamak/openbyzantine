// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x411b4b
// name: FUN_00411b4b
// ranges: 0x411b4b-0x411db0
// calls: 0x411b4b, 0x44ba00, 0x44bc20, 0x44c250, 0x44c370, 0x44c520
// port: ALL directory parsing [bounded]

FILE * __cdecl FUN_00411b4b(uint *param_1,int *param_2)

{
  int iVar1;
  int local_13c;
  int local_138;
  uint local_134 [3];
  int local_128;
  LONG local_124;
  int local_120;
  char *local_11c;
  FILE *local_118;
  int local_114;
  char local_110 [260];
  uint local_c;
  DWORD local_8;
  
  local_11c = FUN_0044c520((char *)param_1,'.');
  if (local_11c == (char *)0x0) {
    local_118 = (FILE *)0x0;
  }
  else {
    *local_11c = '\0';
    FUN_0044ba00(local_134,param_1);
    *local_11c = '.';
    local_11c = local_11c + 1;
    *param_2 = 0;
    _sprintf(local_110,s_engine__s_all_0045ea58,local_11c);
    local_118 = (FILE *)FUN_0044bc20(local_110,&DAT_0045ea68);
    if (local_118 == (FILE *)0x0) {
      local_118 = (FILE *)0x0;
    }
    else {
      __fread_lk((char *)&local_c,2,1,local_118);
      local_120 = 0;
      local_114 = -1;
      local_128 = 0;
      while ((local_128 < (int)(local_c & 0xffff) && (local_120 == 0))) {
        local_138 = -1;
        do {
          local_138 = local_138 + 1;
          iVar1 = _fgetc(local_118);
          local_110[local_138] = (char)iVar1;
          if ((local_118->_flag & 0x10) != 0) {
            local_120 = 1;
            break;
          }
        } while (local_110[local_138] != '\0');
        iVar1 = __strcmpi(local_110,(char *)local_134);
        if (iVar1 == 0) {
          local_114 = local_128;
          __fread_lk((char *)&local_124,4,1,local_118);
          __fread_lk((char *)&local_13c,4,1,local_118);
        }
        else {
          FUN_0044c250(local_118,8,1);
        }
        local_128 = local_128 + 1;
      }
      if ((local_120 == 0) && (local_114 != -1)) {
        local_8 = FUN_0044c370(&local_118->_ptr);
        FUN_0044c250(local_118,local_124,1);
        *param_2 = local_8 + local_13c;
      }
      else {
        _fclose(local_118);
        local_118 = (FILE *)0x0;
      }
    }
  }
  return local_118;
}
