// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43a84d
// name: FUN_0043a84d
// ranges: 0x43a84d-0x43aa16
// calls: 0x40b263, 0x40b445, 0x43a84d, 0x43ac17, 0x43ad1e, 0x43adce, 0x43af73

undefined4 __fastcall FUN_0043a84d(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  LONG LVar4;
  undefined4 local_8;
  
  iVar1 = FUN_0040b263(param_1[1]);
  if (iVar1 == 0) {
    do {
      if ((int)param_1[0x20] <= param_1[0x1e] + 1) goto LAB_0043aa00;
      iVar1 = FUN_0040b445(param_1[1]);
      if (iVar1 + -0x10 < -0x10) {
        InterlockedExchange(param_1 + 8,1);
      }
      else {
        iVar1 = AVIStreamTimeToSample(param_1[10],iVar1 + -0x10);
        if ((int)param_1[0x1e] < iVar1) {
          iVar3 = FUN_0043ad1e(param_1,iVar1,&local_8);
          if (((iVar3 == 0) && (iVar3 = FUN_0043adce(param_1,0,local_8), iVar3 == 0)) &&
             (iVar3 = FUN_0043af73(param_1,1), iVar3 == 0)) {
            param_1[0x21] = param_1[0x21] + 1;
          }
          param_1[0x1d] = iVar1;
        }
        param_1[0x1e] = param_1[0x1d];
      }
      LVar4 = InterlockedExchange(param_1 + 8,0);
    } while (LVar4 == 0);
    InterlockedExchange(param_1 + 8,1);
LAB_0043aa00:
    FUN_0043ac17(param_1);
    uVar2 = 0;
  }
  else {
    FUN_0043ac17(param_1);
    uVar2 = 0xfffffff9;
  }
  return uVar2;
}
