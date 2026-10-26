// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4236b5
// name: FUN_004236b5
// ranges: 0x4236b5-0x42382a
// calls: 0x4236b5, 0x42382b, 0x423cdf, 0x4241e2, 0x44a820, 0x44ac60
// port: Panorama geometry and source setup [partial]

undefined4 __thiscall FUN_004236b5(void *this,int param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  undefined4 extraout_EDX;
  float10 fVar2;
  float10 extraout_ST0;
  ulonglong uVar3;
  int local_8;
  
  if (param_1 == 0) {
    uVar1 = 1;
  }
  else {
    if ((*(int *)((int)this + 0x24) != param_2) || (*(int *)((int)this + 0x20) != param_3)) {
      if (*(int *)((int)this + 0x34) != 0) {
        FUN_0044a820(*(LPVOID *)((int)this + 0x34));
      }
      *(int *)((int)this + 0x24) = param_2;
      *(int *)((int)this + 0x20) = param_3;
      if (DAT_00462ae8 == 0) {
        fVar2 = (float10)param_2 / (float10)6.28318530718;
      }
      else {
        __adj_fdiv_m64(0x54442eea,0x401921fb);
        fVar2 = extraout_ST0;
      }
      *(double *)((int)this + 0x28) = (double)fVar2;
      uVar3 = FUN_00423cdf(this);
      *(int *)((int)this + 0x38) = (int)uVar3 + 1;
      *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x38) * 2 + param_2;
      uVar1 = FUN_0044ac60(*(int *)((int)this + 0x1c) * *(int *)((int)this + 0x20) * 2);
      *(undefined4 *)((int)this + 0x34) = uVar1;
      if (*(int *)((int)this + 0x34) == 0) {
        return 1;
      }
    }
    for (local_8 = 0; local_8 < *(int *)((int)this + 0x20); local_8 = local_8 + 1) {
      FID_conflict__memcpy
                ((void *)(*(int *)((int)this + 0x38) * 2 + *(int *)((int)this + 0x1c) * local_8 * 2
                         + *(int *)((int)this + 0x34)),(void *)(local_8 * param_2 * 2 + param_1),
                 param_2 * 2);
    }
    FUN_0042382b((int)this);
    FUN_004241e2((int)this,extraout_EDX);
    uVar1 = 0;
  }
  return uVar1;
}
