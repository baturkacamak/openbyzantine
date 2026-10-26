// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4076b4
// name: FUN_004076b4
// ranges: 0x4076b4-0x4077b1
// calls: 0x4076b4

undefined4 __cdecl FUN_004076b4(int *param_1,short param_2,short param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 local_90 [8];
  undefined4 local_88 [2];
  undefined4 local_80;
  undefined4 local_7c;
  int local_1c;
  uint local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  if (param_1 == (int *)0x0) {
    uVar1 = 1;
  }
  else {
    _memset(local_88,0,0x6c);
    local_88[0] = 0x6c;
    local_1c = (**(code **)(*param_1 + 0x58))(param_1,local_88);
    if (local_1c == 0) {
      local_14 = 0;
      local_10 = 0;
      local_8 = local_80;
      local_c = local_7c;
      local_1c = (**(code **)(*param_1 + 0x40))(param_1,8,local_90);
      local_18 = (uint)(local_1c == 0);
      iVar2 = (**(code **)(*DAT_0045d1b0 + 0x1c))
                        (DAT_0045d1b0,(int)param_2,(int)param_3,param_1,&local_14,local_18);
      if (iVar2 == 0) {
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  return uVar1;
}
