// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x416ec0
// name: FUN_00416ec0
// ranges: 0x416ec0-0x416fb7
// calls: 0x416710, 0x416ec0, 0x4177c7, 0x417a17, 0x417e9c, 0x4200bb

void FUN_00416ec0(void)

{
  if (DAT_0045f218 == 0) {
    FUN_00416710(s_setupart_goggles_tga_0045f244,&DAT_0045f218,1,(uint *)0x0,(uint *)0x0);
  }
  if (DAT_0045f21c == 0) {
    FUN_00416710(s_setupart_gogexith_tga_0045f25c,&DAT_0045f21c,1,&DAT_0046c530,&DAT_0046c518);
  }
  if (DAT_0046c520 == 0) {
    FUN_004200bb(0x46c520,s_setupart_detail_rle_0045f274);
  }
  FUN_00417e9c();
  if (DAT_0046c538 == 0) {
    FUN_004200bb(0x46c538,s_setupart_cmps_x_rle_0045f288);
  }
  if (DAT_0046c5b8 == 0) {
    FUN_004200bb(0x46c5b8,s_setupart_cmps_v_rle_0045f29c);
  }
  FUN_004177c7((char *)0x0);
  FUN_00417a17(DAT_0045f214,0);
  return;
}
