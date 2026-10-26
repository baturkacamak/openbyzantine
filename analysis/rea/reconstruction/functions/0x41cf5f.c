// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41cf5f
// name: FUN_0041cf5f
// ranges: 0x41cf5f-0x41d234
// calls: 0x40ac38, 0x40ac43, 0x40c624, 0x41199e, 0x41cf5f, 0x44ba00, 0x44ba08, 0x44bc20, 0x44c2f0, 0x4554c0

int __cdecl FUN_0041cf5f(uint *param_1)

{
  uint unaff_EDI;
  uint local_218 [65];
  FILE *local_114;
  uint local_110;
  uint local_10c [65];
  int local_8;
  
  local_8 = 0;
  DAT_00483624 = (char *)0x0;
  DAT_00483604 = (char *)0x0;
  DAT_0048242c = 0;
  DAT_004859d0 = -1;
  if (DAT_00485c6c == '\0') {
    FUN_0044ba00(local_10c,param_1);
    FUN_0044ba08(local_10c,(uint *)&DAT_0045f364);
    FUN_0044ba08(local_10c,param_1);
    FUN_0040c624(local_218,(uint *)s_PHONEMES_0045f370,local_10c,(uint *)&DAT_0045f368);
    local_114 = (FILE *)FUN_0044bc20((char *)local_218,&DAT_0045f37c);
    if (local_114 == (FILE *)0x0) {
      local_8 = 1;
      local_114 = (FILE *)0x0;
    }
    else {
      local_110 = FUN_0041199e(local_114);
      DAT_00483604 = (char *)FUN_0044c2f0(local_110 << 2);
      __fread_lk(DAT_00483604,local_110,4,local_114);
      _fclose(local_114);
      FUN_0040c624(local_218,(uint *)s_DIALOGUE_0045f388,local_10c,(uint *)&DAT_0045f380);
      local_114 = (FILE *)FUN_0044bc20((char *)local_218,&DAT_0045f394);
      if (local_114 == (FILE *)0x0) {
        local_8 = 1;
      }
      else {
        local_110 = FUN_0041199e(local_114);
        DAT_00483624 = (char *)FUN_0044c2f0(local_110 << 2);
        __fread_lk(DAT_00483624,local_110,4,local_114);
        _fclose(local_114);
      }
    }
    if (local_8 == 0) {
      FUN_0040c624(local_218,(uint *)s_PHONEMES_0045f3a0,local_10c,(uint *)&DAT_0045f398);
      DAT_0048242c = FUN_0044bc20((char *)local_218,&DAT_0045f3ac);
      if (DAT_0048242c == 0) {
        local_8 = 1;
      }
      else {
        FUN_0040c624(local_218,(uint *)s_DIALOGUE_0045f3b8,local_10c,(uint *)&DAT_0045f3b0);
        DAT_004859d0 = FUN_004554c0((LPCSTR)local_218,0x8000,unaff_EDI);
        if (DAT_004859d0 == -1) {
          local_8 = 1;
        }
      }
    }
    if (local_8 != 0) {
      FUN_0040ac43();
      FUN_0040ac38();
    }
  }
  else {
    local_8 = 1;
  }
  return local_8;
}
