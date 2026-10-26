// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x404fe3
// name: FUN_00404fe3
// ranges: 0x404fe3-0x40513e
// calls: 0x404fe3

MMRESULT __cdecl FUN_00404fe3(HMMIO param_1,uint param_2,int param_3,int param_4,uint *param_5)

{
  MMRESULT MVar1;
  uint local_5c;
  size_t local_58;
  _MMIOINFO local_54;
  MMRESULT local_c;
  uint local_8;
  
  local_c = 0;
  MVar1 = mmioGetInfo(param_1,&local_54,0);
  local_c = (MMRESULT)(MVar1 != 0);
  if (local_c == 0) {
    local_5c = param_2;
    if (*(uint *)(param_4 + 4) < param_2) {
      local_5c = *(uint *)(param_4 + 4);
    }
    *(int *)(param_4 + 4) = *(int *)(param_4 + 4) - local_5c;
    for (local_8 = 0; local_8 < local_5c; local_8 = local_8 + local_58) {
      if (local_54.pchNext == local_54.pchEndRead) {
        local_c = mmioAdvance(param_1,&local_54,0);
        if (local_c != 0) goto LAB_00405129;
        local_c = 0;
        if (local_54.pchNext == local_54.pchEndRead) {
          local_c = 0xe103;
          goto LAB_00405129;
        }
      }
      local_58 = (int)local_54.pchEndRead - (int)local_54.pchNext;
      if (local_5c - local_8 < local_58) {
        local_58 = local_5c - local_8;
      }
      FID_conflict__memcpy((void *)(local_8 + param_3),local_54.pchNext,local_58);
      local_54.pchNext = local_54.pchNext + local_58;
    }
    local_c = mmioSetInfo(param_1,&local_54,0);
    if (local_c == 0) {
      *param_5 = local_5c;
      return 0;
    }
  }
LAB_00405129:
  *param_5 = 0;
  return local_c;
}
