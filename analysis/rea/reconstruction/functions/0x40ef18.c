// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ef18
// name: FUN_0040ef18
// ranges: 0x40ef18-0x40f074
// calls: 0x40ede0, 0x40ef18, 0x44a460, 0x44ba00, 0x44bc20

INT_PTR __cdecl FUN_0040ef18(HWND param_1)

{
  FILE *pFVar1;
  INT_PTR IVar2;
  
  pFVar1 = (FILE *)FUN_0044bc20(s_drives_btb_0045e770,&DAT_0045e76c);
  if (pFVar1 != (FILE *)0x0) {
    __fread_lk(&DAT_0045e764,1,1,pFVar1);
    __fread_lk(&DAT_0045e768,1,1,pFVar1);
    _fclose(pFVar1);
  }
  FUN_0044ba00((uint *)&DAT_0046c470,(uint *)s_Columbus1_0045e77c);
  DAT_00486a08 = DAT_0045e764;
  IVar2 = DialogBoxParamA((HINSTANCE)0x0,(LPCSTR)0x6f,param_1,FUN_0040ede0,0);
  if (IVar2 == 0) {
    IVar2 = 0;
  }
  else {
    DAT_0045e764 = DAT_00486a08;
    FUN_0044ba00((uint *)&DAT_0046c470,(uint *)s_Columbus2_0045e788);
    DAT_00486a08 = DAT_0045e768;
    IVar2 = DialogBoxParamA((HINSTANCE)0x0,(LPCSTR)0x6f,param_1,FUN_0040ede0,0);
    if (IVar2 != 0) {
      DAT_0045e768 = DAT_00486a08;
    }
    pFVar1 = (FILE *)FUN_0044bc20(s_drives_btb_0045e798,&DAT_0045e794);
    if (pFVar1 != (FILE *)0x0) {
      FUN_0044a460(&DAT_0045e764,1,1,pFVar1);
      FUN_0044a460(&DAT_0045e768,1,1,pFVar1);
      _fclose(pFVar1);
    }
  }
  return IVar2;
}
