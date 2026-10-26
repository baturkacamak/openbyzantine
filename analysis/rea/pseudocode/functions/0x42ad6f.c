// rea decompile_all
// name: FUN_0042ad6f
// address: 0x42ad6f
// body: available
// range: 0x42ad6f-0x42aebe inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0042ad6f(int *param_1,undefined1 *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_78;
  undefined4 local_74;
  uint local_20;
  uint local_1c;
  uint local_18;
  char local_8;
  
  local_78 = 0x6c;
  local_74 = 0x1000;
  iVar1 = (**(code **)(*param_1 + 0x58))(param_1,&local_78);
  if (iVar1 == 0) {
    if (local_20 == 0) {
      uVar2 = 1;
    }
    else {
      local_8 = '\0';
      for (; (local_20 & 1) == 0; local_20 = local_20 >> 1) {
        local_8 = local_8 + '\x01';
      }
      param_2[2] = (undefined1)local_20;
      param_2[10] = local_8;
      if (local_20 == 0x1f) {
        param_2[6] = 3;
      }
      else {
        param_2[6] = 2;
      }
      local_8 = '\0';
      for (; (local_1c & 1) == 0; local_1c = local_1c >> 1) {
        local_8 = local_8 + '\x01';
      }
      param_2[1] = (undefined1)local_1c;
      param_2[9] = local_8;
      if (local_1c == 0x1f) {
        param_2[5] = 3;
      }
      else {
        param_2[5] = 2;
      }
      local_8 = '\0';
      for (; (local_18 & 1) == 0; local_18 = local_18 >> 1) {
        local_8 = local_8 + '\x01';
      }
      *param_2 = (undefined1)local_18;
      param_2[8] = local_8;
      if (local_18 == 0x1f) {
        param_2[4] = 3;
      }
      else {
        param_2[4] = 2;
      }
      uVar2 = 1;
    }
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}

