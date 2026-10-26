// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41c0f5
// name: FUN_0041c0f5
// ranges: 0x41c0f5-0x41c224
// calls: 0x41c0f5

bool __thiscall FUN_0041c0f5(void *this,undefined4 param_1,int param_2,int param_3)

{
  int iVar1;
  
  *(undefined4 *)((int)this + 200) = 0x80000400;
  if ((param_2 == 0) || (param_3 == 0)) {
    *(undefined4 *)((int)this + 0x10c) = 10;
    *(int *)((int)this + 0x110) = DAT_0045f34c + 9;
  }
  else {
    *(int *)((int)this + 0x10c) = param_2;
    *(int *)((int)this + 0x110) = param_3;
  }
  *(int *)((int)this + 0x114) = *(int *)((int)this + 0x18) + 0x28;
  *(undefined4 *)((int)this + 0xc0) = 2;
  ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0xb4,*(undefined4 *)((int)this + 0xb4));
  ICSendMessage(*(undefined4 *)this,0x401d,*(undefined4 *)((int)this + 0x18),0);
  *(undefined4 *)((int)this + 0x118) = param_1;
  *(undefined4 *)((int)this + 200) = 0x80000800;
  iVar1 = ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0xb4,
                        *(undefined4 *)((int)this + 0xb4));
  return iVar1 == 0;
}
