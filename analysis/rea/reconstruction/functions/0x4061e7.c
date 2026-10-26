// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4061e7
// name: FUN_004061e7
// ranges: 0x4061e7-0x4062bb
// calls: 0x405bf4, 0x4061e7

int __cdecl FUN_004061e7(int param_1,int param_2)

{
  byte bVar1;
  int local_18;
  int local_14;
  int local_10;
  
  local_10 = 0;
  local_18 = 0;
  for (local_14 = 0; *(char *)(local_14 + param_1) != '\0'; local_14 = local_14 + 1) {
    if (*(char *)(local_14 + param_1) == '\n') {
      if (local_10 < local_18) {
        local_10 = local_18;
      }
      local_18 = 0;
    }
    else {
      bVar1 = *(byte *)(local_14 + param_1);
      if (0x7f < bVar1) {
        bVar1 = FUN_00405bf4();
      }
      local_18 = local_18 +
                 (uint)*(ushort *)
                        ((&DAT_00487518)[param_2 * 0x12] + 4 + (uint)(byte)(bVar1 - 0x20) * 8);
    }
  }
  if (local_10 <= local_18) {
    local_10 = local_18;
  }
  return local_10;
}
