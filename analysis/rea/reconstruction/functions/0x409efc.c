// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409efc
// name: FUN_00409efc
// ranges: 0x409efc-0x409f8c
// calls: 0x4094f2, 0x409efc, 0x40a1e0, 0x40a40d, 0x4599c0

void __cdecl FUN_00409efc(int param_1)

{
  int local_8;
  
  FUN_0040a1e0(param_1,s_ANIMRATE_0045d4a0);
  FUN_0040a40d(param_1,1,'\x01');
  for (local_8 = 2; local_8 < 8; local_8 = local_8 + 1) {
    if (*(char *)(local_8 + 1 + param_1) == '\x02') {
      FUN_004599c0(&DAT_0045d4ac);
    }
  }
  FUN_004094f2();
  return;
}
