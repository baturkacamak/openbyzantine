// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41bdbd
// name: FUN_0041bdbd
// ranges: 0x41bdbd-0x41beda
// calls: 0x41bdbd

void __thiscall FUN_0041bdbd(void *this,int param_1,int param_2,int param_3,int param_4)

{
  if ((((*(int *)((int)this + 0xc) == 0x31347669) && (-0x100 < param_1)) && (param_1 < 0x100)) &&
     (((-0x100 < param_2 && (param_2 < 0x100)) && ((-0x100 < param_3 && (param_3 < 0x100)))))) {
    *(undefined4 *)((int)this + 200) = 0x800000e0;
    if (param_4 == 0) {
      *(int *)((int)this + 0xfc) = param_1;
      *(int *)((int)this + 0x100) = param_2;
      *(int *)((int)this + 0x104) = param_3;
      *(undefined4 *)((int)this + 0xc0) = 2;
      ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0xb4,0x70);
    }
    else {
      *(undefined4 *)((int)this + 0xc0) = 1;
      ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0xb4,*(undefined4 *)((int)this + 0xb4));
    }
  }
  return;
}
