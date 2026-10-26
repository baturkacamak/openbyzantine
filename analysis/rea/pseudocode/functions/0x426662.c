// rea decompile_all
// name: FUN_00426662
// address: 0x426662
// body: available
// range: 0x426662-0x426787 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00426662(int param_1,int *param_2,FILE *param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  undefined1 local_14;
  undefined4 local_10;
  undefined4 local_8;
  
  local_8 = 0;
  iVar1 = _fgetc(param_3);
  if ((char)iVar1 == -1) {
    *param_2 = 0;
  }
  else {
    if (*param_2 == 0) {
      iVar2 = FUN_0044c2f0(8);
      *param_2 = iVar2;
    }
    *(char *)*param_2 = (char)iVar1;
    uVar3 = _fgetc(param_3);
    uVar4 = _fgetc(param_3);
    local_10 = *(int *)(param_1 + 0x58c + (uVar3 & 0xff) * 4);
    local_14 = 0;
    for (; ((uint)local_14 < (uVar4 & 0xff) && (local_10 != 0)); local_10 = *(int *)(local_10 + 8))
    {
      local_14 = local_14 + 1;
    }
    if (local_10 == 0) {
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 1;
    }
    else {
      *(int *)(*param_2 + 4) = local_10;
    }
  }
  return local_8;
}

