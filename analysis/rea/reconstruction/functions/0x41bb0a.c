// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41bb0a
// name: FUN_0041bb0a
// ranges: 0x41bb0a-0x41bc40
// calls: 0x41bb0a

bool __thiscall FUN_0041bb0a(void *this,int param_1)

{
  int iVar1;
  uint uVar2;
  int local_c;
  int local_8;
  
  if (param_1 == 0) {
    local_8 = ICSendMessage(*(undefined4 *)this,0x401d,0,0);
  }
  else {
    DAT_0045f34c = (uint)*(ushort *)(param_1 + 2);
    iVar1 = *(int *)((int)this + 0x18);
    uVar2 = (uint)*(ushort *)(param_1 + 2);
    if (0xeb < *(ushort *)(param_1 + 2)) {
      uVar2 = 0xec;
    }
    for (local_c = 0; local_c < (int)uVar2; local_c = local_c + 1) {
      *(undefined1 *)(iVar1 + 0x52 + local_c * 4) = *(undefined1 *)(param_1 + 4 + local_c * 4);
      *(undefined1 *)(iVar1 + 0x51 + local_c * 4) = *(undefined1 *)(param_1 + 5 + local_c * 4);
      *(undefined1 *)(iVar1 + 0x50 + local_c * 4) = *(undefined1 *)(param_1 + 6 + local_c * 4);
    }
    *(undefined4 *)(*(int *)((int)this + 0x18) + 0x20) = 0x100;
    local_8 = ICSendMessage(*(undefined4 *)this,0x401d,*(undefined4 *)((int)this + 0x18),0);
    if (local_8 != 0) {
      ICSendMessage(*(undefined4 *)this,0x401d,0,0);
    }
  }
  return local_8 == 0;
}
