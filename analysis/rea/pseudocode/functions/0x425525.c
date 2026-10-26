// rea decompile_all
// name: FUN_00425525
// address: 0x425525
// body: available
// range: 0x425525-0x4255d0 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00425525(int param_1,FILE *param_2)

{
  undefined4 local_c;
  undefined1 local_8;
  
  local_8 = '\0';
  if (param_1 == 0) {
    _fputc(0xff,param_2);
  }
  else {
    for (local_c = param_1; local_c != 0; local_c = *(int *)(local_c + 8)) {
      local_8 = local_8 + '\x01';
    }
    _fputc((int)local_8,param_2);
    for (local_c = param_1; local_c != 0; local_c = *(int *)(local_c + 8)) {
      _fputc((uint)*(byte *)(local_c + 0x10),param_2);
      _fputc((uint)*(byte *)(local_c + 0x11),param_2);
    }
  }
  return;
}

