// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a4bc
// name: FUN_0040a4bc
// ranges: 0x40a4bc-0x40a573
// calls: 0x40a4bc, 0x40aa42, 0x4599c0

void __cdecl FUN_0040a4bc(int param_1,uint param_2,char param_3,char param_4)

{
  uint local_10 [3];
  
  if (param_3 == '\x01') {
    FUN_004599c0(&DAT_0045d574);
  }
  FUN_004599c0(&DAT_0045d578);
  if (param_4 == '\x01') {
    FUN_0040aa42(local_10,(uint)*(ushort *)
                                 (&DAT_0047e2f0 +
                                 (uint)*(ushort *)
                                        (&DAT_0046ee00 +
                                        (uint)*(ushort *)(param_1 + 10 + (param_2 & 0xffff) * 4) * 2
                                        + (uint)DAT_00482428 * 600) * 2));
    FUN_004599c0((byte *)s____s__0045d57c);
  }
  return;
}
