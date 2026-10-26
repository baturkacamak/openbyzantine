// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40562e
// name: FUN_0040562e
// ranges: 0x40562e-0x4056c5; 0x4056e4-0x40571d; 0x405728-0x4057c4
// calls: 0x40562e, 0x4057c5

undefined4 __cdecl
FUN_0040562e(undefined4 *param_1,LPMMCKINFO param_2,MMCKINFO *param_3,undefined4 *param_4)

{
  uint uVar1;
  LONG LVar2;
  MMRESULT MVar3;
  undefined4 local_8;
  
  LVar2 = mmioSeek((HMMIO)*param_1,param_3->dwDataOffset + 4,0);
  if (LVar2 == -1) {
    local_8 = 0xe102;
  }
  else {
    local_8 = 0;
    while ((MVar3 = mmioDescend((HMMIO)*param_1,param_2,param_3,0), MVar3 == 0 &&
           (param_2->dwDataOffset + param_2->cksize <= param_3->cksize + param_3->dwDataOffset))) {
      uVar1 = param_2->ckid;
      if ((0x20657563 < uVar1) && (0x4b4e554a < uVar1)) {
        if (uVar1 < 0x61746165) {
          if ((uVar1 != 0x61746164) && (uVar1 == 0x50534944)) {
            FUN_004057c5((HMMIO)*param_1,(HMMIO)*param_4,&param_2->ckid);
          }
        }
        else if ((uVar1 != 0x74636166) && (uVar1 == 0x74736c70)) {
          FUN_004057c5((HMMIO)*param_1,(HMMIO)*param_4,&param_2->ckid);
        }
      }
      mmioAscend((HMMIO)*param_1,param_2,0);
    }
  }
  mmioSeek((HMMIO)*param_1,param_3->dwDataOffset + 4,0);
  return local_8;
}
