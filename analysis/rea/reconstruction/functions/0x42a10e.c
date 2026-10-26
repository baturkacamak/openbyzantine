// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a10e
// name: FUN_0042a10e
// ranges: 0x42a10e-0x42a1af; 0x42a1b5-0x42a216
// calls: 0x429d47, 0x42a10e, 0x42a790, 0x42a8a0

undefined4 __cdecl FUN_0042a10e(undefined4 *param_1)

{
  uint uVar1;
  undefined4 uVar2;
  int local_18;
  uint local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_18 = 0;
  do {
    if (0x17 < local_18) {
      return 0;
    }
    uVar1 = FUN_0042a790((&DAT_0046ca98)[local_18]);
    if (uVar1 != 0) {
      FUN_0042a8a0((void *)(&DAT_0046ca98)[local_18],&local_14);
      uVar1 = FUN_0042a790((&DAT_0046ca98)[local_14]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a790((&DAT_0046ca98)[local_10]), uVar1 != 0)) {
        uVar2 = FUN_00429d47(local_18,local_14,local_10);
        *param_1 = uVar2;
        return 1;
      }
      uVar1 = FUN_0042a790((&DAT_0046ca98)[local_c]);
      if ((uVar1 != 0) && (uVar1 = FUN_0042a790((&DAT_0046ca98)[local_8]), uVar1 != 0)) {
        uVar2 = FUN_00429d47(local_18,local_c,local_8);
        *param_1 = uVar2;
        return 1;
      }
    }
    local_18 = local_18 + 1;
  } while( true );
}
