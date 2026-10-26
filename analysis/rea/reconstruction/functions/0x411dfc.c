// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x411dfc
// name: FUN_00411dfc
// ranges: 0x411dfc-0x412205
// calls: 0x411b4b, 0x411db1, 0x411dfc, 0x44ac40, 0x44ba00, 0x44bc20, 0x44c2f0, 0x44c580

uint __cdecl FUN_00411dfc(int param_1,uint *param_2,ushort *param_3)

{
  ushort uVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  char *pcVar6;
  int local_110;
  FILE *local_10c;
  uint local_108 [65];
  
  local_110 = -1;
  for (uVar1 = 0; uVar1 < 0x15e; uVar1 = uVar1 + 1) {
    if (*(int *)(param_1 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(param_1 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(param_1 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(param_1 + (uint)uVar1 * 4));
      *(undefined4 *)(param_1 + (uint)uVar1 * 4) = 0;
    }
  }
  *param_3 = 0;
  if (DAT_0045e7a8 == 0) {
    _sprintf((char *)local_108,s_engine__s_0045ea6c,param_2);
    local_10c = (FILE *)FUN_0044bc20((char *)local_108,&DAT_0045ea78);
    if (local_10c == (FILE *)0x0) {
      return 0;
    }
  }
  else {
    local_10c = FUN_00411b4b(param_2,&local_110);
    if (local_10c == (FILE *)0x0) {
      return 0;
    }
  }
  uVar1 = *param_3;
  while (uVar3 = FUN_00411db1(&local_10c->_ptr,local_110), uVar3 == 0) {
    uVar2 = 0xffff;
    do {
      uVar2 = uVar2 + 1;
      iVar4 = _fgetc(local_10c);
      *(char *)((int)local_108 + (uint)uVar2) = (char)iVar4;
      if (*(char *)((int)local_108 + (uint)uVar2) == '\0') break;
      uVar3 = FUN_00411db1(&local_10c->_ptr,local_110);
    } while (uVar3 == 0);
    uVar3 = FUN_00411db1(&local_10c->_ptr,local_110);
    if (uVar3 == 0) {
      if (uVar1 == 0x15e) break;
      uVar5 = FUN_0044c2f0(0xc);
      *(undefined4 *)(param_1 + (uint)uVar1 * 4) = uVar5;
      pcVar6 = FUN_0044c580(local_108);
      uVar5 = FUN_0044c2f0((SIZE_T)(pcVar6 + 1));
      **(undefined4 **)(param_1 + (uint)uVar1 * 4) = uVar5;
      FUN_0044ba00((uint *)**(undefined4 **)(param_1 + (uint)uVar1 * 4),local_108);
      uVar2 = 0xffff;
      do {
        uVar2 = uVar2 + 1;
        iVar4 = _fgetc(local_10c);
        *(char *)((int)local_108 + (uint)uVar2) = (char)iVar4;
        if (*(char *)((int)local_108 + (uint)uVar2) == '\0') break;
        uVar3 = FUN_00411db1(&local_10c->_ptr,local_110);
      } while (uVar3 == 0);
      pcVar6 = FUN_0044c580(local_108);
      uVar5 = FUN_0044c2f0((SIZE_T)(pcVar6 + 1));
      *(undefined4 *)(*(int *)(param_1 + (uint)uVar1 * 4) + 4) = uVar5;
      FUN_0044ba00(*(uint **)(*(int *)(param_1 + (uint)uVar1 * 4) + 4),local_108);
      iVar4 = _fgetc(local_10c);
      *(char *)(*(int *)(param_1 + (uint)uVar1 * 4) + 8) = (char)iVar4;
      *(undefined1 *)(*(int *)(param_1 + (uint)uVar1 * 4) + 9) = 0;
      uVar1 = uVar1 + 1;
    }
  }
  uVar3 = _fclose(local_10c);
  *param_3 = uVar1;
  return uVar3 & 0xffff0000;
}
