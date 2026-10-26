// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41bc41
// name: FUN_0041bc41
// ranges: 0x41bc41-0x41bce3
// calls: 0x41bc41

bool __thiscall FUN_0041bc41(void *this,int param_1)

{
  int iVar1;
  bool bVar2;
  
  if (*(int *)((int)this + 0xc) == 0x31347669) {
    *(undefined4 *)((int)this + 0x70) = 0x80000008;
    *(undefined4 *)((int)this + 0x68) = 4;
    if (param_1 == 1) {
      *(undefined4 *)((int)this + 0x84) = 0;
    }
    else {
      *(undefined4 *)((int)this + 0x84) = 1;
    }
    iVar1 = ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0x5c,
                          *(undefined4 *)((int)this + 0x5c));
    bVar2 = iVar1 == 0;
  }
  else {
    bVar2 = false;
  }
  return bVar2;
}
