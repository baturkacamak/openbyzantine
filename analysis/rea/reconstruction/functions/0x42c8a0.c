// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42c8a0
// name: FUN_0042c8a0
// ranges: 0x42c8a0-0x42d1e1
// calls: 0x408662, 0x40c619, 0x40f350, 0x40f3b0, 0x40f3f0, 0x40f4e7, 0x40f524, 0x40f6a5, 0x40f969, 0x40fd7b, 0x410607, 0x4117a6, 0x411b4b, 0x4124d9, 0x412d35, 0x412f77, 0x41316f, 0x4131a1, 0x4138a8, 0x41393c, 0x41399a, 0x4139f8, 0x413a59, 0x416fb8, 0x4177c7, 0x41de4b, 0x41e289, 0x41f21e, 0x41f753, 0x4210af, 0x424dc0, 0x427389, 0x42b280, 0x42c8a0, 0x42d383, 0x42d52f, 0x42d594, 0x432303, 0x43237c, 0x437cad, 0x438cd0, 0x44ba00, 0x44ba08, 0x44bc20, 0x44c580
// port: New-game zero state [partial]

/* WARNING: Removing unreachable block (ram,0x0042ca09) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __cdecl FUN_0042c8a0(undefined4 param_1,uint *param_2)

{
  bool bVar1;
  short sVar2;
  ushort uVar3;
  int iVar4;
  uint uVar5;
  char *pcVar6;
  undefined4 uVar7;
  undefined3 extraout_var;
  char local_138 [260];
  uint local_34;
  int local_30;
  undefined4 local_2c;
  undefined4 local_28;
  int local_24;
  FILE *local_20;
  int local_14;
  uint local_c;
  undefined4 local_8;
  
  local_30 = 0;
  DAT_00482acd = 0;
  DAT_004859cd = 0;
  DAT_0046edf4 = 0;
  DAT_0047734a = 0;
  DAT_0047a8a2 = 0;
  DAT_00476338 = 0;
  DAT_00464878 = 0xffffffff;
  DAT_0047734b = 1;
  DAT_00485c74 = 0;
  DAT_00483070 = 0;
  DAT_00485c6c = 0;
  DAT_00485990 = 1;
  DAT_0047a89c = 0;
  local_14 = 1;
  _DAT_0046e828 = 0;
  _DAT_00484308 = 0;
  _DAT_0046e81c = 0;
  _DAT_0046edf0 = 0;
  DAT_0047a280 = 0;
  _DAT_004859fc = 1;
  DAT_004848a8 = 0;
  _DAT_0046e270 = 0;
  _DAT_00485c70 = 1;
  DAT_004859b0 = 1000;
  DAT_004859b4 = 1000;
  DAT_00483050 = 1000;
  FUN_004210af((FILE *)0x0);
  FUN_0041f753(0);
  local_28 = (uint)local_28._2_2_ << 0x10;
  while ((local_28 & 0xffff) < 0x15e) {
    (&DAT_00464888)[local_28 & 0xffff] = 0;
    local_28 = CONCAT22(local_28._2_2_,(short)local_28 + 1);
  }
  local_28 = CONCAT22(local_28._2_2_,1);
  DAT_00485c6c = '\0';
  _DAT_0046e828 = 1;
  local_8 = (uint)local_8._2_2_ << 0x10;
  local_c = local_c & 0xffffff00;
  local_2c = (uint)local_2c._2_2_ << 0x10;
  while ((local_2c & 0xffff) < 3000) {
    *(undefined4 *)(&DAT_00477350 + (local_2c & 0xffff) * 4) = 0;
    *(undefined2 *)(&DAT_0047e2f0 + (local_2c & 0xffff) * 2) = 0;
    local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
  }
  local_2c = (uint)local_2c._2_2_ << 0x10;
  while ((local_2c & 0xffff) < 3000) {
    *(undefined4 *)(&DAT_0047ae80 + (local_2c & 0xffff) * 4) = 0;
    local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
  }
  local_2c = (uint)local_2c._2_2_ << 0x10;
  while ((local_2c & 0xffff) < 0x32) {
    *(undefined4 *)(&DAT_00482460 + (local_2c & 0xffff) * 4) = 0xffff;
    local_34 = local_34 & 0xffff0000;
    while ((local_34 & 0xffff) < 300) {
      *(short *)(&DAT_0046ee00 + (local_34 & 0xffff) * 2 + (local_2c & 0xffff) * 600) =
           (short)local_34;
      local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
    }
    local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
  }
  local_2c = (uint)local_2c._2_2_ << 0x10;
  while ((local_2c & 0xffff) < 0x96) {
    (&DAT_00485a10)[local_2c & 0xffff] = 0;
    local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
  }
  local_2c = (uint)local_2c._2_2_ << 0x10;
  while ((local_2c & 0xffff) < 0x15e) {
    (&DAT_00482540)[local_2c & 0xffff] = 0;
    local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
  }
  DAT_00482528 = 0;
  DAT_00482428 = 0;
  FUN_0044ba00((uint *)&DAT_0046e260,param_2);
  FUN_0044ba00((uint *)&DAT_004859c0,(uint *)&DAT_0046e260);
  iVar4 = FUN_00437cad();
  if (iVar4 == 0) {
    if (DAT_00482528 == 0) {
      pcVar6 = FUN_0044c580((uint *)&DAT_0046e260);
      pcVar6[0x46e25d] = '\0';
      FUN_0044ba00((uint *)&DAT_00483bc0,(uint *)&DAT_0046e260);
      _DAT_00482460 = 0;
    }
    FUN_0040f524();
    while ((local_8 & 0xffff) == 0) {
      FUN_0044ba00((uint *)&DAT_0046e260,(uint *)(&DAT_00483bc0 + (uint)DAT_00482428 * 9));
      FUN_0044ba08((uint *)&DAT_0046e260,(uint *)&DAT_00460c2c);
      FUN_00412d35((uint *)&DAT_0046e260);
      uVar7 = FUN_004124d9();
      local_8._0_2_ = (short)uVar7 + (short)local_8;
      sVar2 = FUN_00412f77();
      local_8._0_2_ = sVar2 + (short)local_8;
      uVar7 = FUN_00408662((uint)DAT_00485a00);
      local_8._0_2_ = (short)uVar7 + (short)local_8;
      if ((short)local_8 != 0) {
        local_8 = CONCAT22(local_8._2_2_,0xfffc);
        break;
      }
      FUN_0040f3f0();
      FUN_0040f4e7();
      if (DAT_0045e7a8 == 0) {
        _sprintf(local_138,s_engine__s_00460c30,&DAT_0046e260);
        local_20 = (FILE *)FUN_0040f350(local_138,&DAT_00460c3c);
      }
      else {
        local_20 = FUN_00411b4b((uint *)&DAT_0046e260,&local_24);
      }
      if (local_20 == (FILE *)0x0) {
        local_8 = CONCAT22(local_8._2_2_,0xfffc);
        DAT_0047734b = 1;
        break;
      }
      sVar2 = FUN_0040f3b0(local_20);
      local_8._0_2_ = sVar2 + (short)local_8;
      _fclose(local_20);
      FUN_0042d52f();
      if (DAT_00485c6c == '\0') {
        local_8._0_2_ = FUN_0040fd7b();
        uVar7 = FUN_004117a6();
        local_8._0_2_ = (short)uVar7 + (short)local_8;
      }
      uVar5 = FUN_00410607();
      local_8._0_2_ = (short)uVar5 + (short)local_8;
      sVar2 = FUN_0042d383();
      sVar2 = sVar2 + (short)local_8;
      local_8 = CONCAT22(local_8._2_2_,sVar2);
      if (sVar2 != 0) {
        local_8 = CONCAT22(local_8._2_2_,0xfffc);
        break;
      }
      DAT_0047ae58 = 0;
      _DAT_0046e810 = 0;
      DAT_00485978 = 0;
      if ((DAT_00485c6c == '\0') && (local_30 == 0)) {
        local_30 = 1;
        DAT_0047734b = 0;
      }
      if ((local_14 == 1) && (DAT_00485c6c == '\0')) {
        local_20 = (FILE *)0x0;
        bVar1 = FUN_0041f21e();
        if ((CONCAT31(extraout_var,bVar1) == 0) || (local_20 != (FILE *)0x0)) {
          DAT_0045f490 = 0;
          local_20 = (FILE *)FUN_0044bc20(PTR_s_savegame_new_can_00460688,&DAT_00460c40);
          if (local_20 == (FILE *)0x0) {
            FUN_0042b280(DAT_0046e82c,0x4b000);
            DAT_00484e28 = DAT_0047a238;
            DAT_0047a230 = DAT_0047a238;
            FUN_0044ba00((uint *)&DAT_00484e30,(uint *)s_New_game_00460c44);
            FUN_00424dc0(PTR_s_savegame_new_can_00460688);
            FUN_00427389(0x35);
          }
          else {
            _fclose(local_20);
          }
          goto LAB_0042d097;
        }
        uVar5 = FUN_0041e289(1);
        local_8 = CONCAT22(local_8._2_2_,(short)uVar5);
        if ((uVar5 & 0xffff) == 2) {
          local_c = CONCAT31(local_c._1_3_,2);
          goto LAB_0042d097;
        }
        if ((short)uVar5 != 0) goto LAB_0042d097;
        local_14 = 0;
      }
      else {
LAB_0042d097:
        if ((local_8 & 0xffff) != 1) {
          do {
            local_14 = 0;
            uVar3 = FUN_0042d594(DAT_0047a238,(char)local_c);
            local_8 = CONCAT22(local_8._2_2_,uVar3);
            if ((uVar3 == 1) && (DAT_00485c6c == '\0')) {
              iVar4 = FUN_0041de4b();
              uVar7 = FUN_0041e289(iVar4);
              local_8 = CONCAT22(local_8._2_2_,(short)uVar7);
              FUN_00438cd0();
              if ((local_8 & 0xffff) == 0) {
                local_c = (uint)local_c._1_3_ << 8;
              }
              else {
                local_c = CONCAT31(local_c._1_3_,2);
              }
            }
            else {
              local_c = local_c & 0xffffff00;
              FUN_00432303();
              FUN_0043237c();
            }
            if ((local_8 & 0xffff) == 1) {
              FUN_00432303();
              FUN_0043237c();
            }
          } while (((local_8 & 0xffff) == 0xffff) || ((local_8 & 0xffff) == 2));
        }
      }
      FUN_0040f6a5();
      FUN_0040f969();
      FUN_004138a8();
      FUN_004177c7((char *)0x0);
    }
    FUN_00416fb8();
    FUN_0041316f();
    FUN_004131a1();
    FUN_0041393c();
    FUN_0041399a();
    FUN_004139f8();
    FUN_00413a59();
    if (DAT_00485c6c == '\0') {
      FUN_0040c619();
    }
    uVar5 = local_8 & 0xffff;
  }
  else {
    uVar5 = 0xfffc;
  }
  return uVar5;
}
