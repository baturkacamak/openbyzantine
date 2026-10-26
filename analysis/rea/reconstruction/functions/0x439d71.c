// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x439d71
// name: FUN_00439d71
// ranges: 0x439d71-0x439dba
// calls: 0x439d71

undefined4 __thiscall FUN_00439d71(void *this,undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = AVIFileOpenA((int)this + 0x14,param_1,0,0);
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    *(undefined4 *)((int)this + 0x14) = 0;
    uVar2 = 0xffffffff;
  }
  return uVar2;
}
