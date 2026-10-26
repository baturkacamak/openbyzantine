// rea decompile_all
// name: FUN_00425ab4
// address: 0x425ab4
// body: available
// range: 0x425ab4-0x426260 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


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

