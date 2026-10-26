// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x423e03
// name: FUN_00423e03
// ranges: 0x423e03-0x42409a; 0x4240a0-0x4240ad
// calls: 0x423e03, 0x44bd80, 0x44bd8a

undefined4 __thiscall FUN_00423e03(void *this,int *param_1,int *param_2)

{
  double *pdVar1;
  int iVar2;
  undefined4 uVar3;
  float10 fVar4;
  float10 extraout_ST0;
  longlong lVar5;
  undefined8 local_24;
  
  if (*(int *)((int)this + 0x34) == 0) {
    uVar3 = 0;
  }
  else if ((param_1 == (int *)0x0) || (param_2 == (int *)0x0)) {
    uVar3 = 0;
  }
  else if ((((*param_1 < *(int *)((int)this + 0x24)) && (-1 < *param_1)) &&
           (param_1[1] < *(int *)((int)this + 0x20))) && (-1 < param_1[1])) {
    pdVar1 = (double *)((int)this + 0x28);
    if (DAT_00462ae8 == 0) {
      fVar4 = ((float10)(*param_1 - (*(int *)((int)this + 0x24) >> 1)) -
              (float10)*(double *)((int)this + 0x50) * (float10)*pdVar1) / (float10)*pdVar1;
    }
    else {
      __adj_fdiv_m64(*(undefined4 *)pdVar1,*(uint *)((int)this + 0x2c));
      fVar4 = extraout_ST0;
    }
    for (local_24 = (double)fVar4; local_24 < -3.14159265359; local_24 = local_24 + 6.28318530718) {
    }
    for (; 3.14159265359 < local_24; local_24 = local_24 - 6.28318530718) {
    }
    if ((local_24 < -1.0471975511966667) || (1.0471975511966667 < local_24)) {
      uVar3 = 0;
    }
    else {
      FUN_0044bd8a();
      FUN_0044bd80((uint)local_24,local_24._4_4_);
      FUN_0044bd8a();
      FUN_0044bd80(*(uint *)((int)this + 0x58),*(int *)((int)this + 0x5c));
      if (DAT_00462ae8 != 0) {
        __adj_fdivr_m64(0,0x3ff00000);
      }
      iVar2 = *(int *)((int)this + 4);
      lVar5 = __ftol();
      *param_2 = (iVar2 >> 1) + (int)lVar5;
      iVar2 = *(int *)((int)this + 8);
      lVar5 = __ftol();
      param_2[1] = (iVar2 >> 1) + (int)lVar5;
      if (((*param_2 < 0) || (*(int *)((int)this + 4) <= *param_2)) ||
         ((param_2[1] < 0 || (*(int *)((int)this + 8) <= param_2[1])))) {
        uVar3 = 0;
      }
      else {
        uVar3 = 1;
      }
    }
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}
