// rea decompile_all
// name: FUN_00440ca0
// address: 0x440ca0
// body: available
// range: 0x440ca0-0x440d4f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00440ca0(int *param_1)

{
  byte bVar1;
  undefined4 *puVar2;
  uint uVar3;
  byte *pbVar4;
  int iVar5;
  
  puVar2 = (undefined4 *)param_1[5];
  pbVar4 = (byte *)*puVar2;
  iVar5 = puVar2[1];
  if (iVar5 == 0) {
    uVar3 = (*(code *)puVar2[3])(param_1);
    if ((char)uVar3 == '\0') {
      return uVar3 & 0xffffff00;
    }
    pbVar4 = (byte *)*puVar2;
    iVar5 = puVar2[1];
  }
  iVar5 = iVar5 + -1;
  bVar1 = *pbVar4;
  pbVar4 = pbVar4 + 1;
  if (iVar5 == 0) {
    uVar3 = (*(code *)puVar2[3])(param_1);
    if ((char)uVar3 == '\0') {
      return uVar3 & 0xffffff00;
    }
    pbVar4 = (byte *)*puVar2;
    iVar5 = puVar2[1];
  }
  uVar3 = (uint)*pbVar4;
  if ((bVar1 != 0xff) || (uVar3 != 0xd8)) {
    *(undefined4 *)(*param_1 + 0x14) = 0x34;
    *(uint *)(*param_1 + 0x18) = (uint)bVar1;
    *(uint *)(*param_1 + 0x1c) = uVar3;
    (**(code **)*param_1)(param_1);
  }
  param_1[0x5e] = uVar3;
  *puVar2 = pbVar4 + 1;
  puVar2[1] = iVar5 + -1;
  return 1;
}

