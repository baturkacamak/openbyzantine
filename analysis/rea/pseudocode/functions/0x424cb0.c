// rea decompile_all
// name: FUN_00424cb0
// address: 0x424cb0
// body: available
// range: 0x424cb0-0x424dbc inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00424cb0(char *param_1,int param_2)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_00418040((int)DAT_0046e82c);
  if (param_2 == -1) {
    FUN_0042b2db(DAT_0046e82c,DAT_0047a8a4,0x4b000);
    FUN_0042b2db(DAT_00477344,DAT_0047a8a4,0x4b000);
    FUN_004063e0(param_1,0x4b,0x32,0,piVar1,0,3,4);
    DAT_0046edf4 = 0;
  }
  else if (param_2 == -3) {
    FUN_0042b2db(DAT_0046e82c,DAT_0047a8a4,0x4b000);
    FUN_0042b2db(DAT_00477344,DAT_0047a8a4,0x4b000);
    FUN_0041842b((int)DAT_0046e82c,0x4b000);
  }
  else {
    FUN_004063e0(param_1,100,0x8f,0,piVar1,1,3,DAT_0045fdb8);
    FUN_0041842b((int)DAT_0046e82c,0x4b000);
  }
  return;
}

