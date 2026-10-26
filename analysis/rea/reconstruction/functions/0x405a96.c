// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x405a96
// name: FUN_00405a96
// ranges: 0x405a96-0x405b5f
// calls: 0x405195, 0x40537d, 0x4053fa, 0x405499, 0x405a96

MMRESULT __cdecl
FUN_00405a96(LPSTR param_1,uint param_2,undefined4 param_3,short *param_4,int param_5)

{
  _MMCKINFO local_80;
  _MMCKINFO local_6c;
  _MMIOINFO local_58;
  HMMIO local_10;
  int local_c;
  MMRESULT local_8;
  
  local_8 = FUN_00405195(param_1,(int *)&local_10,param_4,&local_80,&local_6c);
  if (((local_8 == 0) && (local_8 = FUN_0040537d(&local_10,&local_80,&local_58), local_8 == 0)) &&
     (local_8 = FUN_004053fa(local_10,param_2,param_5,&local_80,&local_c,&local_58), local_8 == 0))
  {
    local_8 = FUN_00405499((int *)&local_10,&local_80,&local_6c,&local_58);
  }
  return local_8;
}
