// rea decompile_all
// name: FUN_004223c1
// address: 0x4223c1
// body: available
// range: 0x4223c1-0x4224ff inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl
FUN_004223c1(int param_1,int param_2,int param_3,int param_4,int param_5,int *param_6,int param_7,
            int param_8)

{
  int iVar1;
  int local_20;
  int local_1c;
  int local_18;
  int local_10;
  char *local_c;
  int local_8;
  
  local_18 = param_5;
  local_1c = param_1;
  while( true ) {
    if (param_2 <= local_1c) {
      return 0;
    }
    local_20 = 6;
    local_8 = param_4;
    local_10 = 1;
    local_c = *(char **)(param_3 + local_1c * 4);
    if (**(char **)(param_3 + local_1c * 4) == '$') {
      local_20 = 5;
      local_c = (char *)(*(int *)(param_3 + local_1c * 4) + 1);
    }
    if (*local_c == '>') {
      local_10 = 0;
      local_c = local_c + 1;
      local_8 = param_4 + 10;
    }
    else if (*local_c == '<') {
      local_10 = 2;
      local_c = local_c + 1;
      local_8 = param_4 + -10;
    }
    FUN_004063e0(local_c,local_8,local_18,local_20,param_6,0,local_10,1);
    iVar1 = FUN_00422375(*(char **)(param_3 + local_1c * 4),*(char **)(param_3 + 4 + local_1c * 4));
    if ((iVar1 != 0) && (local_18 = local_18 + param_8, param_7 <= local_18)) break;
    local_1c = local_1c + 1;
  }
  return 0;
}

