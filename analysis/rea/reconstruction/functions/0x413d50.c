// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x413d50
// name: FUN_00413d50
// ranges: 0x413d50-0x413e10
// calls: 0x413d50

undefined4 * __thiscall
FUN_00413d50(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  *(undefined4 *)this = param_1;
  *(undefined4 *)((int)this + 4) = param_2;
  *(undefined4 *)((int)this + 8) = param_3;
  *(undefined4 *)((int)this + 0x98) = param_4;
  _memset((void *)((int)this + 0xc),0,0x6c);
  *(undefined4 *)((int)this + 0xc) = 0x6c;
  *(undefined4 *)((int)this + 0x10) = 0;
  (**(code **)(**(int **)((int)this + 8) + 0x58))(*(undefined4 *)((int)this + 8),(int)this + 0xc);
  _memset((void *)((int)this + 0x78),0,0x20);
  *(undefined4 *)((int)this + 0x78) = 0x20;
  *(undefined4 *)((int)this + 0x7c) = 0;
  (**(code **)(**(int **)((int)this + 8) + 0x54))(*(undefined4 *)((int)this + 8),(int)this + 0x78);
  return this;
}
