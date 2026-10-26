// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x423c42
// name: FUN_00423c42
// ranges: 0x423c42-0x423cde
// calls: 0x423c42, 0x44ad87, 0x44d431
// port: Pitch-limit formula [bounded]

void __thiscall FUN_00423c42(void *this,double param_1)

{
  double dVar1;
  int iVar2;
  int iVar3;
  float10 fVar4;
  
  iVar2 = *(int *)((int)this + 8) >> 1;
  iVar3 = *(int *)((int)this + 0x20) >> 1;
  dVar1 = *(double *)((int)this + 0x28) * param_1;
  if (DAT_00462ae8 == 0) {
    fVar4 = ((float10)iVar3 * (float10)dVar1 -
            (float10)*(double *)((int)this + 0x28) * (float10)iVar2) /
            ((float10)*(double *)((int)this + 0x28) * (float10)dVar1 +
            (float10)iVar3 * (float10)iVar2);
  }
  else {
    fVar4 = FUN_0044ad87();
  }
  FUN_0044d431(SUB84((double)fVar4,0),(int)((ulonglong)(double)fVar4 >> 0x20));
  return;
}
