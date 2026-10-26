// rea decompile_all
// name: FUN_0043b130
// address: 0x43b130
// body: available
// range: 0x43b130-0x43b215 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl
FUN_0043b130(int param_1,int param_2,short param_3,short param_4,int *param_5,uint param_6,
            int param_7)

{
  int *local_8;
  
  FUN_0041624a();
  if (*(int *)(param_1 + 0x38) == 0) {
    if ((param_7 == 1) && (*(char *)(param_1 + 0x28) != '\0')) {
      local_8 = *(int **)(param_1 + 0x18);
    }
    else {
      local_8 = *(int **)(param_1 + 0x1c);
    }
    while( true ) {
      FUN_0040bbb0(param_3,param_4,local_8[4],param_6);
      FUN_0040bbfd(*local_8,param_2,param_3,param_4,param_5,*(byte *)(param_1 + 9) & 0x80);
      if (*(int **)(param_1 + 0x1c) == local_8) break;
      local_8 = (int *)local_8[1];
    }
  }
  else {
    FUN_0041cd96(param_1,param_2,(int)param_3,(int)param_4,param_6,param_5);
  }
  FUN_0041624a();
  return;
}

