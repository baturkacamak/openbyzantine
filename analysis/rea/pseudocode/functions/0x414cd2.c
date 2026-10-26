// rea decompile_all
// name: FUN_00414cd2
// address: 0x414cd2
// body: available
// range: 0x414cd2-0x414db5 inclusive
// range: 0x414dbb-0x415062 inclusive
// range: 0x415068-0x415098 inclusive
// range: 0x41509e-0x415169 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x414fb5 to EXTERNAL:0x2d KERNEL32.DLL::ExitProcess (external=true, thunk=false, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


LRESULT FUN_00414cd2(HWND param_1,uint param_2,uint param_3,int *param_4)

{
  uint uVar1;
  HWND hWnd;
  LRESULT LVar2;
  char *lpText;
  uint *lpCaption;
  UINT uType;
  uint local_15c;
  undefined4 local_158;
  uint local_154 [20];
  char local_104 [80];
  char *local_b4;
  uint local_b0 [20];
  uint local_60 [20];
  int *local_10;
  int *local_c;
  LPVOID local_8;
  
  FUN_00422c2d(param_1,param_2,(short)param_3);
  if (param_2 < 6) {
    if (param_2 == 5) {
      if ((DAT_0046c4ec != 0) && (DAT_0045f080 != 0)) {
        PostMessageA(DAT_0046c4e8,2,0,0);
      }
    }
    else if ((param_2 == 2) && (DAT_0045f080 != 0)) {
      if (DAT_0045f06c != 0) {
        return 1;
      }
      DAT_0045f06c = 1;
      FUN_00413b81();
      FUN_0040d9f7();
      ShowCursor(1);
      FUN_0041d45a(local_154,0x47);
      local_b4 = FUN_0044c580(local_154);
      local_b4 = local_b4 + -1;
      if (*(char *)((int)local_154 + (int)local_b4) == '\n') {
        *(char *)((int)local_154 + (int)local_b4) = '\0';
      }
      FUN_0041d45a(local_60,0x4b);
      if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
        if ((DAT_0045d1d0 == 0) || (DAT_0045f498 != 0)) {
          FUN_0041d45a(local_b0,0x4a);
        }
        else {
          FUN_0042515f(0xbe);
          FUN_0042515f(0xd2);
          FUN_0041d45a(local_b0,0x48);
        }
      }
      else {
        FUN_0041d45a(local_b0,0x49);
      }
      _sprintf(local_104,s__s__s_0045f0b8,local_60,local_b0);
      uType = 0;
      lpCaption = local_154;
      lpText = local_104;
      hWnd = (HWND)FUN_004161f6();
      MessageBoxA(hWnd,lpText,(LPCSTR)lpCaption,uType);
      FUN_004152dd();
      if (DAT_0045f084 != 0) {
        SystemParametersInfoA(0x11,1,(PVOID)0x0,0);
      }
                    /* WARNING: Subroutine does not return */
      ExitProcess(0);
    }
  }
  else if (param_2 < 0x1d) {
    if (param_2 == 0x1c) {
      DAT_0045f07c = param_3;
    }
    else if (param_2 == 0x14) {
      return 1;
    }
  }
  else if (param_2 < 0x112) {
    if (param_2 == 0x111) {
      uVar1 = param_3 & 0xffff;
      if (uVar1 == 0x9c44) {
        DAT_0045f060 = 1;
        DAT_0045f064 = param_4;
      }
      else if ((0xc352 < uVar1) && (uVar1 < 0xc355)) {
        local_c = param_4;
        CloseHandle((HANDLE)param_4[5]);
        DAT_00460198 = DAT_00460198 + -1;
        if (param_3 == 0xc353) {
          local_8 = (LPVOID)local_c[4];
          if (*(int *)(&DAT_00485c90 + *local_c * 4) == 0) {
            *(LPVOID *)(&DAT_00485c90 + *local_c * 4) = local_8;
          }
          else {
            local_10 = *(int **)((int)local_8 + 8);
            (**(code **)(*local_10 + 8))(local_10);
            FUN_0044ac40(local_8);
          }
        }
        FUN_0044ac40(local_c);
      }
    }
    else if (param_2 == 0x100) {
      if (param_3 == 0xd) {
        DAT_0045f050 = 1;
      }
      else if (param_3 == 0x1b) {
        DAT_0045f058 = 1;
      }
      else if (param_3 == 0x20) {
        DAT_0045f054 = 1;
      }
    }
  }
  else if (param_2 == 0x200) {
    local_15c = (uint)param_4 & 0xffff;
    local_158 = (uint)param_4 >> 0x10;
    FUN_0040d988((int *)&local_15c);
  }
  else if (param_2 == 0x201) {
    if (param_3 == 1) {
      FUN_0040d809();
    }
  }
  else if ((param_2 == 0x204) && (param_3 == 2)) {
    FUN_0040d82b();
  }
  LVar2 = DefWindowProcA(param_1,param_2,param_3,(LPARAM)param_4);
  return LVar2;
}

