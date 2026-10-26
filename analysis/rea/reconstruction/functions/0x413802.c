// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x413802
// name: FUN_00413802
// ranges: 0x413802-0x413839
// calls: 0x413802, 0x44ac40

void __cdecl FUN_00413802(LPVOID param_1)

{
  LPVOID pvVar1;
  
  while (param_1 != (LPVOID)0x0) {
    pvVar1 = *(LPVOID *)((int)param_1 + 0x2c);
    FUN_0044ac40(param_1);
    param_1 = pvVar1;
  }
  return;
}
