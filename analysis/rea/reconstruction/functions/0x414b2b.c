// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x414b2b
// name: FUN_00414b2b
// ranges: 0x414b2b-0x414b88
// calls: 0x414b2b, 0x41a878

void __thiscall FUN_00414b2b(void *this,int param_1)

{
  undefined4 local_8;
  
  if (param_1 == 0) {
    local_8 = 0;
  }
  else if (param_1 == 2) {
    local_8 = 1;
  }
  else {
    local_8 = 2;
  }
  FUN_0041a878(*(int **)((int)this + 8),local_8);
  return;
}
