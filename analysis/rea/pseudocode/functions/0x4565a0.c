// rea decompile_all
// name: FUN_004565a0
// address: 0x4565a0
// body: available
// range: 0x4565a0-0x456662 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_004565a0(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  uVar1 = *(uint *)(param_1 + 0x14);
  if ((((int)uVar1 < 0x43) || (iVar2 = *(int *)(param_1 + 0x10), iVar2 < 3)) || (9 < iVar2)) {
    return 0;
  }
  if ((3 < iVar2) && (iVar2 < 9)) {
    return 1;
  }
  if (((int)uVar1 < 0x57) || (iVar2 != 3)) {
    iVar4 = *(int *)(&DAT_00463f6c + iVar2 * 4);
  }
  else {
    iVar4 = iRam00463f74 + 7;
  }
  if ((uVar1 & 3) == 0) {
    iVar4 = iVar4 + 1;
  }
  iVar4 = iVar4 - (int)(((int)(uVar1 - 1) >> 2) + uVar1 * 0x16d + -0x63db + iVar4) % 7;
  iVar3 = *(int *)(param_1 + 0x1c);
  if (iVar2 == 3) {
    if ((iVar3 <= iVar4) && ((iVar4 != iVar3 || (*(int *)(param_1 + 8) < 2)))) {
      return 0;
    }
    return 1;
  }
  if ((iVar4 <= iVar3) && ((iVar4 != iVar3 || (0 < *(int *)(param_1 + 8))))) {
    return 0;
  }
  return 1;
}

