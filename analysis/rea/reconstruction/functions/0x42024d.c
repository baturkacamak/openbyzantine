// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42024d
// name: FUN_0042024d
// ranges: 0x42024d-0x4202c6
// calls: 0x42024d, 0x44ac40

void __cdecl FUN_0042024d(int param_1,int param_2)

{
  undefined4 *puVar1;
  int local_8;
  
  for (local_8 = 0; local_8 < param_2; local_8 = local_8 + 1) {
    puVar1 = *(undefined4 **)(param_1 + local_8 * 4);
    if (puVar1 != (undefined4 *)0x0) {
      FUN_0044ac40((LPVOID)puVar1[1]);
      FUN_0044ac40((LPVOID)*puVar1);
      FUN_0044ac40(puVar1);
      *(undefined4 *)(param_1 + local_8 * 4) = 0;
    }
  }
  return;
}
