// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43d73d
// name: FUN_0043d73d
// ranges: 0x43d73d-0x43d7c1; 0x43d7c7-0x43d844; 0x43d84f-0x43d855
// calls: 0x43d73d

void __thiscall
FUN_0043d73d(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7,int param_8,undefined4 param_9,
            undefined4 param_10,undefined4 param_11,undefined4 param_12,undefined4 param_13)

{
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  int local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  if (*(int *)this == 0x31345649) {
    local_38 = param_1;
    local_34 = param_2;
    local_30 = param_3;
    local_2c = param_8;
    local_28 = param_9;
    local_24 = param_10;
    local_20 = param_11;
    local_1c = param_12;
    local_18 = param_13;
    local_14 = param_4;
    local_10 = param_5;
    local_c = param_6;
    local_8 = param_7;
    ICSendMessage(*(undefined4 *)((int)this + 4),0x403e,&local_38,0x34);
  }
  else {
    *(int *)(param_8 + 8) = -*(int *)(param_8 + 8);
    local_38 = param_1;
    local_34 = param_2;
    local_30 = param_3;
    local_2c = param_8;
    local_28 = param_9;
    local_24 = param_10;
    local_20 = param_11;
    local_1c = param_12;
    local_18 = param_13;
    local_14 = param_4;
    local_10 = param_5;
    local_c = param_6;
    local_8 = param_7;
    ICSendMessage(*(undefined4 *)((int)this + 4),0x403e,&local_38,0x34);
  }
  return;
}
