// rea decompile_all
// name: FUN_00408a1e
// address: 0x408a1e
// body: available
// range: 0x408a1e-0x408adb inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00408a1e(FILE *param_1)

{
  undefined4 local_8;
  
  FUN_0044a460((char *)&DAT_0045d1c0,4,1,param_1);
  FUN_0044a460((char *)&DAT_0045d1c4,4,1,param_1);
  FUN_0044a460((char *)&DAT_0045d1bc,4,1,param_1);
  FUN_0044a460(&DAT_00468cc0,0x28,1,param_1);
  FUN_0044a460(&DAT_00468d78,14000,1,param_1);
  if (DAT_0045d1c8 == 0) {
    local_8 = 0xffffffff;
  }
  else {
    local_8 = *(undefined4 *)(DAT_0045d1c8 + 8);
  }
  FUN_0044a460((char *)&local_8,4,1,param_1);
  FUN_0040892d(param_1);
  return;
}

