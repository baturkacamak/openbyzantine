// rea decompile_all
// name: FUN_00446d00
// address: 0x446d00
// body: available
// range: 0x446d00-0x446d9d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00446d00(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  
  iVar7 = *(int *)(param_1 + 0x1a4);
  iVar1 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x7fc);
  piVar2 = (int *)(iVar1 + 0x3fc);
  iVar3 = 0x10;
  *(int **)(iVar7 + 0x28) = piVar2;
  iVar7 = 0;
  uVar4 = 0x10;
  piVar5 = piVar2;
  piVar8 = piVar2;
  do {
    *piVar8 = iVar7;
    piVar8 = piVar8 + 1;
    iVar6 = -iVar7;
    iVar7 = iVar7 + 1;
    *piVar5 = iVar6;
    iVar3 = iVar3 + -1;
    piVar5 = piVar5 + -1;
  } while (iVar3 != 0);
  piVar5 = (int *)(iVar1 + 0x43c);
  piVar8 = (int *)(iVar1 + 0x3bc);
  do {
    *piVar5 = iVar7;
    piVar5 = piVar5 + 1;
    uVar4 = uVar4 + 1;
    *piVar8 = -iVar7;
    iVar7 = iVar7 + (~uVar4 & 1);
    piVar8 = piVar8 + -1;
  } while ((int)uVar4 < 0x30);
  if ((int)uVar4 < 0x100) {
    piVar5 = piVar2 + uVar4;
    iVar1 = 0x100 - uVar4;
    piVar8 = piVar2 + -uVar4;
    do {
      *piVar5 = iVar7;
      piVar5 = piVar5 + 1;
      iVar1 = iVar1 + -1;
      *piVar8 = -iVar7;
      piVar8 = piVar8 + -1;
    } while (iVar1 != 0);
  }
  return;
}

