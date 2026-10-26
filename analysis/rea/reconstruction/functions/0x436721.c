// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x436721
// name: FUN_00436721
// ranges: 0x436721-0x43679b
// calls: 0x436721, 0x43679c

void __cdecl FUN_00436721(int *param_1,int *param_2)

{
  if (*param_1 == 0) {
    FUN_0043679c(param_1);
    *param_2 = *param_1;
  }
  else {
    *param_2 = *param_1;
    while (*(int *)(*param_2 + 4) != 0) {
      *param_2 = *(int *)(*param_2 + 4);
    }
    FUN_0043679c((int *)(*param_2 + 4));
    *param_2 = *(int *)(*param_2 + 4);
  }
  return;
}
