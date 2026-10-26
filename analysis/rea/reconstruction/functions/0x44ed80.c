// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44ed80
// name: FUN_0044ed80
// ranges: 0x44ed80-0x44ee2f
// calls: 0x44ed80

void __cdecl FUN_0044ed80(double *param_1,char *param_2,size_t param_3,int param_4)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  char *pcVar4;
  STRFLT unaff_EBP;
  bool bVar5;
  
  DAT_0046e0e0 = (int *)__fltout();
  DAT_0046323c = DAT_0046e0e0[1] + -1;
  iVar1 = *DAT_0046e0e0;
  __fptostr(param_2 + (iVar1 == 0x2d),param_3,(int)DAT_0046e0e0,unaff_EBP);
  DAT_00463240 = DAT_0046323c < DAT_0046e0e0[1] + -1;
  iVar3 = DAT_0046e0e0[1] + -1;
  if ((-5 < iVar3) && (iVar3 < (int)param_3)) {
    bVar5 = DAT_0046323c < DAT_0046e0e0[1] + -1;
    pcVar2 = param_2 + (iVar1 == 0x2d);
    DAT_0046323c = iVar3;
    if (bVar5) {
      do {
        pcVar4 = pcVar2;
        pcVar2 = pcVar4 + 1;
      } while (*pcVar4 != '\0');
      pcVar4[-1] = '\0';
    }
    __cftof_g(param_1,param_2,param_3);
    return;
  }
  DAT_0046323c = iVar3;
  __cftoe_g(param_1,param_2,param_3,param_4);
  return;
}
