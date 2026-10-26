// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41a962
// name: FUN_0041a962
// ranges: 0x41a962-0x41ad53
// calls: 0x41a962, 0x41ad54, 0x44ac60

undefined4 __thiscall FUN_0041a962(void *this,int param_1,void *param_2,int param_3)

{
  int *piVar1;
  undefined4 uVar2;
  
  *(undefined4 *)((int)this + 0x10) = 0;
  if (*(int *)this != 0) {
    FUN_0041ad54(this);
  }
  *(undefined4 *)((int)this + 0x1b4) = 0;
  if (param_1 == 0) {
    *(undefined4 *)((int)this + 0xc) = *(undefined4 *)((int)param_2 + 0x10);
  }
  else {
    *(int *)((int)this + 0xc) = param_1;
  }
  if (*(int *)((int)this + 0xc) == 0x31345649) {
    *(undefined4 *)((int)this + 0xc) = 0x31347669;
  }
  uVar2 = ICLocate(0x63646976,param_1,param_2,0,2);
  *(undefined4 *)this = uVar2;
  if (*(int *)this == 0) {
    uVar2 = 0x80044071;
  }
  else {
    ICSendMessage(*(undefined4 *)this,0x401d,0,0);
    uVar2 = FUN_0044ac60(0x428);
    *(undefined4 *)((int)this + 0x14) = uVar2;
    uVar2 = FUN_0044ac60(0x428);
    *(undefined4 *)((int)this + 0x18) = uVar2;
    if ((*(int *)((int)this + 0x14) == 0) || (*(int *)((int)this + 0x18) == 0)) {
      uVar2 = 0x80044067;
    }
    else {
      FID_conflict__memcpy(*(void **)((int)this + 0x14),param_2,0x28);
      FID_conflict__memcpy(*(void **)((int)this + 0x18),*(void **)((int)this + 0x14),0x28);
      if (param_3 == 1) {
        piVar1 = (int *)(*(int *)((int)this + 0x18) + 4);
        *piVar1 = *piVar1 << 1;
        piVar1 = (int *)(*(int *)((int)this + 0x18) + 8);
        *piVar1 = *piVar1 << 1;
      }
      *(undefined2 *)(*(int *)((int)this + 0x18) + 0xe) = 0x10;
      *(undefined4 *)(*(int *)((int)this + 0x18) + 0x10) = 0;
      *(undefined4 *)(*(int *)((int)this + 0x18) + 0x14) = 0;
      *(undefined4 *)((int)this + 0x20) = 0;
      *(undefined4 *)((int)this + 0x1c) = *(undefined4 *)((int)this + 0x20);
      *(undefined4 *)((int)this + 0x24) = *(undefined4 *)(*(int *)((int)this + 0x14) + 4);
      *(undefined4 *)((int)this + 0x28) = *(undefined4 *)(*(int *)((int)this + 0x14) + 8);
      *(undefined4 *)((int)this + 0x30) = 0;
      *(undefined4 *)((int)this + 0x2c) = *(undefined4 *)((int)this + 0x30);
      *(undefined4 *)((int)this + 0x34) = *(undefined4 *)(*(int *)((int)this + 0x18) + 4);
      *(undefined4 *)((int)this + 0x38) = *(undefined4 *)(*(int *)((int)this + 0x18) + 8);
      *(undefined4 *)((int)this + 0x40) = 0;
      *(undefined4 *)((int)this + 0x3c) = *(undefined4 *)((int)this + 0x40);
      *(undefined4 *)((int)this + 0x44) = *(undefined4 *)(*(int *)((int)this + 0x18) + 4);
      *(undefined4 *)((int)this + 0x48) = *(undefined4 *)(*(int *)((int)this + 0x18) + 8);
      *(undefined4 *)((int)this + 0x50) = 0;
      *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)((int)this + 0x50);
      *(undefined4 *)((int)this + 0x54) = *(undefined4 *)(*(int *)((int)this + 0x18) + 4);
      *(undefined4 *)((int)this + 0x58) = *(undefined4 *)(*(int *)((int)this + 0x18) + 8);
      if (param_3 == 1) {
        *(undefined4 *)((int)this + 0x50) = 0x14;
      }
      else if (param_3 == 2) {
        *(undefined4 *)((int)this + 0x4c) = 0xa0;
        *(undefined4 *)((int)this + 0x50) = 0x78;
      }
      if (*(int *)((int)this + 0xc) == 0x31347669) {
        *(undefined4 *)((int)this + 0x88) = 0x2c;
        *(undefined4 *)((int)this + 0x8c) = 0x31345649;
        *(undefined4 *)((int)this + 0x90) = 0x10002;
        *(undefined4 *)((int)this + 0x94) = 3;
        *(undefined4 *)((int)this + 0x98) = 2;
        *(undefined4 *)((int)this + 0x9c) = 0x8000000f;
        ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0x88,0x2c);
        FID_conflict__memcpy((void *)((int)this + 0x5c),(void *)((int)this + 0x88),0x2c);
        *(undefined4 *)((int)this + 0x68) = 4;
        *(undefined4 *)((int)this + 0x124) = 0x70;
        *(undefined4 *)((int)this + 0x128) = 0x31345649;
        *(undefined4 *)((int)this + 300) = 0x10002;
        *(undefined4 *)((int)this + 0x130) = 1;
        *(undefined4 *)((int)this + 0x134) = 2;
        *(undefined4 *)((int)this + 0x138) = 0x800000e7;
        ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0x124,0x70);
        FID_conflict__memcpy((void *)((int)this + 0xb4),(void *)((int)this + 0x124),0x70);
        *(undefined4 *)((int)this + 0xc0) = 2;
        *(int *)((int)this + 0xf0) = (int)this + 0x194;
        *(undefined4 *)((int)this + 200) = 0x80000008;
        ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0xb4,0x70);
      }
      uVar2 = 0;
    }
  }
  return uVar2;
}
