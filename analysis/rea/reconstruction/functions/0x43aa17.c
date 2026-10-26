// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43aa17
// name: FUN_0043aa17
// ranges: 0x43aa17-0x43ac16
// calls: 0x40b263, 0x40b445, 0x40b912, 0x43aa17, 0x43ac17, 0x43ad1e, 0x43adce, 0x43af73

undefined4 __fastcall FUN_0043aa17(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  LONG LVar5;
  int local_90;
  undefined4 local_8;
  
  iVar1 = FUN_0040b263(param_1[1]);
  if (iVar1 == 0) {
    do {
      if ((int)param_1[0x20] <= param_1[0x1f] + 1) goto LAB_0043ac00;
      iVar1 = param_1[0x1f];
      iVar3 = iVar1 + 1;
      uVar2 = FUN_0040b445(param_1[1]);
      uVar2 = AVIStreamTimeToSample(param_1[10],uVar2);
      param_1[0x1d] = uVar2;
      if (param_1[0x1d] == iVar3) {
        iVar4 = FUN_0043ad1e(param_1,param_1[0x1d],&local_8);
        if (((iVar4 == 0) && (iVar4 = FUN_0043adce(param_1,0,local_8), iVar4 == 0)) &&
           (iVar4 = FUN_0043af73(param_1,1), iVar4 == 0)) {
          param_1[0x21] = param_1[0x21] + 1;
        }
        iVar4 = FUN_0040b912(param_1[1]);
        local_90 = iVar3;
        if (iVar4 == 0) {
          local_90 = iVar1;
        }
      }
      else {
        local_90 = iVar1;
        if (iVar3 < (int)param_1[0x1d]) {
          param_1[0x22] = param_1[0x22] + 1;
          iVar4 = FUN_0040b912(param_1[1]);
          local_90 = iVar3;
          if (iVar4 == 0) {
            local_90 = iVar1;
          }
        }
      }
      param_1[0x1f] = local_90;
      LVar5 = InterlockedExchange(param_1 + 8,0);
    } while (LVar5 == 0);
    InterlockedExchange(param_1 + 8,1);
LAB_0043ac00:
    FUN_0043ac17(param_1);
    uVar2 = 0;
  }
  else {
    FUN_0043ac17(param_1);
    uVar2 = 0xfffffff9;
  }
  return uVar2;
}
