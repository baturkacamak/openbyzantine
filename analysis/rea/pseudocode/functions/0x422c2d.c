// rea decompile_all
// name: FUN_00422c2d
// address: 0x422c2d
// body: available
// range: 0x422c2d-0x422d3e inclusive
// range: 0x422d44-0x422e6d inclusive
// range: 0x422e73-0x422efe inclusive
// range: 0x422f09-0x422f36 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00422c2d(HWND param_1,int param_2,short param_3)

{
  MMRESULT MVar1;
  LONG LVar2;
  
  if (param_2 == 2) {
    SendMessageA(param_1,0x111,0x39c47,0);
    if (DAT_0045fd20 != 0) {
      timeKillEvent(DAT_0045fd24);
      timeEndPeriod(0x32);
      while (LVar2 = InterlockedExchange((LONG *)&DAT_004618b0,1), LVar2 != 0) {
        Sleep(100);
      }
      DAT_0045fd20 = 0;
    }
  }
  else if (param_2 == 0x111) {
    if (param_3 == -0x63ba) {
      if (DAT_0045fd1c != 0) {
        SendMessageA(param_1,0x111,0x9c47,0);
      }
      if ((DAT_0045fd18 == 0) || (DAT_0045fd14 == (int *)0x0)) {
        DAT_0045fd20 = 0;
        DAT_0045fd1c = 0;
      }
      else {
        (**(code **)(*DAT_0045fd14 + 0x34))(DAT_0045fd14,0);
        (**(code **)(*DAT_0045fd14 + 0x30))(DAT_0045fd14,0,0,1);
        MVar1 = timeBeginPeriod(0x32);
        if (MVar1 == 0) {
          _DAT_004618b0 = 0;
          DAT_0045fd24 = timeSetEvent(0x32,10,FUN_0043cda0,0,1);
          if (DAT_0045fd24 != 0) {
            DAT_0045fd20 = 1;
          }
          DAT_0045fd1c = 1;
        }
        else {
          (**(code **)(*DAT_0045fd14 + 0x48))(DAT_0045fd14);
          DAT_0045fd20 = 0;
          DAT_0045fd1c = 0;
        }
      }
    }
    else if (param_3 == -0x63b9) {
      DAT_004867e4 = 1;
      if (DAT_0045fd20 != 0) {
        timeKillEvent(DAT_0045fd24);
        timeEndPeriod(0x32);
        while (LVar2 = InterlockedExchange((LONG *)&DAT_004618b0,1), LVar2 != 0) {
          Sleep(100);
        }
        DAT_0045fd20 = 0;
      }
      if (DAT_0045fd1c != 0) {
        DAT_0045fd1c = 0;
        (**(code **)(*DAT_0045fd14 + 0x48))(DAT_0045fd14);
      }
      if (DAT_0045fd18 != 0) {
        FUN_0040513f(&DAT_004867a4,&DAT_004867a0);
        if (DAT_0045fd14 != (int *)0x0) {
          (**(code **)(*DAT_0045fd14 + 8))(DAT_0045fd14);
        }
        DAT_0045fd14 = (int *)0x0;
        DAT_0045fd18 = 0;
        FUN_00422770();
      }
    }
  }
  else if (param_2 == 0x500) {
    SendMessageA(param_1,0x111,0x9c47,0);
  }
  return;
}

