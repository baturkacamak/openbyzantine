// rea decompile_all
// name: FUN_004067e2
// address: 0x4067e2
// body: available
// range: 0x4067e2-0x4069a3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004067e2(int *param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 local_34;
  int local_28;
  int local_20;
  int local_18;
  int local_14;
  int local_10;
  uint local_8;
  
  iVar1 = param_1[2];
  iVar2 = *param_1;
  iVar3 = param_1[3];
  iVar4 = param_1[1];
  local_18 = param_1[1] * param_1[5] + *param_1;
  local_14 = param_1[9] + (param_1[7] * param_1[4] + param_1[6]) * 2;
  local_10 = param_1[10] + local_18;
  if (param_1[0xb] == 0) {
    local_18 = 0;
  }
  else {
    local_18 = param_1[0xb] + local_18;
  }
  for (local_20 = 0; local_20 < iVar3 - iVar4; local_20 = local_20 + 1) {
    for (local_28 = 0; local_28 < iVar1 - iVar2; local_28 = local_28 + 1) {
      uVar6 = (uint)*(byte *)(local_28 + local_10);
      local_34 = CONCAT22(local_34._2_2_,*(undefined2 *)(local_14 + local_28 * 2));
      if (local_18 == 0) {
        local_8 = 0;
      }
      else {
        local_8 = (uint)*(byte *)(local_28 + local_18);
        if (local_8 == 0xff) {
          local_34 = (uint)local_34._2_2_ << 0x10;
        }
      }
      if (uVar6 == 0xff) {
        *(undefined2 *)(local_14 + local_28 * 2) = (undefined2)param_2;
      }
      else if ((uVar6 != 0) || (local_8 != 0)) {
        if (local_18 != 0) {
          uVar5 = FUN_004066c6(0,local_34,local_8);
          local_34 = CONCAT22(local_34._2_2_,(short)uVar5);
        }
        uVar5 = FUN_004066c6(param_2,local_34,uVar6);
        *(short *)(local_14 + local_28 * 2) = (short)uVar5;
      }
    }
    local_14 = local_14 + param_1[4] * 2;
    local_10 = local_10 + param_1[5];
    if (local_18 != 0) {
      local_18 = local_18 + param_1[5];
    }
  }
  return;
}

