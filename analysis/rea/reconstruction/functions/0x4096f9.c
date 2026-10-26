// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4096f9
// name: FUN_004096f9
// ranges: 0x4096f9-0x40976b; 0x409771-0x409784; 0x409795-0x409880
// calls: 0x409457, 0x4094f2, 0x4096f9, 0x40a897, 0x424780, 0x436652, 0x44ba00, 0x44ba08, 0x44c580, 0x4599c0

void __cdecl FUN_004096f9(int param_1,int param_2,undefined4 param_3)

{
  byte bVar1;
  ushort uVar2;
  char *pcVar3;
  byte bVar4;
  uint local_34 [12];
  
  if (DAT_00485c6c == '\0') {
    uVar2 = FUN_00436652(param_2,2);
    FUN_00424780(param_1,(uint)uVar2);
  }
  else {
    switch(param_3) {
    case 0:
      FUN_00409457(s_VOICE_0045d3dc,'\x01');
      break;
    case 1:
      FUN_00409457(s_XVOICE_0045d3e4,'\x01');
      break;
    case 2:
      FUN_00409457(s_YVOICE_0045d3ec,'\x01');
      break;
    case 3:
      FUN_00409457(s_ZVOICE_0045d3f4,'\x01');
    }
    uVar2 = FUN_00436652(param_2,0);
    FUN_0044ba00(local_34,(uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar2 * 4));
    FUN_0044ba08(local_34,(uint *)&DAT_0045d3fc);
    uVar2 = FUN_00436652(param_2,1);
    FUN_0044ba08(local_34,(uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar2 * 4));
    FUN_0044ba08(local_34,(uint *)&DAT_0045d400);
    FUN_004599c0(&DAT_0045d404);
    bVar4 = 0xc;
    pcVar3 = FUN_0044c580(local_34);
    bVar1 = (char)pcVar3 + 0xc;
    uVar2 = FUN_00436652(param_2,2);
    FUN_0040a897(*(int *)(param_1 + 0x720 + (uint)uVar2 * 4),bVar1,bVar4);
    FUN_004094f2();
  }
  return;
}
