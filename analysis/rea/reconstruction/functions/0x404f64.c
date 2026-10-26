// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x404f64
// name: FUN_00404f64
// ranges: 0x404f64-0x404fe2
// calls: 0x404f64

MMRESULT __cdecl FUN_00404f64(undefined4 *param_1,LPMMCKINFO param_2,MMCKINFO *param_3)

{
  MMRESULT MVar1;
  
  mmioSeek((HMMIO)*param_1,param_3->dwDataOffset + 4,0);
  param_2->ckid = 0x61746164;
  MVar1 = mmioDescend((HMMIO)*param_1,param_2,param_3,0x10);
  return MVar1;
}
