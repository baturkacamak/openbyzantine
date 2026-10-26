// rea decompile_all
// name: FUN_00455fe0
// address: 0x455fe0
// body: available
// range: 0x455fe0-0x456331 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00455fe0(int *param_1,int *param_2,int *param_3,int *param_4,char *param_5)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int *piVar5;
  char *pcVar6;
  uint uVar7;
  SIZE_T SVar8;
  char *pcVar9;
  char *pcVar10;
  int local_c;
  int local_8;
  
  iVar2 = *param_1;
  piVar5 = param_1;
  for (uVar7 = 2; (iVar2 != 0 && (uVar7 < 0x8000)); uVar7 = uVar7 + ~uVar3) {
    uVar3 = 0xffffffff;
    pcVar6 = (char *)*piVar5;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    piVar5 = piVar5 + 1;
    iVar2 = *piVar5;
  }
  if (0x7ffe < uVar7) {
    *param_4 = 0;
    DAT_00462c74 = 7;
    DAT_00462c78 = 10;
    return 0xffffffff;
  }
  iVar2 = FUN_0044c2f0(uVar7);
  *param_3 = iVar2;
  if (iVar2 == 0) {
    *param_4 = 0;
    DAT_00462c74 = 0xc;
    DAT_00462c78 = 8;
    return 0xffffffff;
  }
  if (param_2 != (int *)0x0) {
    iVar2 = *param_2;
    piVar5 = param_2;
    for (uVar3 = 2; (iVar2 != 0 && (uVar3 < 0x8000)); uVar3 = uVar3 + ~uVar4) {
      uVar4 = 0xffffffff;
      pcVar6 = (char *)*piVar5;
      do {
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        cVar1 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar1 != '\0');
      piVar5 = piVar5 + 1;
      iVar2 = *piVar5;
    }
    if (param_2 != (int *)0x0) {
      local_c = 0;
      cVar1 = *DAT_00463184;
      while ((cVar1 != '\0' && (DAT_00463184[local_c] != '='))) {
        uVar4 = 0xffffffff;
        pcVar6 = DAT_00463184 + local_c;
        do {
          if (uVar4 == 0) break;
          uVar4 = uVar4 - 1;
          cVar1 = *pcVar6;
          pcVar6 = pcVar6 + 1;
        } while (cVar1 != '\0');
        local_c = local_c + ~uVar4;
        cVar1 = DAT_00463184[local_c];
      }
      cVar1 = DAT_00463184[local_c];
      local_8 = local_c;
      while ((((cVar1 == '=' && (DAT_00463184[local_8 + 1] != '\0')) &&
              (DAT_00463184[local_8 + 2] == ':')) && (DAT_00463184[local_8 + 3] == '='))) {
        uVar4 = 0xffffffff;
        pcVar6 = DAT_00463184 + local_8 + 4;
        do {
          if (uVar4 == 0) break;
          uVar4 = uVar4 - 1;
          cVar1 = *pcVar6;
          pcVar6 = pcVar6 + 1;
        } while (cVar1 != '\0');
        local_8 = ~uVar4 + 4 + local_8;
        cVar1 = DAT_00463184[local_8];
      }
      SVar8 = (uVar3 - local_c) + local_8;
      uVar3 = 0xffffffff;
      do {
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        cVar1 = *param_5;
        param_5 = param_5 + 1;
      } while (cVar1 != '\0');
      if (0x7ffe < (~uVar3 - 1) + uVar7 + SVar8) {
        FUN_0044ac40((LPVOID)*param_3);
        *param_3 = 0;
        DAT_00462c74 = 7;
        DAT_00462c78 = 10;
        return 0xffffffff;
      }
      iVar2 = FUN_0044c2f0(SVar8);
      *param_4 = iVar2;
      if (iVar2 == 0) {
        FUN_0044ac40((LPVOID)*param_3);
        *param_3 = 0;
        DAT_00462c74 = 0xc;
        DAT_00462c78 = 8;
        return 0xffffffff;
      }
      goto LAB_004560cd;
    }
  }
  *param_4 = 0;
LAB_004560cd:
  pcVar6 = (char *)*param_3;
  if ((char *)*param_1 == (char *)0x0) {
    pcVar6 = pcVar6 + 1;
  }
  else {
    uVar7 = 0xffffffff;
    pcVar9 = (char *)*param_1;
    do {
      pcVar10 = pcVar9;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar10 = pcVar9 + 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar10;
    } while (cVar1 != '\0');
    uVar7 = ~uVar7;
    pcVar9 = pcVar10 + -uVar7;
    pcVar10 = pcVar6;
    for (uVar3 = uVar7 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(undefined4 *)pcVar10 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar10 = pcVar10 + 4;
    }
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar10 = *pcVar9;
      pcVar9 = pcVar9 + 1;
      pcVar10 = pcVar10 + 1;
    }
    uVar7 = 0xffffffff;
    pcVar9 = (char *)*param_1;
    do {
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar9 + 1;
    } while (cVar1 != '\0');
    pcVar6 = pcVar6 + ~uVar7;
    param_1 = param_1 + 1;
  }
  iVar2 = *param_1;
  while (iVar2 != 0) {
    uVar7 = 0xffffffff;
    pcVar9 = (char *)*param_1;
    do {
      pcVar10 = pcVar9;
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      pcVar10 = pcVar9 + 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar10;
    } while (cVar1 != '\0');
    uVar7 = ~uVar7;
    pcVar9 = pcVar10 + -uVar7;
    pcVar10 = pcVar6;
    for (uVar3 = uVar7 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(undefined4 *)pcVar10 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar10 = pcVar10 + 4;
    }
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar10 = *pcVar9;
      pcVar9 = pcVar9 + 1;
      pcVar10 = pcVar10 + 1;
    }
    uVar7 = 0xffffffff;
    pcVar9 = (char *)*param_1;
    do {
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar9 + 1;
    } while (cVar1 != '\0');
    param_1 = param_1 + 1;
    pcVar6[~uVar7 - 1] = ' ';
    pcVar6 = pcVar6 + (~uVar7 - 1) + 1;
    iVar2 = *param_1;
  }
  pcVar6[-1] = '\0';
  *pcVar6 = '\0';
  pcVar6 = (char *)*param_4;
  if (param_2 != (int *)0x0) {
    uVar7 = local_8 - local_c;
    pcVar9 = DAT_00463184 + local_c;
    pcVar10 = pcVar6;
    for (uVar3 = uVar7 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(undefined4 *)pcVar10 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar10 = pcVar10 + 4;
    }
    pcVar6 = pcVar6 + uVar7;
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *pcVar10 = *pcVar9;
      pcVar9 = pcVar9 + 1;
      pcVar10 = pcVar10 + 1;
    }
    iVar2 = *param_2;
    while (iVar2 != 0) {
      uVar7 = 0xffffffff;
      pcVar9 = (char *)*param_2;
      do {
        pcVar10 = pcVar9;
        if (uVar7 == 0) break;
        uVar7 = uVar7 - 1;
        pcVar10 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar10;
      } while (cVar1 != '\0');
      uVar7 = ~uVar7;
      pcVar9 = pcVar10 + -uVar7;
      pcVar10 = pcVar6;
      for (uVar3 = uVar7 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        *(undefined4 *)pcVar10 = *(undefined4 *)pcVar9;
        pcVar9 = pcVar9 + 4;
        pcVar10 = pcVar10 + 4;
      }
      for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
        *pcVar10 = *pcVar9;
        pcVar9 = pcVar9 + 1;
        pcVar10 = pcVar10 + 1;
      }
      uVar7 = 0xffffffff;
      pcVar9 = (char *)*param_2;
      do {
        if (uVar7 == 0) break;
        uVar7 = uVar7 - 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar9 + 1;
      } while (cVar1 != '\0');
      pcVar6 = pcVar6 + ~uVar7;
      param_2 = param_2 + 1;
      iVar2 = *param_2;
    }
  }
  if (pcVar6 != (char *)0x0) {
    if ((char *)*param_4 == pcVar6) {
      *pcVar6 = '\0';
      pcVar6 = pcVar6 + 1;
    }
    *pcVar6 = '\0';
  }
  return 0;
}

