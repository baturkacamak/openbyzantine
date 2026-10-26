// rea decompile_all
// name: FUN_0042515f
// address: 0x42515f
// body: available
// range: 0x42515f-0x425217 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0042515f(int param_1)

{
  uint local_58 [20];
  FILE *local_8;
  
  if (param_1 == 0xbe) {
    FUN_0041d45a(local_58,0x3d);
    DAT_00484e30 = 0x7f;
    DAT_00484e31 = 0x20;
    DAT_00484e32 = 0;
    FUN_0044ba08((uint *)&DAT_00484e30,local_58);
    FUN_00424dc0(s_savegame_tempsave_btb_0045fe40);
  }
  else {
    local_8 = (FILE *)FUN_0044bc20(PTR_s_savegame_tempsave_btb_0045fdd8,&DAT_0045fe58);
    if (local_8 != (FILE *)0x0) {
      _fclose(local_8);
      FUN_0044d360(PTR_s_savegame_autosave_btb_0045fddc);
      FUN_0044d3a0(PTR_s_savegame_tempsave_btb_0045fdd8,PTR_s_savegame_autosave_btb_0045fddc);
    }
  }
  return;
}

