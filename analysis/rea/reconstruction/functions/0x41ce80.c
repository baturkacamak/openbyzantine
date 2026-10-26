// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41ce80
// name: FUN_0041ce80
// ranges: 0x41ce80-0x41ceb8
// calls: 0x41c36f, 0x41ce80, 0x44a820

int * __thiscall FUN_0041ce80(void *this,byte param_1)

{
  FUN_0041c36f(this);
  if ((param_1 & 1) != 0) {
    FUN_0044a820(this);
  }
  return this;
}
