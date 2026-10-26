// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427be3
// name: FUN_00427be3
// ranges: 0x427be3-0x427c42
// calls: 0x427be3

void __cdecl FUN_00427be3(int *param_1)

{
  int *piVar1;
  
  if ((*(int *)(&DAT_00485c90 + *param_1 * 4) != 0) &&
     (piVar1 = *(int **)(*(int *)(&DAT_00485c90 + *param_1 * 4) + 8), piVar1 != (int *)0x0)) {
    (**(code **)(*piVar1 + 0x44))(piVar1,param_1[4]);
  }
  return;
}
