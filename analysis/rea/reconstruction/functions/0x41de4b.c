// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41de4b
// name: FUN_0041de4b
// ranges: 0x41de4b-0x41dec2; 0x41dec8-0x41dede; 0x41deef-0x41df42
// calls: 0x41de4b

undefined4 FUN_0041de4b(void)

{
  undefined4 uVar1;
  
  if (DAT_0045f498 == 0) {
    switch(DAT_0045d1d4) {
    case 1:
      uVar1 = 1;
      break;
    case 2:
      uVar1 = 3;
      break;
    case 3:
      uVar1 = 2;
      break;
    case 4:
      uVar1 = 0;
      break;
    default:
      if (DAT_0045f490 == 2) {
        uVar1 = 2;
      }
      else if (DAT_0045f490 == 0) {
        if (DAT_0045f058 == 0) {
          uVar1 = 7;
        }
        else {
          uVar1 = 6;
        }
      }
      else {
        uVar1 = 1;
      }
    }
  }
  else {
    DAT_0045f490 = (uint)(DAT_0045f7e0 == 0);
    uVar1 = 2;
  }
  return uVar1;
}
