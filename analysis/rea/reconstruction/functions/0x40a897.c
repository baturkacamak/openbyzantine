// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a897
// name: FUN_0040a897
// ranges: 0x40a897-0x40a995
// calls: 0x40a897, 0x4599c0

void __cdecl FUN_0040a897(int param_1,byte param_2,byte param_3)

{
  uint local_10;
  uint local_c;
  int local_8;
  
  if (param_3 == 0) {
    DAT_0047a898 = param_2;
  }
  else {
    DAT_0047a898 = param_3;
  }
  local_8 = 0;
  local_c = (uint)param_2;
  while (*(char *)(local_8 + param_1) != '\0') {
    FUN_004599c0(&DAT_0045d5c0);
    local_8 = local_8 + 1;
    local_c = local_c + 1;
    if ((0x41 < local_c) && (*(char *)(local_8 + param_1) == ' ')) {
      FUN_004599c0(&DAT_0045d5c4);
      for (local_10 = 0; local_10 < DAT_0047a898; local_10 = local_10 + 1) {
        FUN_004599c0(&DAT_0045d5c8);
      }
      local_c = (uint)DAT_0047a898;
      for (; *(char *)(local_8 + param_1) == ' '; local_8 = local_8 + 1) {
      }
    }
  }
  return;
}
