// rea decompile_all
// name: FUN_0043e900
// address: 0x43e900
// body: available
// range: 0x43e900-0x43e94b inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0043e900(undefined4 *param_1)

{
  param_1[1] = &LAB_0043e9c0;
  *param_1 = FUN_0043e950;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[5] = 0;
  param_1[2] = &LAB_0043e980;
  param_1[3] = &LAB_0043ea00;
  param_1[0x1e] = 0;
  param_1[0x1f] = 0;
  param_1[4] = &LAB_0043eab0;
  param_1[0x1c] = &PTR_s_Bogus_message_code__d_0045b1c0;
  param_1[0x1d] = 0x77;
  param_1[0x20] = 0;
  return;
}

