// rea decompile_all
// name: FUN_00422a90
// address: 0x422a90
// body: available
// range: 0x422a90-0x422b03 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int FUN_00422a90(void)

{
  LONG LVar1;
  
  while (DAT_0045fd20 != 0) {
    LVar1 = InterlockedExchange((LONG *)&DAT_004618b0,1);
    if (LVar1 == 0) break;
    Sleep(100);
  }
  if (DAT_0045fd28 != -1) {
    (**(code **)(*DAT_0045fd14 + 0x30))(DAT_0045fd14,0,0,1);
  }
  InterlockedExchange((LONG *)&DAT_004618b0,0);
  return DAT_0045fd28;
}

