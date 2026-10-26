// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40da4c
// name: FUN_0040da4c
// ranges: 0x40da4c-0x40dae4
// calls: 0x40da4c

void FUN_0040da4c(void)

{
  int *piVar1;
  int local_10;
  int local_c;
  
  for (local_c = 0; local_c < 0x20; local_c = local_c + 1) {
    for (local_10 = 0; local_10 < 4; local_10 = local_10 + 1) {
      piVar1 = *(int **)(PTR_PTR_0045e534 + local_c * 0x24 + local_10 * 4 + 0x14);
      if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 8))(piVar1);
        *(undefined4 *)(PTR_PTR_0045e534 + local_c * 0x24 + local_10 * 4 + 0x14) = 0;
      }
    }
  }
  return;
}
