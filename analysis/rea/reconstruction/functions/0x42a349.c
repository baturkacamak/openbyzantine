// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a349
// name: FUN_0042a349
// ranges: 0x42a349-0x42a46a
// calls: 0x42a349, 0x42a8a0, 0x42a900, 0x42a920, 0x42a950

void __cdecl FUN_0042a349(uint param_1)

{
  uint uVar1;
  int local_1c;
  uint local_18 [4];
  uint local_8;
  
  FUN_0042a8a0((void *)(&DAT_0046ca98)[param_1],local_18);
  for (local_1c = 0; local_1c < 4; local_1c = local_1c + 1) {
    uVar1 = FUN_0042a950((&DAT_0046ca98)[local_18[local_1c]]);
    if (uVar1 != 0) {
      local_8 = local_18[local_1c];
      uVar1 = FUN_0042a900((&DAT_0046ca98)[local_8]);
      if (uVar1 == 0) {
        FUN_0042a920((void *)(&DAT_0046ca98)[local_8],1);
      }
    }
  }
  FUN_0042a8a0((void *)(&DAT_0046ca98)[local_8],local_18);
  local_1c = 0;
  while( true ) {
    if (3 < local_1c) {
      return;
    }
    if (((local_18[local_1c] != param_1) &&
        (uVar1 = FUN_0042a950((&DAT_0046ca98)[local_18[local_1c]]), uVar1 != 0)) &&
       (uVar1 = FUN_0042a900((&DAT_0046ca98)[local_18[local_1c]]), uVar1 == 0)) break;
    local_1c = local_1c + 1;
  }
  FUN_0042a920((void *)(&DAT_0046ca98)[local_18[local_1c]],1);
  return;
}
