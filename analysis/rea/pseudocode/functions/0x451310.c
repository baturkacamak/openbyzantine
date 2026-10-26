// rea decompile_all
// name: __spawnvpe
// address: 0x451310
// body: available
// range: 0x451310-0x451502 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __spawnvpe
   
   Library: Visual Studio 1998 Release */

intptr_t __cdecl __spawnvpe(int _Mode,char *_Filename,char **_ArgList,char **_Env)

{
  byte bVar1;
  char cVar2;
  uchar *puVar3;
  char *_Src;
  byte *pbVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  byte *_Str;
  char *pcVar8;
  char *pcVar9;
  byte *pbVar10;
  intptr_t local_4;
  
  _Str = (byte *)0x0;
  local_4 = __spawnve(_Mode,_Filename,_ArgList,_Env);
  if ((((local_4 == -1) && (DAT_00462c74 == 2)) &&
      (puVar3 = __mbschr((uchar *)_Filename,0x2f), puVar3 == (uchar *)0x0)) &&
     (_Src = _getenv(&DAT_004634ec), _Src != (char *)0x0)) {
    _Str = (byte *)FUN_0044c2f0(0x104);
    if (_Str == (byte *)0x0) {
      return -1;
    }
    do {
      do {
        _Src = __getpath(_Src,(char *)_Str,0x103);
        if ((_Src == (char *)0x0) || (*_Str == 0)) goto LAB_004514ea;
        uVar5 = 0xffffffff;
        pbVar4 = _Str;
        do {
          if (uVar5 == 0) break;
          uVar5 = uVar5 - 1;
          bVar1 = *pbVar4;
          pbVar4 = pbVar4 + 1;
        } while (bVar1 != 0);
        bVar1 = _Str[~uVar5 - 2];
        if (bVar1 == 0x5c) {
          pbVar4 = (byte *)FUN_00455c90(_Str,0x5c);
          if (pbVar4 != _Str + (~uVar5 - 2)) {
LAB_004513e2:
            uVar5 = 0xffffffff;
            pcVar8 = &DAT_004634e8;
            do {
              pcVar9 = pcVar8;
              if (uVar5 == 0) break;
              uVar5 = uVar5 - 1;
              pcVar9 = pcVar8 + 1;
              cVar2 = *pcVar8;
              pcVar8 = pcVar9;
            } while (cVar2 != '\0');
            uVar5 = ~uVar5;
            iVar6 = -1;
            pbVar4 = _Str;
            do {
              pbVar10 = pbVar4;
              if (iVar6 == 0) break;
              iVar6 = iVar6 + -1;
              pbVar10 = pbVar4 + 1;
              bVar1 = *pbVar4;
              pbVar4 = pbVar10;
            } while (bVar1 != 0);
            pbVar4 = (byte *)(pcVar9 + -uVar5);
            pbVar10 = pbVar10 + -1;
            for (uVar7 = uVar5 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
              *(undefined4 *)pbVar10 = *(undefined4 *)pbVar4;
              pbVar4 = pbVar4 + 4;
              pbVar10 = pbVar10 + 4;
            }
            for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
              *pbVar10 = *pbVar4;
              pbVar4 = pbVar4 + 1;
              pbVar10 = pbVar10 + 1;
            }
          }
        }
        else if (bVar1 != 0x2f) goto LAB_004513e2;
        uVar5 = 0xffffffff;
        pcVar8 = _Filename;
        do {
          if (uVar5 == 0) break;
          uVar5 = uVar5 - 1;
          cVar2 = *pcVar8;
          pcVar8 = pcVar8 + 1;
        } while (cVar2 != '\0');
        uVar7 = 0xffffffff;
        pbVar4 = _Str;
        do {
          if (uVar7 == 0) break;
          uVar7 = uVar7 - 1;
          bVar1 = *pbVar4;
          pbVar4 = pbVar4 + 1;
        } while (bVar1 != 0);
        if (0x103 < ~uVar7 + (~uVar5 - 2)) goto LAB_004514ea;
        uVar5 = 0xffffffff;
        pcVar8 = _Filename;
        do {
          pcVar9 = pcVar8;
          if (uVar5 == 0) break;
          uVar5 = uVar5 - 1;
          pcVar9 = pcVar8 + 1;
          cVar2 = *pcVar8;
          pcVar8 = pcVar9;
        } while (cVar2 != '\0');
        uVar5 = ~uVar5;
        iVar6 = -1;
        pbVar4 = _Str;
        do {
          pbVar10 = pbVar4;
          if (iVar6 == 0) break;
          iVar6 = iVar6 + -1;
          pbVar10 = pbVar4 + 1;
          bVar1 = *pbVar4;
          pbVar4 = pbVar10;
        } while (bVar1 != 0);
        pbVar4 = (byte *)(pcVar9 + -uVar5);
        pbVar10 = pbVar10 + -1;
        for (uVar7 = uVar5 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
          *(undefined4 *)pbVar10 = *(undefined4 *)pbVar4;
          pbVar4 = pbVar4 + 4;
          pbVar10 = pbVar10 + 4;
        }
        for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
          *pbVar10 = *pbVar4;
          pbVar4 = pbVar4 + 1;
          pbVar10 = pbVar10 + 1;
        }
        local_4 = __spawnve(_Mode,(char *)_Str,_ArgList,_Env);
        if (local_4 != -1) goto LAB_004514ea;
      } while (DAT_00462c74 == 2);
      pbVar4 = __mbschr(_Str,0x5c);
      if ((pbVar4 != _Str) && (pbVar4 = __mbschr(_Str,0x2f), pbVar4 != _Str)) break;
      puVar3 = __mbschr(_Str + 1,0x5c);
    } while (((int)puVar3 - (int)_Str == 1) ||
            (puVar3 = __mbschr(_Str + 1,0x2f), (int)puVar3 - (int)_Str == 1));
  }
LAB_004514ea:
  if (_Str != (byte *)0x0) {
    FUN_0044ac40(_Str);
  }
  return local_4;
}

