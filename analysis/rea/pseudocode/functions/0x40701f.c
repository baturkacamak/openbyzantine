// rea decompile_all
// name: FUN_0040701f
// address: 0x40701f
// body: available
// range: 0x40701f-0x40711a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_0040701f(uint *param_1,int param_2,int param_3)

{
  char *pcVar1;
  uint *puVar2;
  int local_14;
  char *local_c;
  int local_8;
  
  local_c = (char *)0x0;
  local_8 = 1;
  pcVar1 = FUN_0044c580(param_1);
  puVar2 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar1 + 1));
  FUN_0044ba00(puVar2,param_1);
  local_14 = FUN_004061e7((int)param_1,param_3);
  while (param_2 < local_14) {
    local_c = _strrchr((char *)param_1,0x20);
    *local_c = '\0';
    local_14 = FUN_004061e7((int)param_1,param_3);
  }
  if (local_c != (char *)0x0) {
    pcVar1 = FUN_0044c580(param_1);
    FUN_0044ba00((uint *)(local_c + 1),(uint *)(pcVar1 + 1 + (int)puVar2));
    *local_c = '\n';
    local_8 = FUN_0040701f((uint *)(local_c + 1),param_2,param_3);
    local_8 = local_8 + 1;
  }
  FUN_0044ac40(puVar2);
  return local_8;
}

