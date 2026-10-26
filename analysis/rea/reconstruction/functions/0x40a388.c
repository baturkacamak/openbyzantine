// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a388
// name: FUN_0040a388
// ranges: 0x40a388-0x40a40c
// calls: 0x40a388, 0x40aa02, 0x42db22, 0x436652, 0x4599c0

void __cdecl FUN_0040a388(int param_1,byte param_2,char param_3)

{
  ushort uVar1;
  char cVar2;
  
  if (param_3 == '\x01') {
    FUN_004599c0(&DAT_0045d54c);
  }
  uVar1 = FUN_00436652(param_1,param_2);
  if (*(int *)(&DAT_004654e0 + (uint)uVar1 * 4) == param_1) {
    FUN_004599c0((byte *)s_this_instruction_0045d550);
  }
  else {
    FUN_0040aa02();
    cVar2 = '\x01';
    uVar1 = FUN_00436652(param_1,param_2);
    FUN_0042db22((int *)(&DAT_004654e0 + (uint)uVar1 * 4),cVar2);
  }
  return;
}
