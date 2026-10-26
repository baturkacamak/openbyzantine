// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41c225
// name: FUN_0041c225
// ranges: 0x41c225-0x41c2ba
// calls: 0x41c225

uint __thiscall FUN_0041c225(void *this,undefined4 *param_1)

{
  uint uVar1;
  int iVar2;
  
  if (*(int *)((int)this + 0xc) == 0x31347669) {
    *(undefined4 *)((int)this + 200) = 0x80000800;
    *(undefined4 *)((int)this + 0xc0) = 2;
    iVar2 = ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0xb4,
                          *(undefined4 *)((int)this + 0xb4));
    *param_1 = *(undefined4 *)((int)this + 0x118);
    uVar1 = (uint)(iVar2 == 0);
  }
  else {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}
