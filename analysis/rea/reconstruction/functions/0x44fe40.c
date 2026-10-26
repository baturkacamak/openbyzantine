// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44fe40
// name: __getstream
// ranges: 0x44fe40-0x44febd
// calls: 0x44c2f0

/* Library Function - Single Match
    __getstream
   
   Library: Visual Studio 1998 Release */

FILE * __cdecl __getstream(void)

{
  int iVar1;
  int *piVar2;
  FILE *pFVar3;
  int iVar4;
  
  pFVar3 = (FILE *)0x0;
  iVar4 = 0;
  piVar2 = DAT_004a782c;
  if (0 < DAT_004a7828) {
    do {
      if (*piVar2 == 0) {
        iVar1 = FUN_0044c2f0(0x20);
        DAT_004a782c[iVar4] = iVar1;
        if ((FILE *)DAT_004a782c[iVar4] != (FILE *)0x0) {
          pFVar3 = (FILE *)DAT_004a782c[iVar4];
        }
        break;
      }
      if ((*(uint *)(*piVar2 + 0xc) & 0x83) == 0) {
        pFVar3 = (FILE *)DAT_004a782c[iVar4];
        break;
      }
      iVar4 = iVar4 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar4 < DAT_004a7828);
  }
  if (pFVar3 != (FILE *)0x0) {
    pFVar3->_cnt = 0;
    pFVar3->_flag = 0;
    pFVar3->_base = (char *)0x0;
    pFVar3->_ptr = (char *)0x0;
    pFVar3->_tmpfname = (char *)0x0;
    pFVar3->_file = -1;
  }
  return pFVar3;
}
