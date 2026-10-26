// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41c05f
// name: FUN_0041c05f
// ranges: 0x41c05f-0x41c0f4
// calls: 0x41c05f

void __thiscall FUN_0041c05f(void *this,undefined4 param_1)

{
  if (*(int *)((int)this + 0xc) == 0x31347669) {
    *(undefined4 *)((int)this + 0x68) = 4;
    *(undefined4 *)((int)this + 0x70) = 0x80000001;
    ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0x5c,*(undefined4 *)((int)this + 0x5c));
    *(undefined4 *)((int)this + 0x78) = 1;
    *(undefined4 *)((int)this + 0x74) = param_1;
    ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0x5c,*(undefined4 *)((int)this + 0x5c));
  }
  return;
}
