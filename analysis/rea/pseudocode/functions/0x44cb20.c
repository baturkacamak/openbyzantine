// rea decompile_all
// name: __findnext
// address: 0x44cb20
// body: available
// range: 0x44cb20-0x44cc31 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __findnext
   
   Library: Visual Studio 1998 Release */

undefined4 __cdecl __findnext(HANDLE param_1,DWORD *param_2)

{
  char cVar1;
  BOOL BVar2;
  DWORD DVar3;
  uint uVar4;
  uint uVar5;
  CHAR *pCVar6;
  CHAR *pCVar7;
  DWORD *pDVar8;
  _WIN32_FIND_DATAA local_140;
  
  BVar2 = FindNextFileA(param_1,&local_140);
  if (BVar2 == 0) {
    DVar3 = GetLastError();
    switch(DVar3) {
    case 2:
    case 3:
    case 0x12:
      DAT_00462c74 = 2;
      return 0xffffffff;
    default:
      DAT_00462c74 = 0x16;
      return 0xffffffff;
    case 8:
      DAT_00462c74 = 0xc;
      return 0xffffffff;
    }
  }
  if (local_140.dwFileAttributes == 0x80) {
    *param_2 = 0;
  }
  else {
    *param_2 = local_140.dwFileAttributes;
  }
  DVar3 = FUN_0044cc90(&local_140.ftCreationTime);
  param_2[1] = DVar3;
  DVar3 = FUN_0044cc90(&local_140.ftLastAccessTime);
  param_2[2] = DVar3;
  DVar3 = FUN_0044cc90(&local_140.ftLastWriteTime);
  uVar4 = 0xffffffff;
  param_2[3] = DVar3;
  param_2[4] = local_140.nFileSizeLow;
  pCVar6 = local_140.cFileName;
  do {
    pCVar7 = pCVar6;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pCVar7 = pCVar6 + 1;
    cVar1 = *pCVar6;
    pCVar6 = pCVar7;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  pCVar6 = pCVar7 + -uVar4;
  pDVar8 = param_2 + 5;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *pDVar8 = *(DWORD *)pCVar6;
    pCVar6 = pCVar6 + 4;
    pDVar8 = pDVar8 + 1;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(CHAR *)pDVar8 = *pCVar6;
    pCVar6 = pCVar6 + 1;
    pDVar8 = (DWORD *)((int)pDVar8 + 1);
  }
  return 0;
}

