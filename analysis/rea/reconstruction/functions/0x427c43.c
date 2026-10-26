// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427c43
// name: FUN_00427c43
// ranges: 0x427c43-0x427ca9
// calls: 0x427c43

void __cdecl FUN_00427c43(int *param_1)

{
  int *piVar1;
  
  if ((*(int *)(&DAT_00485c90 + *param_1 * 4) != 0) &&
     (piVar1 = *(int **)(*(int *)(&DAT_00485c90 + *param_1 * 4) + 8), piVar1 != (int *)0x0)) {
    (**(code **)(*piVar1 + 0x3c))(piVar1,param_1[7] + -10000);
  }
  return;
}
