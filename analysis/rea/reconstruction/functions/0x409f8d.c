// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409f8d
// name: FUN_00409f8d
// ranges: 0x409f8d-0x40a009
// calls: 0x4094f2, 0x409f8d, 0x40a1e0, 0x40a40d

void __cdecl FUN_00409f8d(int param_1)

{
  ushort uVar1;
  
  FUN_0040a1e0(param_1,s_ANIMSPD_0045d4b4);
  for (uVar1 = 1; uVar1 < 4; uVar1 = uVar1 + 1) {
    if (*(char *)(uVar1 + 1 + param_1) == '@') {
      FUN_0040a40d(param_1,(byte)uVar1,'\x01');
    }
  }
  FUN_004094f2();
  return;
}
