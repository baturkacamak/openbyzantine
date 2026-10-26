// rea decompile_all
// name: FUN_004076b4
// address: 0x4076b4
// body: available
// range: 0x4076b4-0x4077b1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_004076b4(int *param_1,short param_2,short param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 local_90 [8];
  undefined4 local_88 [2];
  undefined4 local_80;
  undefined4 local_7c;
  int local_1c;
  uint local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  if (param_1 == (int *)0x0) {
    uVar1 = 1;
  }
  else {
    _memset(local_88,0,0x6c);
    local_88[0] = 0x6c;
    local_1c = (**(code **)(*param_1 + 0x58))(param_1,local_88);
    if (local_1c == 0) {
      local_14 = 0;
      local_10 = 0;
      local_8 = local_80;
      local_c = local_7c;
      local_1c = (**(code **)(*param_1 + 0x40))(param_1,8,local_90);
      local_18 = (uint)(local_1c == 0);
      iVar2 = (**(code **)(*DAT_0045d1b0 + 0x1c))
                        (DAT_0045d1b0,(int)param_2,(int)param_3,param_1,&local_14,local_18);
      if (iVar2 == 0) {
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  return uVar1;
}

