// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4045f8
// name: FUN_004045f8
// ranges: 0x4045f8-0x404850; 0x404856-0x404a14
// calls: 0x4045f8, 0x404a15, 0x416220, 0x44ba08, 0x44be20

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
