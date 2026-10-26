// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40d750
// name: FUN_0040d750
// ranges: 0x40d750-0x40d808
// calls: 0x40d750

undefined4 FUN_0040d750(void)

{
  undefined4 uVar1;
  
  if (DAT_0045d1d4 == -1) {
    if ((DAT_0045d1c0 == 0) || (DAT_00486a0c < 0x1a4)) {
      if ((DAT_0045f20c == 0) && (DAT_0045f208 == 0)) {
        if ((DAT_00486a0c < 0x14) || (0x1a3 < DAT_00486a0c)) {
          if (DAT_004867f0 == 0) {
            uVar1 = 0;
          }
          else {
            uVar1 = 1;
          }
        }
        else {
          uVar1 = 1;
        }
      }
      else {
        uVar1 = 4;
      }
    }
    else {
      uVar1 = 2;
    }
  }
  else {
    uVar1 = 3;
  }
  return uVar1;
}
