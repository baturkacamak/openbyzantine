// rea decompile_all
// name: FUN_00448520
// address: 0x448520
// body: available
// range: 0x448520-0x44858b inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00448520(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  
  iVar7 = 0;
  iVar1 = *(int *)(param_1 + 0x1a4);
  if (0 < *(int *)(param_1 + 100)) {
    piVar2 = (int *)(iVar1 + 0x20);
    piVar5 = piVar2;
    do {
      iVar3 = 0;
      iVar4 = 0;
      piVar6 = piVar2;
      if (0 < iVar7) {
        do {
          if (*piVar6 == *piVar5) {
            iVar3 = *(int *)(iVar1 + 0x34 + iVar4 * 4);
            break;
          }
          iVar4 = iVar4 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar4 < iVar7);
      }
      if (iVar3 == 0) {
        iVar3 = FUN_00448590(param_1,*piVar5);
      }
      piVar5[5] = iVar3;
      piVar5 = piVar5 + 1;
      iVar7 = iVar7 + 1;
    } while (iVar7 < *(int *)(param_1 + 100));
  }
  return;
}

