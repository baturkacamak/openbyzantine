// rea decompile_all
// name: FUN_004330bf
// address: 0x4330bf
// body: available
// range: 0x4330bf-0x4331d1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


ushort __cdecl FUN_004330bf(int param_1,int param_2)

{
  bool bVar1;
  ushort uVar2;
  int local_14;
  int local_10;
  
  bVar1 = false;
  if (*(char *)((uint)*(ushort *)(param_1 + 8) + param_2) == -1) {
    uVar2 = *(ushort *)(param_1 + 8);
  }
  else {
    local_10 = *(int *)(param_1 + 0x654 + (uint)*(ushort *)(param_1 + 8) * 4);
    local_14 = *(int *)(param_1 + 0x58c + (uint)*(ushort *)(param_1 + 8) * 4);
    while ((local_14 != 0 && (!bVar1))) {
      if ((*(char *)(local_14 + 0x10) != '\0') && (*(char *)(local_14 + 0x11) == '\0')) {
        bVar1 = true;
      }
      local_14 = *(int *)(local_14 + 8);
      if ((local_14 == 0) && (local_10 != 0)) {
        local_14 = *(int *)(local_10 + 8);
        local_10 = *(int *)(local_10 + 4);
      }
    }
    if (bVar1) {
      uVar2 = *(ushort *)(param_1 + 8);
    }
    else {
      uVar2 = (ushort)*(byte *)((uint)*(ushort *)(param_1 + 8) + param_2);
    }
  }
  return uVar2;
}

