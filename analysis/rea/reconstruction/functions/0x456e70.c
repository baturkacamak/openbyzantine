// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x456e70
// name: ___crtMessageBoxA
// ranges: 0x456e70-0x456f01
// calls: -

/* Library Function - Single Match
    ___crtMessageBoxA
   
   Library: Visual Studio 1998 Release */

int __cdecl ___crtMessageBoxA(LPCSTR _LpText,LPCSTR _LpCaption,UINT _UType)

{
  HMODULE hModule;
  int iVar1;
  
  iVar1 = 0;
  if (DAT_00463f9c != (FARPROC)0x0) {
LAB_00456ebf:
    if (DAT_00463fa0 != (FARPROC)0x0) {
      iVar1 = (*DAT_00463fa0)();
    }
    if ((iVar1 != 0) && (DAT_00463fa4 != (FARPROC)0x0)) {
      iVar1 = (*DAT_00463fa4)(iVar1);
    }
    iVar1 = (*DAT_00463f9c)(iVar1,_LpText,_LpCaption,_UType);
    return iVar1;
  }
  hModule = LoadLibraryA(s_user32_dll_00463fd8);
  if (hModule != (HMODULE)0x0) {
    DAT_00463f9c = GetProcAddress(hModule,s_MessageBoxA_00463fcc);
    if (DAT_00463f9c != (FARPROC)0x0) {
      DAT_00463fa0 = GetProcAddress(hModule,s_GetActiveWindow_00463fbc);
      DAT_00463fa4 = GetProcAddress(hModule,s_GetLastActivePopup_00463fa8);
      goto LAB_00456ebf;
    }
  }
  return 0;
}
