// rea decompile_all
// name: FUN_00457190
// address: 0x457190
// body: available
// range: 0x457190-0x45747c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


size_t __cdecl
FUN_00457190(LCID param_1,DWORD param_2,byte *param_3,size_t param_4,byte *param_5,size_t param_6,
            UINT param_7)

{
  size_t in_EAX;
  size_t sVar1;
  BOOL BVar2;
  BYTE *pBVar3;
  PCNZWCH lpWideCharStr;
  int iVar4;
  int iVar5;
  LPWSTR local_20;
  size_t local_18;
  _cpinfo local_14;
  
  if (DAT_00463fe8 == 0) {
    in_EAX = CompareStringA(0,0,&DAT_00463534,1,&DAT_00463534,1);
    if (in_EAX == 0) {
      in_EAX = CompareStringW(0,0,(PCNZWCH)&DAT_00463538,1,(PCNZWCH)&DAT_00463538,1);
      if (in_EAX == 0) {
        return 0;
      }
      DAT_00463fe8 = 1;
    }
    else {
      DAT_00463fe8 = 2;
    }
  }
  local_18 = in_EAX;
  if (0 < (int)param_4) {
    local_18 = _strncnt((char *)param_3,param_4);
    param_4 = local_18;
  }
  if (0 < (int)param_6) {
    local_18 = _strncnt((char *)param_5,param_6);
    param_6 = local_18;
  }
  if (DAT_00463fe8 == 2) {
    sVar1 = CompareStringA(param_1,param_2,(PCNZCH)param_3,param_4,(PCNZCH)param_5,param_6);
    return sVar1;
  }
  if (DAT_00463fe8 == 1) {
    local_18 = 0;
    local_20 = (LPWSTR)0x0;
    if (param_7 == 0) {
      param_7 = DAT_00463558;
    }
    if ((param_4 == 0) || (param_6 == 0)) {
      if (param_6 == param_4) {
        return 2;
      }
      if (1 < (int)param_6) {
        return 1;
      }
      if (1 < (int)param_4) {
        return 3;
      }
      BVar2 = GetCPInfo(param_7,&local_14);
      if (BVar2 == 0) {
        return 0;
      }
      if (0 < (int)param_4) {
        if (local_14.MaxCharSize < 2) {
          return 3;
        }
        pBVar3 = local_14.LeadByte;
        while( true ) {
          if ((local_14.LeadByte[0] == 0) || (pBVar3[1] == 0)) {
            return 3;
          }
          if ((*pBVar3 <= *param_3) && (*param_3 <= pBVar3[1])) break;
          pBVar3 = pBVar3 + 2;
          local_14.LeadByte[0] = *pBVar3;
        }
        return 2;
      }
      if (0 < (int)param_6) {
        if (local_14.MaxCharSize < 2) {
          return 1;
        }
        pBVar3 = local_14.LeadByte;
        while( true ) {
          if ((local_14.LeadByte[0] == 0) || (pBVar3[1] == 0)) {
            return 1;
          }
          if ((*pBVar3 <= *param_5) && (*param_5 <= pBVar3[1])) break;
          pBVar3 = pBVar3 + 2;
          local_14.LeadByte[0] = *pBVar3;
        }
        return 2;
      }
    }
    local_14.MaxCharSize = MultiByteToWideChar(param_7,9,(LPCSTR)param_3,param_4,(LPWSTR)0x0,0);
    if (local_14.MaxCharSize == 0) {
      return 0;
    }
    lpWideCharStr = (PCNZWCH)FUN_0044c2f0(local_14.MaxCharSize * 2);
    if (lpWideCharStr == (PCNZWCH)0x0) {
      return 0;
    }
    iVar4 = MultiByteToWideChar(param_7,1,(LPCSTR)param_3,param_4,lpWideCharStr,local_14.MaxCharSize
                               );
    if ((((iVar4 != 0) &&
         (iVar4 = MultiByteToWideChar(param_7,9,(LPCSTR)param_5,param_6,(LPWSTR)0x0,0), iVar4 != 0))
        && (local_20 = (LPWSTR)FUN_0044c2f0(iVar4 * 2), local_20 != (LPWSTR)0x0)) &&
       (iVar5 = MultiByteToWideChar(param_7,1,(LPCSTR)param_5,param_6,local_20,iVar4), iVar5 != 0))
    {
      local_18 = CompareStringW(param_1,param_2,lpWideCharStr,local_14.MaxCharSize,local_20,iVar4);
    }
    FUN_0044ac40(lpWideCharStr);
    FUN_0044ac40(local_20);
  }
  return local_18;
}

