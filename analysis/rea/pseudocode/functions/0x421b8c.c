// rea decompile_all
// name: FUN_00421b8c
// address: 0x421b8c
// body: available
// range: 0x421b8c-0x421d5d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00421b8c(undefined4 *param_1,int *param_2,undefined4 param_3)

{
  int iVar1;
  char *pcVar2;
  undefined4 *puVar3;
  LPVOID pvVar4;
  int local_20;
  int local_1c;
  int local_18;
  uint *local_10;
  uint *local_8;
  
  while( true ) {
    if (99 < *param_2) {
      return;
    }
    iVar1 = FUN_00421a5d((int)param_1,(byte)*param_2);
    local_1c = param_1[1] - iVar1;
    local_10 = (uint *)FUN_0044c520(*(char **)(param_1[4] + -4 + iVar1 * 4),'\n');
    if ((local_1c == 0) && (local_10 == (uint *)0x0)) break;
    if (local_10 != (uint *)0x0) {
      local_1c = local_1c + 1;
      *(undefined1 *)local_10 = 0;
      local_10 = (uint *)((int)local_10 + 1);
      pcVar2 = FUN_0044c580(local_10);
      local_8 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar2 + 1));
      FUN_0044ba00(local_8,local_10);
    }
    puVar3 = FUN_0042130c(0xffffffff,local_1c,param_3);
    local_20 = iVar1;
    for (local_18 = 0; local_18 < (int)puVar3[1]; local_18 = local_18 + 1) {
      if ((local_10 != (uint *)0x0) && (local_18 == 0)) {
        *(undefined4 *)puVar3[3] = *(undefined4 *)(param_1[3] + -4 + iVar1 * 4);
        *(uint **)puVar3[4] = local_8;
        local_18 = 1;
      }
      *(undefined4 *)(puVar3[3] + local_18 * 4) = *(undefined4 *)(param_1[3] + local_20 * 4);
      *(undefined4 *)(puVar3[4] + local_18 * 4) = *(undefined4 *)(param_1[4] + local_20 * 4);
      local_20 = local_20 + 1;
    }
    pvVar4 = FUN_0044d2d0((LPVOID)param_1[4],iVar1 << 2);
    param_1[4] = pvVar4;
    pvVar4 = FUN_0044d2d0((LPVOID)param_1[3],iVar1 << 2);
    param_1[3] = pvVar4;
    param_1[1] = iVar1;
    *(undefined4 **)(&DAT_0046c8d0 + *param_2 * 4) = param_1;
    *param_2 = *param_2 + 1;
    param_1 = puVar3;
  }
  *(undefined4 **)(&DAT_0046c8d0 + *param_2 * 4) = param_1;
  *param_2 = *param_2 + 1;
  return;
}

