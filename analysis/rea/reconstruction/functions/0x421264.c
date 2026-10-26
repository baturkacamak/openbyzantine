// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x421264
// name: FUN_00421264
// ranges: 0x421264-0x4212b3
// calls: 0x421264, 0x44bc20

void FUN_00421264(void)

{
  FILE *_File;
  
  _File = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fca4);
  if (_File != (FILE *)0x0) {
    FID_conflict__fwprintf(_File,(wchar_t *)s_EndScene_0045fca8);
    _fclose(_File);
  }
  return;
}
