// rea decompile_all
// name: FUN_0042b2fc
// address: 0x42b2fc
// body: available
// range: 0x42b2fc-0x42b376 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0042b2fc(ushort *param_1,int param_2)

{
  ushort uVar1;
  int local_c;
  ushort *local_8;
  
  if (((DAT_00485c8c != 0) || (_DAT_00485c80 != 5)) || (_DAT_00485c78 != 10)) {
    local_8 = param_1;
    for (local_c = 0; local_c < param_2; local_c = local_c + 1) {
      uVar1 = FUN_004164a0(*local_8);
      *local_8 = uVar1;
      local_8 = local_8 + 1;
    }
  }
  return;
}

