// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408f32
// name: FUN_00408f32
// ranges: 0x408f32-0x408fe6
// calls: 0x408f32, 0x4181c5, 0x42b29f

void __cdecl FUN_00408f32(int param_1)

{
  int local_14 [4];
  
  local_14[0] = 0;
  local_14[1] = 0;
  local_14[2] = 0x280;
  local_14[3] = 0x14;
  if (DAT_0045f494 == 0) {
    if (param_1 == 1) {
      if (DAT_0045d1d0 == 0) {
        return;
      }
      (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0,0,DAT_0045d1b8,local_14,0x10);
    }
    else {
      FUN_0042b29f(DAT_0046e82c,0,0x3200);
    }
    FUN_004181c5(DAT_00486888,local_14,0,0);
    DAT_0045d1d4 = 0xffffffff;
  }
  return;
}
