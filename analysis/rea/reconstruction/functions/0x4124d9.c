// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4124d9
// name: FUN_004124d9
// ranges: 0x4124d9-0x412761
// calls: 0x40abb4, 0x40f350, 0x411b4b, 0x411db1, 0x4124d9, 0x44ba00, 0x44c2f0, 0x44c580

undefined4 FUN_004124d9(void)

{
  ushort uVar1;
  ushort uVar2;
  FILE *pFVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  char *pcVar7;
  undefined2 extraout_var;
  int local_1c;
  uint local_18 [5];
  
  local_1c = -1;
  if (DAT_0045e7a8 == 0) {
    _sprintf((char *)local_18,s_engine__s_0045ea8c,&DAT_00483060);
    pFVar3 = (FILE *)FUN_0040f350((char *)local_18,&DAT_0045ea98);
  }
  else {
    pFVar3 = FUN_00411b4b((uint *)&DAT_00483060,&local_1c);
  }
  if (pFVar3 == (FILE *)0x0) {
    uVar5 = 1;
  }
  else {
    __fread_lk((char *)&DAT_0047a8a0,2,1,pFVar3);
    _fclose(pFVar3);
    if (DAT_0045e7a8 == 0) {
      _sprintf((char *)local_18,s_engine__s_0045ea9c,&DAT_00483610);
      pFVar3 = (FILE *)FUN_0040f350((char *)local_18,&DAT_0045eaa8);
    }
    else {
      pFVar3 = FUN_00411b4b((uint *)&DAT_00483610,&local_1c);
    }
    if (pFVar3 == (FILE *)0x0) {
      uVar5 = 1;
    }
    else {
      uVar2 = 0;
      while( true ) {
        uVar4 = FUN_00411db1(&pFVar3->_ptr,local_1c);
        if ((uVar4 != 0) || (DAT_0047a8a0 <= uVar2)) break;
        uVar5 = FUN_0044c2f0(8);
        *(undefined4 *)(&DAT_0047ae80 + (uint)uVar2 * 4) = uVar5;
        uVar1 = 0xffff;
        do {
          uVar1 = uVar1 + 1;
          iVar6 = _fgetc(pFVar3);
          *(char *)((int)local_18 + (uint)uVar1) = (char)iVar6;
        } while (*(char *)((int)local_18 + (uint)uVar1) != '\0');
        pcVar7 = FUN_0044c580(local_18);
        uVar5 = FUN_0044c2f0((SIZE_T)(pcVar7 + 1));
        **(undefined4 **)(&DAT_0047ae80 + (uint)uVar2 * 4) = uVar5;
        FUN_0044ba00((uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar2 * 4),local_18);
        iVar6 = _fgetc(pFVar3);
        *(char *)(*(int *)(&DAT_0047ae80 + (uint)uVar2 * 4) + 4) = (char)iVar6;
        iVar6 = _fgetc(pFVar3);
        *(char *)(*(int *)(&DAT_0047ae80 + (uint)uVar2 * 4) + 5) = (char)iVar6;
        uVar2 = uVar2 + 1;
      }
      _fclose(pFVar3);
      if (DAT_0047a8a0 == uVar2) {
        uVar5 = 0;
      }
      else {
        FUN_0040abb4();
        uVar5 = CONCAT22(extraout_var,1);
      }
    }
  }
  return uVar5;
}
