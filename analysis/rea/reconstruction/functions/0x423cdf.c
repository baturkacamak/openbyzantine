// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x423cdf
// name: FUN_00423cdf
// ranges: 0x423cdf-0x423d9d
// calls: 0x423c42, 0x423cdf, 0x44bd80, 0x44bd8a, 0x44d431
// port: Padding extent formula [bounded]

ulonglong __fastcall FUN_00423cdf(void *param_1)

{
  int iVar1;
  int iVar2;
  double dVar3;
  float10 fVar4;
  float10 fVar5;
  float10 extraout_ST0;
  ulonglong uVar6;
  undefined4 local_1c;
  undefined4 uStack_18;
  
  fVar4 = (float10)FUN_00423c42(param_1,1.0);
  dVar3 = *(double *)((int)param_1 + 0x28);
  iVar1 = *(int *)((int)param_1 + 4);
  iVar2 = *(int *)((int)param_1 + 8);
  uStack_18 = (int)((ulonglong)(double)fVar4 >> 0x20);
  local_1c = SUB84((double)fVar4,0);
  fVar4 = (float10)FUN_0044bd8a();
  fVar5 = (float10)FUN_0044bd80(local_1c,uStack_18);
  if (DAT_00462ae8 == 0) {
    fVar4 = (float10)(iVar1 >> 1) /
            ((float10)(double)(fVar4 * (float10)dVar3) - fVar5 * (float10)(iVar2 >> 1));
  }
  else {
    __adj_fdivr_m32i();
    fVar4 = extraout_ST0;
  }
  FUN_0044d431(SUB84((double)fVar4,0),(int)((ulonglong)(double)fVar4 >> 0x20));
  uVar6 = __ftol();
  return uVar6;
}
