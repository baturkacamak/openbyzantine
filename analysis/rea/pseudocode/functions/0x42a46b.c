// rea decompile_all
// name: FUN_0042a46b
// address: 0x42a46b
// body: available
// range: 0x42a46b-0x42a554 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0042a46b(int *param_1,int *param_2)

{
  uint uVar1;
  int iVar2;
  int local_c;
  int local_8;
  
  local_8 = 0;
  if (*param_2 < 0) {
    *param_1 = -1;
  }
  local_c = 0;
  do {
    if (0x17 < local_c) {
      *param_2 = local_8;
      return;
    }
    uVar1 = FUN_0042a900((&DAT_0046ca98)[local_c]);
    if (uVar1 == 0) {
      iVar2 = FUN_0042a970((&DAT_0046ca98)[local_c]);
      if (*param_1 < iVar2) {
        iVar2 = FUN_0042a970((&DAT_0046ca98)[local_c]);
        *param_1 = iVar2;
        local_8 = 1;
      }
      else {
        iVar2 = FUN_0042a970((&DAT_0046ca98)[local_c]);
        if (iVar2 == *param_1) {
          local_8 = local_8 + 1;
        }
      }
      if ((-1 < *param_2) && (*param_2 == local_8)) {
        *param_2 = local_c;
        return;
      }
    }
    local_c = local_c + 1;
  } while( true );
}

