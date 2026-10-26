// rea decompile_all
// name: FUN_00407fe5
// address: 0x407fe5
// body: available
// range: 0x407fe5-0x408075 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00407fe5(int param_1)

{
  int local_10;
  
  if (param_1 != 0) {
    for (local_10 = 0; local_10 < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4);
        local_10 = local_10 + 1) {
      if (*(int *)(&DAT_00468d78 + local_10 * 4 + DAT_0045d1c4 * 0x578) == *(int *)(param_1 + 8)) {
        return 1;
      }
    }
  }
  return 0;
}

