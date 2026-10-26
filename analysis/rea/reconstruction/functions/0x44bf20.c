// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44bf20
// name: FUN_0044bf20
// ranges: 0x44bf20-0x44bf8a
// calls: 0x44bf20

int __cdecl FUN_0044bf20(int param_1)

{
  FILE *_File;
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = 0;
  iVar4 = 0;
  iVar3 = 0;
  do {
    _File = *(FILE **)(DAT_004a782c + iVar4);
    if ((_File != (FILE *)0x0) && ((_File->_flag & 0x83U) != 0)) {
      if (param_1 == 1) {
        iVar1 = _fflush(_File);
        if (iVar1 != -1) {
          iVar2 = iVar2 + 1;
        }
      }
      else if ((param_1 == 0) && ((_File->_flag & 2U) != 0)) {
        iVar1 = _fflush(_File);
        if (iVar1 == -1) {
          iVar3 = -1;
        }
      }
    }
    iVar4 = iVar4 + 4;
  } while (iVar4 < 0x800);
  if (param_1 != 1) {
    iVar2 = iVar3;
  }
  return iVar2;
}
