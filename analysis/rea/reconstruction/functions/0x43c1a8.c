// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c1a8
// name: FUN_0043c1a8
// ranges: 0x43c1a8-0x43c2ee
// calls: 0x43c1a8

void __cdecl FUN_0043c1a8(short *param_1)

{
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  if (1 < *param_1) {
    local_c = 0xffff;
    local_8 = 0xffff;
    local_18 = 0;
    local_14 = 0;
    for (local_10 = 0; local_10 < *param_1; local_10 = local_10 + 1) {
      if ((int)(&DAT_0046caf8)[local_10 * 4] < local_c) {
        local_c = (&DAT_0046caf8)[local_10 * 4];
      }
      if ((int)(&DAT_0046cafc)[local_10 * 4] < local_8) {
        local_8 = (&DAT_0046cafc)[local_10 * 4];
      }
      if (local_18 < (int)(&DAT_0046cb00)[local_10 * 4]) {
        local_18 = (&DAT_0046cb00)[local_10 * 4];
      }
      if (local_14 < *(int *)(&DAT_0046cb04 + local_10 * 0x10)) {
        local_14 = *(int *)(&DAT_0046cb04 + local_10 * 0x10);
      }
    }
    SetRect((LPRECT)&DAT_0046caf8,local_c,local_8,local_18,local_14);
    for (local_10 = 1; local_10 < *param_1; local_10 = local_10 + 1) {
      (&DAT_0046caf8)[local_10 * 4] = 0xffff;
    }
    *param_1 = 1;
  }
  return;
}
