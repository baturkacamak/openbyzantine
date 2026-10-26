// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a8a0
// name: FUN_0042a8a0
// ranges: 0x42a8a0-0x42a8ff
// calls: 0x42a8a0

void __thiscall FUN_0042a8a0(void *this,uint *param_1)

{
  *param_1 = *(uint *)this & 0xff;
  param_1[1] = *(uint *)this >> 8 & 0xff;
  param_1[2] = *(uint *)this >> 0x10 & 0xff;
  param_1[3] = *(uint *)this >> 0x18;
  return;
}
