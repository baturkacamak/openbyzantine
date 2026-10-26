// rea decompile_all
// name: FUN_0040d2a1
// address: 0x40d2a1
// body: available
// range: 0x40d2a1-0x40d3ae inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0040d2a1(uint *param_1,undefined4 param_2)

{
  FILE *local_210;
  uint local_20c [65];
  uint local_108 [65];
  
  _sprintf((char *)local_108,s_avi__s_avi_0045dba4,param_1);
  local_210 = (FILE *)FUN_0044bc20((char *)local_108,&DAT_0045dbb0);
  if (local_210 == (FILE *)0x0) {
    _sprintf((char *)local_20c,s_dialog__s_0045dbb4,param_2);
    FUN_0040c624(local_108,local_20c,param_1,(uint *)&DAT_0045dbc0);
    local_210 = (FILE *)FUN_0044bc20((char *)local_108,&DAT_0045dbc8);
    if (local_210 == (FILE *)0x0) {
      return 0;
    }
  }
  _fclose(local_210);
  if (DAT_00460684 != 0) {
    FUN_004075a1(1);
    DAT_00460684 = 0;
  }
  FUN_0040d5ff(local_108,4,0,0);
  DAT_00460664 = 0x67;
  return 1;
}

