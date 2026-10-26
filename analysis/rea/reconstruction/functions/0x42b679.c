// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42b679
// name: FUN_0042b679
// ranges: 0x42b679-0x42b736
// calls: 0x42b679

void __cdecl
FUN_0042b679(int param_1,int param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            uint param_7,uint param_8,uint param_9,uint param_10)

{
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  local_10 = (void *)((param_7 & 0xffff) * 2 + (param_8 & 0xffff) * (param_5 & 0xffff) * 2 + param_1
                     );
  local_c = (void *)((param_3 & 0xffff) * 2 + (param_6 & 0xffff) * (param_4 & 0xffff) * 2 + param_2)
  ;
  for (local_14 = 0; local_14 < (int)(param_10 & 0xffff); local_14 = local_14 + 1) {
    FID_conflict__memcpy(local_10,local_c,(param_9 & 0xffff) * 2);
    local_c = (void *)((int)local_c + (param_6 & 0xffff) * 2);
    local_10 = (void *)((int)local_10 + (param_5 & 0xffff) * 2);
  }
  return;
}
