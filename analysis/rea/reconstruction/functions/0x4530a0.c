// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4530a0
// name: FUN_004530a0
// ranges: 0x4530a0-0x453112
// calls: 0x44ac40, 0x4530a0

int FUN_004530a0(void)

{
  FILE *_File;
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  iVar4 = 3;
  if (3 < DAT_004a7828) {
    iVar2 = 0xc;
    do {
      _File = *(FILE **)(DAT_004a782c + iVar2);
      if (_File != (FILE *)0x0) {
        if ((_File->_flag & 0x83U) != 0) {
          iVar1 = _fclose(_File);
          if (iVar1 != -1) {
            iVar3 = iVar3 + 1;
          }
        }
        if (0x4f < iVar2) {
          FUN_0044ac40(*(LPVOID *)(DAT_004a782c + iVar2));
          *(undefined4 *)(DAT_004a782c + iVar2) = 0;
        }
      }
      iVar2 = iVar2 + 4;
      iVar4 = iVar4 + 1;
    } while (iVar4 < DAT_004a7828);
  }
  return iVar3;
}
