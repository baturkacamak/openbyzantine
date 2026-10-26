// rea decompile_all
// name: ___crtCompareStringW
// address: 0x456f10
// body: available
// range: 0x456f10-0x45714a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    ___crtCompareStringW
   
   Library: Visual Studio 1998 Release */

int __cdecl
___crtCompareStringW
          (LPCWSTR _LocaleName,DWORD _DwCmpFlags,LPCWSTR _LpString1,int _CchCount1,
          LPCWSTR _LpString2,int _CchCount2)

{
  int in_EAX;
  int iVar1;
  SIZE_T cbMultiByte;
  PCNZCH lpMultiByteStr;
  SIZE_T cbMultiByte_00;
  LPSTR lpMultiByteStr_00;
  UINT in_stack_0000001c;
  
  if (DAT_00463fe4 == 0) {
    in_EAX = CompareStringW(0,0,(PCNZWCH)&DAT_00463538,1,(PCNZWCH)&DAT_00463538,1);
    if (in_EAX == 0) {
      in_EAX = CompareStringA(0,0,&DAT_00463534,1,&DAT_00463534,1);
      if (in_EAX == 0) {
        return 0;
      }
      DAT_00463fe4 = 2;
    }
    else {
      DAT_00463fe4 = 1;
    }
  }
  if (0 < _CchCount1) {
    in_EAX = wcsncnt(_LpString1,_CchCount1);
    _CchCount1 = in_EAX;
  }
  if (0 < _CchCount2) {
    in_EAX = wcsncnt(_LpString2,_CchCount2);
    _CchCount2 = in_EAX;
  }
  if ((_CchCount1 == 0) || (_CchCount2 == 0)) {
    if (_CchCount2 == _CchCount1) {
      return 2;
    }
    in_EAX = 3;
    if (_CchCount1 - _CchCount2 < 0) {
      in_EAX = 1;
    }
  }
  else {
    if (DAT_00463fe4 == 1) {
      iVar1 = CompareStringW((LCID)_LocaleName,_DwCmpFlags,_LpString1,_CchCount1,_LpString2,
                             _CchCount2);
      return iVar1;
    }
    if (DAT_00463fe4 == 2) {
      lpMultiByteStr_00 = (LPSTR)0x0;
      if (in_stack_0000001c == 0) {
        in_stack_0000001c = DAT_00463558;
      }
      cbMultiByte = WideCharToMultiByte(in_stack_0000001c,0x220,_LpString1,_CchCount1,(LPSTR)0x0,0,
                                        (LPCSTR)0x0,(LPBOOL)0x0);
      if (cbMultiByte == 0) {
        return 0;
      }
      lpMultiByteStr = (PCNZCH)FUN_0044c2f0(cbMultiByte);
      if (lpMultiByteStr == (PCNZCH)0x0) {
        return 0;
      }
      iVar1 = WideCharToMultiByte(in_stack_0000001c,0x220,_LpString1,_CchCount1,lpMultiByteStr,
                                  cbMultiByte,(LPCSTR)0x0,(LPBOOL)0x0);
      if ((((iVar1 != 0) &&
           (cbMultiByte_00 =
                 WideCharToMultiByte(in_stack_0000001c,0x220,_LpString2,_CchCount2,(LPSTR)0x0,0,
                                     (LPCSTR)0x0,(LPBOOL)0x0), cbMultiByte_00 != 0)) &&
          (lpMultiByteStr_00 = (LPSTR)FUN_0044c2f0(cbMultiByte_00), lpMultiByteStr_00 != (LPSTR)0x0)
          ) && (iVar1 = WideCharToMultiByte(in_stack_0000001c,0x220,_LpString2,_CchCount2,
                                            lpMultiByteStr_00,cbMultiByte_00,(LPCSTR)0x0,(LPBOOL)0x0
                                           ), iVar1 != 0)) {
        iVar1 = CompareStringA((LCID)_LocaleName,_DwCmpFlags,lpMultiByteStr,cbMultiByte,
                               lpMultiByteStr_00,cbMultiByte_00);
        FUN_0044ac40(lpMultiByteStr);
        FUN_0044ac40(lpMultiByteStr_00);
        return iVar1;
      }
      FUN_0044ac40(lpMultiByteStr);
      FUN_0044ac40(lpMultiByteStr_00);
      return 0;
    }
  }
  return in_EAX;
}

