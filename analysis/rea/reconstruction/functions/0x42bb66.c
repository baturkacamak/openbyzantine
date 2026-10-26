// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42bb66
// name: FUN_0042bb66
// ranges: 0x42bb66-0x42bc8a
// calls: 0x42bb66

void __cdecl
FUN_0042bb66(int param_1,int param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            uint param_7,uint param_8,uint param_9,uint param_10,short param_11)

{
  int local_14;
  int local_10;
  short *local_c;
  short *local_8;
  
  local_c = (short *)((param_7 & 0xffff) * 2 + (param_8 & 0xffff) * (param_5 & 0xffff) * 2 + param_1
                     );
  local_8 = (short *)((param_3 & 0xffff) * 2 + (param_4 & 0xffff) * (param_6 & 0xffff) * 2 + param_2
                     + ((param_10 & 0xffff) - 1) * (param_6 & 0xffff) * 2);
  for (local_10 = 0; local_10 < (int)(param_10 & 0xffff); local_10 = local_10 + 1) {
    for (local_14 = 0; local_14 < (int)(param_9 & 0xffff); local_14 = local_14 + 1) {
      if (*local_8 != param_11) {
        *local_c = *local_8;
      }
      local_c = local_c + 1;
      local_8 = local_8 + 1;
    }
    local_c = local_c + ((param_5 & 0xffff) - (param_9 & 0xffff));
    local_8 = local_8 + -((param_9 & 0xffff) + (param_6 & 0xffff));
  }
  return;
}
