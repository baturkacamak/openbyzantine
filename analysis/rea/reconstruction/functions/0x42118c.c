// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42118c
// name: FUN_0042118c
// ranges: 0x42118c-0x42120b
// calls: 0x42118c, 0x44bc20, 0x44ce20

void __cdecl FUN_0042118c(FILE *param_1)

{
  FILE *_File;
  char *pcVar1;
  char local_54 [80];
  
  _File = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fc80);
  if (_File != (FILE *)0x0) {
    while ((_File->_flag & 0x10) == 0) {
      pcVar1 = FUN_0044ce20(local_54,0x50,_File);
      if (pcVar1 != (char *)0x0) {
        FID_conflict__fwprintf(param_1,(wchar_t *)&DAT_0045fc84,local_54);
      }
    }
    _fclose(_File);
  }
  return;
}
