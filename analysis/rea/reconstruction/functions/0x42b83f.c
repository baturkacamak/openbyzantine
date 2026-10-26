// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42b83f
// name: FUN_0042b83f
// ranges: 0x42b83f-0x42b8da
// calls: 0x42b83f

void __cdecl
FUN_0042b83f(int param_1,void *param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            uint param_7)

{
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  local_10 = (void *)((param_6 & 0xffff) * 2 + (param_7 & 0xffff) * (param_3 & 0xffff) * 2 + param_1
                     );
  local_c = param_2;
  for (local_14 = 0; local_14 < (int)(param_4 & 0xffff); local_14 = local_14 + 1) {
    FID_conflict__memcpy(local_10,local_c,(param_5 & 0xffff) * 2);
    local_10 = (void *)((int)local_10 + (param_3 & 0xffff) * 2);
    local_c = (void *)((int)local_c + (param_5 & 0xffff) * 2);
  }
  return;
}
