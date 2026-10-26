// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a6aa
// name: FUN_0042a6aa
// ranges: 0x42a6aa-0x42a766
// calls: 0x42913b, 0x429af7, 0x42a579, 0x42a6aa, 0x44d060, 0x44d4d0

uint __cdecl FUN_0042a6aa(short param_1,undefined2 *param_2,undefined2 *param_3)

{
  undefined2 extraout_var;
  uint uVar1;
  undefined4 uVar2;
  undefined2 extraout_var_00;
  undefined4 local_10;
  int local_c;
  undefined2 local_8 [2];
  
  if (param_1 == 0x20) {
    FUN_00429af7();
    uVar1 = CONCAT22(extraout_var,0x20);
  }
  else if (param_1 == 0x1e) {
    uVar2 = FUN_0044d4d0((int *)0x0);
    FUN_0044d060(uVar2);
    _rand();
    FUN_0042913b();
    DAT_0046026c = 0;
    uVar1 = CONCAT22(extraout_var_00,0x1e);
  }
  else {
    local_c = FUN_0042a579((int)param_1,(uint *)local_8,&local_10);
    *param_2 = local_8[0];
    *param_3 = (short)local_10;
    if ((local_c == 0) && (DAT_0046026c = DAT_0046026c + 1, DAT_0046026c == 9)) {
      local_c = 3;
    }
    uVar1 = CONCAT22((short)((uint)local_10 >> 0x10),(undefined2)local_c);
  }
  return uVar1;
}
