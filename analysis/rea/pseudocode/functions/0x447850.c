// rea decompile_all
// name: FUN_00447850
// address: 0x447850
// body: available
// range: 0x447850-0x447c0f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00447850(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int *piVar6;
  short *psVar7;
  int iVar8;
  short *psVar9;
  int iVar10;
  int *local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int *local_8;
  
  iVar2 = *(int *)(*(int *)(param_1 + 0x1a4) + 0x18);
  local_18 = *param_2;
  local_c = param_2[1];
  local_14 = param_2[2];
  iVar3 = param_2[3];
  piVar4 = (int *)param_2[5];
  local_10 = param_2[4];
  if (local_18 < local_c) {
    local_20 = (int *)(iVar2 + local_18 * 4);
    local_1c = local_18;
    do {
      if (local_14 <= iVar3) {
        psVar9 = (short *)((local_14 * 0x20 + local_10) * 2 + *local_20);
        iVar8 = local_10;
        psVar7 = psVar9;
        iVar5 = local_14;
        do {
          for (; iVar8 <= (int)piVar4; iVar8 = iVar8 + 1) {
            if (*psVar9 != 0) {
              local_18 = local_1c;
              *param_2 = local_1c;
              goto LAB_00447905;
            }
            psVar9 = psVar9 + 1;
          }
          psVar9 = psVar7 + 0x20;
          iVar5 = iVar5 + 1;
          iVar8 = local_10;
          psVar7 = psVar9;
        } while (iVar5 <= iVar3);
      }
      local_1c = local_1c + 1;
      local_20 = local_20 + 1;
    } while (local_1c <= local_c);
  }
LAB_00447905:
  if (local_18 < local_c) {
    local_20 = (int *)(iVar2 + local_c * 4);
    local_1c = local_c;
    do {
      if (local_14 <= iVar3) {
        psVar9 = (short *)((local_14 * 0x20 + local_10) * 2 + *local_20);
        iVar8 = local_10;
        iVar5 = local_14;
        psVar7 = psVar9;
        do {
          for (; iVar8 <= (int)piVar4; iVar8 = iVar8 + 1) {
            if (*psVar9 != 0) {
              local_c = local_1c;
              param_2[1] = local_1c;
              goto LAB_0044797d;
            }
            psVar9 = psVar9 + 1;
          }
          psVar9 = psVar7 + 0x20;
          iVar5 = iVar5 + 1;
          iVar8 = local_10;
          psVar7 = psVar9;
        } while (iVar5 <= iVar3);
      }
      local_1c = local_1c + -1;
      local_20 = local_20 + -1;
    } while (local_18 <= local_1c);
  }
LAB_0044797d:
  if (local_14 < iVar3) {
    local_20 = (int *)((local_14 * 0x20 + local_10) * 2);
    local_1c = local_14;
    do {
      if (local_18 <= local_c) {
        piVar6 = (int *)(iVar2 + local_18 * 4);
        iVar8 = local_18;
        do {
          psVar9 = (short *)((int)local_20 + *piVar6);
          for (iVar5 = local_10; iVar5 <= (int)piVar4; iVar5 = iVar5 + 1) {
            if (*psVar9 != 0) {
              local_14 = local_1c;
              param_2[2] = local_1c;
              goto LAB_004479f5;
            }
            psVar9 = psVar9 + 1;
          }
          piVar6 = piVar6 + 1;
          iVar8 = iVar8 + 1;
        } while (iVar8 <= local_c);
      }
      local_20 = (int *)((int)local_20 + 0x40);
      local_1c = local_1c + 1;
    } while (local_1c <= iVar3);
LAB_004479f5:
    if (local_14 < iVar3) {
      local_20 = (int *)((iVar3 * 0x20 + local_10) * 2);
      local_1c = iVar3;
      do {
        if (local_18 <= local_c) {
          piVar6 = (int *)(iVar2 + local_18 * 4);
          iVar8 = local_18;
          do {
            psVar9 = (short *)(*piVar6 + (int)local_20);
            for (iVar5 = local_10; iVar5 <= (int)piVar4; iVar5 = iVar5 + 1) {
              if (*psVar9 != 0) {
                param_2[3] = local_1c;
                iVar3 = local_1c;
                goto LAB_00447a67;
              }
              psVar9 = psVar9 + 1;
            }
            piVar6 = piVar6 + 1;
            iVar8 = iVar8 + 1;
          } while (iVar8 <= local_c);
        }
        local_1c = local_1c + -1;
        local_20 = (int *)((int)local_20 + -0x40);
      } while (local_14 <= local_1c);
    }
  }
LAB_00447a67:
  if (local_10 < (int)piVar4) {
    iVar8 = (local_14 * 0x20 + local_10) * 2;
    local_20 = (int *)local_10;
    do {
      if (local_18 <= local_c) {
        piVar6 = (int *)(iVar2 + local_18 * 4);
        iVar5 = local_18;
        do {
          psVar9 = (short *)(*piVar6 + iVar8);
          for (iVar1 = local_14; iVar1 <= iVar3; iVar1 = iVar1 + 1) {
            if (*psVar9 != 0) {
              local_10 = (int)local_20;
              param_2[4] = (int)local_20;
              goto LAB_00447adf;
            }
            psVar9 = psVar9 + 0x20;
          }
          piVar6 = piVar6 + 1;
          iVar5 = iVar5 + 1;
        } while (iVar5 <= local_c);
      }
      iVar8 = iVar8 + 2;
      local_20 = (int *)((int)local_20 + 1);
    } while ((int)local_20 <= (int)piVar4);
LAB_00447adf:
    if (local_10 < (int)piVar4) {
      iVar8 = (local_14 * 0x20 + (int)piVar4) * 2;
      local_20 = piVar4;
      do {
        if (local_18 <= local_c) {
          piVar6 = (int *)(iVar2 + local_18 * 4);
          iVar5 = local_18;
          do {
            psVar9 = (short *)(*piVar6 + iVar8);
            for (iVar1 = local_14; iVar1 <= iVar3; iVar1 = iVar1 + 1) {
              if (*psVar9 != 0) {
                param_2[5] = (int)local_20;
                piVar4 = local_20;
                goto LAB_00447b4d;
              }
              psVar9 = psVar9 + 0x20;
            }
            piVar6 = piVar6 + 1;
            iVar5 = iVar5 + 1;
          } while (iVar5 <= local_c);
        }
        iVar8 = iVar8 + -2;
        local_20 = (int *)((int)local_20 + -1);
      } while (local_10 <= (int)local_20);
    }
  }
LAB_00447b4d:
  iVar1 = (local_c - local_18) * 0x10;
  iVar8 = (iVar3 - local_14) * 0xc;
  iVar5 = ((int)piVar4 - local_10) * 8;
  iVar10 = 0;
  param_2[6] = iVar5 * iVar5 + iVar8 * iVar8 + iVar1 * iVar1;
  if (local_18 <= local_c) {
    local_8 = (int *)(iVar2 + local_18 * 4);
    local_20 = (int *)((local_c - local_18) + 1);
    do {
      if (local_14 <= iVar3) {
        psVar9 = (short *)((local_14 * 0x20 + local_10) * 2 + *local_8);
        iVar2 = (iVar3 - local_14) + 1;
        do {
          if (local_10 <= (int)piVar4) {
            iVar8 = ((int)piVar4 - local_10) + 1;
            psVar7 = psVar9;
            do {
              if (*psVar7 != 0) {
                iVar10 = iVar10 + 1;
              }
              psVar7 = psVar7 + 1;
              iVar8 = iVar8 + -1;
            } while (iVar8 != 0);
          }
          psVar9 = psVar9 + 0x20;
          iVar2 = iVar2 + -1;
        } while (iVar2 != 0);
      }
      local_8 = local_8 + 1;
      local_20 = (int *)((int)local_20 + -1);
    } while (local_20 != (int *)0x0);
  }
  param_2[7] = iVar10;
  return;
}

