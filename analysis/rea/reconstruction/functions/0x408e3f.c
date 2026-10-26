// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408e3f
// name: FUN_00408e3f
// ranges: 0x408e3f-0x408ee1
// calls: 0x408e3f, 0x416710, 0x4200bb

undefined4 FUN_00408e3f(void)

{
  undefined4 uVar1;
  int iVar2;
  
  if (DAT_0045d1b8 == 0) {
    iVar2 = FUN_00416710(s_setupart_top_bar_tga_0045d2e8,&DAT_0045d1b8,0,(uint *)0x0,(uint *)0x0);
    if (iVar2 == 0) {
      iVar2 = FUN_004200bb(0x468ce8,s_setupart_hotbar_rle_0045d300);
      if (iVar2 == 5) {
        iVar2 = FUN_004200bb(0x468d60,s_setupart_dwnbar_rle_0045d314);
        if (iVar2 == 5) {
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
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
