// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f1ac
// name: FUN_0041f1ac
// ranges: 0x41f1ac-0x41f21d
// calls: 0x41f1ac, 0x44bc20

bool __cdecl FUN_0041f1ac(char *param_1)

{
  FILE *_File;
  char *pcVar1;
  
  _File = (FILE *)FUN_0044bc20(PTR_s_savegame_btb_sav_0045f480,&DAT_0045fa54);
  if (_File != (FILE *)0x0) {
    pcVar1 = _strrchr(param_1,0x5c);
    FID_conflict__fwprintf(_File,(wchar_t *)&DAT_0045fa58,pcVar1 + 1);
    _fclose(_File);
  }
  return _File == (FILE *)0x0;
}
