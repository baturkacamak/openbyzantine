// rea decompile_all
// name: FUN_00443a70
// address: 0x443a70
// body: available
// range: 0x443a70-0x443c4b inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


byte * __cdecl FUN_00443a70(int param_1,int param_2,int *param_3)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  char cVar8;
  int iVar9;
  byte *unaff_EBP;
  int iVar10;
  char *pcVar11;
  undefined4 *puVar12;
  int iStack_520;
  int iStack_51c;
  int *piStack_514;
  int iStack_50c;
  char local_508 [256];
  int aiStack_408 [258];
  
  if (*param_3 == 0) {
    iVar3 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x5d4);
    *param_3 = iVar3;
  }
  iVar3 = *param_3;
  iVar10 = 0;
  iVar9 = 1;
  *(int *)(iVar3 + 0xd0) = param_2;
  do {
    bVar1 = *(byte *)(iVar9 + param_2);
    if (bVar1 != 0) {
      pcVar11 = local_508 + iVar10;
      cVar8 = (char)iVar9;
      iVar10 = iVar10 + (uint)bVar1;
      for (uVar5 = (uint)(bVar1 >> 2); uVar5 != 0; uVar5 = uVar5 - 1) {
        *(uint *)pcVar11 = CONCAT22(CONCAT11(cVar8,cVar8),CONCAT11(cVar8,cVar8));
        pcVar11 = pcVar11 + 4;
      }
      for (uVar5 = bVar1 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar11 = cVar8;
        pcVar11 = pcVar11 + 1;
      }
    }
    iVar9 = iVar9 + 1;
  } while (iVar9 < 0x11);
  iVar6 = 0;
  iVar9 = 0;
  local_508[iVar10] = '\0';
  iVar10 = (int)local_508[0];
  while (local_508[iVar6] != '\0') {
    if (local_508[iVar6] == iVar10) {
      piVar4 = aiStack_408 + iVar6 + 1;
      iVar7 = iVar6;
      do {
        *piVar4 = iVar9;
        piVar4 = piVar4 + 1;
        iVar6 = iVar7 + 1;
        iVar9 = iVar9 + 1;
        iVar2 = iVar7 + 1;
        iVar7 = iVar6;
      } while (local_508[iVar2] == iVar10);
    }
    iVar9 = iVar9 * 2;
    iVar10 = iVar10 + 1;
  }
  iVar10 = 0;
  iVar9 = 1;
  piVar4 = (int *)(iVar3 + 0x48);
  do {
    if (*(byte *)(param_2 + iVar9) == 0) {
      *piVar4 = -1;
    }
    else {
      iVar6 = aiStack_408[iVar10 + 1];
      piVar4[0x12] = iVar10;
      piVar4[-0x11] = iVar6;
      iVar10 = iVar10 + (uint)*(byte *)(param_2 + iVar9);
      *piVar4 = aiStack_408[iVar10];
    }
    piVar4 = piVar4 + 1;
    iVar9 = iVar9 + 1;
  } while (iVar9 < 0x11);
  *(undefined4 *)(iVar3 + 0x88) = 0xfffff;
  puVar12 = (undefined4 *)(iVar3 + 0xd4);
  for (iVar9 = 0x100; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar12 = 0;
    puVar12 = puVar12 + 1;
  }
  iStack_520 = 1;
  iStack_51c = 0;
  do {
    iStack_50c = 1;
    if (*(byte *)(iStack_520 + param_2) != 0) {
      piStack_514 = aiStack_408 + iStack_51c + 1;
      iVar9 = 1 << (8U - (char)iStack_520 & 0x1f);
      do {
        iVar10 = *piStack_514 << (8U - (char)iStack_520 & 0x1f);
        if (0 < iVar9) {
          piVar4 = (int *)(iVar3 + 0xd4 + iVar10 * 4);
          iVar6 = iVar9;
          do {
            *piVar4 = iStack_520;
            piVar4 = piVar4 + 1;
            iVar10 = iVar10 + 1;
            iVar6 = iVar6 + -1;
            *(undefined1 *)(iVar3 + 0x4d3 + iVar10) = *(undefined1 *)(iStack_51c + 0x11 + param_2);
          } while (iVar6 != 0);
        }
        iStack_50c = iStack_50c + 1;
        iStack_51c = iStack_51c + 1;
        piStack_514 = piStack_514 + 1;
      } while (iStack_50c <= (int)(uint)*(byte *)(iStack_520 + param_2));
    }
    iStack_520 = iStack_520 + 1;
  } while (iStack_520 < 9);
  return unaff_EBP;
}

