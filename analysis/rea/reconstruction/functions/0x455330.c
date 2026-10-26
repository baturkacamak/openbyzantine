// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x455330
// name: FUN_00455330
// ranges: 0x455330-0x4553bf
// calls: 0x455330

undefined4 __cdecl FUN_00455330(uint param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  DWORD nStdHandle;
  
  if (param_1 < DAT_004a7710) {
    piVar1 = (int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7) >> 3));
    iVar2 = (param_1 & 0x1f) * 8;
    piVar3 = (int *)(*piVar1 + iVar2);
    if (((*(byte *)(piVar3 + 1) & 1) != 0) && (*piVar3 != -1)) {
      if (DAT_00463194 == 1) {
        if (param_1 == 0) {
          nStdHandle = 0xfffffff6;
        }
        else if (param_1 == 1) {
          nStdHandle = 0xfffffff5;
        }
        else {
          if (param_1 != 2) goto LAB_00455396;
          nStdHandle = 0xfffffff4;
        }
        SetStdHandle(nStdHandle,(HANDLE)0x0);
      }
LAB_00455396:
      *(undefined4 *)(*piVar1 + iVar2) = 0xffffffff;
      return 0;
    }
  }
  DAT_00462c74 = 9;
  DAT_00462c78 = 0;
  return 0xffffffff;
}
