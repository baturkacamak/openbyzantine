// rea decompile_all
// name: FUN_00442590
// address: 0x442590
// body: available
// range: 0x442590-0x4426e9 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00442590(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  int local_14;
  int local_10;
  int *local_c;
  int local_8;
  
  iVar1 = *(int *)(param_1 + 0x180);
  iVar2 = *(int *)(param_1 + 0x114);
  local_8 = 0;
  local_10 = *(int *)(param_1 + 0xc4);
  if (0 < *(int *)(param_1 + 0x20)) {
    local_14 = 0;
    local_c = (int *)(iVar1 + 8);
    do {
      iVar5 = (*(int *)(local_10 + 0x24) * *(int *)(local_10 + 0xc)) / *(int *)(param_1 + 0x114);
      puVar3 = *(undefined4 **)(*(int *)(iVar1 + 0x38) + local_14);
      puVar9 = *(undefined4 **)(*(int *)(iVar1 + 0x3c) + local_14);
      puVar12 = (undefined4 *)*local_c;
      iVar6 = iVar5 * (iVar2 + 2);
      puVar8 = puVar9;
      puVar10 = puVar3;
      puVar11 = puVar12;
      if (0 < iVar6) {
        do {
          uVar4 = *puVar11;
          *puVar8 = uVar4;
          *puVar10 = uVar4;
          iVar6 = iVar6 + -1;
          puVar8 = puVar8 + 1;
          puVar10 = puVar10 + 1;
          puVar11 = puVar11 + 1;
        } while (iVar6 != 0);
      }
      if (0 < iVar5 * 2) {
        iVar7 = (iVar2 + -2) * iVar5;
        puVar8 = puVar9 + iVar7;
        puVar10 = puVar12 + iVar2 * iVar5;
        iVar6 = iVar5 * 2;
        puVar9 = puVar9 + iVar2 * iVar5;
        puVar12 = puVar12 + iVar7;
        do {
          *puVar8 = *puVar10;
          puVar10 = puVar10 + 1;
          puVar8 = puVar8 + 1;
          *puVar9 = *puVar12;
          iVar6 = iVar6 + -1;
          puVar9 = puVar9 + 1;
          puVar12 = puVar12 + 1;
        } while (iVar6 != 0);
      }
      if (0 < iVar5) {
        puVar9 = puVar3 + -iVar5;
        do {
          iVar5 = iVar5 + -1;
          *puVar9 = *puVar3;
          puVar9 = puVar9 + 1;
        } while (iVar5 != 0);
      }
      local_8 = local_8 + 1;
      local_c = local_c + 1;
      local_14 = local_14 + 4;
      local_10 = local_10 + 0x54;
    } while (local_8 < *(int *)(param_1 + 0x20));
  }
  return;
}

