// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4206d6
// name: FUN_004206d6
// ranges: 0x4206d6-0x4207a5
// calls: 0x41f065, 0x41f279, 0x41f41c, 0x4202ff, 0x42044d, 0x4206d6

void FUN_004206d6(void)

{
  int iVar1;
  
  iVar1 = DAT_0046c884;
  FUN_0041f065(0);
  if ((iVar1 < DAT_0046c884) && (DAT_0046c5eb == '\x01')) {
    for (; (DAT_0045f7cc * 5 + 5) * 2 < DAT_0046c884; DAT_0045f7cc = DAT_0045f7cc + 1) {
    }
  }
  else if ((DAT_0046c884 < iVar1) && ((DAT_0046c884 < DAT_0045f7cc * 10 + 1 && (0 < DAT_0045f7cc))))
  {
    DAT_0045f7cc = DAT_0045f7cc + -1;
  }
  FUN_0041f41c();
  DAT_0045f7d0 = 0xffffffff;
  FUN_0041f279(-1);
  FUN_0042044d(1);
  FUN_004202ff(0x29,1);
  return;
}
