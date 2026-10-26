// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x417a17
// name: FUN_00417a17
// ranges: 0x417a17-0x417c32
// calls: 0x4063e0, 0x4179ac, 0x417a17, 0x44ac40, 0x44ba00, 0x44c2f0, 0x44c580

void __cdecl FUN_00417a17(uint *param_1,int param_2)

{
  char *pcVar1;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  int local_80;
  int local_7c;
  undefined4 local_78 [9];
  int local_54;
  int local_8;
  
  local_7c = DAT_00487508 * 3;
  local_8 = local_7c + 0x2e;
  if (param_1 == (uint *)0x0) {
    param_2 = 2;
  }
  if (DAT_0045f218 != (int *)0x0) {
    _memset(local_78,0,0x6c);
    local_78[0] = 0x6c;
    if ((param_2 == 2) || (DAT_0045f240 != 2)) {
      (**(code **)(*DAT_0045f218 + 100))(DAT_0045f218,0,local_78,1,0);
      local_80 = local_54;
      FID_conflict__memcpy
                ((void *)(local_54 + 0xe100),(void *)(local_8 * 0x500 + local_54),local_7c * 0x500);
      (**(code **)(*DAT_0045f218 + 0x80))(DAT_0045f218,0);
    }
    if (param_2 == 2) {
      if (DAT_0045f214 != (uint *)0x0) {
        FUN_0044ac40(DAT_0045f214);
        DAT_0045f214 = (uint *)0x0;
      }
    }
    else {
      if ((param_2 != 1) && (param_1 != DAT_0045f214)) {
        if (DAT_0045f214 != (uint *)0x0) {
          FUN_0044ac40(DAT_0045f214);
        }
        pcVar1 = FUN_0044c580(param_1);
        DAT_0045f214 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar1 + 1));
        FUN_0044ba00(DAT_0045f214,param_1);
      }
      if ((param_2 == 0) && (DAT_0045f240 == 1)) {
        param_2 = 1;
      }
      else if (DAT_0045f210 != 0) {
        FUN_004063e0((char *)param_1,0x26e,0x2d,3,DAT_0045f218,0,4,3);
      }
    }
    DAT_0045f240 = param_2;
    if (DAT_0045c060 == 0) {
      local_90 = 0;
      local_8c = 0;
      local_88 = 0x280;
      local_84 = 400;
      FUN_004179ac();
      (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0,0x14,DAT_0045f218,&local_90,1);
    }
  }
  return;
}
