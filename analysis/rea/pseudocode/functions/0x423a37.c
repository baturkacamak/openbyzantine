// rea decompile_all
// name: FUN_00423a37
// address: 0x423a37
// body: available
// range: 0x423a37-0x423c41 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined8 __fastcall FUN_00423a37(int param_1,undefined4 param_2)

{
  double *pdVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  float10 fVar14;
  float10 fVar15;
  float10 fVar16;
  float10 fVar17;
  float10 fVar18;
  float10 fVar19;
  float10 fVar20;
  double local_4c;
  int local_44;
  int local_34;
  
  iVar2 = *(int *)(param_1 + 4);
  iVar12 = *(int *)(param_1 + 8);
  iVar3 = *(int *)(param_1 + 0x1c);
  pdVar1 = (double *)(param_1 + 0x28);
  dVar9 = *pdVar1;
  dVar8 = *pdVar1;
  iVar4 = *(int *)(param_1 + 0x44);
  iVar5 = *(int *)(param_1 + 0x4c);
  dVar7 = *(double *)(param_1 + 0x60) * *pdVar1;
  iVar6 = *(int *)(param_1 + 0x20);
  piVar13 = *(int **)(param_1 + 0x3c);
  local_4c = (double)-(iVar12 >> 1);
  iVar10 = -(iVar2 >> 1);
  fVar14 = (float10)6.755399441056e+15;
  fVar15 = (float10)fcos((float10)*(double *)(param_1 + 0x58));
  fVar16 = (float10)fsin((float10)*(double *)(param_1 + 0x58));
  do {
    fVar17 = (fVar15 * (float10)local_4c + fVar16 * (float10)dVar7) * (float10)dVar8;
    fVar20 = (float10)1 / (fVar15 * (float10)dVar7 - fVar16 * (float10)local_4c);
    fVar18 = (float10)iVar10;
    iVar11 = iVar2;
    do {
      local_34 = SUB84((double)(fVar20 * fVar18 * (float10)1000.0 + fVar14),0);
      fVar19 = (float10)*(float *)((local_34 + -0x100) * 4 + iVar5);
      local_34 = SUB84((double)(fVar19 * (float10)dVar9 + fVar14),0);
      local_44 = SUB84((double)(fVar19 * (float10)1000.0 + fVar14),0);
      local_44 = SUB84((double)((float10)*(float *)((local_44 + -0x100) * 4 + iVar4) * fVar17 *
                                fVar20 + fVar14),0);
      *piVar13 = (local_44 + -0x100 + (iVar6 >> 1)) * iVar3 + local_34 + -0x100 + (iVar3 >> 1);
      piVar13 = piVar13 + 1;
      fVar18 = fVar18 + (float10)1.0;
      iVar11 = iVar11 + -1;
    } while (iVar11 != 0);
    ffree(fVar18);
    ffree(fVar20);
    ffree(fVar17);
    local_4c = local_4c + 1.0;
    iVar12 = iVar12 + -1;
  } while (iVar12 != 0);
  ffree(fVar16);
  ffree(fVar15);
  ffree(fVar14);
  return CONCAT44(param_2,iVar10);
}

