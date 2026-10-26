// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x412aea
// name: FUN_00412aea
// ranges: 0x412aea-0x412cc3
// calls: 0x412762, 0x412aea, 0x412cc4

undefined2 __cdecl FUN_00412aea(int param_1,byte param_2,FILE *param_3)

{
  byte local_28 [4];
  byte local_24 [4];
  byte local_20;
  byte local_1c;
  byte local_18;
  char *local_14;
  byte local_10;
  char *local_c;
  undefined2 local_8;
  
  local_8 = 0;
  local_18 = 0;
  local_10 = 0;
  while (local_10 < param_2) {
    *(undefined4 *)(param_1 + (uint)local_18 * 4) = 0;
    local_c = (char *)0x0;
    local_1c = 0;
    __fread_lk((char *)local_28,1,1,param_3);
    for (; local_1c < local_28[0]; local_1c = local_1c + 1) {
      if (local_1c == 0) {
        FUN_00412cc4((int *)((uint)local_18 * 4 + param_1));
        local_c = *(char **)(param_1 + (uint)local_18 * 4);
      }
      else {
        FUN_00412cc4((int *)(local_c + 8));
        local_c = *(char **)(local_c + 8);
      }
      __fread_lk(local_c,2,1,param_3);
      __fread_lk(local_c + 2,2,1,param_3);
      FUN_00412762((undefined4 *)(local_c + 4),param_3);
      __fread_lk((char *)local_24,1,1,param_3);
      local_14 = (char *)0x0;
      for (local_20 = 0; local_20 < local_24[0]; local_20 = local_20 + 1) {
        if (local_20 == 0) {
          FUN_00412cc4((int *)(local_c + 0xc));
          local_14 = *(char **)(local_c + 0xc);
        }
        else {
          FUN_00412cc4((int *)(local_14 + 0xc));
          local_14 = *(char **)(local_14 + 0xc);
        }
        __fread_lk(local_14,2,1,param_3);
        __fread_lk(local_14 + 2,2,1,param_3);
        FUN_00412762((undefined4 *)(local_14 + 4),param_3);
      }
    }
    if (local_28[0] != 0) {
      local_10 = local_10 + 1;
    }
    local_18 = local_18 + 1;
  }
  return local_8;
}
