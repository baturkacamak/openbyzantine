// rea decompile_all
// name: FUN_00415bcc
// address: 0x415bcc
// body: available
// range: 0x415bcc-0x415c41 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_00415bcc(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  DAT_00486a04 = 0;
  DAT_004869d4 = 0;
  DAT_004869d8 = 0;
  DAT_00486884 = 0;
  _DAT_0046c4f0 = 0;
  DAT_0046c4ec = 0;
  DAT_004869f0 = param_1;
  DAT_004869f4 = param_2;
  _DAT_004869f8 = 0;
  DAT_004869fc = param_3;
  return 1;
}

