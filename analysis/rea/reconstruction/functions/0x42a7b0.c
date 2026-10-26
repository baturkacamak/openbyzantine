// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a7b0
// name: FUN_0042a7b0
// ranges: 0x42a7b0-0x42a7f4
// calls: 0x42a7b0, 0x42a800

void * __thiscall
FUN_0042a7b0(void *this,int param_1,int param_2,int param_3,int param_4,undefined4 param_5,
            undefined4 param_6)

{
  FUN_0042a800(this,param_1,param_2,param_3,param_4);
  *(undefined4 *)((int)this + 4) = param_5;
  *(undefined4 *)((int)this + 8) = param_6;
  return this;
}
