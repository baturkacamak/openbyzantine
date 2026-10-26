// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42b998
// name: FUN_0042b998
// ranges: 0x42b998-0x42ba61
// calls: 0x42b998

void __cdecl
FUN_0042b998(int param_1,short *param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            uint param_7,short param_8)

{
  int local_14;
  int local_10;
  short *local_c;
  short *local_8;
  
  local_c = (short *)((param_6 & 0xffff) * 2 + (param_7 & 0xffff) * (param_3 & 0xffff) * 2 + param_1
                     );
  local_8 = param_2;
  for (local_10 = 0; local_10 < (int)(param_4 & 0xffff); local_10 = local_10 + 1) {
    for (local_14 = 0; local_14 < (int)(param_5 & 0xffff); local_14 = local_14 + 1) {
      if (*local_8 != param_8) {
        *local_c = *local_8;
      }
      local_c = local_c + 1;
      local_8 = local_8 + 1;
    }
    local_c = local_c + ((param_3 & 0xffff) - (param_5 & 0xffff));
  }
  return;
}
