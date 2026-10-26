// rea decompile_all
// name: FUN_0040e1a8
// address: 0x40e1a8
// body: available
// range: 0x40e1a8-0x40e23d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0040e1a8(void)

{
  if (DAT_0045dbd4 != (int *)0x0) {
    (**(code **)(*DAT_0045dbd4 + 8))(DAT_0045dbd4);
  }
  if (DAT_0045dbd8 != (int *)0x0) {
    (**(code **)(*DAT_0045dbd8 + 8))(DAT_0045dbd8);
  }
  if (DAT_0045dbdc != (int *)0x0) {
    (**(code **)(*DAT_0045dbdc + 8))(DAT_0045dbdc);
  }
  DAT_0045dbdc = (int *)0x0;
  DAT_0045dbd8 = (int *)0x0;
  DAT_0045dbd4 = (int *)0x0;
  FUN_0040da4c();
  DAT_0045dbd0 = 0;
  ShowCursor(1);
  return;
}

