// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x416d05
// name: FUN_00416d05
// ranges: 0x416d05-0x416eba
// calls: 0x40f135, 0x416d05, 0x42b83f, 0x44ac40

/* WARNING: Type propagation algorithm not settling */

undefined4 __cdecl
FUN_00416d05(char *param_1,undefined4 *param_2,int param_3,uint *param_4,uint *param_5)

{
  undefined4 uVar1;
  uint local_8c;
  uint local_88;
  uint local_84 [4];
  uint local_74;
  int local_70;
  int local_5c;
  undefined4 local_18;
  int local_14;
  uint local_10;
  ushort *local_c;
  int *local_8;
  
  local_c = FUN_0040f135(param_1,(int *)local_84,&local_10,(ushort *)0x0,0x280);
  if (local_c == (ushort *)0x0) {
    uVar1 = 1;
  }
  else {
    _memset(local_84 + 1,0,0x6c);
    local_84[1] = 0x6c;
    local_84[2] = 7;
    local_18 = 0x840;
    local_74 = local_84[0];
    local_84[3] = local_10;
    local_14 = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,local_84 + 1,param_2,0);
    if (local_14 == 0) {
      local_8 = (int *)*param_2;
      local_14 = (**(code **)(*local_8 + 0x58))(local_8,local_84 + 1);
      if (param_3 != 0) {
        local_8c = (uint)DAT_00485c88;
        local_88 = (uint)DAT_00485c88;
        local_14 = (**(code **)(*local_8 + 0x74))(local_8,8,&local_8c);
        if (local_14 != 0) {
          return 1;
        }
      }
      local_14 = (**(code **)(*local_8 + 100))(local_8,0,local_84 + 1,1,0);
      if (local_14 == 0) {
        FUN_0042b83f(local_5c,local_c,local_70 >> 1,local_10,local_84[0],0,0);
        (**(code **)(*local_8 + 0x80))(local_8,0);
        FUN_0044ac40(local_c);
        if (param_4 != (uint *)0x0) {
          *param_4 = local_84[0];
        }
        if (param_5 != (uint *)0x0) {
          *param_5 = local_10;
        }
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
