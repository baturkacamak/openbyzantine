// rea decompile_all
// name: FUN_004428a0
// address: 0x4428a0
// body: available
// range: 0x4428a0-0x4429be inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004428a0(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  int local_24;
  int local_20;
  undefined4 *local_14;
  undefined4 *local_10;
  undefined4 *local_c;
  undefined4 *local_8;
  int local_4;
  
  iVar1 = *(int *)(param_1 + 0x180);
  iVar2 = *(int *)(param_1 + 0x114);
  local_4 = 0;
  local_20 = *(int *)(param_1 + 0xc4);
  if (0 < *(int *)(param_1 + 0x20)) {
    local_24 = 0;
    do {
      iVar4 = (*(int *)(local_20 + 0x24) * *(int *)(local_20 + 0xc)) / *(int *)(param_1 + 0x114);
      local_10 = *(undefined4 **)(*(int *)(iVar1 + 0x38) + local_24);
      puVar9 = *(undefined4 **)(*(int *)(iVar1 + 0x3c) + local_24);
      if (0 < iVar4) {
        iVar5 = (iVar2 + 2) * iVar4;
        iVar6 = (iVar2 + 1) * iVar4;
        local_8 = puVar9 + iVar6;
        puVar10 = local_10 + iVar6;
        puVar7 = local_10 + -iVar4;
        puVar8 = puVar9 + -iVar4;
        local_14 = puVar9 + iVar5;
        local_c = local_10 + iVar5;
        do {
          *puVar7 = *puVar10;
          uVar3 = *local_8;
          puVar10 = puVar10 + 1;
          puVar7 = puVar7 + 1;
          local_8 = local_8 + 1;
          *puVar8 = uVar3;
          *local_c = *local_10;
          iVar4 = iVar4 + -1;
          *local_14 = *puVar9;
          puVar8 = puVar8 + 1;
          puVar9 = puVar9 + 1;
          local_14 = local_14 + 1;
          local_10 = local_10 + 1;
          local_c = local_c + 1;
        } while (iVar4 != 0);
      }
      local_4 = local_4 + 1;
      local_24 = local_24 + 4;
      local_20 = local_20 + 0x54;
    } while (local_4 < *(int *)(param_1 + 0x20));
  }
  return;
}

