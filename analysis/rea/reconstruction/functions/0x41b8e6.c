// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41b8e6
// name: FUN_0041b8e6
// ranges: 0x41b8e6-0x41b95f
// calls: 0x41b8e6

undefined4 __thiscall FUN_0041b8e6(void *this,int *param_1)

{
  undefined4 uVar1;
  
  if (*(int *)((int)this + 0xc) == 0x31347669) {
    *param_1 = *(int *)((int)this + 0x1a4);
    param_1[1] = *(int *)((int)this + 0x1a8);
    param_1[2] = *(int *)((int)this + 0x1ac) + *param_1;
    param_1[3] = *(int *)((int)this + 0x1b0) + param_1[1];
    uVar1 = 0;
  }
  else {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}
