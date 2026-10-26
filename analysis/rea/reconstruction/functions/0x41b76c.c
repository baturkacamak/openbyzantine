// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41b76c
// name: FUN_0041b76c
// ranges: 0x41b76c-0x41b7bd
// calls: 0x41b76c

void __thiscall FUN_0041b76c(void *this,int *param_1)

{
  *(int *)((int)this + 0x2c) = *param_1;
  *(int *)((int)this + 0x30) = param_1[1];
  *(int *)((int)this + 0x34) = param_1[2] - *param_1;
  *(int *)((int)this + 0x38) = param_1[3] - param_1[1];
  return;
}
