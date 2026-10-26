// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x402ec0
// name: FUN_00402ec0
// ranges: 0x402ec0-0x403059
// calls: 0x402ec0, 0x404a15, 0x416220, 0x44ba08, 0x44be20

void __cdecl
FUN_00402ec0(uint *param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int iVar1;
  undefined1 local_1008 [2048];
  int local_808;
  undefined1 local_804 [2016];
  undefined4 uStackY_24;
  
  FUN_0044be20();
  local_804[0] = '\0';
  local_1008[0] = '\0';
  FUN_0044ba08((uint *)local_804,(uint *)s_ASSERT__0045c224);
  if (param_4 != 0) {
    _sprintf(local_1008,&DAT_0045c230);
    FUN_0044ba08((uint *)local_804,(uint *)local_1008);
  }
  if (param_5 != -0x29a) {
    _sprintf(local_1008,s___Data____d_0045c234);
    FUN_0044ba08((uint *)local_804,(uint *)local_1008);
  }
  FUN_0044ba08((uint *)local_804,(uint *)&DAT_0045c240);
  FUN_0044ba08((uint *)local_804,param_1);
  _sprintf(local_1008,s___Line____d__0045c244);
  FUN_0044ba08((uint *)local_804,(uint *)local_1008);
  _sprintf(local_1008,s_File____s_0045c254);
  FUN_0044ba08((uint *)local_804,(uint *)local_1008);
  uStackY_24 = 0x403001;
  FUN_00404a15((uint *)local_804,-0x29a,(uint *)0x0,-0x29a);
  iVar1 = FUN_00416220();
  if (iVar1 != 0) {
    (**(code **)(*DAT_00486a04 + 0x28))();
  }
  uStackY_24 = 0x40303a;
  local_808 = MessageBoxA((HWND)0x0,local_804,s_Assert_Failed_0045c260,0x10002);
  if (local_808 == 3) {
                    /* WARNING: Subroutine does not return */
    ExitProcess(0);
  }
  return;
}
