// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43dc46
// name: FUN_0043dc46
// ranges: 0x43dc46-0x43dd95
// calls: 0x4161f6, 0x43dc46

undefined4 __cdecl FUN_0043dc46(undefined4 *param_1)

{
  undefined4 local_48 [2];
  undefined4 local_40;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_20;
  int local_1c;
  undefined2 local_18;
  undefined2 local_16;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  local_34 = FUN_004161f6();
  _memset(&local_18,0,0x12);
  local_18 = 1;
  local_16 = 2;
  local_14 = 0x5622;
  local_c = 0x100004;
  local_10 = 0x15888;
  _memset(&local_30,0,0x14);
  local_30 = 0x14;
  local_2c = 1;
  local_28 = 0;
  local_20 = 0;
  local_1c = (**(code **)(*DAT_00461934 + 0x18))(DAT_00461934,local_34,2);
  if (((local_1c == 0) &&
      (local_1c = (**(code **)(*DAT_00461934 + 0xc))(DAT_00461934,&local_30,&DAT_00461938,0),
      local_1c == 0)) &&
     (local_1c = (**(code **)(*DAT_00461938 + 0x38))(DAT_00461938,&local_18), local_1c == 0)) {
    local_48[0] = 0x14;
    (**(code **)(*DAT_00461938 + 0xc))(DAT_00461938,local_48);
    *param_1 = local_40;
    return 1;
  }
  if (DAT_00461938 != (int *)0x0) {
    (**(code **)(*DAT_00461938 + 8))(DAT_00461938);
    DAT_00461938 = (int *)0x0;
  }
  *param_1 = 0;
  return 0;
}
