// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426b9d
// name: FUN_00426b9d
// ranges: 0x426b9d-0x426ecf
// calls: 0x40ac38, 0x40ac43, 0x40cf73, 0x40fe70, 0x4269c6, 0x426b9d, 0x426ed0, 0x44ac40, 0x44c2f0

int __cdecl FUN_00426b9d(FILE *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int local_20;
  uint local_1c;
  char *local_18;
  char *local_14;
  char local_10;
  undefined2 *local_c;
  int local_8;
  
  local_8 = 0;
  local_14 = (char *)FUN_0044c2f0(0x40);
  local_1c = 0;
  do {
    if (0x15d < (int)local_1c) {
LAB_00426eb7:
      FUN_0044ac40(local_14);
      return local_8;
    }
    iVar1 = _fgetc(param_1);
    local_10 = (char)iVar1;
    if (local_10 != -1) {
      if (local_10 == '\x01') {
        FUN_0040fe70(local_1c,(int *)(&DAT_004649e8 + local_1c * 4));
      }
      local_c = *(undefined2 **)(&DAT_004649e8 + local_1c * 4);
      if (local_c == (undefined2 *)0x0) {
        if (*(int *)(&DAT_0047dd70 + local_1c * 4) == 0) {
          FUN_0040ac43();
        }
        else {
          FUN_0040ac43();
        }
        FUN_0040ac38();
        local_8 = 1;
        goto LAB_00426eb7;
      }
      __fread_lk(local_14,0x40,1,param_1);
      *local_c = *(undefined2 *)local_14;
      local_c[1] = *(undefined2 *)(local_14 + 2);
      local_c[2] = *(undefined2 *)(local_14 + 4);
      local_c[3] = *(undefined2 *)(local_14 + 6);
      *(char *)((int)local_c + 9) = local_14[9];
      *(char *)(local_c + 4) = local_14[8];
      local_c[5] = *(undefined2 *)(local_14 + 10);
      local_c[6] = *(undefined2 *)(local_14 + 0xc);
      local_c[7] = *(undefined2 *)(local_14 + 0xe);
      local_c[8] = *(undefined2 *)(local_14 + 0x10);
      local_c[9] = *(undefined2 *)(local_14 + 0x12);
      local_c[10] = *(undefined2 *)(local_14 + 0x14);
      local_c[0x12] = *(undefined2 *)(local_14 + 0x24);
      local_c[0x13] = *(undefined2 *)(local_14 + 0x26);
      *(char *)((int)local_c + 0x29) = local_14[0x29];
      __fread_lk((char *)&local_20,4,1,param_1);
      iVar1 = FUN_0040cf73((int)local_c,local_20);
      local_8 = local_8 + iVar1;
      __fread_lk((char *)&local_20,4,1,param_1);
      iVar1 = FUN_00426ed0((int)local_c,local_20);
      local_8 = local_8 + iVar1;
      iVar1 = FUN_004269c6((char *)(local_c + 0x16),param_1);
      local_8 = local_8 + iVar1;
      iVar1 = FUN_004269c6((char *)(local_c + 0x18),param_1);
      local_8 = local_8 + iVar1;
      *(undefined4 *)(local_c + 0x1a) = 0;
      iVar1 = _fgetc(param_1);
      local_10 = (char)iVar1;
      while (local_10 != '\0') {
        if (*(int *)(local_c + 0x1a) == 0) {
          uVar2 = FUN_0044c2f0(0xc);
          *(undefined4 *)(local_c + 0x1a) = uVar2;
          local_18 = *(char **)(local_c + 0x1a);
        }
        else {
          uVar2 = FUN_0044c2f0(0xc);
          *(undefined4 *)(local_18 + 8) = uVar2;
          local_18 = *(char **)(local_18 + 8);
        }
        iVar1 = FUN_004269c6(local_18,param_1);
        local_8 = local_8 + iVar1;
        __fread_lk(local_18 + 4,4,1,param_1);
        local_18[8] = '\0';
        local_18[9] = '\0';
        local_18[10] = '\0';
        local_18[0xb] = '\0';
      }
    }
    local_1c = local_1c + 1;
  } while( true );
}
