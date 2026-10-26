// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43d52b
// name: FUN_0043d52b
// ranges: 0x43d52b-0x43d576; 0x43d57c-0x43d58c
// calls: 0x43d52b

undefined4 __thiscall FUN_0043d52b(void *this,undefined4 param_1)

{
  undefined4 uVar1;
  
  *(undefined4 *)this = param_1;
  ICClose(*(undefined4 *)((int)this + 4));
  uVar1 = ICOpen(0x63646976,*(undefined4 *)this,3);
  *(undefined4 *)((int)this + 4) = uVar1;
  if (*(int *)((int)this + 4) == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
