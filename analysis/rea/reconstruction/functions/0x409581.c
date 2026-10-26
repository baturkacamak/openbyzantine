// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409581
// name: FUN_00409581
// ranges: 0x409581-0x40963b; 0x409641-0x409660; 0x4096b4-0x4096c5
// calls: 0x409457, 0x4094f2, 0x409581, 0x40a40d, 0x40a470, 0x40a4bc, 0x4599c0

void __cdecl FUN_00409581(int param_1)

{
  FUN_00409457(s_RETURN_0045d3c4,'\x01');
  switch(*(undefined1 *)(param_1 + 1)) {
  case 2:
    FUN_0040a4bc(param_1,0,'\0','\x01');
    break;
  case 8:
    FUN_0040a470((byte)*(undefined2 *)(param_1 + 10),'\0');
    FUN_004599c0(&DAT_0045d3d0);
    break;
  case 0x10:
    FUN_0040a470((byte)*(undefined2 *)(param_1 + 10),'\0');
    FUN_004599c0(&DAT_0045d3cc);
    FUN_0040a470((byte)*(undefined2 *)(param_1 + 0xc),'\0');
    break;
  case 0x40:
    FUN_0040a40d(param_1,0,'\0');
  }
  FUN_004094f2();
  return;
}
