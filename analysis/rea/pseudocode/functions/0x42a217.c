// rea decompile_all
// name: FUN_0042a217
// address: 0x42a217
// body: available
// range: 0x42a217-0x42a2cb inclusive
// range: 0x42a2d1-0x42a348 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int FUN_0042a217(void)

{
  uint uVar1;
  int local_18;
  uint local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_18 = 0;
  do {
    if (0x17 < local_18) {
      return -1;
    }
    uVar1 = FUN_0042a900((&DAT_0046ca98)[local_18]);
    if (uVar1 == 0) {
      FUN_0042a8a0((void *)(&DAT_0046ca98)[local_18],&local_14);
      uVar1 = FUN_0042a770((&DAT_0046ca98)[local_14]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a770((&DAT_0046ca98)[local_10]), uVar1 != 0)) {
        return local_18;
      }
      uVar1 = FUN_0042a770((&DAT_0046ca98)[local_c]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a770((&DAT_0046ca98)[local_8]), uVar1 != 0)) {
        return local_18;
      }
      uVar1 = FUN_0042a790((&DAT_0046ca98)[local_14]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a790((&DAT_0046ca98)[local_10]), uVar1 != 0)) {
        return local_18;
      }
      uVar1 = FUN_0042a790((&DAT_0046ca98)[local_c]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a790((&DAT_0046ca98)[local_8]), uVar1 != 0)) {
        return local_18;
      }
    }
    local_18 = local_18 + 1;
  } while( true );
}

