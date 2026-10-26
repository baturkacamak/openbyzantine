// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x456b80
// name: FUN_00456b80
// ranges: 0x456b80-0x456dc7
// calls: 0x456b80, 0x456dd0

undefined4 __cdecl
FUN_00456b80(int param_1,int param_2,int param_3,uint param_4,undefined4 *param_5)

{
  double dVar1;
  int iVar2;
  undefined8 local_8;
  
  dVar1 = ABS((double)CONCAT44(param_2,param_1));
  if ((param_4 == 0x7ff00000) && (param_3 == 0)) {
    if (1.0 < dVar1) {
      param_5[1] = DAT_0046341c;
      *param_5 = DAT_00463418;
      return 0;
    }
    if (dVar1 < 1.0) {
      *param_5 = 0;
      param_5[1] = 0;
      return 0;
    }
    param_5[1] = DAT_00463424;
    *param_5 = DAT_00463420;
    return 1;
  }
  if ((param_4 == 0xfff00000) && (param_3 == 0)) {
    if (1.0 < dVar1) {
      *param_5 = 0;
      param_5[1] = 0;
      return 0;
    }
    if (dVar1 < 1.0) {
      param_5[1] = DAT_0046341c;
      *param_5 = DAT_00463418;
      return 0;
    }
    param_5[1] = DAT_00463424;
    *param_5 = DAT_00463420;
    return 1;
  }
  if ((param_2 != 0x7ff00000) || (param_1 != 0)) {
    if ((param_2 == -0x100000) && (param_1 == 0)) {
      iVar2 = FUN_00456dd0(param_3,param_4);
      if (0.0 < (double)CONCAT44(param_4,param_3)) {
        if (iVar2 == 1) {
          local_8 = -(double)CONCAT44(DAT_0046341c,DAT_00463418);
        }
        else {
          local_8 = (double)CONCAT44(DAT_0046341c,DAT_00463418);
        }
        param_5[1] = local_8._4_4_;
        *param_5 = (undefined4)local_8;
        return 0;
      }
      if ((double)CONCAT44(param_4,param_3) < 0.0) {
        if (iVar2 == 1) {
          local_8._0_4_ = DAT_00463438;
          local_8._4_4_ = DAT_0046343c;
        }
        else {
          local_8._0_4_ = 0;
          local_8._4_4_ = 0;
        }
        param_5[1] = local_8._4_4_;
        *param_5 = (undefined4)local_8;
        return 0;
      }
      *param_5 = 0;
      param_5[1] = 0x3ff00000;
    }
    return 0;
  }
  if (0.0 < (double)CONCAT44(param_4,param_3)) {
    param_5[1] = DAT_0046341c;
    *param_5 = DAT_00463418;
    return 0;
  }
  *param_5 = 0;
  if ((double)CONCAT44(param_4,param_3) < 0.0) {
    param_5[1] = 0;
    return 0;
  }
  param_5[1] = 0x3ff00000;
  return 0;
}
