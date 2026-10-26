// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43af73
// name: FUN_0043af73
// ranges: 0x43af73-0x43afdf
// calls: 0x413e6b, 0x414625, 0x43af73

int __thiscall FUN_0043af73(void *this,int param_1)

{
  undefined4 local_8;
  
  if (*(int *)((int)this + 0x1c) == 0) {
    local_8 = FUN_00413e6b(*(void **)((int)this + 8),*(int *)((int)this + 0x2c),
                           *(int *)((int)this + 0x30),*(int *)((int)this + 0x4c),
                           *(int *)((int)this + 0x50),param_1);
  }
  else {
    local_8 = FUN_00414625(*(void **)((int)this + 8),param_1);
  }
  return local_8;
}
