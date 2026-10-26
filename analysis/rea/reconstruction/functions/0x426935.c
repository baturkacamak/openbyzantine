// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426935
// name: FUN_00426935
// ranges: 0x426935-0x42699f
// calls: 0x408f32, 0x40da1f, 0x426935, 0x431ec2, 0x432295

void FUN_00426935(void)

{
  if (DAT_0045c060 == 0) {
    if (*(int *)(&DAT_00484e80 + (uint)DAT_0048253e * 4) != 0) {
      FUN_00431ec2(DAT_0048253e,100,1);
    }
  }
  else {
    FUN_00432295(DAT_00480330,1,-1,100);
  }
  FUN_00408f32(0);
  FUN_0040da1f();
  return;
}
