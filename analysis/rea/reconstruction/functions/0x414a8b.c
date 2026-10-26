// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x414a8b
// name: FUN_00414a8b
// ranges: 0x414a8b-0x414b2a
// calls: 0x414a8b

int __thiscall
FUN_00414a8b(void *this,undefined4 param_1,int param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  
  iVar1 = (**(code **)(**(int **)((int)this + 8) + 100))
                    (*(undefined4 *)((int)this + 8),0,(int)this + 0xc,1,0);
  if (iVar1 == 0) {
    _memset((void *)(*(int *)((int)this + 0x1c) * param_2 + *(int *)((int)this + 0x30)),0,
            *(int *)((int)this + 0x1c) * param_4);
    iVar1 = (**(code **)(**(int **)((int)this + 8) + 0x80))
                      (*(undefined4 *)((int)this + 8),(int)this + 0xc);
  }
  return iVar1;
}
