// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4221b1
// name: FUN_004221b1
// ranges: 0x4221b1-0x422317
// calls: 0x4221b1, 0x44ba00, 0x44bc20, 0x44c2f0, 0x44c580, 0x44ce20

int __cdecl FUN_004221b1(char *param_1,int param_2,int param_3)

{
  int iVar1;
  FILE *_File;
  char *pcVar2;
  undefined4 uVar3;
  int local_814;
  undefined4 local_808;
  uint local_804 [512];
  
  local_808 = 0;
  for (local_814 = 0; local_814 < param_3; local_814 = local_814 + 1) {
    *(undefined4 *)(param_2 + local_814 * 4) = 0;
  }
  _File = (FILE *)FUN_0044bc20(param_1,&DAT_0045fcf8);
  if (_File == (FILE *)0x0) {
    local_808 = 0;
  }
  else {
    do {
      if ((_File->_flag & 0x10) != 0) break;
      FUN_0044ce20((char *)local_804,0x800,_File);
      pcVar2 = FUN_0044c580(local_804);
      if (*(char *)((int)local_804 + (int)(pcVar2 + -1)) != '\n') break;
      *(char *)((int)local_804 + (int)(pcVar2 + -1)) = '\0';
      uVar3 = FUN_0044c2f0((SIZE_T)pcVar2);
      *(undefined4 *)(param_2 + local_808 * 4) = uVar3;
      iVar1 = local_808 * 4;
      local_808 = local_808 + 1;
      FUN_0044ba00(*(uint **)(param_2 + iVar1),local_804);
    } while (local_808 != param_3);
    _fclose(_File);
  }
  return local_808;
}
