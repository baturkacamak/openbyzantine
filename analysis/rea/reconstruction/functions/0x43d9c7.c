// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43d9c7
// name: FUN_0043d9c7
// ranges: 0x43d9c7-0x43dabd; 0x43dac3-0x43db2d
// calls: 0x43d9c7

undefined4 __thiscall FUN_0043d9c7(void *this,int param_1)

{
  undefined4 uVar1;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_84;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_8;
  
  if (*(int *)this == 0x31345649) {
    local_a4 = 0x2c;
    local_a0 = 0x31345649;
    local_9c = 0x10002;
    local_98 = 4;
    local_94 = 2;
    local_78 = 0x70;
    local_74 = 0x31345649;
    local_70 = 0x10002;
    local_6c = 2;
    local_68 = 2;
    local_84 = 2;
    local_64 = 0x80000001;
    local_60 = 0x32;
    if (param_1 == 1) {
      local_7c = 0;
    }
    else if (param_1 == 2) {
      local_7c = 1;
    }
    else {
      local_7c = 0;
    }
    local_90 = 0x8000000a;
    local_8 = ICSendMessage(*(undefined4 *)((int)this + 4),0x5001,&local_78,0x70);
    uVar1 = ICSendMessage(*(undefined4 *)((int)this + 4),0x5001,&local_a4,0x2c);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
