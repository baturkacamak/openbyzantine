// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41b6c7
// name: FUN_0041b6c7
// ranges: 0x41b6c7-0x41b718
// calls: 0x41b6c7

void __thiscall FUN_0041b6c7(void *this,int *param_1)

{
  *(int *)((int)this + 0x1c) = *param_1;
  *(int *)((int)this + 0x20) = param_1[1];
  *(int *)((int)this + 0x24) = param_1[2] - *param_1;
  *(int *)((int)this + 0x28) = param_1[3] - param_1[1];
  return;
}
