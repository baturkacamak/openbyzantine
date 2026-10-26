// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42062a
// name: FUN_0042062a
// ranges: 0x42062a-0x420682
// calls: 0x41f41c, 0x4202ff, 0x42044d, 0x420587, 0x42062a

void __cdecl FUN_0042062a(int param_1)

{
  DAT_0045f7d0 = 0xffffffff;
  if (param_1 == 0x1c) {
    DAT_0045f7cc = DAT_0045f7cc + 1;
  }
  else {
    DAT_0045f7cc = DAT_0045f7cc + -1;
  }
  FUN_0042044d(1);
  FUN_00420587(param_1,1);
  FUN_0041f41c();
  FUN_004202ff(0x29,1);
  return;
}
