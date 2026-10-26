// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4212b4
// name: FUN_004212b4
// ranges: 0x4212b4-0x42130b
// calls: 0x4212b4, 0x44bc20

void __cdecl FUN_004212b4(undefined4 param_1,undefined4 param_2)

{
  FILE *_File;
  
  _File = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fcb4);
  if (_File != (FILE *)0x0) {
    FID_conflict__fwprintf(_File,(wchar_t *)s__d___d_0045fcb8,param_1,param_2);
    _fclose(_File);
  }
  return;
}
