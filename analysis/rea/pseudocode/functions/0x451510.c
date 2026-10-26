// rea decompile_all
// name: __spawnve
// address: 0x451510
// body: available
// range: 0x451510-0x451736 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __spawnve
   
   Library: Visual Studio 1998 Release */

intptr_t __cdecl __spawnve(int _Mode,char *_Filename,char **_ArgList,char **_Env)

{
  char cVar1;
  byte *pbVar2;
  byte *pbVar3;
  char *pcVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  undefined **ppuVar9;
  char *pcVar10;
  char *pcVar11;
  char *local_c;
  HANDLE local_8;
  
  local_c = _Filename;
  pbVar2 = (byte *)FUN_00455c90((byte *)_Filename,0x5c);
  pbVar3 = (byte *)FUN_00455c90((byte *)_Filename,0x2f);
  if (pbVar3 == (byte *)0x0) {
    if ((pbVar2 == (byte *)0x0) &&
       (pbVar2 = __mbschr((uchar *)_Filename,0x3a), pbVar2 == (byte *)0x0)) {
      uVar5 = 0xffffffff;
      pcVar4 = _Filename;
      do {
        if (uVar5 == 0) break;
        uVar5 = uVar5 - 1;
        cVar1 = *pcVar4;
        pcVar4 = pcVar4 + 1;
      } while (cVar1 != '\0');
      local_c = (char *)FUN_0044c2f0(~uVar5 + 2);
      if (local_c == (char *)0x0) {
        return -1;
      }
      *(undefined2 *)local_c = DAT_00463528;
      local_c[2] = DAT_0046352a;
      uVar5 = 0xffffffff;
      pcVar4 = _Filename;
      do {
        pcVar10 = pcVar4;
        if (uVar5 == 0) break;
        uVar5 = uVar5 - 1;
        pcVar10 = pcVar4 + 1;
        cVar1 = *pcVar4;
        pcVar4 = pcVar10;
      } while (cVar1 != '\0');
      uVar5 = ~uVar5;
      iVar6 = -1;
      pcVar4 = local_c;
      do {
        pcVar11 = pcVar4;
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pcVar11 = pcVar4 + 1;
        cVar1 = *pcVar4;
        pcVar4 = pcVar11;
      } while (cVar1 != '\0');
      pcVar4 = pcVar10 + -uVar5;
      pcVar10 = pcVar11 + -1;
      for (uVar7 = uVar5 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
        *(undefined4 *)pcVar10 = *(undefined4 *)pcVar4;
        pcVar4 = pcVar4 + 4;
        pcVar10 = pcVar10 + 4;
      }
      for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar10 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        pcVar10 = pcVar10 + 1;
      }
      pbVar2 = (byte *)(local_c + 2);
    }
  }
  else if ((pbVar2 == (byte *)0x0) || (pbVar2 < pbVar3)) {
    pbVar2 = pbVar3;
  }
  local_8 = (HANDLE)0xffffffff;
  iVar6 = FUN_00455c90(pbVar2,0x2e);
  if (iVar6 == 0) {
    uVar5 = 0xffffffff;
    pcVar4 = local_c;
    do {
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar4 + 1;
    } while (cVar1 != '\0');
    pcVar4 = (char *)FUN_0044c2f0(~uVar5 + 4);
    if (pcVar4 == (char *)0x0) {
      return -1;
    }
    uVar5 = 0xffffffff;
    pcVar10 = local_c;
    do {
      pcVar11 = pcVar10;
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      pcVar11 = pcVar10 + 1;
      cVar1 = *pcVar10;
      pcVar10 = pcVar11;
    } while (cVar1 != '\0');
    uVar5 = ~uVar5;
    pcVar10 = pcVar11 + -uVar5;
    pcVar11 = pcVar4;
    for (uVar7 = uVar5 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
      *(undefined4 *)pcVar11 = *(undefined4 *)pcVar10;
      pcVar10 = pcVar10 + 4;
      pcVar11 = pcVar11 + 4;
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar11 = *pcVar10;
      pcVar10 = pcVar10 + 1;
      pcVar11 = pcVar11 + 1;
    }
    uVar5 = 0xffffffff;
    ppuVar9 = &PTR_DAT_00463524;
    pcVar10 = local_c;
    do {
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      cVar1 = *pcVar10;
      pcVar10 = pcVar10 + 1;
    } while (cVar1 != '\0');
    do {
      uVar7 = 0xffffffff;
      pcVar10 = *ppuVar9;
      do {
        pcVar11 = pcVar10;
        if (uVar7 == 0) break;
        uVar7 = uVar7 - 1;
        pcVar11 = pcVar10 + 1;
        cVar1 = *pcVar10;
        pcVar10 = pcVar11;
      } while (cVar1 != '\0');
      uVar7 = ~uVar7;
      pcVar10 = pcVar11 + -uVar7;
      pcVar11 = pcVar4 + (~uVar5 - 1);
      for (uVar8 = uVar7 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
        *(undefined4 *)pcVar11 = *(undefined4 *)pcVar10;
        pcVar10 = pcVar10 + 4;
        pcVar11 = pcVar11 + 4;
      }
      for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
        *pcVar11 = *pcVar10;
        pcVar10 = pcVar10 + 1;
        pcVar11 = pcVar11 + 1;
      }
      iVar6 = __access(pcVar4,0);
      if (iVar6 != -1) {
        local_8 = comexecmd(_Mode,pcVar4,(int *)_ArgList,(int *)_Env);
        break;
      }
      ppuVar9 = ppuVar9 + -1;
    } while ((undefined **)0x463517 < ppuVar9);
    FUN_0044ac40(pcVar4);
  }
  else {
    iVar6 = __access(local_c,0);
    if (iVar6 != -1) {
      local_8 = comexecmd(_Mode,local_c,(int *)_ArgList,(int *)_Env);
    }
  }
  if (_Filename != local_c) {
    FUN_0044ac40(local_c);
  }
  return (intptr_t)local_8;
}

