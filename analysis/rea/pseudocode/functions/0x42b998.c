// rea decompile_all
// name: FUN_0042b998
// address: 0x42b998
// body: available
// range: 0x42b998-0x42ba61 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl
FUN_0042b998(int param_1,short *param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            uint param_7,short param_8)

{
  int local_14;
  int local_10;
  short *local_c;
  short *local_8;
  
  local_c = (short *)((param_6 & 0xffff) * 2 + (param_7 & 0xffff) * (param_3 & 0xffff) * 2 + param_1
                     );
  local_8 = param_2;
  for (local_10 = 0; local_10 < (int)(param_4 & 0xffff); local_10 = local_10 + 1) {
    for (local_14 = 0; local_14 < (int)(param_5 & 0xffff); local_14 = local_14 + 1) {
      if (*local_8 != param_8) {
        *local_c = *local_8;
      }
      local_c = local_c + 1;
      local_8 = local_8 + 1;
    }
    local_c = local_c + ((param_3 & 0xffff) - (param_5 & 0xffff));
  }
  return;
}

