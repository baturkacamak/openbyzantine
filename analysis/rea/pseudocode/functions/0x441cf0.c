// rea decompile_all
// name: FUN_00441cf0
// address: 0x441cf0
// body: available
// range: 0x441cf0-0x441d71 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00441cf0(int param_1)

{
  int iVar1;
  int iVar2;
  uint3 uVar3;
  
  uVar3 = (uint3)((uint)param_1 >> 8);
  if ((*(char *)(param_1 + 0x48) != '\0') || (*(char *)(param_1 + 0x108) != '\0')) {
    return (uint)uVar3 << 8;
  }
  if ((((*(int *)(param_1 + 0x24) == 3) && (*(int *)(param_1 + 0x20) == 3)) &&
      (*(int *)(param_1 + 0x28) == 2)) && (*(int *)(param_1 + 100) == 3)) {
    iVar1 = *(int *)(param_1 + 0xc4);
    if ((((*(int *)(iVar1 + 8) == 2) && (*(int *)(iVar1 + 0x5c) == 1)) &&
        ((*(int *)(iVar1 + 0xb0) == 1 &&
         ((*(int *)(iVar1 + 0xc) < 3 && (*(int *)(iVar1 + 0x60) == 1)))))) &&
       (*(int *)(iVar1 + 0xb4) == 1)) {
      iVar2 = *(int *)(param_1 + 0x114);
      uVar3 = (uint3)((uint)iVar2 >> 8);
      if (((*(int *)(iVar1 + 0x24) == iVar2) && (*(int *)(iVar1 + 0x78) == iVar2)) &&
         (*(int *)(iVar1 + 0xcc) == iVar2)) {
        return CONCAT31(uVar3,1);
      }
      return (uint)uVar3 << 8;
    }
    return (uint)uVar3 << 8;
  }
  return (uint)uVar3 << 8;
}

