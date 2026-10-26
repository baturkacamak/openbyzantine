// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427d49
// name: FUN_00427d49
// ranges: 0x427d49-0x427de0
// calls: 0x427d49, 0x428143

void __cdecl FUN_00427d49(int *param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = FUN_00428143(*param_1);
  if (((iVar2 == 0) && (*(int *)(&DAT_00485c90 + *param_1 * 4) != 0)) &&
     (piVar1 = *(int **)(*(int *)(&DAT_00485c90 + *param_1 * 4) + 8), piVar1 != (int *)0x0)) {
    if (param_1[5] == 0) {
      (**(code **)(*piVar1 + 0x30))(piVar1,0,0,0);
    }
    else {
      (**(code **)(*piVar1 + 0x30))(piVar1,0,0,1);
    }
  }
  return;
}
