// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4201cc
// name: FUN_004201cc
// ranges: 0x4201cc-0x42024c
// calls: 0x4200bb, 0x4201cc

undefined4 FUN_004201cc(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_004200bb(0x46c620,s_setupart_roll_rle_0045fbe0);
  if (iVar1 == 0xf) {
    iVar1 = FUN_004200bb(0x46ca60,s_setupart_on_off_rle_0045fbf4);
    if (iVar1 == 0xd) {
      iVar1 = FUN_004200bb(0x46c7f0,s_setupart_yellow_rle_0045fc08);
      if (iVar1 == 4) {
        uVar2 = 0;
      }
      else {
        uVar2 = 1;
      }
    }
    else {
      uVar2 = 1;
    }
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}
