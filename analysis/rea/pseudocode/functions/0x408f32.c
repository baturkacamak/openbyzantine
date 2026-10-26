// rea decompile_all
// name: FUN_00408f32
// address: 0x408f32
// body: available
// range: 0x408f32-0x408fe6 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00408f32(int param_1)

{
  int local_14 [4];
  
  local_14[0] = 0;
  local_14[1] = 0;
  local_14[2] = 0x280;
  local_14[3] = 0x14;
  if (DAT_0045f494 == 0) {
    if (param_1 == 1) {
      if (DAT_0045d1d0 == 0) {
        return;
      }
      (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0,0,DAT_0045d1b8,local_14,0x10);
    }
    else {
      FUN_0042b29f(DAT_0046e82c,0,0x3200);
    }
    FUN_004181c5(DAT_00486888,local_14,0,0);
    DAT_0045d1d4 = 0xffffffff;
  }
  return;
}

