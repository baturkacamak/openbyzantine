// rea decompile_all
// name: FUN_004429c0
// address: 0x4429c0
// body: available
// range: 0x4429c0-0x442a55 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004429c0(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 *puVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  
  iVar7 = 0;
  iVar2 = *(int *)(param_1 + 0x180);
  iVar8 = *(int *)(param_1 + 0xc4);
  if (0 < *(int *)(param_1 + 0x20)) {
    do {
      uVar6 = *(int *)(iVar8 + 0x24) * *(int *)(iVar8 + 0xc);
      iVar3 = (int)uVar6 / *(int *)(param_1 + 0x114);
      uVar4 = *(uint *)(iVar8 + 0x2c) % uVar6;
      if (uVar4 == 0) {
        uVar4 = uVar6;
      }
      if (iVar7 == 0) {
        *(int *)(iVar2 + 0x48) = (int)(uVar4 - 1) / iVar3 + 1;
      }
      if (0 < iVar3 * 2) {
        puVar1 = (undefined4 *)
                 (*(int *)(*(int *)(iVar2 + 0x38 + *(int *)(iVar2 + 0x40) * 4) + iVar7 * 4) +
                 uVar4 * 4);
        iVar3 = iVar3 * 2;
        puVar5 = puVar1;
        do {
          iVar3 = iVar3 + -1;
          *puVar5 = puVar1[-1];
          puVar5 = puVar5 + 1;
        } while (iVar3 != 0);
      }
      iVar7 = iVar7 + 1;
      iVar8 = iVar8 + 0x54;
    } while (iVar7 < *(int *)(param_1 + 0x20));
  }
  return;
}

