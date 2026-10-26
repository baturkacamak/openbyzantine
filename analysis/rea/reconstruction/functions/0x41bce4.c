// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41bce4
// name: FUN_0041bce4
// ranges: 0x41bce4-0x41bdbc
// calls: 0x41bce4

void __thiscall
FUN_0041bce4(void *this,undefined4 *param_1,undefined4 *param_2,undefined4 *param_3,int param_4)

{
  if (*(int *)((int)this + 0xc) == 0x31347669) {
    *(undefined4 *)((int)this + 200) = 0x800000e0;
    if (param_4 == 0) {
      *(undefined4 *)((int)this + 0xc0) = 2;
      ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0xb4,*(undefined4 *)((int)this + 0xb4));
    }
    else {
      *(undefined4 *)((int)this + 0xc0) = 1;
      ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0xb4,*(undefined4 *)((int)this + 0xb4));
    }
    *param_1 = *(undefined4 *)((int)this + 0xfc);
    *param_2 = *(undefined4 *)((int)this + 0x100);
    *param_3 = *(undefined4 *)((int)this + 0x104);
  }
  return;
}
