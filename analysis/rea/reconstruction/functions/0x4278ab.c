// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4278ab
// name: FUN_004278ab
// ranges: 0x4278ab-0x42793e
// calls: 0x422927, 0x4278ab, 0x44ac40

void FUN_004278ab(void)

{
  LPVOID pvVar1;
  int *piVar2;
  undefined4 local_8;
  
  local_8 = DAT_0046019c;
  while (local_8 != (LPVOID)0x0) {
    pvVar1 = *(LPVOID *)((int)local_8 + 0x58);
    if (*(int *)((int)local_8 + 0x14) == 2) {
      FUN_00422927();
    }
    else {
      piVar2 = *(int **)(*(int *)((int)local_8 + 0x54) + 8);
      (**(code **)(*piVar2 + 0x48))(piVar2);
      (**(code **)(*piVar2 + 8))(piVar2);
    }
    FUN_0044ac40(*(LPVOID *)((int)local_8 + 0x54));
    FUN_0044ac40(local_8);
    local_8 = pvVar1;
  }
  DAT_0046019c = (LPVOID)0x0;
  return;
}
