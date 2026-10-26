// rea decompile_all
// name: FUN_00416fb8
// address: 0x416fb8
// body: available
// range: 0x416fb8-0x417053 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00416fb8(void)

{
  FUN_00417a17((uint *)0x0,2);
  if (DAT_0045f218 != (int *)0x0) {
    (**(code **)(*DAT_0045f218 + 8))(DAT_0045f218);
    DAT_0045f218 = (int *)0x0;
  }
  if (DAT_0045f21c != (int *)0x0) {
    (**(code **)(*DAT_0045f21c + 8))(DAT_0045f21c);
    DAT_0045f21c = (int *)0x0;
  }
  FUN_0042024d(0x46c520,4);
  FUN_0042024d(0x46c538,0x20);
  FUN_0042024d(0x46c5b8,6);
  DAT_0045f220 = 0;
  return;
}

