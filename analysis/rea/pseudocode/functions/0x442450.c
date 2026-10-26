// rea decompile_all
// name: FUN_00442450
// address: 0x442450
// body: available
// range: 0x442450-0x44250c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00442450(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uStack_8;
  
  iVar1 = *(int *)(param_1 + 0x180);
  iVar2 = *(int *)(param_1 + 0x114);
  uVar3 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,*(int *)(param_1 + 0x20) << 3);
  iVar8 = 0;
  *(undefined4 *)(iVar1 + 0x38) = uVar3;
  uStack_8 = 0;
  *(int *)(iVar1 + 0x3c) = *(int *)(param_1 + 0x20) * 4 + *(int *)(iVar1 + 0x38);
  iVar6 = *(int *)(param_1 + 0xc4);
  if (0 < *(int *)(param_1 + 0x20)) {
    do {
      iVar8 = iVar8 + 4;
      iVar4 = (*(int *)(iVar6 + 0x24) * *(int *)(iVar6 + 0xc)) / *(int *)(param_1 + 0x114);
      iVar6 = iVar6 + 0x54;
      iVar7 = iVar4 * (iVar2 + 4);
      iVar5 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,iVar7 * 8);
      uStack_8 = uStack_8 + 1;
      iVar5 = iVar5 + iVar4 * 4;
      *(int *)(*(int *)(iVar1 + 0x38) + -4 + iVar8) = iVar5;
      *(int *)(*(int *)(iVar1 + 0x3c) + -4 + iVar8) = iVar5 + iVar7 * 4;
    } while (uStack_8 < *(int *)(param_1 + 0x20));
  }
  return;
}

