// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41383a
// name: FUN_0041383a
// ranges: 0x41383a-0x4138a7
// calls: 0x41383a, 0x44ac40

void __cdecl FUN_0041383a(int param_1)

{
  ushort uVar1;
  LPVOID pvVar2;
  undefined4 local_8;
  
  for (uVar1 = 0; uVar1 < 0x32; uVar1 = uVar1 + 1) {
    local_8 = *(LPVOID *)(param_1 + (uint)uVar1 * 4);
    while (local_8 != (LPVOID)0x0) {
      pvVar2 = *(LPVOID *)((int)local_8 + 4);
      FUN_0044ac40(local_8);
      local_8 = pvVar2;
    }
  }
  return;
}
