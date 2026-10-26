// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a40d
// name: FUN_0040a40d
// ranges: 0x40a40d-0x40a46f
// calls: 0x40a40d, 0x40aa42, 0x436652, 0x4599c0

void __cdecl FUN_0040a40d(int param_1,byte param_2,char param_3)

{
  ushort uVar1;
  uint local_10 [3];
  
  uVar1 = FUN_00436652(param_1,param_2);
  FUN_0040aa42(local_10,(uint)uVar1);
  if (param_3 == '\x01') {
    FUN_004599c0(&DAT_0045d564);
  }
  FUN_004599c0(&DAT_0045d568);
  return;
}
