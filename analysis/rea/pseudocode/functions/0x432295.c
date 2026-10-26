// rea decompile_all
// name: FUN_00432295
// address: 0x432295
// body: available
// range: 0x432295-0x432302 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00432295(int param_1,int param_2,int param_3,int param_4)

{
  float10 fVar1;
  
  if (0 < param_3) {
    fVar1 = (float10)FUN_00401c63();
    _DAT_0047a8dc = (float)fVar1;
  }
  DAT_00480330 = param_1;
  DAT_0045c060 = 1;
  FUN_0040186e(**(undefined4 **)(&DAT_00483d90 + param_1 * 4),param_2,param_4);
  FUN_0040da1f();
  FUN_004075fe(1);
  DAT_004859cd = 0;
  return;
}

