// rea decompile_all
// name: FUN_0040241c
// address: 0x40241c
// body: available
// range: 0x40241c-0x4025c6 inclusive
// range: 0x4025cc-0x40271a inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl
FUN_0040241c(short *param_1,short *param_2,short param_3,short param_4,LPRECT param_5)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  int local_68;
  int local_64;
  int local_60 [2];
  int local_58;
  int local_54;
  int local_50 [2];
  int local_48 [2];
  int local_40;
  int local_3c;
  int local_38 [2];
  int local_30;
  int local_2c;
  int local_28 [2];
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_64 = *param_2 + -0x14;
  local_10 = local_64;
  if ((DAT_0045c060 == 0) || (DAT_00464880 == (void *)0x0)) {
    sVar1 = *param_1;
    local_14 = (int)sVar1;
    local_c = param_3 + local_14 + -1;
    local_8 = param_4 + local_64 + -1;
    if ((local_c < 0) || (((local_8 < 0 || (0x27f < local_14)) || (399 < local_64)))) {
      uVar3 = 0;
    }
    else {
      *param_1 = sVar1;
      *param_2 = (short)local_64 + 0x14;
      if (local_14 < 0) {
        param_3 = param_3 + sVar1;
        local_14 = 0;
      }
      if (0x280 < param_3 + local_14) {
        param_3 = 0x280 - (short)local_14;
      }
      local_18 = local_64;
      if (local_64 < 0) {
        param_4 = param_4 + (short)local_64;
        local_18 = 0;
      }
      if (400 < param_4 + local_18) {
        param_4 = 400 - (short)local_18;
      }
      if ((param_3 < 1) || (param_4 < 1)) {
        uVar3 = 0;
      }
      else {
        local_18 = local_18 + 0x14;
        SetRect(param_5,local_14,local_18,param_3 + local_14 + -1,param_4 + local_18 + -1);
        uVar3 = 1;
      }
    }
  }
  else {
    local_40 = (int)*param_1;
    local_68 = (int)*param_1 + (int)param_3 + -1;
    local_54 = param_4 + local_64 + -1;
    local_20 = ((int)param_3 >> 1) + (int)*param_1;
    local_1c = ((int)param_4 >> 1) + local_64;
    local_58 = local_68;
    local_3c = local_64;
    local_30 = local_40;
    local_2c = local_54;
    iVar2 = FUN_00423e03(DAT_00464880,&local_20,local_50);
    if (((iVar2 == 0) && (iVar2 = FUN_00423e03(DAT_00464880,&local_40,local_28), iVar2 == 0)) &&
       ((iVar2 = FUN_00423e03(DAT_00464880,&local_58,local_38), iVar2 == 0 &&
        ((iVar2 = FUN_00423e03(DAT_00464880,&local_68,local_48), iVar2 == 0 &&
         (iVar2 = FUN_00423e03(DAT_00464880,&local_30,local_60), iVar2 == 0)))))) {
      uVar3 = 0;
    }
    else {
      uVar3 = 1;
    }
  }
  return uVar3;
}

