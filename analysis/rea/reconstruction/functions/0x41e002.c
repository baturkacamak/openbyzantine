// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41e002
// name: FUN_0041e002
// ranges: 0x41e002-0x41e288
// calls: 0x40f135, 0x40f98c, 0x410669, 0x411dfc, 0x41842b, 0x41df43, 0x41e002, 0x42b2db, 0x432d6e

void FUN_0041e002(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  int local_12c [4];
  uint local_11c;
  undefined2 local_118;
  ushort *local_114;
  undefined2 local_110;
  undefined4 local_8;
  
  uVar2 = DAT_00464878;
  uVar1 = DAT_0045c060;
  local_12c[0] = 0x280;
  local_12c[1] = 0x1e0;
  local_8 = DAT_0045f534;
  local_12c[3] = DAT_0045f540;
  local_118 = DAT_00482428;
  local_12c[2] = 0;
  do {
    if (DAT_00482528 <= (uint)local_12c[2]) {
LAB_0041e0b9:
      if (local_12c[2] != DAT_00482528) {
        local_114 = FUN_0040f135(s_setupart_hints_jpg_0045f964,local_12c,(uint *)(local_12c + 1),
                                 DAT_0047a8a4,0x280);
        FUN_0042b2db(DAT_0046e82c,DAT_0047a8a4,0x4b000);
        FUN_0042b2db(DAT_00477344,DAT_0047a8a4,0x4b000);
        if (local_114 != (ushort *)0x0) {
          uVar4 = FUN_00411dfc(0x483080,(uint *)s_hints_cv_0045f978,(ushort *)&DAT_0046e862);
          if ((uVar4 & 0xffff) == 0) {
            iVar3 = FUN_00410669(0xff,&local_11c,(uint *)s_hints1_0045f984);
            if (iVar3 == 0) {
              DAT_00482428 = local_110;
              DAT_0045f494 = 1;
              DAT_00464878 = 0xffffffff;
              DAT_0045c060 = 0;
              DAT_0045f534 = 0;
              DAT_0045f540 = 1;
              DAT_0045fdb8 = 8;
              FUN_0041842b((int)DAT_0046e82c,0x4b000);
              FUN_00432d6e(-1,DAT_00482ecc,0);
              FUN_00411dfc(0x483080,(uint *)&DAT_0047fa60,(ushort *)&DAT_0046e862);
              DAT_00482428 = local_118;
              DAT_0045f494 = 0;
              DAT_0045f534 = local_8;
              DAT_0045f540 = local_12c[3];
              DAT_0045fdb8 = 3;
              DAT_0045c060 = uVar1;
              DAT_00464878 = uVar2;
              FUN_0041df43();
              FUN_0040f98c(0xff);
            }
          }
          else {
            FUN_00411dfc(0x483080,(uint *)&DAT_0047fa60,(ushort *)&DAT_0046e862);
          }
        }
      }
      return;
    }
    iVar3 = __strcmpi(&DAT_00483bc0 + local_12c[2] * 9,s_hints_0045f95c);
    if (iVar3 == 0) {
      local_110 = (undefined2)local_12c[2];
      goto LAB_0041e0b9;
    }
    local_12c[2] = local_12c[2] + 1;
  } while( true );
}
