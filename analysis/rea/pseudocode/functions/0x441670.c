// rea decompile_all
// name: FUN_00441670
// address: 0x441670
// body: available
// range: 0x441670-0x4416f9 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00441670(undefined4 param_1,int *param_2,char param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = 0;
  iVar1 = param_2[2];
  iVar5 = param_2[6] * iVar1;
  if (0 < param_2[4]) {
    do {
      iVar4 = param_2[4] - iVar3;
      if (param_2[5] <= param_2[4] - iVar3) {
        iVar4 = param_2[5];
      }
      iVar2 = param_2[7] - (param_2[6] + iVar3);
      if (iVar2 <= iVar4) {
        iVar4 = iVar2;
      }
      iVar2 = param_2[1] - (param_2[6] + iVar3);
      if (iVar2 <= iVar4) {
        iVar4 = iVar2;
      }
      if (iVar4 < 1) {
        return;
      }
      iVar4 = iVar4 * iVar1;
      if (param_3 == '\0') {
        (*(code *)param_2[10])
                  (param_1,param_2 + 10,*(undefined4 *)(*param_2 + iVar3 * 4),iVar5,iVar4);
      }
      else {
        (*(code *)param_2[0xb])(param_1);
      }
      iVar5 = iVar5 + iVar4;
      iVar3 = iVar3 + param_2[5];
    } while (iVar3 < param_2[4]);
  }
  return;
}

