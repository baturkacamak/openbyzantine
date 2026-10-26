// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420c51
// name: FUN_00420c51
// ranges: 0x420c51-0x420dc8
// calls: 0x420c51, 0x44bc20, 0x44ce20, 0x44d340

int FUN_00420c51(void)

{
  int iVar1;
  int local_70;
  int local_6c;
  int local_68;
  int local_64;
  char local_5c [80];
  int local_c;
  FILE *local_8;
  
  local_70 = 0;
  local_8 = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fc34);
  for (local_68 = 0; local_68 < 0x10; local_68 = local_68 + 1) {
    *(undefined4 *)(&DAT_0046c800 + local_68 * 4) = 0xffffffff;
  }
  if (local_8 == (FILE *)0x0) {
    local_70 = 0;
  }
  else {
    while ((local_8->_flag & 0x10) == 0) {
      iVar1 = FUN_0044d340(local_8,(byte *)s_Scene___d_Dialog___d_0045fc38);
      if (iVar1 == 2) {
        local_68 = 0;
        while ((local_68 < 0x20 &&
               (local_c = FUN_0044d340(local_8,(byte *)s__d___d_0045fc50), local_c == 2))) {
          if (local_64 != -1) {
            for (local_6c = 0;
                (local_6c < local_70 && (*(int *)(&DAT_0046c800 + local_6c * 4) != local_64));
                local_6c = local_6c + 1) {
            }
            if ((local_70 == local_6c) && (local_64 != -3)) {
              *(int *)(&DAT_0046c800 + local_70 * 4) = local_64;
              local_70 = local_70 + 1;
            }
          }
          local_68 = local_68 + 1;
        }
      }
      FUN_0044ce20(local_5c,0x50,local_8);
    }
    _fclose(local_8);
  }
  return local_70;
}
