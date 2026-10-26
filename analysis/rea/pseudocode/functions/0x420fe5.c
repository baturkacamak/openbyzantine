// rea decompile_all
// name: FUN_00420fe5
// address: 0x420fe5
// body: available
// range: 0x420fe5-0x4210ae inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00420fe5(int param_1,int param_2)

{
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_10 = -1;
  local_8 = 0x6c;
  local_c = 0x80;
  if ((0x172 < param_1) && (param_1 < 0x230)) {
    for (local_14 = 0; local_14 < 0x10; local_14 = local_14 + 1) {
      if ((local_8 <= param_2) && (param_2 <= local_c)) {
        local_10 = *(int *)(&DAT_0046c800 + local_14 * 4);
        break;
      }
      local_c = local_8 + 0x28;
      local_8 = local_8 + 0x14;
    }
  }
  if (DAT_0045f7d4 != local_10) {
    DAT_0045f7d4 = local_10;
    FUN_00420dc9(local_10,1);
    FUN_00420f0a(local_10);
  }
  return;
}

