// rea decompile_all
// name: FUN_0043264d
// address: 0x43264d
// body: available
// range: 0x43264d-0x4326ea inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0043264d(short *param_1)

{
  short *local_c;
  short *local_8;
  
  local_8 = *(short **)(param_1 + 0x1e);
  local_c = (short *)0x0;
  while( true ) {
    if (local_8 == (short *)0x0) {
      return;
    }
    if ((*local_8 == *param_1) && (local_8[1] == param_1[1])) break;
    local_c = local_8;
    local_8 = *(short **)(local_8 + 2);
  }
  if (local_c == (short *)0x0) {
    *(undefined4 *)(param_1 + 0x1e) = *(undefined4 *)(local_8 + 2);
  }
  else {
    *(undefined4 *)(local_c + 2) = *(undefined4 *)(local_8 + 2);
  }
  FUN_0044ac40(local_8);
  return;
}

