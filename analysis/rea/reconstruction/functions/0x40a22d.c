// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a22d
// name: FUN_0040a22d
// ranges: 0x40a22d-0x40a2c2
// calls: 0x409457, 0x4094f2, 0x40a22d, 0x4599c0

void __cdecl FUN_0040a22d(int param_1,undefined4 param_2,undefined4 param_3)

{
  int local_8;
  
  FUN_00409457(param_3,'\x01');
  for (local_8 = 0; local_8 < 8; local_8 = local_8 + 1) {
    if (*(char *)(local_8 + 1 + param_1) == '\x02') {
      if (local_8 != 0) {
        FUN_004599c0(&DAT_0045d534);
      }
      FUN_004599c0(&DAT_0045d538);
    }
  }
  FUN_004094f2();
  return;
}
