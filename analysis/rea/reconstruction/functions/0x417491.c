// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x417491
// name: FUN_00417491
// ranges: 0x417491-0x4176d7
// calls: 0x40bbb0, 0x40bbfd, 0x417314, 0x417491, 0x417c33

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00417491(float param_1,float param_2,int *param_3)

{
  longlong lVar1;
  undefined4 local_94 [4];
  int local_84;
  int local_70;
  int *local_28;
  short local_24;
  short local_20;
  int local_1c;
  float local_18;
  tagRECT local_14;
  
  local_14.left = 0;
  local_14.top = 0;
  local_14.right = 0x280;
  local_14.bottom = 400;
  if ((DAT_0045c060 == 0) || (DAT_0045f210 == 0)) {
    if (DAT_0045f220 == 1) {
      FUN_00417314(0);
      DAT_0045f220 = 0;
    }
    (**(code **)(*param_3 + 0x1c))(param_3,0,0x14,DAT_0045f218,&local_14,1);
  }
  else {
    if (DAT_0045f220 == 0) {
      FUN_00417314(1);
      DAT_0045f220 = 1;
    }
    (**(code **)(*param_3 + 0x1c))(param_3,0,0x14,DAT_0045f218,&local_14,1);
    if (param_1 == 360.0) {
      param_1 = 0.0;
    }
    local_18 = param_1 - _DAT_0047a8dc;
    if (local_18 < 0.0) {
      local_18 = local_18 + 360.0;
    }
    if (DAT_00462ae8 != 0) {
      __adj_fdiv_m64(0x189374bc,0x40768004);
    }
    lVar1 = __ftol();
    local_1c = (int)lVar1;
    local_28 = (int *)(&DAT_0046c538)[local_1c];
    if (local_28 != (int *)0x0) {
      _memset(local_94,0,0x6c);
      local_94[0] = 0x6c;
      local_20 = *(short *)(local_28[1] + 6);
      local_24 = *(short *)(local_28[1] + 8);
      SetRect(&local_14,(int)local_20,(int)local_24,(int)*(short *)(local_28[1] + 2) + (int)local_20
              ,(int)*(short *)(local_28[1] + 4) + (int)local_24);
      (**(code **)(*param_3 + 100))(param_3,0,local_94,1,0);
      FUN_0040bbb0(local_20,local_24,(int)*(short *)(local_28[1] + 4),local_84 >> 1);
      FUN_0040bbfd(*local_28,local_70,local_20,local_24,&local_14.left,0);
      (**(code **)(*param_3 + 0x80))(param_3,0);
    }
    FUN_00417c33(param_1,param_2,param_3);
  }
  return;
}
