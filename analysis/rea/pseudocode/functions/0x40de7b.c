// rea decompile_all
// name: FUN_0040de7b
// address: 0x40de7b
// body: available
// range: 0x40de7b-0x40decf inclusive
// range: 0x40ded5-0x40df04 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_0040de7b(int param_1)

{
  if ((DAT_0045c060 == 0) && ((undefined **)PTR_PTR_0045e534 != &PTR_s_default_0045e090)) {
    if ((param_1 == 0) || (param_1 == 0x17)) {
      param_1 = 0x1d;
    }
    else if (param_1 == 0x1c) {
      param_1 = 0x1e;
    }
  }
  return param_1;
}

