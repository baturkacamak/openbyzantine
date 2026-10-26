// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40e627
// name: FUN_0040e627
// ranges: 0x40e627-0x40e7ec
// calls: 0x40e627

void __cdecl FUN_0040e627(int *param_1,int *param_2,int *param_3,int *param_4)

{
  undefined4 local_a0 [2];
  int local_98;
  int local_94;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  undefined4 local_8;
  
  local_28 = DAT_0046c450;
  local_24 = DAT_0046c454;
  local_20 = DAT_0046c458;
  local_1c = DAT_0046c45c;
  local_2c = *param_2 + DAT_0046c448;
  local_34 = *param_3 + DAT_0046c44c;
  local_10 = DAT_0046c458 + local_2c;
  local_c = DAT_0046c45c + local_34;
  local_18 = local_2c;
  local_14 = local_34;
  _memset(local_a0,0,0x6c);
  local_a0[0] = 0x6c;
  local_8 = (**(code **)(*param_1 + 0x58))(param_1,local_a0);
  if (local_2c < 0) {
    local_18 = local_18 - local_2c;
    local_28 = local_28 - local_2c;
    local_2c = 0;
  }
  local_94 = local_10 - local_94;
  if (0 < local_94) {
    local_10 = local_10 - local_94;
    local_20 = local_20 - local_94;
  }
  if (local_34 < 0) {
    local_14 = local_14 - local_34;
    local_24 = local_24 - local_34;
    local_34 = 0;
  }
  local_30 = local_c - local_98;
  if (0 < local_30) {
    local_c = local_c - local_30;
    local_1c = local_1c - local_30;
  }
  local_8 = (**(code **)(*DAT_0045dbd8 + 0x1c))
                      (DAT_0045dbd8,local_28,local_24,param_1,&local_18,0x10);
  (**(code **)(*param_1 + 0x1c))(param_1,local_18,local_14,DAT_0045dbd4,&local_28,0x11);
  *param_2 = local_2c;
  *param_3 = local_34;
  *param_4 = local_28;
  param_4[1] = local_24;
  param_4[2] = local_20;
  param_4[3] = local_1c;
  return;
}
