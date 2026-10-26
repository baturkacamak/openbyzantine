// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450ed0
// name: __stbuf
// ranges: 0x450ed0-0x450f61
// calls: 0x44c2f0, 0x454380

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    __stbuf
   
   Library: Visual Studio 1998 Release */

int __cdecl __stbuf(FILE *_File)

{
  int *piVar1;
  char *pcVar2;
  byte bVar3;
  undefined3 extraout_var;
  int iVar4;
  
  bVar3 = FUN_00454380(_File->_file);
  if (CONCAT31(extraout_var,bVar3) == 0) {
    return 0;
  }
  if (_File == (FILE *)&DAT_00462f20) {
    iVar4 = 0;
  }
  else {
    if (_File != (FILE *)&DAT_00462f40) {
      return 0;
    }
    iVar4 = 1;
  }
  _DAT_00463180 = _DAT_00463180 + 1;
  if ((_File->_flag & 0x10cU) != 0) {
    return 0;
  }
  piVar1 = &DAT_004634b8 + iVar4;
  if (*piVar1 == 0) {
    iVar4 = FUN_0044c2f0(0x1000);
    *piVar1 = iVar4;
    if (iVar4 == 0) {
      return 0;
    }
  }
  pcVar2 = (char *)*piVar1;
  _File->_base = pcVar2;
  _File->_ptr = pcVar2;
  _File->_bufsiz = 0x1000;
  _File->_cnt = 0x1000;
  _File->_flag = _File->_flag | 0x1102;
  return 1;
}
