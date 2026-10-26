// rea decompile_all
// name: FUN_00421373
// address: 0x421373
// body: available
// range: 0x421373-0x42151d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 * __cdecl FUN_00421373(FILE *param_1,int param_2,undefined4 param_3)

{
  bool bVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 local_170;
  int local_164;
  int local_15c;
  char local_158 [80];
  int aiStack_108 [32];
  int local_88;
  int aiStack_84 [32];
  
  bVar1 = false;
  iVar2 = FUN_0044d340(param_1,(byte *)s_Scene___d_Dialog___d_0045fcc0);
  if (iVar2 == 2) {
    for (local_15c = 0; iVar2 = local_15c, local_15c < 0x20; local_15c = local_15c + 1) {
      local_88 = FUN_0044d340(param_1,(byte *)s__d___d_0045fcd8);
      if (local_88 != 2) break;
      if (aiStack_108[local_15c] == param_2) {
        bVar1 = true;
      }
    }
    FUN_0044ce20(local_158,0x50,param_1);
    if (bVar1) {
      puVar3 = FUN_0042130c(local_170,local_15c,param_3);
      for (local_15c = 0; local_15c < iVar2; local_15c = local_15c + 1) {
        *(int *)(puVar3[3] + local_15c * 4) = aiStack_108[local_15c];
        FUN_0041db33(local_164,aiStack_84[local_15c],(undefined4 *)(puVar3[4] + local_15c * 4));
      }
    }
    else {
      puVar3 = (undefined4 *)0x0;
    }
  }
  else {
    puVar3 = (undefined4 *)0x0;
  }
  return puVar3;
}

