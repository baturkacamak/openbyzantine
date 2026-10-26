// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x425ab4
// name: FUN_00425ab4
// ranges: 0x425ab4-0x426260
// calls: 0x40105d, 0x40114a, 0x408adc, 0x408dbd, 0x40ac38, 0x40ac43, 0x40c7b4, 0x40df05, 0x40f350, 0x40f3b0, 0x40f3f0, 0x40f4e7, 0x40f969, 0x40fd7b, 0x4117a6, 0x411b4b, 0x4124d9, 0x412d35, 0x412f77, 0x41316f, 0x4131a1, 0x4138a8, 0x41393c, 0x41399a, 0x4139f8, 0x413a59, 0x41624a, 0x416ec0, 0x4209d2, 0x4210af, 0x425ab4, 0x426261, 0x426b9d, 0x42722b, 0x42d383, 0x42d52f, 0x42dad9, 0x432464, 0x437cad, 0x44ac40, 0x44ba00, 0x44ba08, 0x44bc20, 0x44c2f0, 0x44c580

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __cdecl FUN_00425ab4(uint *param_1)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  char *pcVar4;
  undefined4 uVar5;
  int local_22c [2];
  int local_224;
  uint local_220 [65];
  FILE *local_11c;
  uint local_118;
  FILE *local_114;
  char local_110;
  uint local_8;
  
  local_8 = 0;
  FUN_0044ba00(local_220,(uint *)s_savegame__0045ff00);
  FUN_0044ba08(local_220,param_1);
  local_11c = (FILE *)FUN_0044bc20((char *)local_220,&DAT_0045ff0c);
  if (local_11c == (FILE *)0x0) {
    FUN_0040ac43();
    FUN_0040ac38();
    local_8 = 1;
  }
  else {
    FUN_0040df05(0x1f);
    __fread_lk((char *)&local_224,4,1,local_11c);
    iVar2 = _strcmp((char *)param_1,s_new_can_0045ff40);
    if (iVar2 != 0) {
      iVar2 = FUN_0040c7b4(local_224);
      if (iVar2 != 0) {
        _fclose(local_11c);
        return 1;
      }
      DAT_0045da78 = local_224;
    }
    FUN_0041316f();
    FUN_004131a1();
    FUN_0040f969();
    FUN_004138a8();
    FUN_0041393c();
    FUN_0041399a();
    FUN_004139f8();
    FUN_00413a59();
    DAT_0045f490 = 1;
    FUN_0040105d();
    FUN_0041624a();
    local_118 = 0;
    do {
      iVar2 = _fgetc(local_11c);
      (&DAT_004859c0)[local_118] = (char)iVar2;
      uVar3 = local_118 + 1;
      pcVar4 = &DAT_004859c0 + local_118;
      local_118 = uVar3;
    } while (*pcVar4 != '\0');
    FUN_0044ba00((uint *)&DAT_0046e260,(uint *)&DAT_004859c0);
    local_118 = 0;
    do {
      iVar2 = _fgetc(local_11c);
      (&DAT_00484e30)[local_118] = (char)iVar2;
      uVar3 = local_118 + 1;
      pcVar4 = &DAT_00484e30 + local_118;
      local_118 = uVar3;
    } while (*pcVar4 != '\0');
    FUN_004209d2(local_11c);
    __fread_lk((char *)&DAT_00482428,2,1,local_11c);
    DAT_00482528 = 0;
    iVar2 = FUN_00437cad();
    if (iVar2 == 0) {
      FUN_0044ba00((uint *)&DAT_0046e260,(uint *)(&DAT_00483bc0 + (uint)DAT_00482428 * 9));
      FUN_0044ba08((uint *)&DAT_0046e260,(uint *)&DAT_0045ff48);
      FUN_00412d35((uint *)&DAT_0046e260);
      uVar3 = FUN_004124d9();
      local_8 = local_8 + (uVar3 & 0xffff);
      uVar1 = FUN_00412f77();
      local_8 = local_8 + uVar1;
      FUN_0040f3f0();
      FUN_0040f4e7();
      if (DAT_0045e7a8 == 0) {
        _sprintf(&DAT_0046e260,s_engine__s_cc_0045ff4c,&DAT_00483bc0 + (uint)DAT_00482428 * 9);
        local_114 = (FILE *)FUN_0040f350(&DAT_0046e260,&DAT_0045ff5c);
      }
      else {
        local_114 = FUN_00411b4b((uint *)&DAT_0046e260,local_22c);
      }
      if (local_114 == (FILE *)0x0) {
        local_8 = 1;
      }
      if (local_8 == 0) {
        uVar1 = FUN_0040f3b0(local_114);
        local_8 = local_8 + uVar1;
        FUN_0042d52f();
        uVar1 = FUN_0040fd7b();
        local_8 = (uint)uVar1;
        iVar2 = FUN_004117a6();
        local_8 = local_8 + iVar2;
        _fclose(local_114);
        uVar1 = FUN_0042d383();
        local_8 = local_8 + uVar1;
        if (local_8 == 0) {
          local_8 = 0;
          for (local_118 = 0; (int)local_118 < 0x15e; local_118 = local_118 + 1) {
            iVar2 = FUN_00426261(local_118,local_11c);
            local_8 = local_8 + iVar2;
          }
          __fread_lk(&DAT_0047e2f0,2,3000,local_11c);
          FUN_00408adc(local_11c);
          __fread_lk((char *)&DAT_0045f210,4,1,local_11c);
          __fread_lk((char *)&DAT_00460668,4,1,local_11c);
          __fread_lk((char *)&DAT_0046066c,4,1,local_11c);
          if (DAT_0045f214 != (uint *)0x0) {
            FUN_0044ac40(DAT_0045f214);
            DAT_0045f214 = (uint *)0x0;
          }
          iVar2 = _fgetc(local_11c);
          local_110 = (char)iVar2;
          if (local_110 != '\0') {
            local_118 = 1;
            DAT_0047fa70 = local_110;
            while (local_110 != '\0') {
              iVar2 = _fgetc(local_11c);
              local_110 = (char)iVar2;
              (&DAT_0047fa70)[local_118] = local_110;
              local_118 = local_118 + 1;
            }
            pcVar4 = FUN_0044c580((uint *)&DAT_0047fa70);
            DAT_0045f214 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar4 + 1));
            FUN_0044ba00(DAT_0045f214,(uint *)&DAT_0047fa70);
          }
          __fread_lk((char *)&DAT_0045c060,4,1,local_11c);
          if (DAT_0045c060 == 0) {
            __fread_lk((char *)&DAT_0048253e,2,1,local_11c);
            DAT_00480330 = 0xffffffff;
          }
          else {
            FUN_0040114a(local_11c);
            DAT_0048253e = 0xffff;
          }
          iVar2 = FUN_00426b9d(local_11c);
          local_8 = local_8 + iVar2;
          for (local_118 = 0; (int)local_118 < 0x15e; local_118 = local_118 + 1) {
            iVar2 = _fgetc(local_11c);
            local_110 = (char)iVar2;
            if (local_110 == '\0') {
              uVar5 = FUN_0044c2f0(0x18);
              *(undefined4 *)(&DAT_00464300 + local_118 * 4) = uVar5;
              __fread_lk(*(char **)(&DAT_00464300 + local_118 * 4),0x18,1,local_11c);
            }
          }
          DAT_0047a230 = FUN_00432464(DAT_0047a238);
          DAT_004848a8 = 0;
          FUN_0042722b(local_11c);
          for (local_118 = 0; (int)local_118 < 0x15e; local_118 = local_118 + 1) {
            iVar2 = _fgetc(local_11c);
            local_110 = (char)iVar2;
            if (local_110 == '\0') {
              uVar5 = FUN_0044c2f0(0x14);
              *(undefined4 *)(&DAT_00464f60 + local_118 * 4) = uVar5;
              __fread_lk(*(char **)(&DAT_00464f60 + local_118 * 4),0x14,1,local_11c);
            }
          }
          __fread_lk((char *)&local_118,4,1,local_11c);
          if (local_118 == DAT_00464878) {
            if (local_118 == 0) {
              FUN_00416ec0();
            }
          }
          else {
            FUN_0042dad9(local_118);
          }
          FUN_00408dbd(local_11c);
          FUN_004210af(local_11c);
          _fclose(local_11c);
          if (local_8 == 0) {
            _DAT_00485c70 = 1;
          }
          DAT_004859cd = 1;
        }
        else {
          _fclose(local_11c);
        }
      }
      else {
        if (local_114 != (FILE *)0x0) {
          _fclose(local_114);
        }
        _fclose(local_11c);
        local_8 = 1;
      }
    }
    else {
      _fclose(local_11c);
      local_8 = 1;
    }
  }
  return local_8;
}
