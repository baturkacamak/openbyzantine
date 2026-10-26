// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x413729
// name: FUN_00413729
// ranges: 0x413729-0x413801
// calls: 0x413729, 0x413802, 0x44ac40

void __cdecl FUN_00413729(int param_1)

{
  ushort uVar1;
  LPVOID pvVar2;
  LPVOID pvVar3;
  undefined4 local_8;
  
  for (uVar1 = 0; uVar1 < 0x32; uVar1 = uVar1 + 1) {
    local_8 = *(LPVOID *)(param_1 + (uint)uVar1 * 4);
    while (pvVar3 = local_8, local_8 != (LPVOID)0x0) {
      FUN_00413802(*(LPVOID *)((int)local_8 + 4));
      *(undefined4 *)((int)local_8 + 4) = 0;
      local_8 = *(LPVOID *)((int)local_8 + 0xc);
      while (local_8 != (LPVOID)0x0) {
        FUN_00413802(*(LPVOID *)((int)local_8 + 4));
        *(undefined4 *)((int)local_8 + 4) = 0;
        pvVar2 = *(LPVOID *)((int)local_8 + 0xc);
        FUN_0044ac40(local_8);
        local_8 = pvVar2;
      }
      local_8 = *(LPVOID *)((int)pvVar3 + 8);
      FUN_0044ac40(pvVar3);
    }
  }
  return;
}
