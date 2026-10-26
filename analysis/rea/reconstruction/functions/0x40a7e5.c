// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a7e5
// name: FUN_0040a7e5
// ranges: 0x40a7e5-0x40a896
// calls: 0x409457, 0x40a7e5, 0x40a897, 0x40aa02, 0x424780, 0x436652, 0x44c580, 0x4599c0

void __cdecl FUN_0040a7e5(undefined4 param_1,int *param_2,int param_3)

{
  ushort uVar1;
  char *pcVar2;
  byte bVar3;
  
  if (DAT_00485c6c == '\0') {
    FUN_00424780((int)param_2,param_3);
  }
  else {
    FUN_00409457(&DAT_0045d5b0,'\0');
    FUN_00436652(*param_2,2);
    FUN_004599c0((byte *)s___s__s__0045d5b4);
    bVar3 = 0;
    uVar1 = FUN_00436652(*param_2,2);
    pcVar2 = FUN_0044c580((uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar1 * 4));
    FUN_0040a897(param_2[param_3 + 0x1c8],(char)pcVar2 + 2,bVar3);
    FUN_0040aa02();
  }
  return;
}
