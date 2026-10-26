// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40c7b4
// name: FUN_0040c7b4
// ranges: 0x40c7b4-0x40c910
// calls: 0x40c7b4, 0x40c911, 0x40c9ce, 0x40ef18, 0x4161f6, 0x41d6df

undefined4 __cdecl FUN_0040c7b4(int param_1)

{
  int iVar1;
  INT_PTR IVar2;
  HWND pHVar3;
  char *lpText;
  char *lpCaption;
  UINT uType;
  char local_b0 [80];
  int local_60;
  char local_c;
  INT_PTR local_8;
  
  local_60 = DAT_0045da78;
  if (DAT_0046c440 == '\0') {
    if (DAT_0045da74 == 0) {
      iVar1 = FUN_0040c911();
      DAT_0046c440 = (char)iVar1;
    }
    else if (DAT_0045da74 == 2) {
      DAT_0046c440 = 'C';
      DAT_0045da7c = 3;
    }
    else {
      pHVar3 = (HWND)FUN_004161f6();
      IVar2 = FUN_0040ef18(pHVar3);
      if (IVar2 == 0) {
        return 1;
      }
      DAT_0046c440 = DAT_0045e764;
      DAT_0045da7c = 2;
    }
  }
  local_c = FUN_0040c9ce(param_1);
  while( true ) {
    if (local_c != '\0') {
      if (param_1 != 0) {
        DAT_0045da78 = param_1;
      }
      DAT_00486a0e = local_c;
      if ((DAT_0045da74 != 0) && (local_60 != DAT_0045da78)) {
        _sprintf(local_b0,s_Now_accessing_CD__d_0045daac,DAT_0045da78);
        uType = 0;
        lpCaption = s_Beta_Info_Message_0045dac0;
        lpText = local_b0;
        pHVar3 = (HWND)FUN_004161f6();
        MessageBoxA(pHVar3,lpText,lpCaption,uType);
      }
      return 0;
    }
    local_8 = FUN_0041d6df(param_1);
    if (local_8 == 2) break;
    local_c = FUN_0040c9ce(param_1);
  }
  return 1;
}
