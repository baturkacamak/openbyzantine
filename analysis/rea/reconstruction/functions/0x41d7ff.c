// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41d7ff
// name: FUN_0041d7ff
// ranges: 0x41d7ff-0x41db32
// calls: 0x41d45a, 0x41d7ff, 0x439730, 0x44ba00, 0x44bc20, 0x44c2f0, 0x44c580, 0x44ce20, 0x44d2d0

undefined4 FUN_0041d7ff(void)

{
  undefined2 uVar1;
  undefined4 uVar2;
  undefined2 extraout_var;
  undefined2 extraout_var_00;
  undefined2 extraout_var_01;
  undefined2 extraout_var_02;
  undefined2 extraout_var_03;
  undefined2 extraout_var_04;
  undefined2 extraout_var_05;
  uint local_168 [65];
  int local_64;
  char local_60 [80];
  uint *local_10;
  FILE *local_c;
  char *local_8;
  
  FUN_0044ba00(local_168,(uint *)s_setupart_messages_txt_0045f8c4);
  local_c = (FILE *)FUN_0044bc20((char *)local_168,&DAT_0045f8dc);
  if (local_c == (FILE *)0x0) {
    uVar2 = 1;
  }
  else {
    for (local_64 = 0; local_64 < 0x2d; local_64 = local_64 + 1) {
      local_10 = (uint *)FUN_0044ce20(local_60,0x50,local_c);
      if (0xc < local_64) {
        if (local_10 == (uint *)0x0) {
          return 1;
        }
        local_8 = FUN_0044c580(local_10);
        if (local_64 < 0x1c) {
          (local_8 + -1)[(int)local_10] = '\0';
        }
        else {
          (local_8 + -1)[(int)local_10] = ':';
        }
        if (local_64 == 0x1c) {
          DAT_0045f4e4 = (uint *)FUN_0044c2f0((SIZE_T)(local_8 + 1));
          FUN_0044ba00(DAT_0045f4e4,local_10);
        }
        else if ((local_64 < 0xd) || (0x1a < local_64)) {
          if (0x1c < local_64) {
            uVar2 = FUN_0044c2f0((SIZE_T)(local_8 + 1));
            *(undefined4 *)(&DAT_0046c7cc + local_64 * 4) = uVar2;
            FUN_0044ba00(*(uint **)(&DAT_0046c7cc + local_64 * 4),local_10);
          }
        }
        else {
          uVar2 = FUN_0044c2f0((SIZE_T)(local_8 + 1));
          (&DAT_0046c854)[local_64] = (uint *)uVar2;
          FUN_0044ba00((&DAT_0046c854)[local_64],local_10);
        }
      }
    }
    _fclose(local_c);
    uVar1 = FUN_00439730(s_sCujem_0045f8e0);
    if (CONCAT22(extraout_var,uVar1) != 1) {
      DAT_0046c85c = FUN_0044d2d0(DAT_0046c85c,0x50);
      FUN_0041d45a(DAT_0046c85c,0x2d);
    }
    uVar1 = FUN_00439730(s_ssintro_0045f8e8);
    if (CONCAT22(extraout_var_00,uVar1) != 1) {
      DAT_0046c848 = FUN_0044d2d0(DAT_0046c848,0x50);
      FUN_0041d45a(DAT_0046c848,0x32);
    }
    uVar1 = FUN_00439730(s_shintro_0045f8f0);
    if (CONCAT22(extraout_var_01,uVar1) != 1) {
      DAT_0046c864 = FUN_0044d2d0(DAT_0046c864,0x50);
      FUN_0041d45a(DAT_0046c864,0x31);
    }
    uVar1 = FUN_00439730(s_bAM06fst_0045f8f8);
    if (CONCAT22(extraout_var_02,uVar1) != 1) {
      DAT_0046c868 = FUN_0044d2d0(DAT_0046c868,0x50);
      FUN_0041d45a(DAT_0046c868,0x30);
    }
    uVar1 = FUN_00439730(s_sAkGen_0045f904);
    if (CONCAT22(extraout_var_03,uVar1) != 1) {
      uVar1 = FUN_00439730(s_scabar_0045f90c);
      if (CONCAT22(extraout_var_04,uVar1) == 1) {
        _strtok((char *)DAT_0046c854,&DAT_0045f914);
      }
      else {
        uVar1 = FUN_00439730(s_flowChk_0045f918);
        if (CONCAT22(extraout_var_05,uVar1) < 4) {
          DAT_0046c854 = FUN_0044d2d0(DAT_0046c854,0x50);
          FUN_0041d45a(DAT_0046c854,0x2e);
        }
        else {
          DAT_0046c854 = FUN_0044d2d0(DAT_0046c854,0x50);
          FUN_0041d45a(DAT_0046c854,0x2f);
        }
      }
    }
    uVar2 = 0;
  }
  return uVar2;
}
