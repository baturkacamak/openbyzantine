// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41bedb
// name: FUN_0041bedb
// ranges: 0x41bedb-0x41bf9d
// calls: 0x41bedb

void __thiscall FUN_0041bedb(void *this,undefined4 param_1,undefined4 *param_2,int param_3)

{
  if (*(int *)((int)this + 0xc) == 0x31347669) {
    *(undefined4 *)((int)this + 200) = 0x80000001;
    if (param_3 == 0) {
      *(undefined4 *)((int)this + 0xc0) = 2;
      ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0xb4,*(undefined4 *)((int)this + 0xb4));
    }
    else {
      *(undefined4 *)((int)this + 0xc0) = 1;
      ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0xb4,*(undefined4 *)((int)this + 0xb4));
    }
    *param_2 = *(undefined4 *)((int)this + 0xcc);
  }
  return;
}
