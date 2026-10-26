// rea decompile_all
// name: FUN_00416d05
// address: 0x416d05
// body: available
// range: 0x416d05-0x416eba inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Type propagation algorithm not settling */

undefined4 __cdecl
FUN_00416d05(char *param_1,undefined4 *param_2,int param_3,uint *param_4,uint *param_5)

{
  undefined4 uVar1;
  uint local_8c;
  uint local_88;
  uint local_84 [4];
  uint local_74;
  int local_70;
  int local_5c;
  undefined4 local_18;
  int local_14;
  uint local_10;
  ushort *local_c;
  int *local_8;
  
  local_c = FUN_0040f135(param_1,(int *)local_84,&local_10,(ushort *)0x0,0x280);
  if (local_c == (ushort *)0x0) {
    uVar1 = 1;
  }
  else {
    _memset(local_84 + 1,0,0x6c);
    local_84[1] = 0x6c;
    local_84[2] = 7;
    local_18 = 0x840;
    local_74 = local_84[0];
    local_84[3] = local_10;
    local_14 = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,local_84 + 1,param_2,0);
    if (local_14 == 0) {
      local_8 = (int *)*param_2;
      local_14 = (**(code **)(*local_8 + 0x58))(local_8,local_84 + 1);
      if (param_3 != 0) {
        local_8c = (uint)DAT_00485c88;
        local_88 = (uint)DAT_00485c88;
        local_14 = (**(code **)(*local_8 + 0x74))(local_8,8,&local_8c);
        if (local_14 != 0) {
          return 1;
        }
      }
      local_14 = (**(code **)(*local_8 + 100))(local_8,0,local_84 + 1,1,0);
      if (local_14 == 0) {
        FUN_0042b83f(local_5c,local_c,local_70 >> 1,local_10,local_84[0],0,0);
        (**(code **)(*local_8 + 0x80))(local_8,0);
        FUN_0044ac40(local_c);
        if (param_4 != (uint *)0x0) {
          *param_4 = local_84[0];
        }
        if (param_5 != (uint *)0x0) {
          *param_5 = local_10;
        }
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

