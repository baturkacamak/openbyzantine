// rea decompile_all
// name: FUN_00443d50
// address: 0x443d50
// body: available
// range: 0x443d50-0x443e63 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00443d50(undefined4 *param_1,uint param_2,int param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  int local_4;
  
  local_4 = param_5;
  if (param_3 < param_5) {
    uVar1 = FUN_00443c50(param_1,param_2,param_3,param_5);
    if ((char)uVar1 == '\0') {
      return 0xffffffff;
    }
    param_2 = param_1[3];
    param_3 = param_1[4];
  }
  iVar2 = param_3 - param_5;
  uVar4 = (int)param_2 >> ((byte)iVar2 & 0x1f) & (1 << ((byte)param_5 & 0x1f)) - 1U;
  piVar3 = (int *)(param_4 + 0x44 + param_5 * 4);
  if (*piVar3 < (int)uVar4) {
    do {
      if (iVar2 < 1) {
        uVar1 = FUN_00443c50(param_1,param_2,iVar2,1);
        if ((char)uVar1 == '\0') {
          return 0xffffffff;
        }
        param_2 = param_1[3];
        iVar2 = param_1[4];
      }
      iVar2 = iVar2 + -1;
      piVar3 = piVar3 + 1;
      local_4 = local_4 + 1;
      uVar4 = uVar4 * 2 | (int)param_2 >> ((byte)iVar2 & 0x1f) & 1U;
    } while (*piVar3 < (int)uVar4);
  }
  param_1[3] = param_2;
  param_1[4] = iVar2;
  if (local_4 < 0x11) {
    piVar3 = (int *)(param_4 + local_4 * 4);
    return (uint)*(byte *)(((*(int *)(param_4 + 0xd0) + piVar3[0x23]) - *piVar3) + 0x11 + uVar4);
  }
  *(undefined4 *)(*(int *)param_1[5] + 0x14) = 0x72;
  (**(code **)(*(int *)param_1[5] + 4))((int *)param_1[5],0xffffffff);
  return 0;
}

