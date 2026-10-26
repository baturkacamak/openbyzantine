// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a615
// name: FUN_0040a615
// ranges: 0x40a615-0x40a6a4
// calls: 0x409457, 0x4094f2, 0x40a40d, 0x40a470, 0x40a4bc, 0x40a574, 0x40a615

void __cdecl FUN_0040a615(undefined4 param_1,int param_2,char param_3,char param_4)

{
  if (DAT_00485c6c != '\0') {
    FUN_00409457(param_1,'\x01');
    FUN_0040a4bc(param_2,0,'\0','\x01');
    FUN_0040a40d(param_2,1,'\x01');
    if (param_4 == '\x01') {
      FUN_0040a470((byte)*(undefined2 *)(param_2 + 0x12),'\x01');
    }
    else {
      FUN_0040a574(param_2,2,'\x01',param_3);
    }
    FUN_004094f2();
  }
  return;
}
