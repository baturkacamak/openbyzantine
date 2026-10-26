// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426fc4
// name: FUN_00426fc4
// ranges: 0x426fc4-0x42707e
// calls: 0x40ac38, 0x40ac43, 0x40fbf6, 0x425218, 0x426fc4, 0x44bc20

int FUN_00426fc4(void)

{
  FILE *_File;
  int iVar1;
  int local_c;
  int local_8;
  
  local_8 = 0;
  _File = (FILE *)FUN_0044bc20(s_ds9_swp_004600c0,&DAT_004600bc);
  if (_File == (FILE *)0x0) {
    FUN_0040ac43();
    FUN_0040ac38();
    local_8 = 1;
  }
  else {
    for (local_c = 0; local_c < 0x15e; local_c = local_c + 1) {
      iVar1 = FUN_00425218(local_c,_File);
      local_8 = local_8 + iVar1;
    }
    _fclose(_File);
    FUN_0040fbf6();
  }
  return local_8;
}
