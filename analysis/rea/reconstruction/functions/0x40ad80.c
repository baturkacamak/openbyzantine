// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ad80
// name: FUN_0040ad80
// ranges: 0x40ad80-0x40ae41
// calls: 0x40ad80

int __thiscall FUN_0040ad80(void *this,int *param_1)

{
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined2 *local_20;
  undefined2 local_18;
  undefined2 local_16;
  undefined4 local_14;
  undefined4 local_10;
  undefined2 local_c;
  undefined2 local_a;
  undefined2 local_8;
  
  *(undefined4 *)((int)this + 0x10) = 0;
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined4 *)((int)this + 0x14) = 0x2b110;
  *(undefined4 *)((int)this + 0x18) = 1;
  *(undefined4 *)((int)this + 0x1c) = 0;
  local_18 = 1;
  local_16 = 2;
  local_14 = 0x5622;
  local_10 = 0x15888;
  local_c = 4;
  local_a = 0x10;
  local_8 = 0;
  _memset(&local_30,0,0x14);
  local_30 = 0x14;
  local_2c = 0xe0;
  local_28 = *(undefined4 *)((int)this + 0x14);
  local_20 = &local_18;
  (**(code **)(*param_1 + 0xc))(param_1,&local_30,(int)this + 4,0);
  return (int)this;
}
