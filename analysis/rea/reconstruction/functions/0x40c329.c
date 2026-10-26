// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40c329
// name: FUN_0040c329
// ranges: 0x40c329-0x40c38c; 0x40c392-0x40c5ef
// calls: 0x407bd7, 0x407c5a, 0x4083d4, 0x408f32, 0x408fe7, 0x40c02c, 0x40c329, 0x40df05, 0x41624a, 0x4177c7

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_0040c329(void)

{
  bool bVar1;
  ushort uVar2;
  uint uVar3;
  undefined2 extraout_var_00;
  undefined2 extraout_var_01;
  undefined2 extraout_var_02;
  undefined3 extraout_var;
  undefined2 extraout_var_03;
  uint local_8;
  
  FUN_0041624a();
  uVar3 = FUN_0040c02c();
  local_8 = uVar3 & 0xffff;
  if ((uVar3 & 0xffff) == 0xfffc) {
    if (_DAT_00476330 == 0xfffc) {
      uVar3 = FUN_00408fe7();
    }
    else {
      FUN_00408f32(1);
      uVar3 = uVar3 & 0xffff;
      _DAT_00476330 = uVar3;
    }
    uVar3 = CONCAT22((short)(uVar3 >> 0x10),0xfffe);
  }
  else {
    if (_DAT_00476330 == 0xfffc) {
      FUN_00408f32(0);
      _DAT_00476330 = 0xfffe;
    }
    if ((uVar3 & 0xffff) == 0xfffb) {
      FUN_0040df05(0xb);
      uVar3 = CONCAT22(extraout_var_00,0xfffe);
    }
    else if ((uVar3 & 0xffff) == 0xfffd) {
      FUN_004083d4();
      uVar3 = CONCAT22(extraout_var_01,0xfffe);
    }
    else if ((uVar3 & 0xffff) == 0xffff) {
      FUN_0040df05(DAT_0045da80);
      uVar3 = CONCAT22(extraout_var_02,DAT_00476330);
    }
    else {
      if (DAT_004603d4 != -1) {
        FUN_004177c7((char *)0x0);
      }
      if ((uVar3 & 0xffff) == 0xfffe) {
        uVar3 = FUN_00407bd7(0);
        uVar2 = (ushort)uVar3;
      }
      else if (DAT_0047a8a2 == '\0') {
        uVar2 = (ushort)*(byte *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9);
        DAT_004603d4 = *(int *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 0x14);
        bVar1 = FUN_00407c5a(*(int *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 0x10),
                             *(char *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9));
        if (DAT_0045d1c8 != 0) {
          if (CONCAT31(extraout_var,bVar1) == 0) {
            uVar3 = FUN_00407bd7(0);
            uVar2 = (ushort)uVar3;
          }
          else if (((*(char *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9) == '\x12') ||
                   (*(char *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9) == '\x13')) ||
                  (*(char *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9) == '\x14')) {
            uVar3 = FUN_00407bd7(1);
            uVar2 = (ushort)uVar3;
          }
          else if (uVar2 == 0) {
            uVar3 = FUN_00407bd7(0);
            uVar2 = (ushort)uVar3;
          }
        }
        if ((CONCAT31(extraout_var,bVar1) == 0) && (uVar2 != 0x14)) {
          local_8 = 0xfffe;
        }
      }
      else {
        uVar2 = (short)uVar3 + 0xe;
      }
      FUN_0040df05(uVar2);
      _DAT_00476330 = local_8;
      uVar3 = CONCAT22(extraout_var_03,(undefined2)local_8);
      DAT_0045da80 = uVar2;
    }
  }
  return uVar3;
}
