// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4240ae
// name: FUN_004240ae
// ranges: 0x4240ae-0x4241e1
// calls: 0x4240ae

undefined4 __thiscall FUN_004240ae(void *this,int *param_1,int *param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  longlong lVar4;
  
  if (*(int *)((int)this + 0x34) == 0) {
    uVar2 = 1;
  }
  else if ((param_2 == (int *)0x0) || (param_1 == (int *)0x0)) {
    uVar2 = 1;
  }
  else {
    iVar3 = *param_1;
    iVar1 = param_1[1];
    if ((iVar3 < *(int *)((int)this + 4)) &&
       (((-1 < iVar3 && (iVar1 < *(int *)((int)this + 8))) && (-1 < iVar1)))) {
      lVar4 = __ftol();
      iVar3 = *(int *)(*(int *)((int)this + 0x3c) + (*(int *)((int)this + 4) * iVar1 + iVar3) * 4) +
              (int)lVar4;
      *param_2 = iVar3 % *(int *)((int)this + 0x1c) - *(int *)((int)this + 0x38);
      while (*(int *)((int)this + 0x24) <= *param_2) {
        *param_2 = *param_2 - *(int *)((int)this + 0x24);
      }
      while (*param_2 < 0) {
        *param_2 = *param_2 + *(int *)((int)this + 0x24);
      }
      param_2[1] = iVar3 / *(int *)((int)this + 0x1c);
      uVar2 = 0;
    }
    else {
      uVar2 = 1;
    }
  }
  return uVar2;
}
