// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43d58d
// name: FUN_0043d58d
// ranges: 0x43d58d-0x43d5e0; 0x43d5e6-0x43d5f6
// calls: 0x43d58d

undefined4 __thiscall
FUN_0043d58d(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  *(undefined4 *)this = param_1;
  ICClose(*(undefined4 *)((int)this + 4));
  uVar1 = ICLocate(0x63646976,*(undefined4 *)this,param_2,param_3,3);
  *(undefined4 *)((int)this + 4) = uVar1;
  if (*(int *)((int)this + 4) == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
