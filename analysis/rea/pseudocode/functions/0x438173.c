// rea decompile_all
// name: FUN_00438173
// address: 0x438173
// body: available
// range: 0x438173-0x438af9 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00438173(void)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  char *pcVar4;
  int local_170;
  char local_16c [260];
  uint local_68;
  uint local_64;
  char *local_60;
  int local_5c;
  uint local_58;
  uint local_54 [20];
  
  _sprintf(local_16c,s_engine__s_004615cc,&DAT_0046e250);
  DAT_00483600 = (FILE *)FUN_0044bc20(local_16c,&DAT_004615d8);
  if (DAT_00483600 == (FILE *)0x0) {
    uVar2 = 0xff;
  }
  else {
    for (local_64 = 0; local_64 < 5; local_64 = local_64 + 1) {
      (&DAT_00460670)[local_64] = 0xffffffff;
    }
    local_60 = FUN_0044ce20((char *)local_54,0x50,DAT_00483600);
    while (local_60 != (char *)0x0) {
      for (local_64 = 0; *(char *)((int)local_54 + local_64) != '\0'; local_64 = local_64 + 1) {
        if (*(char *)((int)local_54 + local_64) == '\n') {
          *(undefined1 *)((int)local_54 + local_64) = 0;
        }
        else {
          iVar3 = _tolower((uint)*(byte *)((int)local_54 + local_64));
          *(char *)((int)local_54 + local_64) = (char)iVar3;
        }
      }
      pcVar4 = FUN_0044c580(local_54);
      if ((char *)0x8 < pcVar4) {
        FUN_0040ac43();
        FUN_0040ac38();
        return 1;
      }
      iVar3 = __strcmpi((char *)local_54,s_examine_0046161c);
      if (iVar3 == 0) {
        DAT_00460670 = DAT_00482528;
      }
      else {
        iVar3 = __strcmpi((char *)local_54,s_examvt_00461624);
        if (iVar3 == 0) {
          _DAT_00460674 = DAT_00482528;
        }
        else {
          iVar3 = __strcmpi((char *)local_54,s_examvb_0046162c);
          if (iVar3 == 0) {
            _DAT_00460678 = DAT_00482528;
          }
          else {
            iVar3 = __strcmpi((char *)local_54,s_examvc_00461634);
            if (iVar3 == 0) {
              DAT_0046067c = DAT_00482528;
            }
            else {
              iVar3 = __strcmpi((char *)local_54,s_examvh_0046163c);
              if (iVar3 == 0) {
                _DAT_00460680 = DAT_00482528;
              }
            }
          }
        }
      }
      FUN_0044ba00((uint *)(&DAT_00483bc0 + DAT_00482528 * 9),local_54);
      DAT_00482528 = DAT_00482528 + 1;
      local_60 = FUN_0044ce20((char *)local_54,0x50,DAT_00483600);
    }
    _fclose(DAT_00483600);
    _sprintf(local_16c,s_engine__s_00461644,&DAT_00485980);
    DAT_0047ae70 = (FILE *)FUN_0044bc20(local_16c,&DAT_00461650);
    if (DAT_0047ae70 == (FILE *)0x0) {
      FUN_0040ac43();
      FUN_0040ac38();
      uVar2 = 1;
    }
    else {
      for (local_64 = 0; local_64 < DAT_00482528; local_64 = local_64 + 1) {
        __fread_lk((char *)&local_58,2,1,DAT_0047ae70);
        for (local_68 = 0; local_68 < (local_58 & 0xffff); local_68 = local_68 + 1) {
          __fread_lk(&DAT_0046ee00 + local_64 * 600 + local_68 * 2,2,1,DAT_0047ae70);
        }
      }
      _fclose(DAT_0047ae70);
      _sprintf(local_16c,s_engine__s_00461680,&DAT_00484320);
      DAT_0047a894 = (FILE *)FUN_0044bc20(local_16c,&DAT_0046168c);
      if (DAT_0047a894 == (FILE *)0x0) {
        FUN_0040ac43();
        FUN_0040ac38();
        uVar2 = 1;
      }
      else {
        __fread_lk((char *)&local_58,2,1,DAT_0047a894);
        for (local_68 = 0; local_68 < (local_58 & 0xffff); local_68 = local_68 + 1) {
          local_170 = 0;
          do {
            iVar3 = _fgetc(DAT_0047a894);
            *(char *)((int)local_54 + local_170) = (char)iVar3;
            pcVar4 = (char *)((int)local_54 + local_170);
            local_170 = local_170 + 1;
          } while (*pcVar4 != '\0');
          pcVar4 = FUN_0044c580(local_54);
          uVar2 = FUN_0044c2f0((SIZE_T)(pcVar4 + 1));
          *(undefined4 *)(&DAT_00477350 + local_68 * 4) = uVar2;
          FUN_0044ba00(*(uint **)(&DAT_00477350 + local_68 * 4),local_54);
        }
        _fclose(DAT_0047a894);
        _sprintf(local_16c,s_engine__s_004616b8,&DAT_00482530);
        DAT_0046edec = (FILE *)FUN_0044bc20(local_16c,&DAT_004616c4);
        if (DAT_0046edec == (FILE *)0x0) {
          FUN_0040ac43();
          FUN_0040ac38();
          uVar2 = 1;
        }
        else {
          __fread_lk((char *)&local_58,2,1,DAT_0046edec);
          for (local_68 = 0; local_68 < (local_58 & 0xffff); local_68 = local_68 + 1) {
            local_170 = 0;
            do {
              iVar3 = _fgetc(DAT_0046edec);
              *(char *)((int)local_54 + local_170) = (char)iVar3;
              pcVar4 = (char *)((int)local_54 + local_170);
              local_170 = local_170 + 1;
            } while (*pcVar4 != '\0');
            pcVar4 = FUN_0044c580(local_54);
            uVar2 = FUN_0044c2f0((SIZE_T)(pcVar4 + 1));
            (&DAT_00485a10)[local_68] = uVar2;
            FUN_0044ba00((uint *)(&DAT_00485a10)[local_68],local_54);
          }
          _fclose(DAT_0046edec);
          _sprintf(local_16c,s_engine__s_004616f0,&DAT_0047a290);
          DAT_004859f0 = (FILE *)FUN_0044bc20(local_16c,&DAT_004616fc);
          if (DAT_004859f0 == (FILE *)0x0) {
            FUN_0040ac43();
            FUN_0040ac38();
            uVar2 = 1;
          }
          else {
            __fread_lk((char *)&local_58,2,1,DAT_004859f0);
            for (local_68 = 0; local_68 < (local_58 & 0xffff); local_68 = local_68 + 1) {
              __fread_lk(&DAT_00482460 + local_68 * 4,2,1,DAT_004859f0);
            }
            _fclose(DAT_004859f0);
            _sprintf(local_16c,s_engine__s_00461730,&DAT_00482ac0);
            DAT_00476334 = (FILE *)FUN_0044bc20(local_16c,&DAT_0046173c);
            if (DAT_00476334 == (FILE *)0x0) {
              FUN_0040ac43();
              FUN_0040ac38();
              uVar2 = 1;
            }
            else {
              local_5c = 0;
              local_60 = (char *)FUN_004380ff((int)local_54,0x50,DAT_00476334);
              while (local_60 != (char *)0x0) {
                pcVar4 = FUN_0044c580(local_54);
                uVar2 = FUN_0044c2f0((SIZE_T)(pcVar4 + 1));
                (&DAT_0047a8b0)[local_5c] = uVar2;
                puVar1 = &DAT_0047a8b0 + local_5c;
                local_5c = local_5c + 1;
                FUN_0044ba00((uint *)*puVar1,local_54);
                local_60 = (char *)FUN_004380ff((int)local_54,0x50,DAT_00476334);
                _fgetc(DAT_00476334);
                local_60 = (char *)FUN_004380ff((int)local_54,0x50,DAT_00476334);
              }
              _fclose(DAT_00476334);
              _sprintf(local_16c,s_engine__s_00461774,&DAT_00482430);
              DAT_0046ede8 = (FILE *)FUN_0044bc20(local_16c,&DAT_00461780);
              if (DAT_0046ede8 == (FILE *)0x0) {
                FUN_0040ac43();
                FUN_0040ac38();
                uVar2 = 1;
              }
              else {
                local_5c = 0;
                local_60 = (char *)FUN_004380ff((int)local_54,0x50,DAT_0046ede8);
                while (local_60 != (char *)0x0) {
                  pcVar4 = FUN_0044c580(local_54);
                  uVar2 = FUN_0044c2f0((SIZE_T)(pcVar4 + 1));
                  (&DAT_00482540)[local_5c] = uVar2;
                  puVar1 = &DAT_00482540 + local_5c;
                  local_5c = local_5c + 1;
                  FUN_0044ba00((uint *)*puVar1,local_54);
                  local_60 = (char *)FUN_004380ff((int)local_54,0x50,DAT_0046ede8);
                  _fgetc(DAT_0046ede8);
                  local_60 = (char *)FUN_004380ff((int)local_54,0x50,DAT_0046ede8);
                }
                _fclose(DAT_0046ede8);
                FUN_00408662(local_5c);
                uVar2 = 0;
              }
            }
          }
        }
      }
    }
  }
  return uVar2;
}

