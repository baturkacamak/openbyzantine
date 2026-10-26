// rea decompile_all
// name: FUN_004110e5
// address: 0x4110e5
// body: available
// range: 0x4110e5-0x4111c6 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_004110e5(int param_1,uint *param_2,undefined4 param_3)

{
  int iVar1;
  char *pcVar2;
  undefined4 uVar3;
  int local_8;
  
  iVar1 = _strcmp((char *)param_2,&DAT_0045e9e0);
  if (iVar1 == 0) {
    local_8 = -1;
  }
  else {
    for (local_8 = 0; (local_8 < 0x14 && (*(int *)(param_1 + 0x2c + local_8 * 4) != 0));
        local_8 = local_8 + 1) {
      iVar1 = _strcmp(*(char **)(param_1 + 0x2c + local_8 * 4),(char *)param_2);
      if (iVar1 == 0) {
        return local_8;
      }
    }
    pcVar2 = FUN_0044c580(param_2);
    uVar3 = FUN_0044c2f0((SIZE_T)(pcVar2 + 1));
    *(undefined4 *)(param_1 + 0x2c + local_8 * 4) = uVar3;
    FUN_0044ba00(*(uint **)(param_1 + 0x2c + local_8 * 4),param_2);
    *(undefined4 *)(param_1 + 0x7c + local_8 * 4) = param_3;
  }
  return local_8;
}

