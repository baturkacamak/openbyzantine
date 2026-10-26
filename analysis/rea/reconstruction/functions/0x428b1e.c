// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x428b1e
// name: FUN_00428b1e
// ranges: 0x428b1e-0x428cbc
// calls: 0x428b1e

int __cdecl FUN_00428b1e(int param_1,int param_2)

{
  float fVar1;
  bool bVar2;
  int iVar3;
  float10 fVar4;
  float10 extraout_ST0;
  float10 extraout_ST0_00;
  longlong lVar5;
  int local_1c;
  int local_10;
  
  iVar3 = param_1;
  local_10 = 6000;
  if (param_1 == 0) {
    local_10 = 10000;
  }
  else if (param_2 == 0) {
    local_10 = -10000;
  }
  else {
    bVar2 = param_2 < param_1;
    if (bVar2) {
      param_1 = param_2;
      param_2 = iVar3;
    }
    if (DAT_00462ae8 == 0) {
      fVar4 = (float10)param_1 / (float10)param_2;
    }
    else {
      __adj_fdiv_m32i();
      fVar4 = extraout_ST0;
    }
    fVar1 = (float)(fVar4 * (float10)100.0);
    for (local_1c = 0; local_1c < 5; local_1c = local_1c + 1) {
      if ((fVar1 <= (float)*(int *)(&DAT_004601c0 + local_1c * 4)) &&
         ((float)*(int *)(&DAT_004601c4 + local_1c * 4) <= fVar1)) {
        if (DAT_00462ae8 == 0) {
          fVar4 = ((float10)*(int *)(&DAT_004601c0 + local_1c * 4) - (float10)fVar1) /
                  (float10)(*(int *)(&DAT_004601c0 + local_1c * 4) -
                           *(int *)(&DAT_004601c4 + local_1c * 4));
        }
        else {
          __adj_fdiv_m32i();
          fVar4 = extraout_ST0_00;
        }
        _floor((double)((float)(*(int *)(&DAT_004601ac + local_1c * 4) -
                               *(int *)(&DAT_004601a8 + local_1c * 4)) * (float)fVar4 +
                        (float)*(int *)(&DAT_004601a8 + local_1c * 4) + 0.5));
        lVar5 = __ftol();
        local_10 = (int)lVar5;
        break;
      }
    }
    if (bVar2) {
      local_10 = -local_10;
    }
  }
  return local_10;
}
