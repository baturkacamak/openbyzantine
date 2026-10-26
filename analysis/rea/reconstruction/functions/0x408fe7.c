// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408fe7
// name: FUN_00408fe7
// ranges: 0x408fe7-0x4090c3
// calls: 0x408f32, 0x408fe7, 0x4090c4, 0x40df05

void FUN_00408fe7(void)

{
  int local_20;
  int local_1c;
  int local_18 [5];
  
  local_18[0] = 0;
  local_18[1] = 0xb5;
  local_18[2] = 0x144;
  local_18[3] = 500;
  local_18[4] = 0x280;
  local_20 = (int)DAT_00486a0a;
  if (0x27f < local_20) {
    local_20 = 0x27f;
  }
  if (DAT_0045d1d0 != 0) {
    for (local_1c = 0; (local_1c < 5 && (local_18[local_1c] <= local_20)); local_1c = local_1c + 1)
    {
    }
    if (local_1c != DAT_0045d1d4) {
      FUN_00408f32(1);
      DAT_0045d1d4 = local_1c;
      FUN_004090c4(local_1c,1);
      FUN_0040df05(0x1c);
      DAT_00483050 = 0xffff;
    }
  }
  return;
}
