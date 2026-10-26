// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f7be
// name: FUN_0041f7be
// ranges: 0x41f7be-0x41f80b
// calls: 0x41f7be, 0x44a460, 0x44bc20

void FUN_0041f7be(void)

{
  FILE *_File;
  
  _File = (FILE *)FUN_0044bc20(s_savegame_gamepref_000_0045fb0c,&DAT_0045fb08);
  if (_File != (FILE *)0x0) {
    FUN_0044a460(&DAT_0045f528,4,7,_File);
    _fclose(_File);
  }
  return;
}
