// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422a22
// name: FUN_00422a22
// ranges: 0x422a22-0x422a8f
// calls: 0x422a22

int FUN_00422a22(void)

{
  LONG LVar1;
  
  while (DAT_0045fd20 != 0) {
    LVar1 = InterlockedExchange((LONG *)&DAT_004618b0,1);
    if (LVar1 == 0) break;
    Sleep(100);
  }
  if (DAT_0045fd28 != -1) {
    (**(code **)(*DAT_0045fd14 + 0x48))(DAT_0045fd14);
  }
  InterlockedExchange((LONG *)&DAT_004618b0,0);
  return DAT_0045fd28;
}
