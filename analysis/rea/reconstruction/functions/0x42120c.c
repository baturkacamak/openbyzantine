// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42120c
// name: FUN_0042120c
// ranges: 0x42120c-0x421263
// calls: 0x42120c, 0x44bc20

void __cdecl FUN_0042120c(undefined4 param_1,undefined4 param_2)

{
  FILE *_File;
  
  _File = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fc88);
  if (_File != (FILE *)0x0) {
    FID_conflict__fwprintf(_File,(wchar_t *)s_Scene___d_Dialog___d_0045fc8c,param_1,param_2);
    _fclose(_File);
  }
  return;
}
