// rea decompile_all
// name: FUN_00442a90
// address: 0x442a90
// body: available
// range: 0x442a90-0x442b91 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00442a90(int param_1,char param_2)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  undefined4 *puVar8;
  int iStack_4;
  
  puVar1 = (undefined4 *)(*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x74);
  *(undefined4 **)(param_1 + 0x184) = puVar1;
  *puVar1 = &LAB_00442ba0;
  puVar1[2] = &LAB_00442c20;
  puVar1[0x1c] = 0;
  if (param_2 != '\0') {
    iStack_4 = 0;
    if (0 < *(int *)(param_1 + 0x20)) {
      iVar5 = *(int *)(param_1 + 0xc4);
      puVar8 = puVar1 + 0x12;
      do {
        iVar2 = *(int *)(iVar5 + 0xc);
        iVar6 = iVar2;
        if (*(char *)(param_1 + 200) != '\0') {
          iVar6 = iVar2 * 3;
        }
        iVar2 = FUN_00442230(*(int *)(iVar5 + 0x20),iVar2);
        iVar3 = FUN_00442230(*(int *)(iVar5 + 0x1c),*(int *)(iVar5 + 8));
        uVar4 = (**(code **)(*(int *)(param_1 + 4) + 0x14))(param_1,1,1,iVar3,iVar2,iVar6);
        iStack_4 = iStack_4 + 1;
        *puVar8 = uVar4;
        iVar5 = iVar5 + 0x54;
        puVar8 = puVar8 + 1;
      } while (iStack_4 < *(int *)(param_1 + 0x20));
    }
    puVar1[1] = &LAB_00442ed0;
    puVar1[3] = &LAB_00443100;
    puVar1[4] = puVar1 + 0x12;
    return;
  }
  iVar5 = (**(code **)(*(int *)(param_1 + 4) + 4))(param_1,1,0x500);
  piVar7 = puVar1 + 8;
  iVar2 = 10;
  do {
    *piVar7 = iVar5;
    piVar7 = piVar7 + 1;
    iVar5 = iVar5 + 0x80;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  puVar1[1] = &LAB_00442ec0;
  puVar1[3] = FUN_00442c70;
  puVar1[4] = 0;
  return;
}

