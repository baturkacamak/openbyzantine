// rea decompile_all
// name: FUN_00440b90
// address: 0x440b90
// body: available
// range: 0x440b90-0x440c96 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00440b90(int *param_1)

{
  int iVar1;
  byte bVar2;
  undefined4 *puVar3;
  uint uVar4;
  undefined4 uVar5;
  byte *pbVar6;
  byte *pbVar7;
  int iVar8;
  
  puVar3 = (undefined4 *)param_1[5];
  pbVar6 = (byte *)*puVar3;
  iVar8 = puVar3[1];
  if (iVar8 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar6 = (byte *)*puVar3;
    iVar8 = puVar3[1];
  }
  iVar8 = iVar8 + -1;
  bVar2 = *pbVar6;
  pbVar6 = pbVar6 + 1;
  if (iVar8 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar6 = (byte *)*puVar3;
    iVar8 = puVar3[1];
  }
  iVar8 = iVar8 + -1;
  pbVar7 = pbVar6 + 1;
  if ((uint)bVar2 * 0x100 + (uint)*pbVar6 != 4) {
    *(undefined4 *)(*param_1 + 0x14) = 9;
    (**(code **)*param_1)(param_1);
  }
  if (iVar8 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar8 = puVar3[1];
  }
  iVar8 = iVar8 + -1;
  bVar2 = *pbVar7;
  pbVar7 = pbVar7 + 1;
  if (iVar8 == 0) {
    uVar4 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    iVar8 = puVar3[1];
  }
  iVar1 = (uint)bVar2 * 0x100 + (uint)*pbVar7;
  *(undefined4 *)(*param_1 + 0x14) = 0x51;
  *(int *)(*param_1 + 0x18) = iVar1;
  uVar5 = (**(code **)(*param_1 + 4))(param_1,1);
  param_1[0x3f] = iVar1;
  *puVar3 = pbVar7 + 1;
  puVar3[1] = iVar8 + -1;
  return CONCAT31((int3)((uint)uVar5 >> 8),1);
}

