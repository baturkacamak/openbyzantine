// rea decompile_all
// name: FUN_00424dc0
// address: 0x424dc0
// body: available
// range: 0x424dc0-0x42515e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00424dc0(char *param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  FILE *_File;
  int iVar2;
  int local_10;
  int local_8;
  
  local_8 = 0;
  bVar1 = FUN_00425a54(150000);
  if (CONCAT31(extraout_var,bVar1) == 0) {
    FUN_0041d6df(7);
    local_8 = 1;
  }
  else {
    _File = (FILE *)FUN_0044bc20(param_1,&DAT_0045fe10);
    if (_File == (FILE *)0x0) {
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 1;
    }
    else {
      if (DAT_0045da78 == 1) {
        DAT_0045da78 = 2;
      }
      FUN_0044a460((char *)&DAT_0045da78,4,1,_File);
      _fputs(&DAT_004859c0,_File);
      _fputc(0,_File);
      _fputs(&DAT_00484e30,_File);
      _fputc(0,_File);
      FUN_00420888(DAT_0046e82c,_File);
      FUN_0044a460((char *)&DAT_00482428,2,1,_File);
      for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
        iVar2 = FUN_00425218(local_10,_File);
        local_8 = local_8 + iVar2;
      }
      FUN_0044a460(&DAT_0047e2f0,2,3000,_File);
      FUN_00408a1e(_File);
      FUN_0044a460((char *)&DAT_0045f210,4,1,_File);
      FUN_0044a460((char *)&DAT_00460668,4,1,_File);
      FUN_0044a460((char *)&DAT_0046066c,4,1,_File);
      if (DAT_0045f214 != 0) {
        for (local_10 = 0; *(char *)(local_10 + DAT_0045f214) != '\0'; local_10 = local_10 + 1) {
          _fputc((int)*(char *)(local_10 + DAT_0045f214),_File);
        }
      }
      _fputc(0,_File);
      FUN_0044a460((char *)&DAT_0045c060,4,1,_File);
      if (DAT_0045c060 == 0) {
        FUN_0044a460((char *)&DAT_0048253e,2,1,_File);
      }
      else {
        FUN_00401000(_File);
      }
      iVar2 = FUN_004269ec(_File);
      local_8 = local_8 + iVar2;
      for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
        if (*(int *)(&DAT_00464300 + local_10 * 4) == 0) {
          _fputc(0xff,_File);
        }
        else {
          _fputc(0,_File);
          FUN_0044a460(*(char **)(&DAT_00464300 + local_10 * 4),0x18,1,_File);
        }
      }
      FUN_00427135(_File);
      for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
        if (*(int *)(&DAT_00464f60 + local_10 * 4) == 0) {
          _fputc(0xff,_File);
        }
        else {
          _fputc(0,_File);
          FUN_0044a460(*(char **)(&DAT_00464f60 + local_10 * 4),0x14,1,_File);
        }
      }
      FUN_0044a460((char *)&DAT_00464878,4,1,_File);
      FUN_00408d3f(_File);
      FUN_0042118c(_File);
      _fclose(_File);
    }
  }
  return local_8;
}

