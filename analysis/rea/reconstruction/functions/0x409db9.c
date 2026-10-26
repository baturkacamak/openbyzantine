// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409db9
// name: FUN_00409db9
// ranges: 0x409db9-0x409e39
// calls: 0x4094f2, 0x409db9, 0x40a1e0, 0x4599c0

void __cdecl FUN_00409db9(int param_1)

{
  int local_8;
  
  FUN_0040a1e0(param_1,s_ANIMPOS_0045d474);
  for (local_8 = 1; local_8 < 8; local_8 = local_8 + 1) {
    if (*(char *)(local_8 + 1 + param_1) == '\x02') {
      FUN_004599c0(&DAT_0045d47c);
    }
  }
  FUN_004094f2();
  return;
}
