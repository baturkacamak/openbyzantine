// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4075fe
// name: FUN_004075fe
// ranges: 0x4075fe-0x4076b3
// calls: 0x4075a1, 0x4075fe, 0x4181c5

void __cdecl FUN_004075fe(int param_1)

{
  int local_28 [4];
  int *local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_18 = DAT_00486888;
  local_14 = 0;
  local_10 = 0;
  local_c = 0x280;
  local_8 = 0x3c;
  if ((DAT_0045d1c0 == 0) && (param_1 != 0)) {
    FUN_004075a1(param_1);
  }
  else {
    (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0,0x1a4,DAT_0045d1b0,&local_14,0x10);
    if (param_1 != 0) {
      local_28[0] = 0;
      local_28[1] = 0x1a4;
      local_28[2] = 0x280;
      local_28[3] = 0x1e0;
      FUN_004181c5(local_18,local_28,0,0x1a4);
    }
  }
  return;
}
