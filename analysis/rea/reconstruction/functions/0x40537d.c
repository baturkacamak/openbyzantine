// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40537d
// name: FUN_0040537d
// ranges: 0x40537d-0x4053f9
// calls: 0x40537d

MMRESULT __cdecl FUN_0040537d(undefined4 *param_1,LPMMCKINFO param_2,LPMMIOINFO param_3)

{
  MMRESULT local_8;
  
  param_2->ckid = 0x61746164;
  param_2->cksize = 0;
  local_8 = mmioCreateChunk((HMMIO)*param_1,param_2,0);
  if (local_8 == 0) {
    local_8 = mmioGetInfo((HMMIO)*param_1,param_3,0);
  }
  return local_8;
}
