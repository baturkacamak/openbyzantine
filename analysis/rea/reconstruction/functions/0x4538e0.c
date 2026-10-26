// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4538e0
// name: ___crtGetEnvironmentStringsW
// ranges: 0x4538e0-0x453adc
// calls: 0x44ac40, 0x44c2f0, 0x456e50

/* Library Function - Single Match
    ___crtGetEnvironmentStringsW
   
   Library: Visual Studio 1998 Release */

LPVOID __cdecl ___crtGetEnvironmentStringsW(void)

{
  char cVar1;
  WCHAR WVar2;
  LPWSTR in_EAX;
  LPWSTR pWVar3;
  undefined4 *puVar4;
  int iVar5;
  uint uVar6;
  WCHAR *pWVar7;
  uint uVar9;
  LPWCH pWVar10;
  LPWSTR pWVar11;
  LPWSTR pWVar12;
  undefined4 *puVar13;
  LPWSTR pWVar14;
  int local_4;
  WCHAR *pWVar8;
  
  local_4 = 0;
  pWVar3 = (LPWSTR)0x0;
  if (DAT_0046390c == 0) {
    in_EAX = GetEnvironmentStringsW();
    if (in_EAX == (LPWCH)0x0) {
      in_EAX = (LPWSTR)GetEnvironmentStrings();
      if (in_EAX == (LPWSTR)0x0) {
        return (LPVOID)0x0;
      }
      DAT_0046390c = 2;
      pWVar3 = in_EAX;
    }
    else {
      DAT_0046390c = 1;
      pWVar3 = in_EAX;
    }
  }
  if (DAT_0046390c == 1) {
    if ((pWVar3 == (LPWSTR)0x0) && (pWVar3 = GetEnvironmentStringsW(), pWVar3 == (LPWCH)0x0)) {
      return (LPVOID)0x0;
    }
    WVar2 = *pWVar3;
    pWVar7 = pWVar3;
    while (WVar2 != L'\0') {
      do {
        pWVar8 = pWVar7;
        pWVar7 = pWVar8 + 1;
      } while (*pWVar7 != L'\0');
      pWVar7 = pWVar8 + 2;
      WVar2 = *pWVar7;
    }
    uVar9 = (int)pWVar7 + (2 - (int)pWVar3);
    puVar4 = (undefined4 *)FUN_0044c2f0(uVar9);
    if (puVar4 != (undefined4 *)0x0) {
      pWVar10 = pWVar3;
      puVar13 = puVar4;
      for (uVar6 = uVar9 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
        *puVar13 = *(undefined4 *)pWVar10;
        pWVar10 = pWVar10 + 2;
        puVar13 = puVar13 + 1;
      }
      for (uVar9 = uVar9 & 3; uVar9 != 0; uVar9 = uVar9 - 1) {
        *(char *)puVar13 = (char)*pWVar10;
        pWVar10 = (LPWCH)((int)pWVar10 + 1);
        puVar13 = (undefined4 *)((int)puVar13 + 1);
      }
      FreeEnvironmentStringsW(pWVar3);
      return puVar4;
    }
    FreeEnvironmentStringsW(pWVar3);
    return (LPVOID)0x0;
  }
  if (DAT_0046390c == 2) {
    if ((pWVar3 == (LPWSTR)0x0) && (pWVar3 = (LPWSTR)GetEnvironmentStrings(), pWVar3 == (LPWSTR)0x0)
       ) {
      return (LPVOID)0x0;
    }
    cVar1 = (char)*pWVar3;
    pWVar11 = pWVar3;
    while (cVar1 != '\0') {
      iVar5 = MultiByteToWideChar(DAT_00463558,1,(LPCSTR)pWVar11,-1,(LPWSTR)0x0,0);
      if (iVar5 == 0) {
        return (LPVOID)0x0;
      }
      uVar9 = 0xffffffff;
      local_4 = local_4 + iVar5;
      pWVar12 = pWVar11;
      do {
        if (uVar9 == 0) break;
        uVar9 = uVar9 - 1;
        WVar2 = *pWVar12;
        pWVar12 = (LPWSTR)((int)pWVar12 + 1);
      } while ((char)WVar2 != '\0');
      pWVar11 = (LPWSTR)((int)pWVar11 + ~uVar9);
      cVar1 = (char)*pWVar11;
    }
    in_EAX = (LPWSTR)FUN_0044c2f0((local_4 + 1) * 2);
    if (in_EAX == (LPWSTR)0x0) {
      FreeEnvironmentStringsA((LPCH)pWVar3);
      return (LPVOID)0x0;
    }
    cVar1 = (char)*pWVar3;
    pWVar11 = in_EAX;
    pWVar12 = pWVar3;
    while (cVar1 != '\0') {
      iVar5 = MultiByteToWideChar(DAT_00463558,1,(LPCSTR)pWVar12,-1,pWVar11,
                                  (local_4 + 1) - ((int)pWVar11 - (int)in_EAX >> 1));
      if (iVar5 == 0) {
        FUN_0044ac40(in_EAX);
        FreeEnvironmentStringsA((LPCH)pWVar3);
        return (LPVOID)0x0;
      }
      uVar9 = 0xffffffff;
      pWVar14 = pWVar12;
      do {
        if (uVar9 == 0) break;
        uVar9 = uVar9 - 1;
        WVar2 = *pWVar14;
        pWVar14 = (LPWSTR)((int)pWVar14 + 1);
      } while ((char)WVar2 != '\0');
      pWVar12 = (LPWSTR)((int)pWVar12 + ~uVar9);
      iVar5 = FUN_00456e50(pWVar11);
      pWVar11 = pWVar11 + iVar5 + 1;
      cVar1 = (char)*pWVar12;
    }
    *pWVar11 = L'\0';
    FreeEnvironmentStringsA((LPCH)pWVar3);
  }
  return in_EAX;
}
