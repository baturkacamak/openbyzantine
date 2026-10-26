// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427b60
// name: FUN_00427b60
// ranges: 0x427b60-0x427be2
// calls: 0x422927, 0x427b60, 0x44ac40

void __cdecl FUN_00427b60(int param_1)

{
  int *piVar1;
  
  if (*(int *)(&DAT_00485c90 + param_1 * 4) != 0) {
    piVar1 = *(int **)(*(int *)(&DAT_00485c90 + param_1 * 4) + 8);
    if (piVar1 != (int *)0x0) {
      if (piVar1 == DAT_0045fd14) {
        FUN_00422927();
      }
      else {
        (**(code **)(*piVar1 + 8))(piVar1);
      }
    }
    FUN_0044ac40(*(LPVOID *)(&DAT_00485c90 + param_1 * 4));
    *(undefined4 *)(&DAT_00485c90 + param_1 * 4) = 0;
  }
  return;
}
