// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x402074
// name: FUN_00402074
// ranges: 0x402074-0x402284
// calls: 0x402074, 0x402e70, 0x427c43, 0x427caa, 0x44bd80, 0x44bd8a, 0x44bdb0

void FUN_00402074(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  float10 fVar4;
  float10 extraout_ST0;
  float10 extraout_ST0_00;
  float10 fVar5;
  longlong lVar6;
  float local_28;
  int local_20;
  
  fVar4 = (float10)FUN_00402e70();
  local_28 = (float)(fVar4 + (float10)180.0);
  if (fVar4 + (float10)180.0 == (float10)360.0) {
    local_28 = 0.0;
  }
  for (local_20 = 0;
      (local_20 < 0x15e && (piVar1 = *(int **)(&DAT_00486220 + local_20 * 4), piVar1 != (int *)0x0))
      ; local_20 = local_20 + 1) {
    if (piVar1[0xc] != -1) {
      if (DAT_00462ae8 == 0) {
        fVar4 = (float10)piVar1[0xc] / (float10)DAT_0046487c;
      }
      else {
        __adj_fdiv_m32i();
        fVar4 = extraout_ST0;
      }
      if (DAT_00462ae8 == 0) {
        fVar4 = ((fVar4 * (float10)360.0 - (float10)local_28) * (float10)2.0 *
                (float10)3.14159265359) / (float10)360.0;
      }
      else {
        __adj_fdiv_m64(0,0x40768000);
        fVar4 = extraout_ST0_00;
      }
      fVar5 = (float10)FUN_0044bd8a();
      _floor((double)(fVar5 * (float10)40.0 + (float10)0.5));
      lVar6 = __ftol();
      iVar3 = FUN_0044bdb0((uint)lVar6);
      fVar4 = (float10)FUN_0044bd80(SUB84((double)(float)fVar4,0),
                                    (int)((ulonglong)(double)(float)fVar4 >> 0x20));
      if (fVar4 < (float10)0.0) {
        piVar1[9] = iVar3 + 10;
        piVar1[8] = 100 - piVar1[9];
      }
      else {
        piVar1[8] = iVar3 + 10;
        piVar1[9] = 100 - piVar1[8];
      }
      FUN_00427caa(piVar1);
      if ((int)(uint)lVar6 < 0) {
        iVar2 = piVar1[7];
        piVar1[7] = piVar1[7] + iVar3 * -10;
        FUN_00427c43(piVar1);
        piVar1[7] = iVar2;
      }
      else {
        FUN_00427c43(piVar1);
      }
    }
  }
  return;
}
