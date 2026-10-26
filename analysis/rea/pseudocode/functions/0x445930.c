// rea decompile_all
// name: FUN_00445930
// address: 0x445930
// body: available
// range: 0x445930-0x445b3a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00445930(int *param_1)

{
  bool bVar1;
  bool bVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  undefined4 *puVar9;
  int iVar10;
  int iStack_8;
  
  puVar3 = (undefined4 *)(**(code **)param_1[1])(param_1,1,0xa0);
  param_1[0x67] = (int)puVar3;
  *puVar3 = &LAB_00445b40;
  puVar3[1] = FUN_00445b60;
  *(undefined1 *)(puVar3 + 2) = 0;
  if ((char)param_1[0x42] != '\0') {
    *(undefined4 *)(*param_1 + 0x14) = 0x17;
    (**(code **)*param_1)(param_1);
  }
  if (((char)param_1[0x12] == '\0') || (param_1[0x45] < 2)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  iStack_8 = 0;
  iVar8 = param_1[0x31];
  if (0 < param_1[8]) {
    puVar9 = puVar3 + 0xd;
    do {
      bVar2 = true;
      iVar4 = (*(int *)(iVar8 + 8) * *(int *)(iVar8 + 0x24)) / param_1[0x45];
      iVar5 = (*(int *)(iVar8 + 0xc) * *(int *)(iVar8 + 0x24)) / param_1[0x45];
      iVar10 = param_1[0x43];
      iVar6 = param_1[0x44];
      puVar9[0xc] = iVar5;
      if (*(char *)(iVar8 + 0x30) == '\0') {
        bVar2 = false;
        *puVar9 = &LAB_00445c60;
      }
      else if ((iVar10 == iVar4) && (iVar6 == iVar5)) {
        bVar2 = false;
        *puVar9 = &LAB_00445c50;
      }
      else if (iVar10 == iVar4 * 2) {
        if (iVar6 == iVar5) {
          if ((bVar1) && (2 < *(uint *)(iVar8 + 0x28))) {
            *puVar9 = &LAB_00445e60;
          }
          else {
            *puVar9 = &LAB_00445d80;
          }
        }
        else {
          if ((iVar10 != iVar4 * 2) || (iVar5 * 2 - iVar6 != 0)) goto LAB_00445a8e;
          if ((bVar1) && (2 < *(uint *)(iVar8 + 0x28))) {
            *puVar9 = &LAB_00445f20;
            *(undefined1 *)(puVar3 + 2) = 1;
          }
          else {
            *puVar9 = &LAB_00445de0;
          }
        }
      }
      else {
LAB_00445a8e:
        if ((iVar10 % iVar4 == 0) && (iVar6 % iVar5 == 0)) {
          *puVar9 = &LAB_00445c70;
          *(char *)((int)puVar3 + iStack_8 + 0x8c) = (char)(iVar10 / iVar4);
          *(char *)((int)puVar3 + iStack_8 + 0x96) = (char)(iVar6 / iVar5);
        }
        else {
          *(undefined4 *)(*param_1 + 0x14) = 0x25;
          (**(code **)*param_1)(param_1);
        }
      }
      if (bVar2) {
        iVar10 = param_1[0x44];
        iVar6 = FUN_00442230(param_1[0x17],param_1[0x43]);
        uVar7 = (**(code **)(param_1[1] + 8))(param_1,1,iVar6,iVar10);
        puVar9[-10] = uVar7;
      }
      puVar9 = puVar9 + 1;
      iVar8 = iVar8 + 0x54;
      iStack_8 = iStack_8 + 1;
    } while (iStack_8 < param_1[8]);
  }
  return;
}

