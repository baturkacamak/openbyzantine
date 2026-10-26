// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40434d
// name: FUN_0040434d
// ranges: 0x40434d-0x404492; 0x404498-0x4044bc; 0x40457d-0x4045f7
// calls: 0x40434d, 0x404a15, 0x416220, 0x44ba08, 0x44be20

void __cdecl
FUN_0040434d(uint *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,int param_5)

{
  int iVar1;
  undefined1 local_1004 [2048];
  undefined1 local_804 [2016];
  undefined4 uStackY_24;
  
  FUN_0044be20();
  local_804[0] = '\0';
  local_1004[0] = '\0';
  FUN_0044ba08((uint *)local_804,(uint *)s_ASSERT_MMRESULT__0045cdb8);
  FUN_0044ba08((uint *)local_804,param_1);
  _sprintf(local_1004,s___Line____d__0045cdcc);
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  _sprintf(local_1004,s_File____s_0045cddc);
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  switch(param_4) {
  case 0:
    _sprintf(local_1004,s_JOYERE_NOERROR_0045cde8);
    break;
  default:
    _sprintf(local_1004,s_no_error_associated_0045ce48);
    break;
  case 6:
    _sprintf(local_1004,s_MMSYSERR_NODRIVER_0045cdfc);
    break;
  case 0xa5:
    _sprintf(local_1004,s_JOYERR_PARMS_0045ce10);
    break;
  case 0xa6:
    _sprintf(local_1004,s_JOYERR_NOCANDO_0045ce20);
    break;
  case 0xa7:
    _sprintf(local_1004,s_JOYERR_UNPLUGGED_0045ce34);
  }
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  uStackY_24 = 0x4045ab;
  FUN_00404a15((uint *)local_804,-0x29a,(uint *)0x0,-0x29a);
  iVar1 = FUN_00416220();
  if (iVar1 != 0) {
    (**(code **)(*DAT_00486a04 + 0x28))();
  }
  uStackY_24 = 0x4045e1;
  MessageBoxA((HWND)0x0,local_804,s_Assert_Failed_0045ce60,0);
  if (param_5 != 0) {
                    /* WARNING: Subroutine does not return */
    ExitProcess(0);
  }
  return;
}
