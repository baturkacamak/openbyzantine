// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43b070
// name: FUN_0043b070
// ranges: 0x43b070-0x43b0a8
// calls: 0x40ae42, 0x43b070, 0x44a820

LPVOID __thiscall FUN_0043b070(void *this,byte param_1)

{
  FUN_0040ae42((int)this);
  if ((param_1 & 1) != 0) {
    FUN_0044a820(this);
  }
  return this;
}
