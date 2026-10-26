// rea decompile_all
// name: FUN_004444c0
// address: 0x4444c0
// body: available
// range: 0x4444c0-0x44452e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_004444c0(int param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  undefined4 *puVar6;
  
  iVar2 = *(int *)(param_1 + 0x194);
  piVar1 = (int *)(*(int *)(param_1 + 400) + 0x58);
  *piVar1 = *piVar1 + ((int)(*(int *)(iVar2 + 0xc) + (*(int *)(iVar2 + 0xc) >> 0x1f & 7U)) >> 3);
  *(undefined4 *)(iVar2 + 0xc) = 0;
  uVar4 = (**(code **)(*(int *)(param_1 + 400) + 8))(param_1);
  if ((char)uVar4 == '\0') {
    return uVar4 & 0xffffff00;
  }
  iVar5 = 0;
  if (0 < *(int *)(param_1 + 0x120)) {
    puVar6 = (undefined4 *)(iVar2 + 0x14);
    do {
      *puVar6 = 0;
      puVar6 = puVar6 + 1;
      iVar5 = iVar5 + 1;
    } while (iVar5 < *(int *)(param_1 + 0x120));
  }
  uVar3 = *(undefined4 *)(param_1 + 0xfc);
  *(undefined4 *)(iVar2 + 0x24) = uVar3;
  *(undefined1 *)(iVar2 + 0x10) = 0;
  return CONCAT31((int3)((uint)uVar3 >> 8),1);
}

