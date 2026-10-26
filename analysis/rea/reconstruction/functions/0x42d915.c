// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42d915
// name: FUN_0042d915
// ranges: 0x42d915-0x42da3c; 0x42da42-0x42da61; 0x42dab5-0x42dad8
// calls: 0x40ac38, 0x40ac43, 0x42d915, 0x436652, 0x437c72

ushort FUN_0042d915(void)

{
  ushort uVar1;
  ushort local_8;
  
  switch(*(undefined1 *)(DAT_0047a230 + 1)) {
  case 2:
    uVar1 = FUN_00436652(DAT_0047a230,0);
    local_8 = *(ushort *)
               (&DAT_0047e2f0 +
               (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar1 * 2 + (uint)DAT_00482428 * 600) * 2);
    break;
  case 8:
    local_8 = *(short *)(DAT_0047a230 + 10) * 0x100 + *(short *)(DAT_0047a230 + 0xc) + 0x80;
    break;
  case 0x10:
    local_8 = *(short *)(DAT_0047a230 + 10) * 0x100 + *(short *)(DAT_0047a230 + 0xc) + 0x40;
    break;
  case 0x40:
    local_8 = FUN_00436652(DAT_0047a230,0);
    if (2 < local_8) {
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 0x8001;
    }
  }
  local_8 = local_8 | 0x8000;
  FUN_00437c72();
  return local_8;
}
