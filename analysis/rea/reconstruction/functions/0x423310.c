// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x423310
// name: FUN_00423310
// ranges: 0x423310-0x423625
// calls: 0x423310, 0x44ac60, 0x44bd8a, 0x44d431
// port: Projection tables [partial]

undefined4 * __thiscall FUN_00423310(void *this,int *param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  float10 fVar2;
  float10 extraout_ST0;
  float10 extraout_ST0_00;
  undefined4 local_88 [2];
  undefined4 local_80;
  undefined4 local_7c;
  undefined2 local_2c;
  int local_1c;
  double local_18;
  uint local_10;
  undefined4 local_c;
  uint local_8;
  
  *(undefined4 *)((int)this + 0x1c) = 0;
  *(undefined4 *)((int)this + 0x20) = 0;
  *(int **)this = param_1;
  *(undefined4 *)((int)this + 0x34) = 0;
  *(undefined4 *)((int)this + 0x3c) = 0;
  _memset(local_88,0,0x6c);
  local_88[0] = 0x6c;
  local_c = (**(code **)(*param_1 + 0x58))(param_1,local_88);
  if (param_2 == (undefined4 *)0x0) {
    *(undefined4 *)((int)this + 0xc) = 0;
    *(undefined4 *)((int)this + 0x14) = local_7c;
    *(undefined4 *)((int)this + 0x10) = 0;
    *(undefined4 *)((int)this + 0x18) = local_80;
  }
  else {
    *(undefined4 *)((int)this + 0xc) = *param_2;
    *(undefined4 *)((int)this + 0x10) = param_2[1];
    *(undefined4 *)((int)this + 0x14) = param_2[2];
    *(undefined4 *)((int)this + 0x18) = param_2[3];
  }
  *(int *)((int)this + 4) = *(int *)((int)this + 0x14) - *(int *)((int)this + 0xc);
  *(int *)((int)this + 8) = *(int *)((int)this + 0x18) - *(int *)((int)this + 0x10);
  *(undefined2 *)((int)this + 0x30) = local_2c;
  uVar1 = FUN_0044ac60(*(int *)((int)this + 4) * *(int *)((int)this + 8) * 4);
  *(undefined4 *)((int)this + 0x3c) = uVar1;
  local_8 = 0xc45;
  uVar1 = FUN_0044ac60(0x3114);
  *(undefined4 *)((int)this + 0x40) = uVar1;
  *(uint *)((int)this + 0x44) = (local_8 & 0xfffffffe) * 2 + *(int *)((int)this + 0x40);
  for (local_1c = 0; local_1c < (int)local_8 >> 1; local_1c = local_1c + 1) {
    if (DAT_00462ae8 == 0) {
      fVar2 = (float10)local_1c / (float10)1000.0;
    }
    else {
      __adj_fdiv_m64(0,0x408f4000);
      fVar2 = extraout_ST0;
    }
    local_18 = (double)fVar2;
    fVar2 = (float10)FUN_0044bd8a();
    *(float *)(*(int *)((int)this + 0x44) + local_1c * 4) = (float)fVar2;
    fVar2 = (float10)FUN_0044bd8a();
    *(float *)(local_1c * -4 + *(int *)((int)this + 0x44)) = (float)fVar2;
  }
  local_10 = 4000;
  uVar1 = FUN_0044ac60(16000);
  *(undefined4 *)((int)this + 0x48) = uVar1;
  *(uint *)((int)this + 0x4c) = (local_10 & 0xfffffffe) * 2 + *(int *)((int)this + 0x48);
  for (local_1c = 0; local_1c < (int)local_10 >> 1; local_1c = local_1c + 1) {
    if (DAT_00462ae8 == 0) {
      fVar2 = (float10)local_1c / (float10)1000.0;
    }
    else {
      __adj_fdiv_m64(0,0x408f4000);
      fVar2 = extraout_ST0_00;
    }
    local_18 = (double)fVar2;
    fVar2 = (float10)FUN_0044d431(local_18._0_4_,(int)((ulonglong)local_18 >> 0x20));
    *(float *)(*(int *)((int)this + 0x4c) + local_1c * 4) = (float)fVar2;
    fVar2 = (float10)FUN_0044d431(SUB84(-local_18,0),(int)((ulonglong)-local_18 >> 0x20));
    *(float *)(local_1c * -4 + *(int *)((int)this + 0x4c)) = (float)fVar2;
  }
  return this;
}
