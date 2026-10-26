// rea decompile_all
// name: FUN_0042aebf
// address: 0x42aebf
// body: available
// range: 0x42aebf-0x42b038 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0042aebf(char *param_1,undefined4 *param_2,int *param_3,int *param_4)

{
  undefined4 uVar1;
  undefined4 local_a8;
  undefined4 local_a4;
  int local_a0;
  int local_9c;
  undefined4 local_40;
  int local_3c;
  int *local_38;
  FILE *local_34;
  char *local_30 [3];
  HGLOBAL local_24;
  int local_20;
  int local_1c;
  uint local_18;
  int local_8;
  
  local_34 = (FILE *)FUN_0044bc20(param_1,&DAT_004603cc);
  if (local_34 == (FILE *)0x0) {
    uVar1 = 1;
  }
  else {
    _fclose(local_34);
    _memset(local_30,0,0x28);
    local_30[0] = param_1;
    local_8 = FUN_0042b039(local_30);
    if (local_8 == 0) {
      uVar1 = 1;
    }
    else {
      _memset(&local_a8,0,0x6c);
      local_a8 = 0x6c;
      local_a4 = 7;
      local_40 = 0x840;
      local_9c = local_20;
      local_a0 = local_1c;
      local_3c = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,&local_a8,param_2,0);
      if (local_3c == 0) {
        local_38 = (int *)*param_2;
        local_3c = (**(code **)(*local_38 + 0x58))(local_38,&local_a8);
        if ((local_9c == local_20) && (local_1c == local_a0)) {
          local_8 = FUN_0042aa1a(local_38,(int)local_24,local_20,local_1c,local_18,0);
          GlobalFree(local_24);
          *param_3 = local_20;
          *param_4 = local_1c;
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
  }
  return uVar1;
}

