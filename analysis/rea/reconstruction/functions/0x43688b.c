// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43688b
// name: FUN_0043688b
// ranges: 0x43688b-0x436b00
// calls: 0x40a7e5, 0x40aa42, 0x43340a, 0x4365fa, 0x43688b, 0x436b01, 0x437c72, 0x437ca2, 0x44ba08

uint __cdecl FUN_0043688b(int *param_1,uint param_2)

{
  ushort *puVar1;
  uint uVar2;
  uint local_1ac [52];
  ushort *local_dc;
  undefined2 local_d8;
  uint local_d4 [52];
  
  local_d8 = 0xffff;
  DAT_00483d82 = 0;
  if (*(char *)((int)param_1 + 10) == '\x01') {
    uVar2 = FUN_00436b01(param_1,param_2 & 0xffff);
  }
  else {
    FUN_00437ca2();
    FUN_004365fa();
    do {
      local_dc = (ushort *)param_1[*(ushort *)(param_1 + 2) + 0x131];
      *(short *)(param_1 + 2) = (short)param_1[2] + 1;
      if (local_dc != (ushort *)0x0) break;
    } while ((short)param_1[2] != 0x32);
    if ((local_dc == (ushort *)0x0) || ((short)param_1[2] == 0x32)) {
      *(undefined1 *)((int)param_1 + 0xe) = 1;
      uVar2 = CONCAT22((short)((uint)param_1 >> 0x10),local_d8);
    }
    else {
      *(undefined1 *)((int)param_1 + 10) = 1;
      *(short *)(param_1 + 2) = (short)param_1[2] + -1;
      if (param_1[*(ushort *)(param_1 + 2) + 0xf2] != 0) {
        local_dc = (ushort *)param_1[*(ushort *)(param_1 + 2) + 0xf2];
      }
      FUN_0040aa42(local_d4,(uint)*(ushort *)(param_1 + 2));
      FUN_0044ba08(local_d4,(uint *)&DAT_0046142c);
      FUN_0040aa42(local_1ac,(uint)*(byte *)(*(ushort *)(param_1 + 2) + 0x490 + (int)param_1));
      FUN_0044ba08(local_d4,local_1ac);
      FUN_0040a7e5(local_d4,param_1,(uint)*local_dc);
      puVar1 = local_dc;
      for (local_dc = *(ushort **)(local_dc + 6); local_dc != (ushort *)0x0;
          local_dc = *(ushort **)(local_dc + 6)) {
        FUN_0043340a(param_1,*(byte **)(local_dc + 2));
        if (DAT_0047a8d8 != '\0') {
          DAT_0047a8d8 = '\0';
        }
        DAT_00483d82 = 1;
        FUN_0040a7e5(local_d4,param_1,(uint)*local_dc);
      }
      FUN_0043340a(param_1,*(byte **)(puVar1 + 2));
      if (DAT_0047a8d8 != '\0') {
        DAT_0047a8d8 = '\0';
      }
      DAT_00483d82 = 1;
      if (*(char *)((int)param_1 + 10) == '\0') {
        param_1 = (int *)FUN_00437c72();
      }
      uVar2 = CONCAT22((short)((uint)param_1 >> 0x10),local_d8);
    }
  }
  return uVar2;
}
