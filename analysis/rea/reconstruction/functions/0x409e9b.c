// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409e9b
// name: FUN_00409e9b
// ranges: 0x409e9b-0x409efb
// calls: 0x4094f2, 0x409e9b, 0x40a1e0, 0x40a40d, 0x40a4bc

void __cdecl FUN_00409e9b(int param_1)

{
  FUN_0040a1e0(param_1,s_ANIMPRIO_0045d490);
  if (*(char *)(param_1 + 2) == '@') {
    FUN_0040a40d(param_1,2,'\x01');
  }
  else {
    FUN_0040a4bc(param_1,1,'\x01','\x01');
  }
  FUN_004094f2();
  return;
}
