// rea decompile_all
// name: FUN_00442370
// address: 0x442370
// body: available
// range: 0x442370-0x442447 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00442370(int *param_1,char param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iStack_4;
  
  puVar1 = (undefined4 *)(**(code **)param_1[1])(param_1,1,0x50);
  param_1[0x60] = (int)puVar1;
  *puVar1 = &LAB_00442510;
  if (param_2 != '\0') {
    *(undefined4 *)(*param_1 + 0x14) = 4;
    (**(code **)*param_1)(param_1);
  }
  if (*(char *)(param_1[0x67] + 8) == '\0') {
    iStack_4 = param_1[0x45];
  }
  else {
    if (param_1[0x45] < 2) {
      *(undefined4 *)(*param_1 + 0x14) = 0x2e;
      (**(code **)*param_1)(param_1);
    }
    FUN_00442450((int)param_1);
    iStack_4 = param_1[0x45] + 2;
  }
  iVar3 = 0;
  if (0 < param_1[8]) {
    iVar4 = param_1[0x31];
    puVar1 = puVar1 + 2;
    do {
      iVar3 = iVar3 + 1;
      uVar2 = (**(code **)(param_1[1] + 8))
                        (param_1,1,*(int *)(iVar4 + 0x1c) * *(int *)(iVar4 + 0x24),
                         iStack_4 *
                         ((*(int *)(iVar4 + 0xc) * *(int *)(iVar4 + 0x24)) / param_1[0x45]));
      *puVar1 = uVar2;
      iVar4 = iVar4 + 0x54;
      puVar1 = puVar1 + 1;
    } while (iVar3 < param_1[8]);
  }
  return;
}

