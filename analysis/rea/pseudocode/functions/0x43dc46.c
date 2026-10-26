// rea decompile_all
// name: FUN_0043dc46
// address: 0x43dc46
// body: available
// range: 0x43dc46-0x43dd95 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0043dc46(undefined4 *param_1)

{
  undefined4 local_48 [2];
  undefined4 local_40;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_20;
  int local_1c;
  undefined2 local_18;
  undefined2 local_16;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  local_34 = FUN_004161f6();
  _memset(&local_18,0,0x12);
  local_18 = 1;
  local_16 = 2;
  local_14 = 0x5622;
  local_c = 0x100004;
  local_10 = 0x15888;
  _memset(&local_30,0,0x14);
  local_30 = 0x14;
  local_2c = 1;
  local_28 = 0;
  local_20 = 0;
  local_1c = (**(code **)(*DAT_00461934 + 0x18))(DAT_00461934,local_34,2);
  if (((local_1c == 0) &&
      (local_1c = (**(code **)(*DAT_00461934 + 0xc))(DAT_00461934,&local_30,&DAT_00461938,0),
      local_1c == 0)) &&
     (local_1c = (**(code **)(*DAT_00461938 + 0x38))(DAT_00461938,&local_18), local_1c == 0)) {
    local_48[0] = 0x14;
    (**(code **)(*DAT_00461938 + 0xc))(DAT_00461938,local_48);
    *param_1 = local_40;
    return 1;
  }
  if (DAT_00461938 != (int *)0x0) {
    (**(code **)(*DAT_00461938 + 8))(DAT_00461938);
    DAT_00461938 = (int *)0x0;
  }
  *param_1 = 0;
  return 0;
}

