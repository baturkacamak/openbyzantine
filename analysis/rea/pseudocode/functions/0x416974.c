// rea decompile_all
// name: FUN_00416974
// address: 0x416974
// body: available
// range: 0x416974-0x416ad7 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Type propagation algorithm not settling */

undefined4 __cdecl
FUN_00416974(char *param_1,int *param_2,int param_3,undefined4 param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int *local_94;
  undefined4 local_90 [3];
  uint local_84;
  uint local_24;
  int local_20;
  uint local_1c [6];
  
  local_1c[1] = 0;
  local_1c[2] = 0;
  local_1c[3] = 0;
  local_1c[4] = 0;
  local_1c[5] = 0x10;
  if (param_2 == (int *)0x0) {
    param_2 = DAT_00486888;
  }
  _memset(local_90,0,0x6c);
  local_90[0] = 0x6c;
  local_20 = (**(code **)(*param_2 + 0x58))(param_2,local_90);
  if (local_20 == 0) {
    iVar2 = FUN_00416710(param_1,&local_94,param_5,&local_24,local_1c);
    if (iVar2 == 0) {
      if (param_3 < 0) {
        local_24 = local_24 + param_3;
        param_3 = 0;
        local_1c[1] = 0;
      }
      else if (local_84 < param_3 + local_24) {
        local_24 = local_84 - param_3;
      }
      local_1c[4] = local_1c[0];
      local_1c[3] = local_24;
      if (param_5 != 0) {
        local_1c[5] = local_1c[5] | 1;
      }
      local_20 = (**(code **)(*param_2 + 0x1c))
                           (param_2,param_3,param_4,local_94,local_1c + 1,local_1c[5]);
      (**(code **)(*local_94 + 8))(local_94);
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}

