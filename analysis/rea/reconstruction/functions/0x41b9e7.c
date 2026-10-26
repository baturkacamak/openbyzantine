// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41b9e7
// name: FUN_0041b9e7
// ranges: 0x41b9e7-0x41bb09
// calls: 0x41b0db, 0x41b9e7

int __thiscall FUN_0041b9e7(void *this,int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  undefined4 local_c;
  
  if (param_2 == 0) {
    param_2 = *(int *)((int)this + 0x2c);
  }
  if (param_3 == 0) {
    param_3 = *(int *)((int)this + 0x30);
  }
  if (param_4 == 0) {
    param_4 = *(int *)((int)this + 0x34);
  }
  if (param_5 == 0) {
    param_5 = *(int *)((int)this + 0x38);
  }
  if (param_1 == 0) {
    local_c = *(int *)((int)this + 0x18);
  }
  else {
    local_c = param_1;
  }
  if ((((param_2 < 0) || (param_3 < 0)) || (*(int *)(local_c + 4) < param_4 + param_2)) ||
     (*(int *)(local_c + 8) < param_3 + param_5)) {
    iVar1 = -1;
  }
  else {
    iVar1 = FUN_0041b0db(*(undefined4 *)this,0,*(undefined4 *)((int)this + 0x14),0,
                         *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                         *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),local_c
                         ,0,param_2,param_3,param_4,param_5);
    if (iVar1 == 0) {
      iVar1 = 0;
    }
  }
  return iVar1;
}
