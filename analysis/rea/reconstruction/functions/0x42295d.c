// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42295d
// name: FUN_0042295d
// ranges: 0x42295d-0x422a0c
// calls: 0x42295d

LONG FUN_0042295d(void)

{
  LONG LVar1;
  MMRESULT MVar2;
  LONG local_54;
  _MMIOINFO local_50;
  uint local_8;
  
  local_54 = 0;
  while ((DAT_0045fd20 != 0 && (LVar1 = InterlockedExchange((LONG *)&DAT_004618b0,1), LVar1 != 0)))
  {
    Sleep(100);
  }
  if ((DAT_0045fd28 != -1) && (DAT_0045fd14 != 0)) {
    MVar2 = mmioGetInfo(DAT_004867a4,&local_50,0);
    local_8 = (uint)(MVar2 == 0);
    if (local_8 != 0) {
      local_54 = local_50.lDiskOffset;
    }
  }
  InterlockedExchange((LONG *)&DAT_004618b0,0);
  return local_54;
}
