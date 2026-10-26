// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f5e0
// name: FUN_0041f5e0
// ranges: 0x41f5e0-0x41f752
// calls: 0x41f5e0, 0x44bc20, 0x44ce20, 0x44d360, 0x44d3a0

uint __cdecl FUN_0041f5e0(int param_1)

{
  FILE *_File;
  FILE *_File_00;
  char *pcVar1;
  uint uVar2;
  uint local_60;
  char local_5c [80];
  int local_c;
  int local_8;
  
  local_60 = 0;
  local_8 = _sscanf((char *)(&DAT_0046c660)[param_1],&DAT_0045fab0,&param_1);
  if (local_8 == 1) {
    _File = (FILE *)FUN_0044bc20(PTR_s_savegame_btb_sav_0045f480,&DAT_0045fab4);
    if (_File == (FILE *)0x0) {
      local_60 = 1;
    }
    else {
      _File_00 = (FILE *)FUN_0044bc20(s_savegame_jnk_sav_0045fabc,&DAT_0045fab8);
      if (_File_00 == (FILE *)0x0) {
        _fclose(_File);
        local_60 = 1;
      }
      else {
        while ((_File->_flag & 0x10) == 0) {
          pcVar1 = FUN_0044ce20(local_5c,0x50,_File);
          if (pcVar1 != (char *)0x0) {
            local_8 = _sscanf(local_5c,&DAT_0045fad0,&local_c);
            if (local_8 != 1) {
              local_60 = 1;
              break;
            }
            if (local_c != param_1) {
              FID_conflict__fwprintf(_File_00,(wchar_t *)&DAT_0045fad4,local_5c);
            }
          }
        }
        _fclose(_File);
        _fclose(_File_00);
        if (local_60 == 0) {
          local_60 = FUN_0044d360(PTR_s_savegame_btb_sav_0045f480);
          uVar2 = FUN_0044d3a0(s_savegame_jnk_sav_0045fad8,PTR_s_savegame_btb_sav_0045f480);
          local_60 = local_60 | uVar2;
        }
      }
    }
  }
  else {
    local_60 = 1;
  }
  return local_60;
}
