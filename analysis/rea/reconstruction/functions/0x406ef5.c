// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x406ef5
// name: FUN_00406ef5
// ranges: 0x406ef5-0x406f98
// calls: 0x405db2, 0x405e7e, 0x406ef5

void FUN_00406ef5(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  
  bVar1 = DAT_00485c80 & 0x1f;
  bVar2 = DAT_00485c78 & 0x1f;
  bVar3 = DAT_00485c80 & 0x1f;
  bVar4 = DAT_00485c78 & 0x1f;
  DAT_00468cb8 = FUN_00405db2(s_fonts_white_sfn_0045d188);
  FUN_00405e7e((int)DAT_00468cb8,5,(ushort)(0xe << bVar1) | (ushort)(0xf << bVar2) | 0xe,0);
  DAT_00468cbc = FUN_00405db2(s_fonts_yellow_sfn_0045d198);
  FUN_00405e7e((int)DAT_00468cbc,6,(ushort)(0xe << bVar3) | (ushort)(0xe << bVar4) | 0xe,0);
  return;
}
