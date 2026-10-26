// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42318c
// name: FUN_0042318c
// ranges: 0x42318c-0x42330e
// calls: 0x404f64, 0x404fe3, 0x42318c

void FUN_0042318c(void)

{
  uint local_28;
  int local_24;
  undefined1 local_20 [4];
  uint local_1c;
  uint local_18;
  int local_14;
  MMRESULT local_10;
  undefined4 local_c;
  int local_8;
  
  local_14 = (**(code **)(*DAT_0045fd14 + 0x2c))
                       (DAT_0045fd14,0,DAT_004867d4,&local_8,&local_18,&local_c,local_20,0);
  if (local_14 == 0) {
    if ((local_18 != 0) &&
       (local_10 = FUN_00404fe3(DAT_004867a4,local_18,local_8,0x4867a8,&local_1c),
       local_1c < local_18)) {
      if (DAT_004867e8 == 0) {
        DAT_004867ec = 1;
        DAT_004867e0 = local_1c;
        _memset((void *)(local_1c + local_8),(uint)(-(*(short *)(DAT_004867a0 + 0xe) == 8) & 0x80),
                local_18 - local_1c);
      }
      else {
        local_24 = local_8;
        local_28 = local_18;
        do {
          local_24 = local_24 + local_1c;
          local_28 = local_28 - local_1c;
          local_10 = FUN_00404f64(&DAT_004867a4,(LPMMCKINFO)&DAT_004867a8,(MMCKINFO *)&DAT_004867bc)
          ;
          local_10 = FUN_00404fe3(DAT_004867a4,local_28,local_24,0x4867a8,&local_1c);
        } while (local_1c < local_28);
      }
    }
    (**(code **)(*DAT_0045fd14 + 0x4c))(DAT_0045fd14,local_8,local_18,local_c,0);
    DAT_004867dc = 0;
    DAT_004867d8 = 0;
  }
  return;
}
