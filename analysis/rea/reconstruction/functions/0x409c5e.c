// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409c5e
// name: FUN_00409c5e
// ranges: 0x409c5e-0x409d08
// calls: 0x4094f2, 0x409c5e, 0x40a1e0, 0x40a40d, 0x4599c0

void __cdecl FUN_00409c5e(int param_1)

{
  ushort uVar1;
  
  FUN_0040a1e0(param_1,&DAT_0045d458);
  if (*(char *)(param_1 + 2) != '\0') {
    FUN_004599c0(&DAT_0045d460);
  }
  for (uVar1 = 2; uVar1 < 5; uVar1 = uVar1 + 1) {
    if (*(char *)(uVar1 + 1 + param_1) == '@') {
      FUN_0040a40d(param_1,(byte)uVar1,'\x01');
    }
  }
  FUN_004094f2();
  return;
}
