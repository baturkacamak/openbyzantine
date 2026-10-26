// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41b811
// name: FUN_0041b811
// ranges: 0x41b811-0x41b892
// calls: 0x41b811

void __thiscall FUN_0041b811(void *this,int *param_1)

{
  *(int *)((int)this + 0x3c) = *param_1;
  *(int *)((int)this + 0x40) = param_1[1];
  *(int *)((int)this + 0x44) = param_1[2] - *param_1;
  *(int *)((int)this + 0x48) = param_1[3] - param_1[1];
  *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)((int)this + 0x3c);
  *(undefined4 *)((int)this + 0x50) = *(undefined4 *)((int)this + 0x40);
  *(undefined4 *)((int)this + 0x54) = *(undefined4 *)((int)this + 0x44);
  *(undefined4 *)((int)this + 0x58) = *(undefined4 *)((int)this + 0x48);
  return;
}
