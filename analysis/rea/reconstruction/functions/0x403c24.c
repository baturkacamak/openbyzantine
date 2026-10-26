// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x403c24
// name: FUN_00403c24
// ranges: 0x403c24-0x403d53
// calls: 0x40305a, 0x403c24, 0x404a15, 0x416220, 0x44ba08, 0x44be20

void __cdecl
FUN_00403c24(uint *param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int iVar1;
  undefined1 local_1004 [2048];
  undefined1 local_804 [2016];
  undefined4 uStackY_24;
  
  FUN_0044be20();
  local_804[0] = '\0';
  local_1004[0] = '\0';
  FUN_0044ba08((uint *)local_804,(uint *)s_ASSERT_DDERR__0045c90c);
  FUN_0044ba08((uint *)local_804,param_1);
  _sprintf(local_1004,s___Line____d__0045c91c);
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  _sprintf(local_1004,s_File____s_0045c92c);
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  FUN_0040305a(param_4,local_1004);
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  uStackY_24 = 0x403d07;
  FUN_00404a15((uint *)local_804,-0x29a,(uint *)0x0,-0x29a);
  iVar1 = FUN_00416220();
  if (iVar1 != 0) {
    (**(code **)(*DAT_00486a04 + 0x28))();
  }
  uStackY_24 = 0x403d3d;
  MessageBoxA((HWND)0x0,local_804,s_Assert_Failed_0045c938,0);
  if (param_5 != 0) {
                    /* WARNING: Subroutine does not return */
    ExitProcess(0);
  }
  return;
}
