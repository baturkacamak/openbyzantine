// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x404b97
// name: FUN_00404b97
// ranges: 0x404b97-0x404ce9
// calls: 0x404a15, 0x404b97, 0x44ba00, 0x44ba08

void __cdecl FUN_00404b97(uint *param_1,short param_2,uint *param_3,int param_4)

{
  uint local_804 [512];
  
  _memset(local_804,0,0x800);
  _memset(&DAT_0045c120,0,0x100);
  FUN_0044ba00((uint *)&DAT_0045c120,param_1);
  if (param_4 != -0x29a) {
    _sprintf((char *)local_804,&DAT_0045d094,param_4);
    FUN_0044ba08((uint *)&DAT_0045c120,local_804);
  }
  _sprintf((char *)local_804,s____d__0045d09c,(int)param_2);
  FUN_0044ba08((uint *)&DAT_0045c120,local_804);
  FUN_0044ba08((uint *)&DAT_0045c120,param_3);
  _memset(local_804,0,0x800);
  FUN_0044ba00(local_804,param_1);
  FUN_0044ba08(local_804,(uint *)&DAT_0045d0a4);
  if (param_3 != (uint *)0x0) {
    if (param_4 == 0x29a) {
      FUN_00404a15(local_804,param_2,param_3,-0x29a);
    }
    else {
      FUN_00404a15(local_804,param_2,param_3,param_4);
    }
  }
  return;
}
