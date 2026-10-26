// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40885b
// name: FUN_0040885b
// ranges: 0x40885b-0x40892c
// calls: 0x40885b, 0x44ac40

void FUN_0040885b(void)

{
  LPVOID pvVar1;
  int local_c;
  
  for (local_c = 0; local_c < 0x15e; local_c = local_c + 1) {
    pvVar1 = *(LPVOID *)(&DAT_00486f90 + local_c * 4);
    if (pvVar1 != (LPVOID)0x0) {
      if (*(int *)((int)pvVar1 + 0xc) != 0) {
        if (*(int *)((int)pvVar1 + 0xc) != *(int *)((int)pvVar1 + 0x10)) {
          (**(code **)(**(int **)((int)pvVar1 + 0x10) + 8))(*(undefined4 *)((int)pvVar1 + 0x10));
        }
        (**(code **)(**(int **)((int)pvVar1 + 0xc) + 8))(*(undefined4 *)((int)pvVar1 + 0xc));
        (**(code **)(**(int **)((int)pvVar1 + 0x14) + 8))(*(undefined4 *)((int)pvVar1 + 0x14));
      }
      FUN_0044ac40(pvVar1);
      *(undefined4 *)(&DAT_00486f90 + local_c * 4) = 0;
    }
  }
  if (DAT_0045d1b4 != (int *)0x0) {
    (**(code **)(*DAT_0045d1b4 + 8))(DAT_0045d1b4);
  }
  return;
}
