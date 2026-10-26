// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x410607
// name: FUN_00410607
// ranges: 0x410607-0x410668
// calls: 0x410607, 0x410669

uint FUN_00410607(void)

{
  int in_EAX;
  int iVar1;
  uint local_10;
  uint local_c;
  uint local_8;
  
  local_8 = local_8 & 0xffff0000;
  for (local_10 = 0; (int)local_10 < 0x15e; local_10 = local_10 + 1) {
    iVar1 = FUN_00410669(local_10,&local_c,(uint *)&DAT_0045e8dc);
    in_EAX = iVar1 + (local_8 & 0xffff);
    local_8 = CONCAT22(local_8._2_2_,(short)in_EAX);
  }
  return CONCAT22((short)((uint)in_EAX >> 0x10),(undefined2)local_8);
}
