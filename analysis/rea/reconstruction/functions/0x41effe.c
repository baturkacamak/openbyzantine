// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41effe
// name: FUN_0041effe
// ranges: 0x41effe-0x41f064
// calls: 0x41effe, 0x44bc20

void __cdecl FUN_0041effe(char *param_1)

{
  FILE *_File;
  int local_8;
  
  local_8 = 0;
  while( true ) {
    _sprintf(param_1,s_savegame__02d_btb_0045fa08,local_8);
    _File = (FILE *)FUN_0044bc20(param_1,&DAT_0045fa1c);
    if (_File == (FILE *)0x0) break;
    _fclose(_File);
    local_8 = local_8 + 1;
  }
  return;
}
