// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x410fee
// name: FUN_00410fee
// ranges: 0x410fee-0x4110e4
// calls: 0x410fee, 0x44ac40

void __cdecl FUN_00410fee(undefined4 *param_1)

{
  LPVOID pvVar1;
  LPVOID pvVar2;
  LPVOID local_18;
  int local_10;
  LPVOID local_8;
  
  pvVar1 = (LPVOID)*param_1;
  local_8 = *(LPVOID *)((int)pvVar1 + 0xcc);
  for (local_10 = 0; local_10 < 0x14; local_10 = local_10 + 1) {
    if (*(int *)((int)pvVar1 + local_10 * 4 + 0x2c) != 0) {
      FUN_0044ac40(*(LPVOID *)((int)pvVar1 + local_10 * 4 + 0x2c));
    }
  }
  while (local_8 != (LPVOID)0x0) {
    for (local_10 = 0; local_10 < 5; local_10 = local_10 + 1) {
      local_18 = *(LPVOID *)((int)local_8 + local_10 * 4);
      while (local_18 != (LPVOID)0x0) {
        pvVar2 = *(LPVOID *)((int)local_18 + 4);
        FUN_0044ac40(local_18);
        local_18 = pvVar2;
      }
    }
    pvVar2 = *(LPVOID *)((int)local_8 + 0x28);
    FUN_0044ac40(local_8);
    local_8 = pvVar2;
  }
  FUN_0044ac40(pvVar1);
  *param_1 = 0;
  return;
}
