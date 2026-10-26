// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x438afa
// name: FUN_00438afa
// ranges: 0x438afa-0x438c7f
// calls: 0x438afa

undefined4 FUN_00438afa(void)

{
  char *_Str1;
  int iVar1;
  undefined4 uVar2;
  
  _Str1 = &DAT_00483bc0 + (uint)DAT_00482428 * 9;
  iVar1 = __strcmpi(_Str1,&DAT_004617b4);
  if (iVar1 == 0) {
    uVar2 = 5;
  }
  else {
    iVar1 = __strcmpi(_Str1,&DAT_004617b8);
    if (iVar1 == 0) {
      uVar2 = 4;
    }
    else {
      iVar1 = __strcmpi(_Str1,&DAT_004617bc);
      if ((iVar1 != 0) && (iVar1 = __strcmpi(_Str1,s_hstmed_004617c0), iVar1 != 0)) {
        iVar1 = __strcmpi(_Str1,&DAT_004617c8);
        if ((iVar1 == 0) ||
           ((iVar1 = __strcmpi(_Str1,s_lintel_004617cc), iVar1 == 0 ||
            (iVar1 = __strcmpi(_Str1,s_morris_004617d4), iVar1 == 0)))) {
          return 3;
        }
        iVar1 = __strcmpi(_Str1,&DAT_004617dc);
        if ((iVar1 != 0) &&
           ((((iVar1 = __strcmpi(_Str1,s_clock_004617e0), iVar1 != 0 &&
              (iVar1 = __strcmpi(_Str1,s_voice_004617e8), iVar1 != 0)) &&
             (iVar1 = __strcmpi(_Str1,&DAT_004617f0), iVar1 != 0)) &&
            (iVar1 = __strcmpi(_Str1,&DAT_004617f8), iVar1 != 0)))) {
          return 0;
        }
        return 1;
      }
      uVar2 = 2;
    }
  }
  return uVar2;
}
