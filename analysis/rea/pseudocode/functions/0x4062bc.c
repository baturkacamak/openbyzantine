// rea decompile_all
// name: FUN_004062bc
// address: 0x4062bc
// body: available
// range: 0x4062bc-0x406357 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_004062bc(char *param_1,int param_2,int param_3)

{
  int iVar1;
  char *local_14;
  int local_c;
  char *local_8;
  
  local_14 = param_1;
  local_c = 0;
  while ((local_c < param_3 && (local_8 = FUN_0044c520(param_1,'\n'), local_8 != (char *)0x0))) {
    local_14 = param_1;
    param_1 = local_8 + 1;
    local_c = local_c + 1;
  }
  if (local_8 != (char *)0x0) {
    *local_8 = '\0';
  }
  iVar1 = FUN_004061e7((int)local_14,param_2);
  if (local_8 != (char *)0x0) {
    *local_8 = '\n';
  }
  return iVar1;
}

