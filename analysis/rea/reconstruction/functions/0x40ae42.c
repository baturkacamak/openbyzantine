// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ae42
// name: FUN_0040ae42
// ranges: 0x40ae42-0x40ae92
// calls: 0x40ae42

void __fastcall FUN_0040ae42(int param_1)

{
  if (*(int *)(param_1 + 4) != 0) {
    (**(code **)(**(int **)(param_1 + 4) + 0x48))(*(undefined4 *)(param_1 + 4));
    (**(code **)(**(int **)(param_1 + 4) + 8))(*(undefined4 *)(param_1 + 4));
    *(undefined4 *)(param_1 + 4) = 0;
  }
  return;
}
