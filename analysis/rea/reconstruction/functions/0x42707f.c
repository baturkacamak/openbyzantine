// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42707f
// name: FUN_0042707f
// ranges: 0x42707f-0x427134
// calls: 0x40ac38, 0x40ac43, 0x426261, 0x42707f, 0x44bc20

int FUN_0042707f(void)

{
  FILE *_File;
  int iVar1;
  uint local_c;
  int local_8;
  
  local_8 = 0;
  _File = (FILE *)FUN_0044bc20(s_ds9_swp_00460110,&DAT_0046010c);
  if (_File == (FILE *)0x0) {
    FUN_0040ac43();
    FUN_0040ac38();
    local_8 = 1;
  }
  else {
    for (local_c = 0; (int)local_c < 0x15e; local_c = local_c + 1) {
      iVar1 = FUN_00426261(local_c,_File);
      local_8 = local_8 + iVar1;
    }
    _fclose(_File);
  }
  return local_8;
}
