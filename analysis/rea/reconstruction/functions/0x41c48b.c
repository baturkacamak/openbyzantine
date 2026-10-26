// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41c48b
// name: FUN_0041c48b
// ranges: 0x41c48b-0x41c703
// calls: 0x41b48b, 0x41c48b, 0x42b679

DWORD __thiscall
FUN_0041c48b(void *this,int param_1,int param_2,uint param_3,uint param_4,uint param_5,
            undefined4 *param_6)

{
  DWORD DVar1;
  int iVar2;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74 [9];
  int local_50;
  DWORD local_8;
  
  _memset(local_74,0,0x6c);
  local_74[0] = 0x6c;
  if (*(int *)((int)this + param_2 * 4 + 4) == 0) {
    DVar1 = AVIStreamRead(*(undefined4 *)((int)this + 0x270),param_2,1,*(undefined4 *)this,
                          *(undefined4 *)((int)this + 0x134),0,0);
    if (DVar1 != 0) {
      return DVar1;
    }
    local_78 = *(undefined4 *)this;
    local_8 = 0;
  }
  else {
    local_78 = *(undefined4 *)((int)this + param_2 * 4 + 4);
  }
  local_8 = FUN_0041b48b(*(void **)((int)this + 0x26c),local_78,0,*(int **)((int)this + 0x268));
  if (local_8 == 0) {
    if (*(int *)((int)this + 0x280) == 0) {
      iVar2 = (**(code **)(**(int **)((int)this + 0x268) + 100))
                        (*(undefined4 *)((int)this + 0x268),0,local_74,1,0);
      if (iVar2 != 0) {
        return 1;
      }
      FUN_0042b679(param_1,local_50,0,0,param_5,*(uint *)((int)this + 0x278),param_3,param_4,
                   *(uint *)((int)this + 0x278),*(uint *)((int)this + 0x27c));
      (**(code **)(**(int **)((int)this + 0x268) + 0x80))(*(undefined4 *)((int)this + 0x268),0);
    }
    else {
      local_98 = 0;
      local_94 = 0;
      local_90 = *(undefined4 *)((int)this + 0x278);
      local_8c = *(undefined4 *)((int)this + 0x27c);
      local_88 = 0;
      local_84 = 0x14;
      local_80 = 0x280;
      local_7c = 0x1a4;
      (**(code **)(*DAT_00486888 + 0x14))
                (DAT_00486888,&local_88,*(undefined4 *)((int)this + 0x268),&local_98,0x1000000,0);
      (**(code **)(*DAT_00486888 + 0x1c))
                (DAT_00486888,local_88,local_84,DAT_004869e4,&local_88,0x11);
      *param_6 = local_88;
      param_6[1] = local_84;
      param_6[2] = local_80;
      param_6[3] = local_7c;
    }
    local_8 = 0;
  }
  return local_8;
}
