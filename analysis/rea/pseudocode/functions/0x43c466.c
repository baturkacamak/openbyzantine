// rea decompile_all
// name: FUN_0043c466
// address: 0x43c466
// body: available
// range: 0x43c466-0x43c565 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined2 __cdecl FUN_0043c466(undefined4 *param_1,char param_2)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined2 local_10;
  undefined4 *local_c;
  
  iVar2 = FUN_0041620b();
  if (iVar2 == 0) {
    local_10 = 0xffff;
  }
  else {
    if (DAT_0045c060 == 0) {
      local_c = param_1;
      local_10 = FUN_0043b216(param_1,0,0);
      cVar1 = DAT_00482acd;
    }
    else {
      if (DAT_004859cd == '\0') {
        FUN_00401184(0);
      }
      local_10 = 0xffff;
      cVar1 = DAT_00482acd;
    }
    while ((DAT_00482acd != '\0' && (local_c == param_1))) {
      FUN_0041624a();
      if (param_2 == '\x01') {
        uVar3 = FUN_0043c566(param_1,'\0');
        local_10 = (undefined2)uVar3;
      }
      if (local_c == param_1) {
        local_10 = FUN_0043b216(param_1,0,0);
      }
      FUN_0040df05(0);
    }
    if (cVar1 != '\0') {
      FUN_00438cd0();
    }
  }
  return local_10;
}

