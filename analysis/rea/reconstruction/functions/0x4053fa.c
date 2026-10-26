// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4053fa
// name: FUN_004053fa
// ranges: 0x4053fa-0x405498
// calls: 0x4053fa

MMRESULT __cdecl
FUN_004053fa(HMMIO param_1,uint param_2,int param_3,undefined4 param_4,int *param_5,
            LPMMIOINFO param_6)

{
  MMRESULT local_c;
  uint local_8;
  
  local_c = 0;
  *param_5 = 0;
  local_8 = 0;
  do {
    if (param_2 <= local_8) {
      return local_c;
    }
    if (param_6->pchEndWrite == param_6->pchNext) {
      param_6->dwFlags = param_6->dwFlags | 0x10000000;
      local_c = mmioAdvance(param_1,param_6,1);
      if (local_c != 0) {
        return local_c;
      }
    }
    *param_6->pchNext = *(char *)(local_8 + param_3);
    param_6->pchNext = param_6->pchNext + 1;
    *param_5 = *param_5 + 1;
    local_8 = local_8 + 1;
  } while( true );
}
