// rea decompile_all
// name: FUN_00421949
// address: 0x421949
// body: available
// range: 0x421949-0x421a5c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00421949(int param_1,int param_2)

{
  if (param_1 == 0) {
    DAT_0045f7e4 = 0;
    DAT_0045f7e8 = FUN_004215d6(DAT_0045f7d4);
  }
  else if (param_1 == 0x25) {
    DAT_0045f7e4 = DAT_0045f7e4 + 2;
  }
  else if (param_1 == 0x26) {
    DAT_0045f7e4 = DAT_0045f7e4 - 2;
  }
  DAT_0046c5f6 = 0 < (int)DAT_0045f7e4;
  DAT_0046c5f5 = (int)(DAT_0045f7e4 + 2) < DAT_0045f7e8;
  if (param_1 != -1) {
    if (param_2 != 0) {
      FUN_0042b2db(DAT_00477344,DAT_0047a8a4,0x4b000);
    }
    FUN_004216fe(DAT_0045f7e4);
    FUN_004216fe(DAT_0045f7e4 + 1);
    if (param_2 != 0) {
      FUN_0041842b((int)DAT_00477344,0x4b000);
      FUN_0040df05(0x1d);
    }
  }
  return;
}

