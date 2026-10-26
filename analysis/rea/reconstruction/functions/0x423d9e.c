// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x423d9e
// name: FUN_00423d9e
// ranges: 0x423d9e-0x423e02
// calls: 0x423d9e

bool __thiscall
FUN_00423d9e(void *this,int *param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4)

{
  bool bVar1;
  
  bVar1 = *(int *)((int)this + 0x34) != 0;
  if (bVar1) {
    *param_1 = *(int *)((int)this + 0x38) * 2 + *(int *)((int)this + 0x34);
    *param_2 = *(undefined4 *)((int)this + 0x24);
    *param_3 = *(undefined4 *)((int)this + 0x20);
    *param_4 = *(undefined4 *)((int)this + 0x1c);
  }
  return !bVar1;
}
