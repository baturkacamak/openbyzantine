// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x405499
// name: FUN_00405499
// ranges: 0x405499-0x40562d
// calls: 0x405499

MMRESULT __cdecl
FUN_00405499(int *param_1,LPMMCKINFO param_2,LPMMCKINFO param_3,LPCMMIOINFO param_4)

{
  MMRESULT MVar1;
  MMRESULT local_8;
  
  if (*param_1 == 0) {
    local_8 = 0;
  }
  else {
    param_4->dwFlags = param_4->dwFlags | 0x10000000;
    local_8 = mmioSetInfo((HMMIO)*param_1,param_4,0);
    if (((local_8 == 0) && (local_8 = mmioAscend((HMMIO)*param_1,param_2,0), local_8 == 0)) &&
       (local_8 = mmioAscend((HMMIO)*param_1,param_3,0), local_8 == 0)) {
      mmioSeek((HMMIO)*param_1,0,0);
      local_8 = mmioDescend((HMMIO)*param_1,param_3,(MMCKINFO *)0x0,0);
      if (local_8 == 0) {
        param_2->ckid = 0x74636166;
        MVar1 = mmioDescend((HMMIO)*param_1,param_2,param_3,0x10);
        if (MVar1 == 0) {
          mmioWrite((HMMIO)*param_1,&stack0x00000014,4);
          mmioAscend((HMMIO)*param_1,param_2,0);
        }
        local_8 = mmioAscend((HMMIO)*param_1,param_3,0);
      }
    }
    if (*param_1 != 0) {
      mmioClose((HMMIO)*param_1,0);
      *param_1 = 0;
    }
  }
  return local_8;
}
