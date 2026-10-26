// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432602
// name: FUN_00432602
// ranges: 0x432602-0x43264c
// calls: 0x432602, 0x44ac40

void __cdecl FUN_00432602(int param_1)

{
  LPVOID pvVar1;
  undefined4 local_8;
  
  local_8 = *(LPVOID *)(param_1 + 0x3c);
  while (local_8 != (LPVOID)0x0) {
    pvVar1 = *(LPVOID *)((int)local_8 + 4);
    FUN_0044ac40(local_8);
    local_8 = pvVar1;
  }
  *(undefined4 *)(param_1 + 0x3c) = 0;
  return;
}
