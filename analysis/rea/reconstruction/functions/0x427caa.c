// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427caa
// name: FUN_00427caa
// ranges: 0x427caa-0x427d19
// calls: 0x427caa, 0x428b1e

void __cdecl FUN_00427caa(int *param_1)

{
  int *piVar1;
  int iVar2;
  
  if ((*(int *)(&DAT_00485c90 + *param_1 * 4) != 0) &&
     (piVar1 = *(int **)(*(int *)(&DAT_00485c90 + *param_1 * 4) + 8), piVar1 != (int *)0x0)) {
    iVar2 = FUN_00428b1e(param_1[8],param_1[9]);
    (**(code **)(*piVar1 + 0x40))(piVar1,iVar2);
  }
  return;
}
