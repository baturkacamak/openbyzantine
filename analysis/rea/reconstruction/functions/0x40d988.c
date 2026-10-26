// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40d988
// name: FUN_0040d988
// ranges: 0x40d988-0x40d9f6
// calls: 0x40d88a, 0x40d988, 0x40e23e, 0x40e25f, 0x40e29f

void __cdecl FUN_0040d988(int *param_1)

{
  if (((*param_1 != (int)DAT_00486a0a) || (param_1[1] != (int)DAT_00486a0c)) && (DAT_0045dbd0 != 0))
  {
    DAT_0045dbd0 = 0;
    FUN_0040e25f();
    FUN_0040d88a();
    FUN_0040e29f();
    FUN_0040e23e();
    DAT_0045dbd0 = 1;
  }
  return;
}
