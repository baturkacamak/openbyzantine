// rea decompile_all
// name: FUN_00448270
// address: 0x448270
// body: available
// range: 0x448270-0x4483be inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00448270(int param_1)

{
  int *piVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int local_14;
  int iStack_c;
  int iStack_8;
  int *piStack_4;
  
  iVar2 = *(int *)(param_1 + 0x1a4);
  if (*(int *)(param_1 + 0x4c) == 1) {
    local_14 = 0x1fe;
    *(undefined1 *)(iVar2 + 0x1c) = 1;
  }
  else {
    local_14 = 0;
    *(undefined1 *)(iVar2 + 0x1c) = 0;
  }
  uVar4 = (**(code **)(*(int *)(param_1 + 4) + 8))
                    (param_1,1,local_14 + 0x100,*(undefined4 *)(param_1 + 100));
  iVar9 = 0;
  *(undefined4 *)(iVar2 + 0x18) = uVar4;
  iStack_c = *(int *)(iVar2 + 0x14);
  if (0 < *(int *)(param_1 + 100)) {
    piStack_4 = (int *)(iVar2 + 0x20);
    iStack_8 = 0;
    do {
      iVar6 = *piStack_4;
      iStack_c = iStack_c / iVar6;
      if (local_14 != 0) {
        piVar1 = (int *)(*(int *)(iVar2 + 0x18) + iStack_8);
        *piVar1 = *piVar1 + 0xff;
      }
      iVar7 = 0;
      puVar3 = *(undefined1 **)(*(int *)(iVar2 + 0x18) + iStack_8);
      iVar5 = FUN_004483c0(param_1,iVar9,0,iVar6 + -1);
      for (iVar8 = 0; iVar8 < 0x100; iVar8 = iVar8 + 1) {
        while (iVar5 < iVar8) {
          iVar7 = iVar7 + 1;
          iVar5 = FUN_004483c0(param_1,iVar9,iVar7,iVar6 + -1);
        }
        puVar3[iVar8] = (char)iVar7 * (char)iStack_c;
      }
      if (local_14 != 0) {
        iVar6 = 1;
        do {
          iVar5 = iVar6 + 1;
          puVar3[-iVar6] = *puVar3;
          puVar3[iVar6 + 0xff] = puVar3[0xff];
          iVar6 = iVar5;
        } while (iVar5 < 0x100);
      }
      iStack_8 = iStack_8 + 4;
      piStack_4 = piStack_4 + 1;
      iVar9 = iVar9 + 1;
    } while (iVar9 < *(int *)(param_1 + 100));
  }
  return;
}

