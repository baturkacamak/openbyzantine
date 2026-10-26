// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409e3a
// name: FUN_00409e3a
// ranges: 0x409e3a-0x409e9a
// calls: 0x4094f2, 0x409e3a, 0x40a1e0, 0x40a40d, 0x40a4bc

void __cdecl FUN_00409e3a(int param_1)

{
  FUN_0040a1e0(param_1,s_ANIMSET_0045d484);
  if (*(char *)(param_1 + 2) == '@') {
    FUN_0040a40d(param_1,2,'\x01');
  }
  else {
    FUN_0040a4bc(param_1,1,'\x01','\x01');
  }
  FUN_004094f2();
  return;
}
