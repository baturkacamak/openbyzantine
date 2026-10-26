// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a00a
// name: FUN_0040a00a
// ranges: 0x40a00a-0x40a086
// calls: 0x4094f2, 0x40a00a, 0x40a1e0, 0x40a40d

void __cdecl FUN_0040a00a(int param_1)

{
  ushort uVar1;
  
  FUN_0040a1e0(param_1,s_ANIMDEST_0045d4c0);
  for (uVar1 = 1; uVar1 < 4; uVar1 = uVar1 + 1) {
    if (*(char *)(uVar1 + 1 + param_1) == '@') {
      FUN_0040a40d(param_1,(byte)uVar1,'\x01');
    }
  }
  FUN_004094f2();
  return;
}
