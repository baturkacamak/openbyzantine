// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4057c5
// name: FUN_004057c5
// ranges: 0x4057c5-0x4058fc
// calls: 0x4057c5

undefined4 __cdecl FUN_004057c5(HMMIO param_1,HMMIO param_2,FOURCC *param_3)

{
  HGLOBAL pvVar1;
  undefined4 uVar2;
  MMRESULT MVar3;
  FOURCC FVar4;
  _MMCKINFO local_1c;
  HPSTR local_8;
  
  pvVar1 = GlobalAlloc(0x42,param_3[1]);
  local_8 = GlobalLock(pvVar1);
  if (local_8 == (HPSTR)0x0) {
    uVar2 = 0;
  }
  else {
    local_1c.ckid = *param_3;
    local_1c.cksize = param_3[1];
    MVar3 = mmioCreateChunk(param_2,&local_1c,0);
    if ((((MVar3 == 0) && (FVar4 = mmioRead(param_1,local_8,param_3[1]), FVar4 == param_3[1])) &&
        (FVar4 = mmioWrite(param_2,local_8,param_3[1]), FVar4 == param_3[1])) &&
       (MVar3 = mmioAscend(param_2,&local_1c,0), MVar3 == 0)) {
      if (local_8 != (HPSTR)0x0) {
        pvVar1 = GlobalHandle(local_8);
        GlobalUnlock(pvVar1);
        pvVar1 = GlobalHandle(local_8);
        GlobalFree(pvVar1);
      }
      uVar2 = 1;
    }
    else {
      if (local_8 != (HPSTR)0x0) {
        pvVar1 = GlobalHandle(local_8);
        GlobalUnlock(pvVar1);
        pvVar1 = GlobalHandle(local_8);
        GlobalFree(pvVar1);
      }
      uVar2 = 0;
    }
  }
  return uVar2;
}
