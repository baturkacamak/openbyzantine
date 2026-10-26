// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4210af
// name: FUN_004210af
// ranges: 0x4210af-0x42118b
// calls: 0x4210af, 0x44bc20, 0x44d360

void __cdecl FUN_004210af(FILE *param_1)

{
  int iVar1;
  char local_34 [40];
  FILE *local_c;
  int local_8;
  
  local_c = (FILE *)0x0;
  FUN_0044d360(PTR_s_savegame_notebook_000_0045f4e0);
  if ((param_1 == (FILE *)0x0) ||
     (local_c = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fc78),
     local_c == (FILE *)0x0)) {
    return;
  }
  do {
    local_8 = 0;
    do {
      iVar1 = _fgetc(param_1);
      local_34[local_8] = (char)iVar1;
      iVar1 = local_8;
      if ((param_1->_flag & 0x10) != 0) break;
      local_8 = local_8 + 1;
    } while (local_34[iVar1] != '\n');
    if ((param_1->_flag & 0x10) != 0) {
      _fclose(local_c);
      return;
    }
    local_34[local_8] = '\0';
    FID_conflict__fwprintf(local_c,(wchar_t *)&DAT_0045fc7c,local_34);
  } while( true );
}
