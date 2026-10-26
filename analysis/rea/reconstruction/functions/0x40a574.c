// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a574
// name: FUN_0040a574
// ranges: 0x40a574-0x40a614
// calls: 0x40a470, 0x40a574, 0x40aa42, 0x4599c0

void __cdecl FUN_0040a574(int param_1,byte param_2,char param_3,char param_4)

{
  uint local_10 [3];
  
  FUN_0040aa42(local_10,(uint)*(ushort *)(param_1 + 10 + (uint)param_2 * 4));
  if (param_3 == '\x01') {
    FUN_004599c0(&DAT_0045d584);
  }
  FUN_004599c0(&DAT_0045d588);
  if (param_4 == '\x01') {
    FUN_0040a470((byte)*(undefined2 *)(param_1 + 0xc + (uint)param_2 * 4),'\0');
  }
  else {
    FUN_004599c0(&DAT_0045d58c);
  }
  return;
}
