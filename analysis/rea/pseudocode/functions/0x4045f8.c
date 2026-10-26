// rea decompile_all
// name: FUN_004045f8
// address: 0x4045f8
// body: available
// range: 0x4045f8-0x404850 inclusive
// range: 0x404856-0x404a14 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x404a0a to EXTERNAL:0x2d KERNEL32.DLL::ExitProcess (external=true, thunk=false, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl
FUN_004045f8(uint *param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int iVar1;
  uint local_1004 [512];
  uint local_804 [504];
  undefined4 uStackY_24;
  
  FUN_0044be20();
  local_804[0]._0_1_ = 0;
  local_1004[0]._0_1_ = 0;
  FUN_0044ba08(local_804,(uint *)s_ASSERT_DSERR__0045ce70);
  FUN_0044ba08(local_804,param_1);
  _sprintf((char *)local_1004,s___Line____d__0045ce80);
  FUN_0044ba08(local_804,local_1004);
  _sprintf((char *)local_1004,s_File____s_0045ce90);
  FUN_0044ba08(local_804,local_1004);
  if (param_4 < -0x7ffbfeef) {
    if (param_4 == -0x7ffbfef0) {
      _sprintf((char *)local_1004,s_DSERR_NOAGGREGATION_0045cf40);
      goto LAB_0040499a;
    }
    if (param_4 == -0x7fffbfff) {
      _sprintf((char *)local_1004,s_DSERR_UNSUPPORTED_0045cfb4);
      goto LAB_0040499a;
    }
    if (param_4 == -0x7fffbffe) {
      _sprintf((char *)local_1004,s_E_NOINTERFACE_0045cfdc);
      goto LAB_0040499a;
    }
  }
  else if (param_4 < -0x7ff8ffa8) {
    if (param_4 == -0x7ff8ffa9) {
      _sprintf((char *)local_1004,s_DSERR_INVALIDPARAM_0045cf28);
      goto LAB_0040499a;
    }
    if (param_4 == -0x7ff8fff2) {
      _sprintf((char *)local_1004,s_DSERR_OUTOFMEMORY_0045cfc8);
      goto LAB_0040499a;
    }
  }
  else if (param_4 < -0x7787ffe1) {
    if (param_4 == -0x7787ffe2) {
      _sprintf((char *)local_1004,s_DSERR_CONTROLUNAVAIL_0045cefc);
      goto LAB_0040499a;
    }
    if (param_4 == -0x7787fff6) {
      _sprintf((char *)local_1004,s_DSERR_ALLOCATED_0045cea4);
      goto LAB_0040499a;
    }
  }
  else if (param_4 < -0x7787ffb9) {
    if (param_4 == -0x7787ffba) {
      _sprintf((char *)local_1004,s_DSERR_PRIOLEVELNEEDED_0045cf84);
      goto LAB_0040499a;
    }
    if (param_4 == -0x7787ffce) {
      _sprintf((char *)local_1004,s_DSERR_INVALIDCALL_0045cf14);
      goto LAB_0040499a;
    }
  }
  else if (param_4 < -0x7787ff87) {
    if (param_4 == -0x7787ff88) {
      _sprintf((char *)local_1004,s_DSERR_NODRIVER_0045cf58);
      goto LAB_0040499a;
    }
    if (param_4 == -0x7787ff9c) {
      _sprintf((char *)local_1004,s_DSERR_BADFORMAT_0045ced4);
      goto LAB_0040499a;
    }
  }
  else if (param_4 < -0x7787ff69) {
    if (param_4 == -0x7787ff6a) {
      _sprintf((char *)local_1004,s_DSERR_BUFFERLOST_0045cee8);
      goto LAB_0040499a;
    }
    if (param_4 == -0x7787ff7e) {
      _sprintf((char *)local_1004,s_DSERR_ALREADYINITIALIZED_0045ceb8);
      goto LAB_0040499a;
    }
  }
  else {
    if (param_4 == -0x7787ff60) {
      _sprintf((char *)local_1004,s_DSERR_OTHERAPPHASPRIO_0045cf6c);
      goto LAB_0040499a;
    }
    if (param_4 == -0x7787ff56) {
      _sprintf((char *)local_1004,s_DSERR_UNINITIALIZED_0045cf9c);
      goto LAB_0040499a;
    }
    if (param_4 == 0) {
      _sprintf((char *)local_1004,s_DS_OK_0045ce9c);
      goto LAB_0040499a;
    }
  }
  _sprintf((char *)local_1004,s_no_error_associated_0045cfec);
LAB_0040499a:
  FUN_0044ba08(local_804,local_1004);
  uStackY_24 = 0x4049c8;
  FUN_00404a15(local_804,-0x29a,(uint *)0x0,-0x29a);
  iVar1 = FUN_00416220();
  if (iVar1 != 0) {
    (**(code **)(*DAT_00486a04 + 0x28))();
  }
  uStackY_24 = 0x4049fe;
  MessageBoxA((HWND)0x0,(LPCSTR)local_804,s_Assert_Failed_0045d004,0);
  if (param_5 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ExitProcess(0);
}

