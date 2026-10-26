// rea decompile_all
// name: FUN_004210af
// address: 0x4210af
// body: available
// range: 0x4210af-0x42118b inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004210af(FILE *param_1)

{
  int iVar1;
  char local_34 [40];
  FILE *local_c;
  int local_8;
  
  local_c = (FILE *)0x0;
  FUN_0044d360(PTR_s_savegame_notebook_000_0045f4e0);
  if ((param_1 == (FILE *)0x0) ||
     (local_c = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fc78),
     local_c == (FILE *)0x0)) {
    return;
  }
  do {
    local_8 = 0;
    do {
      iVar1 = _fgetc(param_1);
      local_34[local_8] = (char)iVar1;
      iVar1 = local_8;
      if ((param_1->_flag & 0x10) != 0) break;
      local_8 = local_8 + 1;
    } while (local_34[iVar1] != '\n');
    if ((param_1->_flag & 0x10) != 0) {
      _fclose(local_c);
      return;
    }
    local_34[local_8] = '\0';
    FID_conflict__fwprintf(local_c,(wchar_t *)&DAT_0045fc7c,local_34);
  } while( true );
}

