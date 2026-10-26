// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41c2bb
// name: FUN_0041c2bb
// ranges: 0x41c2bb-0x41c36e
// calls: 0x41c2bb

undefined4 * __thiscall FUN_0041c2bb(void *this,undefined4 param_1)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x4c; local_8 = local_8 + 1) {
    *(undefined4 *)((int)this + local_8 * 4 + 4) = 0;
  }
  *(undefined4 *)this = 0;
  *(undefined4 *)((int)this + 0x270) = 0;
  *(undefined4 *)((int)this + 0x26c) = 0;
  *(undefined4 *)((int)this + 0x268) = 0;
  *(undefined4 *)((int)this + 0x27c) = 0;
  *(undefined4 *)((int)this + 0x278) = *(undefined4 *)((int)this + 0x27c);
  *(undefined4 *)((int)this + 0x274) = *(undefined4 *)((int)this + 0x278);
  *(undefined4 *)((int)this + 0x280) = param_1;
  return this;
}
