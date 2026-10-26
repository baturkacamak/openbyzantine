// rea decompile_all
// name: FUN_00447dd0
// address: 0x447dd0
// body: available
// range: 0x447dd0-0x447f33 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00447dd0(int param_1,int *param_2,int param_3)

{
  int iVar1;
  ushort *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int local_20;
  ushort *local_1c;
  int local_18;
  int *local_14;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar3 = 0;
  iVar4 = 0;
  local_10 = 0;
  local_c = 0;
  iVar7 = param_2[2];
  iVar1 = param_2[4];
  iVar5 = *param_2;
  if (iVar5 <= param_2[1]) {
    local_18 = iVar5 * 8 + 4;
    local_14 = (int *)(*(int *)(*(int *)(param_1 + 0x1a4) + 0x18) + iVar5 * 4);
    local_4 = (param_2[1] - iVar5) + 1;
    do {
      if (iVar7 <= param_2[3]) {
        local_20 = iVar7 * 4 + 2;
        local_1c = (ushort *)((iVar7 * 0x20 + iVar1) * 2 + *local_14);
        local_8 = (param_2[3] - iVar7) + 1;
        do {
          if (iVar1 <= param_2[5]) {
            iVar5 = iVar1 * 8 + 4;
            iVar8 = (param_2[5] - iVar1) + 1;
            puVar2 = local_1c;
            do {
              uVar6 = (uint)*puVar2;
              if (uVar6 != 0) {
                iVar3 = iVar3 + uVar6;
                iVar4 = iVar4 + local_18 * uVar6;
                local_10 = local_10 + local_20 * uVar6;
                local_c = local_c + iVar5 * uVar6;
              }
              iVar5 = iVar5 + 8;
              iVar8 = iVar8 + -1;
              puVar2 = puVar2 + 1;
            } while (iVar8 != 0);
          }
          local_20 = local_20 + 4;
          local_1c = local_1c + 0x20;
          local_8 = local_8 + -1;
        } while (local_8 != 0);
      }
      local_18 = local_18 + 8;
      local_14 = local_14 + 1;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  iVar7 = iVar3 >> 1;
  *(char *)(**(int **)(param_1 + 0x74) + param_3) = (char)((iVar7 + iVar4) / iVar3);
  *(char *)(*(int *)(*(int *)(param_1 + 0x74) + 4) + param_3) = (char)((local_10 + iVar7) / iVar3);
  *(char *)(*(int *)(*(int *)(param_1 + 0x74) + 8) + param_3) = (char)((local_c + iVar7) / iVar3);
  return;
}

