// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a217
// name: FUN_0042a217
// ranges: 0x42a217-0x42a2cb; 0x42a2d1-0x42a348
// calls: 0x42a217, 0x42a770, 0x42a790, 0x42a8a0, 0x42a900

int FUN_0042a217(void)

{
  uint uVar1;
  int local_18;
  uint local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_18 = 0;
  do {
    if (0x17 < local_18) {
      return -1;
    }
    uVar1 = FUN_0042a900((&DAT_0046ca98)[local_18]);
    if (uVar1 == 0) {
      FUN_0042a8a0((void *)(&DAT_0046ca98)[local_18],&local_14);
      uVar1 = FUN_0042a770((&DAT_0046ca98)[local_14]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a770((&DAT_0046ca98)[local_10]), uVar1 != 0)) {
        return local_18;
      }
      uVar1 = FUN_0042a770((&DAT_0046ca98)[local_c]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a770((&DAT_0046ca98)[local_8]), uVar1 != 0)) {
        return local_18;
      }
      uVar1 = FUN_0042a790((&DAT_0046ca98)[local_14]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a790((&DAT_0046ca98)[local_10]), uVar1 != 0)) {
        return local_18;
      }
      uVar1 = FUN_0042a790((&DAT_0046ca98)[local_c]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a790((&DAT_0046ca98)[local_8]), uVar1 != 0)) {
        return local_18;
      }
    }
    local_18 = local_18 + 1;
  } while( true );
}
